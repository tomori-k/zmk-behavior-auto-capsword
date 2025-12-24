cmake_minimum_required(VERSION 3.13.1)

if((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
  target_sources(app PRIVATE src/behavior_auto_caps_word.c)
endif()
