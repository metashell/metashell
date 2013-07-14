# Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

# This module will define the following:
#   LLVM_FOUND
#   LLVM_INCLUDE_DIR
#   LLVM_CXX_FLAGS
#   LLVM_LIBRARY

find_program(LLVM_CONFIG_EXECUTABLE llvm-config
  PATHS
  $ENV{LLVM_HOME}
  NO_DEFAULT_PATH
)
find_program(LLVM_CONFIG_EXECUTABLE llvm-config)

if (NOT LLVM_CONFIG_EXECUTABLE)
  message(FATAL_ERROR "Could not find llvm-config")
endif (NOT LLVM_CONFIG_EXECUTABLE)

message(STATUS "LLVM llvm-config found at: ${LLVM_CONFIG_EXECUTABLE}")

execute_process(
  COMMAND ${LLVM_CONFIG_EXECUTABLE} --includedir
  OUTPUT_VARIABLE LLVM_INCLUDE_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND ${LLVM_CONFIG_EXECUTABLE} --libdir
  OUTPUT_VARIABLE LLVM_LIBRARY_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# TODO
set(LLVM_CXX_FLAGS -fno-rtti)

execute_process(
  COMMAND ${LLVM_CONFIG_EXECUTABLE} --libnames
  OUTPUT_VARIABLE LLVM_LIBS_TMP
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# -ldl
find_library(DL_LIBRARY NAMES dl)
message(STATUS "LLVM dl: ${DL_LIBRARY}")

# Use absolute path
string(REPLACE " " ";" LLVM_LIBS_TMP2 ${LLVM_LIBS_TMP})
set(LLVM_LIBRARY "")
foreach (LIB ${LLVM_LIBS_TMP2})
  set(LLVM_LIBRARY ${LLVM_LIBRARY} "${LLVM_LIBRARY_DIR}/${LIB}")
endforeach(LIB)
# order is important, it must be the last one
set(LLVM_LIBRARY ${LLVM_LIBRARY} "${DL_LIBRARY}")

# Clang libraries
set(CLANG_LIBS
  clangTooling
  clangFrontendTool
  clangFrontend
  clangDriver
  clangSerialization
  clangCodeGen
  clangParse
  clangSema
  clangStaticAnalyzerFrontend
  clangStaticAnalyzerCheckers
  clangStaticAnalyzerCore
  clangAnalysis
  clangARCMigrate
  clangRewriteFrontend
  clangRewriteCore
  clangEdit
  clangAST
  clangLex
  clangBasic
)

set(CLANG_LIBRARY "")
foreach (LIB ${CLANG_LIBS})
  find_library("CLANG_${LIB}_LIBRARY" NAMES ${LIB})
  message(STATUS "Clang lib ${CLANG_${LIB}_LIBRARY}")
  set(CLANG_LIBRARY ${CLANG_LIBRARY} ${CLANG_${LIB}_LIBRARY})
endforeach(LIB)





