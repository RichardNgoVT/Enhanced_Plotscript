#include "environment.hpp"

#include <cassert>
#include <cmath>

#include "environment.hpp"
#include "semantic_error.hpp"

//independent
#include <complex>
#include <string>       // std::string

using namespace std;



/*********************************************************************** 
Helper Functions
**********************************************************************/

// predicate, the number of args is nargs
bool nargs_equal(const std::vector<Expression> & args, unsigned nargs){
  return args.size() == nargs;
}

/*********************************************************************** 
Each of the functions below have the signature that corresponds to the
typedef'd Procedure function pointer.
**********************************************************************/

// the default procedure always returns an expresison of type None
Expression default_proc(const std::vector<Expression> & args){
  args.size(); // make compiler happy we used this parameter
  return Expression();
};

Expression add(const std::vector<Expression> & args){

  // check all aruments are numbers, while adding
  double result = 0;
  complex<double> resultI;
  bool complexed = false;
  for( auto & a :args){
    if(a.isHeadNumber()){
      result += a.head().asNumber();      
    }
	else if (a.isHeadComplex())
	{
		resultI += a.head().asComplex();
		complexed = true;
	}
    else{
      throw SemanticError("Error in call to add, argument not a number");
    }
  }

  if (complexed == true)
  {
	  resultI = resultI + result;
	  return Expression(resultI);
  }

  return Expression(result);
};

Expression mul(const std::vector<Expression> & args){
 
  // check all aruments are numbers, while multiplying
	double result = 1;
	complex<double> resultI(1,0);
	bool complexed = false;
  for( auto & a :args){
    if(a.isHeadNumber()){
      result *= a.head().asNumber();      
    }
	else if (a.isHeadComplex())
	{
		resultI *= a.head().asComplex();
		complexed = true;
	}
    else{
      throw SemanticError("Error in call to mul, argument not a number");
    }
  }
  if (complexed == true)
  {
	  resultI = resultI * result;
	  return Expression(resultI);
  }
  return Expression(result);
};

Expression subneg(const std::vector<Expression> & args){

  double result = 0;
  complex<double> resultI;
  bool complexed = false;
  // preconditions

  if(nargs_equal(args,1)){
    if(args[0].isHeadNumber()){
      result = -args[0].head().asNumber();
    }
	else if (args[0].isHeadComplex())
	{
		resultI = -1.0 * args[0].head().asComplex();
		complexed = true;
	}
    else{
      throw SemanticError("Error in call to negate: invalid argument.");
    }
  }
  else if(nargs_equal(args,2)){
    if(args[0].isHeadNumber()){
      result = args[0].head().asNumber();
    }
	else if (args[0].isHeadComplex())
	{
		resultI =args[0].head().asComplex();
		complexed = true;
	}
    else{      
      throw SemanticError("Error in call to subtraction: invalid argument.");
    }
	if (args[1].isHeadNumber()) {
		result -= args[1].head().asNumber();
	}
	else if (args[1].isHeadComplex())
	{
		resultI -= args[1].head().asComplex();
		complexed = true;
	}
	else {
		throw SemanticError("Error in call to subtraction: invalid argument.");
	}
  }
  else{
    throw SemanticError("Error in call to subtraction or negation: invalid number of arguments.");
  }
  if (complexed == true)
  {
	  resultI = resultI + result;
	  return Expression(resultI);
  }
  return Expression(result);
};

Expression div(const std::vector<Expression> & args){

  double result = 0;  

  if(nargs_equal(args,2)){
    if( (args[0].isHeadNumber()) && (args[1].isHeadNumber()) ){
      result = args[0].head().asNumber() / args[1].head().asNumber();
    }
    else{      
      throw SemanticError("Error in call to division: invalid argument.");
    }
  }
  else{
    throw SemanticError("Error in call to division: invalid number of arguments.");
  }
  return Expression(result);
};

const double PI = std::atan2(0, -1);
const double EXP = std::exp(1);
const std::complex<double> Im = 1i;



Environment::Environment(){

  reset();
}

bool Environment::is_known(const Atom & sym) const{
  if(!sym.isSymbol()) return false;
  
  return envmap.find(sym.asSymbol()) != envmap.end();
}

bool Environment::is_exp(const Atom & sym) const{
  if(!sym.isSymbol()) return false;
  
  auto result = envmap.find(sym.asSymbol());
  return (result != envmap.end()) && (result->second.type == ExpressionType);
}

Expression Environment::get_exp(const Atom & sym) const{

  Expression exp;
  
  if(sym.isSymbol()){
    auto result = envmap.find(sym.asSymbol());
    if((result != envmap.end()) && (result->second.type == ExpressionType)){
      exp = result->second.exp;
    }
  }

  return exp;
}



void Environment::add_exp(const Atom & sym, const Expression & exp){

  if(!sym.isSymbol()){
    throw SemanticError("Attempt to add non-symbol to environment");
  }
    
  // error if overwriting symbol map
  if(envmap.find(sym.asSymbol()) != envmap.end()){
    throw SemanticError("Attempt to overwrite symbol in environemnt");
  }

  envmap.emplace(sym.asSymbol(), EnvResult(ExpressionType, exp)); 
}

