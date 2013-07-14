# Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

# This module will define the following:
#   Editline_FOUND
#   Editline_INCLUDE_DIR
#   Editline_LIBRARY

find_path(Editline_INCLUDE_DIR editline/readline.h)
find_library(Editline_LIBRARY NAMES edit)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set Editline_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
  Editline DEFAULT_MSG Editline_LIBRARY Editline_INCLUDE_DIR
)

mark_as_advanced(Editline_INCLUDE_DIR, Editline_LIBRARY)

