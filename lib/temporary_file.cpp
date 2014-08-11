
#include <metashell/temporary_file.hpp>

namespace metashell {

temporary_file::temporary_file(
    const boost::filesystem::path& model)
{
  boost::filesystem::path temporary_file_model =
    boost::filesystem::temp_directory_path();
  temporary_file_model /= model;

  path = boost::filesystem::unique_path(temporary_file_model);
}

temporary_file::~temporary_file() {
  if (boost::filesystem::exists(path)) {
    boost::filesystem::remove(path);
  }
}

boost::filesystem::path temporary_file::get_path() const {
  return path;
}

}

