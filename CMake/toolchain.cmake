# Copyright (C) 2014 ARM Limited. All rights reserved. 

cmake_minimum_required(VERSION 2.8)

# default to C99
set(CMAKE_C_FLAGS "-std=c99" CACHE STRING "")

# check that we are actually running on Linux, if we're not then we may pull in
# incorrect dependencies.
if(NOT (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux"))
    message(FATAL_ERROR "This Linux native target will not work on non-Linux platforms, use `yotta target` to set the target.")
endif()

