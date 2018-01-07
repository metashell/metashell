// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <metashell/console_displayer.hpp>
#include <metashell/data/colored_string.hpp>
#include <metashell/get_file_section.hpp>
#include <metashell/highlight_syntax.hpp>
#include <metashell/indenter.hpp>

#include <mindent/display.hpp>
#include <mindent/parser.hpp>
#include <mindent/stream_display.hpp>
#include <mindent/syntax_node.hpp>
#include <mindent/syntax_node_list.hpp>

#include <boost/filesystem/path.hpp>

#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <sstream>

using namespace metashell;

namespace
{
  template <class DisplayF>
  void indent(int width_,
              int indent_step_,
              DisplayF f_,
              const data::cpp_code& s_,
              const std::string& input_filename_)
  {
    std::unique_ptr<iface::tokeniser> tokeniser =
        create_wave_tokeniser(s_, input_filename_);

    mindent::display(
        mindent::parse_syntax_node_list(*tokeniser), width_, indent_step_, f_);
  }

  data::color get_color(int n_)
  {
    data::color cs[] = {data::color::red, data::color::green,
                        data::color::yellow, data::color::blue,
                        data::color::cyan};
    return cs[n_ % (sizeof(cs) / sizeof(cs[0]))];
  }

  void display_trace_graph(int depth_,
                           const std::vector<int>& depth_counter_,
                           bool print_mark_,
                           pager& pager_)
  {
    assert(depth_counter_.size() > static_cast<unsigned int>(depth_));

    if (depth_ > 0)
    {
      // TODO respect the -H (no syntax highlight parameter)
      for (int i = 1; i < depth_; ++i)
      {
        pager_.show(data::colored_string(
            depth_counter_[i] > 0 ? "| " : "  ", get_color(i)));
      }

      const data::color mark_color = get_color(depth_);
      if (print_mark_)
      {
        if (depth_counter_[depth_] == 0)
        {
          pager_.show(data::colored_string("` ", mark_color));
        }
        else
        {
          pager_.show(data::colored_string("+ ", mark_color));
        }
      }
      else if (depth_counter_[depth_] > 0)
      {
        pager_.show(data::colored_string("| ", mark_color));
      }
      else
      {
        pager_.show("  ");
      }
    }
  }

  template <class InputIt>
  void show_filenames(InputIt begin_, InputIt end_, pager& pager_)
  {
    for (InputIt i = begin_; i != end_; ++i)
    {
      pager_.show(data::colored_string(i->string(), data::color::bright_cyan));
      if (!pager_.new_line())
      {
        break;
      }
    }
  }

  class format_visitor : public boost::static_visitor<>
  {
  public:
    typedef std::function<data::colored_string(const data::cpp_code&)>
        code_formatter;

    typedef std::function<data::colored_string(const data::token&)>
        token_formatter;

    typedef std::function<data::colored_string(const boost::filesystem::path&)>
        path_formatter;

    format_visitor(data::colored_string& out_,
                   code_formatter code_formatter_,
                   token_formatter token_formatter_,
                   path_formatter path_formatter_)
      : _out(out_),
        _code_formatter(move(code_formatter_)),
        _token_formatter(move(token_formatter_)),
        _path_formatter(move(path_formatter_))
    {
    }

    void operator()(const data::type& t_) const { _out += _code_formatter(t_); }
    void operator()(const data::cpp_code& c_) const
    {
      _out += _code_formatter(c_);
    }
    void operator()(const data::token& t_) const
    {
      _out += _token_formatter(t_);
    }
    void operator()(const boost::filesystem::path& p_) const
    {
      _out += _path_formatter(p_);
    }

    template <class T>
    void operator()(const unique<T>& u_) const
    {
      operator()(u_.value());
    }

  private:
    data::colored_string& _out;
    code_formatter _code_formatter;
    token_formatter _token_formatter;
    path_formatter _path_formatter;
  };
} // anonymouse namespace

console_displayer::console_displayer(iface::console& console_,
                                     bool indent_,
                                     bool syntax_highlight_)
  : _console(&console_), _indent(indent_), _syntax_highlight(syntax_highlight_)
{
}

void console_displayer::show_raw_text(const std::string& text_)
{
  _console->show(text_);
  _console->new_line();
}

void console_displayer::show_error(const std::string& msg_)
{
  if (!msg_.empty())
  {
    if (_syntax_highlight)
    {
      _console->show(data::colored_string(msg_, data::color::bright_red));
    }
    else
    {
      _console->show(msg_);
    }
    _console->new_line();
  }
}

void console_displayer::show_type(const data::type& type_)
{
  show_cpp_code(type_);
}

void console_displayer::show_comment(const data::text& msg_)
{
  // TODO: handle one-liners differently (with //)
  indenter ind(_console->width());

  ind.raw("/*");

  for (const auto& p : msg_.paragraphs)
  {
    if (p.content.empty())
    {
      ind.raw(" * ");
    }
    else
    {
      ind.left_align(p.content, " * " + p.rest_of_lines_indentation,
                     " * " + p.first_line_indentation);
    }
  }

  ind.raw(" */");

  show_cpp_code(data::cpp_code(ind.str()));
}

void console_displayer::show_cpp_code(const data::cpp_code& code_)
{
  if (!code_.empty())
  {
    if (_indent)
    {
      if (_syntax_highlight)
      {
        indent(_console->width(), 2,
               std::function<void(const data::token&)>(
                   [this](const data::token& t_) {
                     this->_console->show(highlight_syntax(t_.value()));
                   }),
               code_, "<output>");
      }
      else
      {
        indent(_console->width(), 2,
               std::function<void(const data::token&)>(
                   [this](const data::token& t_) {
                     this->_console->show(t_.value().value());
                   }),
               code_, "<output>");
      }
    }
    else
    {
      _console->show(format_code(code_));
    }
    _console->new_line();
  }
}

