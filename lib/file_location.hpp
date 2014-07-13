
#ifndef METASHELL_FILE_LOCATION_HPP
#define METASHELL_FILE_LOCATION_HPP

#include <string>
#include <ostream>

namespace metashell {

struct file_location {
  file_location();
  file_location(const std::string& name, int row, int column);

  std::string name;
  int row;
  int column;
};

std::ostream& operator<<(std::ostream& os, const file_location& location);

}

#endif
