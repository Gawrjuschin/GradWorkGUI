# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\gradwork_gui_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\gradwork_gui_autogen.dir\\ParseCache.txt"
  "QueSys\\CMakeFiles\\QueSys_autogen.dir\\AutogenUsed.txt"
  "QueSys\\CMakeFiles\\QueSys_autogen.dir\\ParseCache.txt"
  "QueSys\\CMakeFiles\\gradwork_nogui_autogen.dir\\AutogenUsed.txt"
  "QueSys\\CMakeFiles\\gradwork_nogui_autogen.dir\\ParseCache.txt"
  "QueSys\\QueSys_autogen"
  "QueSys\\gradwork_nogui_autogen"
  "gradwork_gui_autogen"
  )
endif()
