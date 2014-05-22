#include <metashell/unsaved_file.hpp>

#include "exception.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <iostream>
#include <fstream>

using namespace metashell;

unsaved_file::unsaved_file(
  const std::string& filename_,
  const std::string& content_
) :
  _filename(filename_),
  _content(content_)
{}

CXUnsavedFile unsaved_file::get() const
{
  CXUnsavedFile entry;
  entry.Filename = _filename.c_str();
  entry.Contents = _content.c_str();
  entry.Length = _content.size();
  return entry;
}

const std::string& unsaved_file::filename() const
{
  return _filename;
}

const std::string& unsaved_file::content() const
{
  return _content;
}

void unsaved_file::generate() const
{
  boost::filesystem::path p(_filename);
  p.remove_filename();
  create_directories(p); // Throws when fails to create the directory
  std::ofstream f(_filename.c_str());
  if (f)
  {
    f << _content;
  }
  else
  {
    throw exception("Error creating file " + _filename);
  }
}

