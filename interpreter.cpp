#include "interpreter.hpp"

// system includes
#include <stdexcept>

// module includes
#include "token.hpp"
#include "parse.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "semantic_error.hpp"

bool Interpreter::parseStream(std::istream & expression) noexcept{

  TokenSequenceType tokens = tokenize(expression);

  ast = parse(tokens);

  return (ast != Expression());//false if empty expression is returned
};
				     

Expression Interpreter::evaluate(){

  return ast.eval(env);
}
/*
void Interpreter::exiter() {

	return ast.exitmark();
}
*/