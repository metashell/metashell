
include_directories("${CMAKE_SOURCE_DIR}/3rd/wineditline")
add_library(edit_static STATIC editline.c fn_complete.c history.c)

