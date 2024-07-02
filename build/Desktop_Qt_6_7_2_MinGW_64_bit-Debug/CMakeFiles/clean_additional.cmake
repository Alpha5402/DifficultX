# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\circle_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\circle_autogen.dir\\ParseCache.txt"
  "circle_autogen"
  )
endif()
