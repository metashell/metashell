#ifndef METASHELL_TEMPORARY_FILE_HPP
#define METASHELL_TEMPORARY_FILE_HPP

#include <boost/filesystem.hpp>

namespace metashell {

class temporary_file {
public:
  explicit temporary_file(
      const boost::filesystem::path& model = "%%%%-%%%%-%%%%");

  ~temporary_file();

  boost::filesystem::path get_path() const;

private:
  boost::filesystem::path path;
};

}

#endif
