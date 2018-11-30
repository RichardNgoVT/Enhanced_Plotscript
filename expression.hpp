/*! \file expression.hpp
Defines the Expression type and assiciated functions.
 */
#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <vector>

#include "token.hpp"
#include "atom.hpp"

//bool exitkey = false;
// forward declare Environment
class Environment;

/*! \class Expression
\brief An expression is a tree of Atoms.

An expression is an atom called the head followed by a (possibly empty) 
list of expressions called the tail.
 */
class Expression {
public:

  typedef std::vector<Expression>::const_iterator ConstIteratorType;

  /// Default construct and Expression, whose type in NoneType
  Expression();

  /*! Construct an Expression with given Atom as head an empty tail
    \param atom the atom to make the head
  */
  Expression(const Atom & a);

  /// deep-copy construct an expression (recursive)
  Expression(const Expression & a);

  /// deep-copy assign an expression  (recursive)
  Expression & operator=(const Expression & a);

  /// return a reference to the head Atom
  Atom & head();

  /// return a reference to the head Atom
  std::vector<Expression> & tailVector();

  /// return a const-reference to the head Atom
  const Atom & head() const;

  /// return a const-reference to the head Atom
  const std::vector<Expression> & tailVector() const;

  /// append Atom to tail of the expression
  void append(const Atom & a);

  /// appends Expression to the end of atom
  void append(const Expression & a);

  /// return a pointer to the last expression in the tail, or nullptr
  Expression * tail();

  /// return a const-iterator to the beginning of tail
  ConstIteratorType tailConstBegin() const noexcept;

  /// return a const-iterator to the tail end
  ConstIteratorType tailConstEnd() const noexcept;

  /// convienience member to determine if head atom is a number
  bool isHeadNumber() const noexcept;

  /// convienience member to determine if head atom is a complex number
  bool isHeadComplex() const noexcept;

  /// convienience member to determine if head atom is a symbol
  bool isHeadSymbol() const noexcept;

  /// checks for string
  bool isHeadPString() const noexcept;

  /// checks for list
  bool isHeadList() const noexcept;

  /// checks for procedure type
  bool isHeadProcedure() const noexcept;

  /// checks for property type
  bool isHeadProperty() const noexcept;

  /// checks for none type
  bool isHeadNone() const noexcept;

  bool isHeadError() const noexcept;

  /// Evaluate expression using a post-order traversal (recursive)
  Expression eval(Environment & env);

  /// equality comparison for two expressions (recursive)
  bool operator==(const Expression & exp) const noexcept;

  void markList();
  
  void markProcedure();

  void markProperty();

  void markError();

  //void exitmark();

  Expression exitnow();

  //used to cout expressions (for testing)
  //void HexpressVisual(Atom headman, std::vector<Expression> tailman, Expression express, int layer);

 // void HheadOutputer(Atom headman);

private:

  // the head of the expression
  Atom m_head;

  // the tail list is expressed as a vector for access efficiency
  // and cache coherence, at the cost of wasted memory.
  std::vector<Expression> m_tail;

  // convenience typedef
  typedef std::vector<Expression>::iterator IteratorType;
  
  // internal helper methods
  Expression handle_lookup(const Atom & head, const Environment & env);
  Expression handle_define(Environment & env);
  Expression handle_begin(Environment & env);
  Expression handle_lambda(Environment & env);
  Expression handle_lambdaProcedure(Environment & env);
  Expression handle_apply(Environment & env);
  Expression handle_map(Environment & env);
  Expression handle_getProperty(Environment & env);
  Expression handle_setProperty(Environment & env);
  Expression handle_discretePlot(Environment & env);
  Expression handle_continuousPlot(Environment & env);



  const double N = 20.0;
  const double A = 3.0;
  const double B = 3.0;
  const double C = 2.0;
  const double D = 2.0;
  const double P = 0.5;

  bool exitkey = false;
};

/// Render expression to output stream
std::ostream & operator<<(std::ostream & out, const Expression & exp);

/// inequality comparison for two expressions (recursive)
bool operator!=(const Expression & left, const Expression & right) noexcept;
  
#endif
