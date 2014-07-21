#ifndef METASHELL_PRAGMA_TEMPLIGHT_HPP
#define METASHELL_PRAGMA_TEMPLIGHT_HPP

#include <metashell/templight_trace.hpp>
#include <metashell/pragma_handler_interface.hpp>

#include <string>

namespace metashell
{
  class shell;

  class pragma_templight : public pragma_handler_interface
  {
  public:
    explicit pragma_templight(shell& shell_);

    virtual pragma_handler_interface* clone() const;

    virtual std::string arguments() const;
    virtual std::string description() const;

    virtual void run(
      const token_iterator& args_begin_,
      const token_iterator& args_end_
    ) const;
  private:
    void command_print(const templight_trace& trace) const;
    void command_print_dot(const templight_trace& trace) const;
    void command_forward_trace(
        const templight_trace& trace,
        const std::vector<char>& type) const;
    void command_unknown() const;

    void run_command(const std::string& command, templight_trace& trace) const;

    shell& _shell;
  };
}

#endif

