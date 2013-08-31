# Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

aux_source_directory(. SOURCES)
add_executable(metashell ${SOURCES})

# Editline
find_package(Editline)
include_directories(${Editline_INCLUDE_DIR})

# Metashell
target_link_libraries(metashell metashell_lib)

# Editline
target_link_libraries(metashell ${Editline_LIBRARY})

# Program_options
target_link_libraries(metashell ${Boost_PROGRAM_OPTIONS_LIBRARIES})

# Clang
target_link_libraries(metashell ${CLANG_LIBRARY})
include_directories(${CLANG_INCLUDE_DIR})

# Wave
target_link_libraries(metashell
  ${Boost_SYSTEM_LIBRARIES}
  ${Boost_THREAD_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARIES}
  ${Boost_WAVE_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS metashell DESTINATION bin)