data::colored_string console_displayer::format_token(const data::token& token_)
{
  return format_code(data::cpp_code(data::format_token(token_)));
}

data::colored_string console_displayer::format_code(const data::cpp_code& code_)
{
  if (_syntax_highlight)
  {
    return highlight_syntax(code_);
  }
  else
  {
    return code_.value();
  }
}

data::colored_string console_displayer::format_time(double time_in_seconds_)
{
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(2);
  ss << time_in_seconds_ * 1000.0 << "ms";
  return ss.str();
}

data::colored_string console_displayer::format_ratio(double ratio_)
{
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(2);
  ss << ratio_ * 100.0 << "%";
  return ss.str();
}

data::colored_string
console_displayer::format_metaprogram_node(const data::metaprogram_node& n_)
{
  data::colored_string result;

  boost::apply_visitor(
      format_visitor(
          result,
          [this](const data::cpp_code& s_) { return this->format_code(s_); },
          [this](const data::token& t_) { return this->format_token(t_); },
          [](const boost::filesystem::path& p_) {
            return data::colored_string(p_.string());
          }),
      n_);

  return result;
}

data::colored_string console_displayer::format_frame(const data::frame& f_)
{
  data::colored_string prefix;
  if (f_.is_profiled())
  {
    prefix = "[" + format_time(f_.time_taken()) + ", " +
             format_ratio(f_.time_taken_ratio()) + "] ";
  }

  std::ostringstream postfix;
  if (f_.is_full())
  {
    postfix << " at " << f_.source_location() << " (" << f_.kind() << " from "
            << f_.point_of_event() << ")";
  }
  return prefix + format_metaprogram_node(f_.node()) + postfix.str();
}

bool console_displayer::display_frame_with_pager(const data::frame& frame_,
                                                 pager& pager_)
{
  pager_.show(format_frame(frame_));
  return pager_.new_line();
}

bool console_displayer::display_node(const data::call_graph_node& node_,
                                     const std::vector<int>& depth_counter_,
                                     pager& pager_)
{
  const auto width = _console->width();

  const data::colored_string element_content =
      format_frame(node_.current_frame());

  const int non_content_length = 2 * node_.depth();

  const int pretty_print_threshold = 10;
  if (width < pretty_print_threshold ||
      non_content_length >= width - pretty_print_threshold)
  {
    // We have no chance to display the graph nicely :(
    display_trace_graph(node_.depth(), depth_counter_, true, pager_);

    pager_.show(element_content);
    return pager_.new_line();
  }
  else
  {
    int content_width = width - non_content_length;
    for (unsigned i = 0; i < element_content.size(); i += content_width)
    {
      display_trace_graph(node_.depth(), depth_counter_, i == 0, pager_);
      pager_.show(element_content.substr(i, content_width));
      if (!pager_.new_line())
      {
        return false;
      }
    }
    return true;
  }
}

void console_displayer::show_frame(const data::frame& frame_)
{
  _console->show(format_frame(frame_));
  _console->new_line();
}

void console_displayer::show_file_section(const data::file_location& location_,
                                          const std::string& env_buffer_)
{
  file_section section;
  if (location_.name == "<stdin>")
  {
    section = get_file_section_from_buffer(env_buffer_, location_.row, 3);
  }
  else
  {
    section = get_file_section_from_file(location_.name, location_.row, 3);
  }

  if (section.empty())
  {
    return;
  }

  int largest_index_length = std::to_string(section.back().line_index).size();

  for (const auto& indexed_line : section)
  {
    std::stringstream ss;
    if (indexed_line.line_index == location_.row)
    {
      ss << "->";
    }
    else
    {
      ss << "  ";
    }
    ss << std::setw(largest_index_length + 1);
    ss << indexed_line.line_index << "  ";

    _console->show(ss.str());
    _console->show(format_code(data::cpp_code(indexed_line.line + "\n")));
  }
}

void console_displayer::show_backtrace(const data::backtrace& trace_)
{
  pager pager(*_console);

  int i = 0;
  for (const data::frame& f : trace_)
  {
    std::ostringstream s;
    s << "#" << i << " ";
    pager.show(data::colored_string(s.str(), data::color::white));
    if (!display_frame_with_pager(f, pager))
    {
      break;
    }
    ++i;
  }
}

void console_displayer::show_call_graph(const iface::call_graph& cg_)
{
  pager pager(*_console);

  std::vector<int> depth_counter(1);

  ++depth_counter[0]; // This value is neved read

  for (const data::call_graph_node& n : cg_)
  {
    --depth_counter[n.depth()];

    if (!display_node(n, depth_counter, pager))
    {
      return;
    }

    if (depth_counter.size() <= static_cast<unsigned int>(n.depth() + 1))
    {
      depth_counter.resize(n.depth() + 1 + 1);
    }

    depth_counter[n.depth() + 1] += n.number_of_children();
  }
}

void console_displayer::show_filename_list(
    const std::vector<boost::filesystem::path>& filenames_)
{
  pager pager(*_console);
  show_filenames(filenames_.begin(), filenames_.end(), pager);
}

void console_displayer::show_filename_set(
    const std::set<boost::filesystem::path>& filenames_)
{
  pager pager(*_console);
  show_filenames(filenames_.begin(), filenames_.end(), pager);
}
