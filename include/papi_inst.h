#ifndef PAPI_ENERGY_OPENEARTH
# define PAPI_ENERGY_OPENEARTH

#include <stdlib.h>
#include <papi.h>
#include <stdint.h>
#include <stddef.h>

// #include <malloc.h>
// #include <papi.h>
// #include <stdint.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
#define start_instrumentaion
#define stop_instrumentation
#define print_instruments


#ifdef PAPI_OPENEARTH

  long long int papi_event_value = 0;
  int eventset = 0;
  const char *env_var_name = "PAPI_EVENT_NAME";
  const char *papi_event_name = getenv(env_var_name);
  int retval = 0;

  void papi_init(){
    retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
      fprintf(stderr, "Error initializing PAPI! %s \n", PAPI_strerror(retval));
      exit(1);
    }
  }
  
  void create_event_set(){
    eventset = PAPI_NULL;
    // papi creating event set
    retval = PAPI_create_eventset(&eventset);
    if (retval != PAPI_OK) {
      fprintf(stderr, "Error creating eventset! %s\n", PAPI_strerror(retval));
    }
    // papi adding event set
    retval = PAPI_add_named_event(eventset, papi_event_name);
    if (retval != PAPI_OK) {
      fprintf(stderr, "Error adding %s: %s\n", papi_event_name,
                PAPI_strerror(retval));
    }
  }

  void papi_start(){
    PAPI_reset(eventset);
    retval = PAPI_start(eventset);
    if (retval != PAPI_OK) {
      fprintf(stderr, "Error PAPI: %s\n", PAPI_strerror(retval));
    }
  }

  void papi_stop(){
    retval = PAPI_stop(eventset, &papi_event_value);
    if (retval != PAPI_OK) {
      fprintf(stderr, "Error stopping:  %s\n", PAPI_strerror(retval));
    }
    //else {
    //   fprintf(stderr, "Measured %lld hw cache misses\n", count);
    // }
  }

  void print_papi(){
    #ifdef HUMAN_READABLE
    fprintf(stderr,"Measured %s event %lld times\n", papi_event_name, papi_event_value);
    #endif
    fprintf(stdout,"%lld",papi_event_value);
  }

# undef start_instrumentaion
# undef stop_instrumentation
# undef print_instruments

#define start_instrumentaion \
papi_init();                 \
create_event_set();          \
papi_start();                \

#define stop_instrumentation \
papi_stop();

#define print_instruments    \
print_papi();

#endif


#ifdef ENERGY_TIME_OPENEARTH

  double time_reading = 0;
  long long energy_reading = 0;
  uint64_t start_time_counter = 0;
  uint64_t end_time_counter = 0;
  long long start_energy_counter = 0;
  long long end_energy_counter = 0;


namespace cpu {
inline static uint64_t get_ticks_acquire() {
  uint32_t low, high;
  __asm__ __volatile__("xor %%eax, %%eax;"
                       "cpuid;"
                       "rdtsc;"
                       : "=a"(low), "=d"(high)
                       :
                       : "%rbx", "%rcx");
  return (uint64_t(high) << 32) | uint64_t(low);
}

inline static uint64_t get_ticks_release() {
  uint32_t low, high;
  __asm__ __volatile__("rdtscp;" : "=a"(low), "=d"(high) : : "%rcx");
  return (uint64_t(high) << 32) | uint64_t(low);
}
} // namespace cpu

long long get_energy() {
  FILE *fp;
  char path[1035];
  long long value;

  /* Open the command for reading. */
  fp = popen("rdmsr -u 1553 | xargs -0 -I{} echo {}", "r");
  if (fp == NULL) {
    fprintf(stderr, "Failed to run command\n");
    exit(1);
  }

  /* Read the output a line at a time - it should be just one line. */
  if (fgets(path, sizeof(path) - 1, fp) != NULL) {
    // fprintf(stderr, "Output: %s", path);
    value = atoll(path); // Convert the output to an integer
  }

  /* Close the pipe and print the value. */
  pclose(fp);
  // fprintf(stderr, "Value: %lld\n", value);

  return value;
}

void start_energy_time(){
  start_time_counter = cpu::get_ticks_acquire();
  start_energy_counter = get_energy();
}

void end_energy_time(){
  end_energy_counter = get_energy();
  end_time_counter = cpu::get_ticks_release();
}

void print_energy_time(){
    energy_reading = end_energy_counter - start_energy_counter;
    time_reading = double(end_time_counter - start_time_counter) / 1.7e+9;
    #ifdef HUMAN_READABLE
    fprintf(stderr,"Measured Energy : %lld \n", energy_reading);
    fprintf(stderr,"Measured Energy : %lf \n", time_reading);
    #endif
    fprintf(stdout,"%lld\n", energy_reading);
    fprintf(stdout,"%lf\n", time_reading);
}

# undef start_instrumentaion
# undef stop_instrumentation
# undef print_instruments

#define start_instrumentaion \
start_energy_time();         \

#define stop_instrumentation \
end_energy_time();

#define print_instruments    \
print_energy_time();

#endif


#endif /* PAPI_ENERGY_OPENEARTH */