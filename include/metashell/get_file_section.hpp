#ifndef METASHELL_GET_FILE_SECTION_HPP
#define METASHELL_GET_FILE_SECTION_HPP

#include <string>
#include <vector>
#include <istream>

namespace metashell {

struct indexed_line {
  int line_index;
  std::string line;
};

using file_section = std::vector<indexed_line>;

// If there are enough lines in the stream, this will return that line and
// +/- offset lines before and after that line
file_section get_file_section(
  std::istream& stream, int middle_line, int offset);

file_section get_file_section_from_file(
  const std::string& file_name, int middle_line, int offset);

} // namespace metashell

#endif
