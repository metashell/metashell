
#include "file_location.hpp"
#include "templight_trace.hpp"

#include <string>
#include <fstream>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
  metashell::file_location,
  (std::string, name)
  (int, row)
  (int, column)
)

namespace metashell {

namespace spirit = boost::spirit;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phx = boost::phoenix;

typedef ascii::space_type skipper_t;

templight_trace create_trace() {
  return templight_trace();
}

templight_trace handle_template_begin(
    instantiation_kind kind,
    const std::string& context,
    const file_location& location,
    double timestamp,
    unsigned long long memory_usage)
{
  std::cout << "Begin: " << context << std::endl;
  return templight_trace();
}

template<class Iterator>
struct templight_grammar :
  qi::grammar<Iterator, templight_trace(), skipper_t> {

  templight_grammar() : templight_grammar::base_type(start) {

    using qi::_val;
    using qi::_1;
    using qi::_2;
    using qi::_3;
    using qi::_4;
    using qi::_5;
    using qi::lit;
    using qi::int_;
    using qi::double_;
    using qi::ulong_long;

    unescaped_characters.add
      ("&lt;", '<')
      ("&gt;", '>')
      ("&apos;", '\'')
      ("&quot;", '"')
      ("&amp;", '&')
    ;

    instantiation_kinds.add
      ("TemplateInstantiation", template_instantiation)
      ("DefaultTemplateArgumentInstantiation", default_template_argument_instantiation)
      ("DefaultFunctionArgumentInstantiation", default_function_argument_instantiation)
      ("ExplicitTemplateArgumentSubstitution", explicit_template_argument_substitution)
      ("DeducedTemplateArgumentSubstitution", deduced_template_argument_substitution)
      ("PriorTemplateArgumentSubstitution", prior_template_argument_substitution)
      ("DefaultTemplateArgumentChecking", default_template_argument_checking)
      ("ExceptionSpecInstantiation", exception_spec_instantiation)
      ("Memoization", memoization)
    ;

    const std::string prologue = "<?xml version=\"1.0\" standalone=\"yes\"?>";

    start %= prologue >> lit("<Trace>") >> body >> lit("</Trace>");

    body = *template_instantiation_rule
      [_val = phx::bind(&create_trace)];

    template_instantiation_rule =
      template_begin >> (*template_instantiation_rule) >> template_end
      [_val = phx::bind(&create_trace)];


    //<TemplateBegin>
    //    <Kind>TemplateInstantiation</Kind>
    //    <Context context = "fib&lt;7&gt;"/>
    //    <PointOfInstantiation><stdin>|1|50</PointOfInstantiation>
    //    <TimeStamp time = "456168594.581344"/>
    //    <MemoryUsage bytes = "0"/>
    //</TemplateBegin>

    template_begin =
      (
      lit("<TemplateBegin>") >>
        "<Kind>" >> instantiation_kinds >> "</Kind>" >>
        "<Context" >> "context" >> "=" >> context >> "/>" >>
        "<PointOfInstantiation>" >> file_location_rule >> "</PointOfInstantiation>" >>
        "<TimeStamp" >> "time" >> "=" >> timestamp >> "/>" >>
        "<MemoryUsage" >> "bytes" >> "=" >> memory_usage >> "/>" >>
      lit("</TemplateBegin>")
      ) [_val = phx::bind(&handle_template_begin, _1, _2, _3, _4, _5)];

    //<TemplateEnd>
    //    <Kind>TemplateInstantiation</Kind>
    //    <TimeStamp time = "456168594.586666"/>
    //    <MemoryUsage bytes = "0"/>
    //</TemplateEnd>

    template_end =
      (
      lit("<TemplateEnd>") >>
        lit("<Kind>") >> instantiation_kinds >> lit("</Kind>") >>
        lit("<TimeStamp") >> "time" >> "=" >> timestamp >> "/>" >>
        lit("<MemoryUsage") >> "bytes" >> "=" >> memory_usage >> "/>" >>
      lit("</TemplateEnd>")
      ) [_val = phx::bind(&create_trace)];

    unescaped_string %= '"' >> *(unescaped_characters | (ascii::print - '"')) >> '"';

    context %= unescaped_string;

    file_location_rule %= *(ascii::print - '|') >> '|' >> int_ >> '|' >> int_;

    memory_usage = '"' >> ulong_long >> '"';
    timestamp = '"' >> double_ >> '"';
  }

  qi::rule<Iterator, templight_trace(), skipper_t> start;
  qi::rule<Iterator, templight_trace(), skipper_t> body;
  qi::rule<Iterator, templight_trace(), skipper_t> template_instantiation_rule;
  qi::rule<Iterator, std::string(), skipper_t> context;
  qi::rule<Iterator, file_location(), skipper_t> file_location_rule;
  qi::rule<Iterator, unsigned long long(), skipper_t> memory_usage;
  qi::rule<Iterator, double(), skipper_t> timestamp;
  qi::rule<Iterator, templight_trace(), skipper_t> template_begin;
  qi::rule<Iterator, templight_trace(), skipper_t> template_end;
  qi::rule<Iterator, std::string()> unescaped_string;

	qi::symbols<char, char> unescaped_characters;
  qi::symbols<char, instantiation_kind> instantiation_kinds;
};

templight_trace templight_trace::create_from_xml(const std::string& file) {

  templight_trace trace;

  std::string file_content;
  std::ifstream in(file.c_str());
  //TODO error handling, not very efficient
  for (std::string line; std::getline(in, line); ) {
    file_content += line + '\n';
  }

  std::cout << "Content:\n" <<
    file_content << std::endl;

  std::string::const_iterator begin = file_content.begin();
  std::string::const_iterator end = file_content.end();

  templight_grammar<std::string::const_iterator> grammar;

  bool success = qi::phrase_parse(
      begin, end, grammar,
      skipper_t(),
      trace);
  if (success && begin == end) {
    return trace;
  } else {
    std::cout << "Failed to parse:\n" <<
      std::string(begin, end) << std::endl;
  }
  return trace;
}

}

