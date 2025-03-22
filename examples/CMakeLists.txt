cmake_minimum_required(VERSION 3.10)

option(ADD_EXAMPLES "Creates examples" OFF)

if(POLICY CMP0002)
    cmake_policy(SET CMP0002 OLD)
    set_property(GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS TRUE)
endif()

if(ADD_EXAMPLES)
    add_subdirectory(linux/sysfs/)
    add_subdirectory(linux/trigger/oneshot/)
    add_subdirectory(linux/trigger/periodic/)
endif()
