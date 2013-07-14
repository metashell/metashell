# Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

aux_source_directory(. SOURCES)
add_library(metashell_lib STATIC ${SOURCES})

# LLVM
include_directories(${LLVM_INCLUDE_DIR})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${LLVM_CXX_FLAGS})
target_link_libraries(metashell_lib ${CLANG_LIBRARY})
target_link_libraries(metashell_lib ${LLVM_LIBRARY})
target_link_libraries(metashell_lib ${CMAKE_THREAD_LIBS_INIT})
