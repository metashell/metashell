
#include "file_location.hpp"

namespace metashell {

file_location::file_location() : name(), row(-1), column(-1) {}

file_location::file_location(const std::string& name, int row, int column) :
  name(name), row(row), column(column) {}

std::ostream& operator<<(std::ostream& os, const file_location& location) {
  os << location.name << ":" << location.row << ":" << location.column;
  return os;
}

}

