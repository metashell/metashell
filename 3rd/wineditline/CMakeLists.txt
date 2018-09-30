project(wineditline)

add_library(${PROJECT_NAME} STATIC editline.c fn_complete.c history.c)

target_include_directories(
  ${PROJECT_NAME} SYSTEM
  PUBLIC ${PROJECT_SOURCE_DIR}
)

