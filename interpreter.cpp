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

void Interpreter::disable() {
	std::lock_guard<std::mutex> lock(the_mutex);
	env.disablemark();
}

void Interpreter::enable()
{
	std::lock_guard<std::mutex> lock(the_mutex);
	env.enablemark();
}

void Interpreter::reseter()
{
	env.reset();
}
