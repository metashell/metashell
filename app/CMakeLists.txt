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

# LLVM
target_link_libraries(metashell ${LLVM_LIBRARY})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${LLVM_CXX_FLAGS})

install(TARGETS metashell DESTINATION bin)

