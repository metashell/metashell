
#ifndef METASHELL_FILE_LOCATION_HPP
#define METASHELL_FILE_LOCATION_HPP

#include <string>

namespace metashell {

struct file_location {
  file_location();
  file_location(const std::string& name, int row, int column);

  std::string name;
  int row;
  int column;
};

}

#endif
