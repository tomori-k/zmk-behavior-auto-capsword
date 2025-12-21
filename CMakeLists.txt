cmake_minimum_required(VERSION 3.13.1)

zephyr_library()

if((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
  zephyr_library_sources(src/behavior_auto_caps_word.c)
endif()
