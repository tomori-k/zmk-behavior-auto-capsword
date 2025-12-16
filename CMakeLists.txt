cmake_minimum_required(VERSION 3.20.0)

if(NOT DEFINED ZEPHYR_BASE)
  message(FATAL_ERROR "This module must be used with a Zephyr-based application.")
endif()

zephyr_library()
zephyr_library_sources(src/auto_capsword.c)
zephyr_include_directories(include)
