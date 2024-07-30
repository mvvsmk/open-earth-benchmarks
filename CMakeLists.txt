cmake_minimum_required(VERSION 3.1)

project(OpenEarthBenchmarks VERSION 1.0
        DESCRIPTION "Open Earth Benchmarks"
        LANGUAGES CXX)

option(ENABLE_PAPI_SUPPORT "Enable PAPI support" OFF)
option(ENABLE_ENERGY_TIME_SUPPORT "Enable Energy Time support" OFF)
option(ENABLE_HUMAN_READABLE_SUPPORT "Enable Human Readable support" OFF)

include_directories(include)
include_directories(kernels)

if(ENABLE_ENERGY_TIME_SUPPORT)
        add_definitions(-DENERGY_TIME_OPENEARTH)
endif(ENABLE_ENERGY_TIME_SUPPORT)
if(ENABLE_PAPI_SUPPORT)
        add_definitions(-DPAPI_OPENEARTH)
    link_libraries(papi)
endif(ENABLE_PAPI_SUPPORT)
if(ENABLE_HUMAN_READABLE_SUPPORT)
        add_definitions(-DHUMAN_READABLE)
endif(ENABLE_HUMAN_READABLE_SUPPORT)


add_executable(laplace src/laplace.cpp)
add_executable(hdiffsa src/hdiffsa.cpp)
add_executable(hdiffsmag src/hdiffsmag.cpp)
add_executable(hadvuv src/hadvuv.cpp)
add_executable(hadvuv5th src/hadvuv5th.cpp)
add_executable(fastwavesuv src/fastwavesuv.cpp)
add_executable(uvbke src/uvbke.cpp)
add_executable(p_grad_c src/p_grad_c.cpp)
add_executable(nh_p_grad src/nh_p_grad.cpp)
add_executable(fvtp2d src/fvtp2d.cpp)
