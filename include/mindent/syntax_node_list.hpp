#ifndef MINDENT_SYNTAX_NODE_LIST_HPP
#define MINDENT_SYNTAX_NODE_LIST_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <map>

namespace mindent
{
  template <class TokenType>
  class syntax_node;
  
  /**
   * @brief Nested types in an angle-bracket expression
   *
   * Instances of this type represent nested types in an angle bracket
   * expression. For example: std::vector<int>::iterator. The containing and the
   * inner types are represented by different syntax_node objects. The type
   * std::vector<int>::iterator is represented by a two element list: one
   * syntax_node object for std::vector<int> and one for iterator.
   *
   * @tparam TokenType the Boost.Wave type used for representeing tokens.
   */
  template <class TokenType>
  class syntax_node_list
  {
  private:
    typedef std::vector<std::pair<syntax_node<TokenType>, TokenType> > nodes_t;
  public:
    /// Creates an empty syntax_node list
    syntax_node_list() {}

    /**
     * @brief Creates a syntax node list from the sequence specified by the
     *        iterators.
     *
     * @tparam Input syntax_node iterator type.
     * @param begin_ the beginning of the syntax_node sequence to copy
     * @param begin_ the end of the syntax_node sequence to copy
     */
    template <class It>
    syntax_node_list(It begin_, It end_) : _nodes(begin_, end_) {}

    /// Random access input iterator for the nodes
    typedef typename nodes_t::const_iterator iterator;

    /// Random access input iterator for the nodes
    typedef typename nodes_t::const_iterator const_iterator;

    /// The beginning of the nodes
    iterator begin() const { return _nodes.begin(); }

    /// The end of the nodes
    iterator end() const { return _nodes.end(); }
  private:
    nodes_t _nodes;
  };
}

#endif

