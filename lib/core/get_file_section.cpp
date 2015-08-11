#include <metashell/get_file_section.hpp>

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

} // namespace metashell
