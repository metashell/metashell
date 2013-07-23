# Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

# This module will define the following:
# CLANG_FOUND
# CLANG_INCLUDE_DIR
# CLANG_LIBRARY

find_path(CLANG_INCLUDE_DIR clang-c/Index.h)
find_library(CLANG_LIBRARY NAMES clang)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set CLANG_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
  CLANG DEFAULT_MSG CLANG_LIBRARY CLANG_INCLUDE_DIR
)

mark_as_advanced(CLANG_INCLUDE_DIR, CLANG_LIBRARY)


