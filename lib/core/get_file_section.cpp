#include <metashell/get_file_section.hpp>

#include <fstream>

namespace metashell {

file_section get_file_section(
  std::istream& stream, int middle_line, int offset)
{
  file_section result;

  std::string line;
  for (int i = 1; std::getline(stream, line); ++i) {
    if (i > middle_line + offset) {
      break;
    }
    if (i < middle_line - offset) {
      continue;
    }
    result.push_back({i, line});
  }
  return result;
}

file_section get_file_section_from_file(
  const std::string& file_name, int middle_line, int offset)
{
  std::ifstream in(file_name);
  if (!in) {
    return {};
  }
  return get_file_section(in, middle_line, offset);
}

} // namespace metashell
