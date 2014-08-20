#ifndef MINDENT_SYNTAX_NODE_HPP
#define MINDENT_SYNTAX_NODE_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <map>
#include <cassert>

/**
 * @brief Entities of the mIndent library
 */
namespace mindent
{
  template <class TokenType>
  class syntax_node_list;

  /**
   * @brief Constants and values of the angle-bracket expressions
   *
   * Instances of this type represent types and constant values in the
   * ange-bracket expression. In case of nested types the containing and the
   * nested types are represented by different syntax_node instances.
   *
   * @tparam TokenType the Boost.Wave type used for representeing tokens.
   */
  template <class TokenType>
  class syntax_node
  {
  private:
    typedef
      std::vector<std::pair<syntax_node_list<TokenType>, TokenType> >
      args_t;

    typedef std::vector<TokenType> name_t;
  public:
    /**
     * @brief Create an empty, non-template syntax node
     */
    syntax_node() :
      _name(),
      _template(false),
      _template_begin(token_traits<TokenType>::empty_token()),
      _template_args(),
      _template_end(token_traits<TokenType>::empty_token())
    {}

    /**
     * @brief Create a non-template syntax node
     *
     * @tparam NameIt the type of the input iterator for tokens representing
     *                the name or value of the node.
     * @param name_begin_ the beginning of the token sequence representing the
     *                    name or the value of the node.
     * @param name_end_ the end of the token sequence representing the name or
     *                  the value of the node.
     */
    template <class NameIt>
    syntax_node(NameIt name_begin_, NameIt name_end_) :
      _name(name_begin_, name_end_),
      _template(false),
      _template_begin(token_traits<TokenType>::empty_token()),
      _template_args(),
      _template_end(token_traits<TokenType>::empty_token())
    {}

    /**
     * @brief Create a template syntax node
     *
     * @tparam NameIt the type of the input iterator for tokens representing
     *                the name of the type.
     * @tparam ArgIt the type of the input iterator for syntax_node objects
     *               representing the arguments of the type.
     * @param name_begin_ the beginning of the token sequence representing the
     *                    name of the type.
     * @param name_end_ the end of the token sequence representing the name of
     *                  the type.
     * @param template_begin_ The < token of the template
     * @param name_begin_ the beginning of the syntax_node sequence representing
     *                    the arguments of the type.
     * @param name_end_ the end of the token sequence representing the arguments
     *                  of the type.
     * @param template_end_ The > token of the template
     */
    template <class NameIt, class ArgIt>
    syntax_node(
      NameIt name_begin_,
      NameIt name_end_,
      const TokenType& template_begin_,
      ArgIt args_begin_,
      ArgIt args_end_,
      const TokenType& template_end_
    ) :
      _name(name_begin_, name_end_),
      _template(true),
      _template_begin(template_begin_),
      _template_args(args_begin_, args_end_),
      _template_end(template_end_)
    {}

    /// Random access input iterator for the template arguments
    typedef typename args_t::const_iterator arg_iterator;

    /// Random access input iterator for the tokens of the name or value
    typedef typename name_t::const_iterator name_iterator;

    /// The beginning of the token sequence representing the name or the value
    name_iterator name_begin() const
    {
      return _name.begin();
    }

    /// The end of the token sequence representing the name or the value
    name_iterator name_end() const
    {
      return _name.end();
    }
    
    /// Returns true when the node represents a template class, false otherwise.
    bool is_template() const
    {
      return _template;
    }
    
    /**
     * The < token of the template type.
     *
     * @pre The node has to represent a template class
     */
    const TokenType& template_begin() const
    {
      assert(is_template());
      return _template_begin;
    }

    /**
     * The beginning of the syntax_node sequence representing the template
     * arguments.
     *
     * @pre The node has to represent a template class
     */
    arg_iterator begin_args() const
    {
      assert(is_template());
      return _template_args.begin();
    }

    /**
     * The end of the syntax_node sequence representing the template arguments.
     *
     * @pre The node has to represent a template class
     */
    arg_iterator end_args() const
    {
      assert(is_template());
      return _template_args.end();
    }

    /**
     * The > token of the template type.
     *
     * @pre The node has to represent a template class
     */
    const TokenType& template_end() const
    {
      assert(is_template());
      return _template_end;
    }
  private:
    name_t _name;
    bool _template;
    TokenType _template_begin;
    args_t _template_args;
    TokenType _template_end;
  };
}

#endif