bool Environment::is_proc(const Atom & sym) const{
  if(!sym.isSymbol()) return false;
  
  auto result = envmap.find(sym.asSymbol());
  return (result != envmap.end()) && (result->second.type == ProcedureType);
}

Procedure Environment::get_proc(const Atom & sym) const{

  //Procedure proc = default_proc;

  if(sym.isSymbol()){
    auto result = envmap.find(sym.asSymbol());
    if((result != envmap.end()) && (result->second.type == ProcedureType)){
      return result->second.proc;
    }
  }

  return default_proc;
}

Expression SquareRoot(const std::vector<Expression> & args) {

	double result = 0;
	if (nargs_equal(args, 1))
	{
		if (args[0].isHeadNumber()) {
			if (args[0].head().asNumber() < 0)
			{
				throw SemanticError("Error in call to sqrt, argument can't be less than 0");
			}
			result = sqrt(args[0].head().asNumber());
		}
		else {

			throw SemanticError("Error in call to sqrt, argument not a number");
		}
	}
	else
	{
		throw SemanticError("Error in call to sqrt: invalid number of arguments.");
	}

	return Expression(result);
};

Expression Power(const std::vector<Expression> & args) {

	double result = 0;
	if (nargs_equal(args, 2))
	{
		if (args[0].isHeadNumber() && args[1].isHeadNumber()) {

			result = pow(args[0].head().asNumber(), args[1].head().asNumber());
		}
		else {

			throw SemanticError("Error in call to pow, argument not a number");
		}
	}
	else
	{
		throw SemanticError("Error in call to pow: invalid number of arguments.");
	}

	return Expression(result);
};

Expression NLog(const std::vector<Expression> & args) {
	double result = 0;
	if (nargs_equal(args, 1))
	{
		if (args[0].isHeadNumber()) {
			if (args[0].head().asNumber() <= 0)
			{
				throw SemanticError("Error in call to ln, argument can't be less than or equal to 0");
			}
			result = log(args[0].head().asNumber());
		}
		else {

			throw SemanticError("Error in call to ln, argument not a number");
		}
	}
	else
	{
		throw SemanticError("Error in call to ln: invalid number of arguments.");
	}

	return Expression(result);
};

Expression SIN(const std::vector<Expression> & args) {
	double result = 0;
	if (nargs_equal(args, 1))
	{
		if (args[0].isHeadNumber()) {
			result = sin(args[0].head().asNumber());
		}
		else {
			throw SemanticError("Error in call to sin, argument not a number");
		}
	}
	else
	{
		throw SemanticError("Error in call to sin: invalid number of arguments.");
	}
	return Expression(result);
};

Expression COS(const std::vector<Expression> & args) {
	double result = 0;
	if (nargs_equal(args, 1))
	{
		if (args[0].isHeadNumber()) {
			result = cos(args[0].head().asNumber());
		}
		else {
			throw SemanticError("Error in call to cos, argument not a number");
		}
	}
	else
	{
		throw SemanticError("Error in call to cos: invalid number of arguments.");
	}
	return Expression(result);
};

Expression TAN(const std::vector<Expression> & args) {
	double result = 0;
	if (nargs_equal(args, 1))
	{
		if (args[0].isHeadNumber()) {
			result = tan(args[0].head().asNumber());
		}
		else {
			throw SemanticError("Error in call to tan, argument not a number");
		}
	}
	else
	{
		throw SemanticError("Error in call to tan: invalid number of arguments.");
	}
	return Expression(result);
};

/*
Reset the environment to the default state. First remove all entries and
then re-add the default ones.
 */
void Environment::reset(){

  envmap.clear();
  
  // Built-In value of pi
  envmap.emplace("pi", EnvResult(ExpressionType, Expression(PI)));

  //Exponential Value
  envmap.emplace("e", EnvResult(ExpressionType, Expression(EXP)));

  envmap.emplace("I", EnvResult(ExpressionType, Expression(Im)));

  // Procedure: add;
  envmap.emplace("+", EnvResult(ProcedureType, add)); 

  // Procedure: subneg;
  envmap.emplace("-", EnvResult(ProcedureType, subneg)); 

  // Procedure: mul;
  envmap.emplace("*", EnvResult(ProcedureType, mul)); 

  // Procedure: div;
  envmap.emplace("/", EnvResult(ProcedureType, div)); 

  //square root
  envmap.emplace("sqrt", EnvResult(ProcedureType, SquareRoot));

  //exponent ^
  envmap.emplace("^", EnvResult(ProcedureType, Power));

  //Natural Log
  envmap.emplace("ln", EnvResult(ProcedureType, NLog));

  //SIN
  envmap.emplace("sin", EnvResult(ProcedureType, SIN));

  //COS
  envmap.emplace("cos", EnvResult(ProcedureType, COS));

  //TAN
  envmap.emplace("tan", EnvResult(ProcedureType, TAN));
}
