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
  complex<double> resultI(0,0);
  bool complexed = false;
  for( auto & a :args){
    if(a.isHeadNumber()){//check for real
      result += a.head().asNumber();      
    }
	else if (a.isHeadComplex())//check for complex
	{
		resultI += a.head().asComplex();
		complexed = true;
	}
    else{
      throw SemanticError("Error in call to add, argument not a number");
    }
  }

  if (complexed == true)//choose format
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
    if(a.isHeadNumber()){//check for real
      result *= a.head().asNumber();      
    }
	else if (a.isHeadComplex())//check for complex
	{
		resultI *= a.head().asComplex();
		complexed = true;
	}
    else{
      throw SemanticError("Error in call to mul, argument not a number");
    }
  }
  if (complexed == true)//check for format
  {
	  resultI = resultI * result;
	  return Expression(resultI);
  }
  return Expression(result);
};

Expression subneg(const std::vector<Expression> & args){

  double result = 0;
  complex<double> resultI(0,0);
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
    if(args[0].isHeadNumber()){//added number
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
	if (args[1].isHeadNumber()) {//subtracting number
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

	double result = 1;
	complex<double> resultI(1, 0);
	bool complexed = false;

  if(nargs_equal(args,2)){
	  if (args[0].isHeadNumber()) {//base number
		  result = args[0].head().asNumber();
	  }
	  else if (args[0].isHeadComplex())
	  {
		  resultI = args[0].head().asComplex();
		  complexed = true;
	  }
	  else {
		  throw SemanticError("Error in call to subtraction: invalid argument.");
	  }
	  if (args[1].isHeadNumber()) {//divided by
		  result /= args[1].head().asNumber();
	  }
	  else if (args[1].isHeadComplex())
	  {
		  resultI /= args[1].head().asComplex();
		  complexed = true;
	  }
	  else {
		  throw SemanticError("Error in call to subtraction: invalid argument.");
	  }
  }
  else{
    throw SemanticError("Error in call to division: invalid number of arguments.");
  }
  if (complexed == true)
  {
	  resultI = resultI * result;
	  return Expression(resultI);
  }
  return Expression(result);
};

const double PI = std::atan2(0, -1);
const double EXP = std::exp(1);//e constant
const std::complex<double> Im = 1i;//I constant



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
	complex<double> resultI(0, 0);
	bool complexed = false;
	if (nargs_equal(args, 1))//check for single input
	{
		if (args[0].isHeadNumber()) {
			if (args[0].head().asNumber() < 0)//convert sqrt negative number to sqrt real number multiplied by i 
			{
				result = args[0].head().asNumber() * -1;
				resultI = sqrt(result) * Im;
				complexed = true;
			}
			else//no complex
			{
				result = sqrt(args[0].head().asNumber());
			}
		}
		else if (args[0].isHeadComplex())//complex
		{
			resultI = sqrt(args[0].head().asComplex());
			complexed = true;
		}
		else {

			throw SemanticError("Error in call to sqrt, argument not a number");
		}
	}
	else
	{
		throw SemanticError("Error in call to sqrt: invalid number of arguments.");
	}
	if (complexed == true)
	{
		return Expression(resultI);
	}
	return Expression(result);
};

Expression Power(const std::vector<Expression> & args) {

	double result = 0;
	complex<double> resultI(0, 0);
	bool complexed = false;
	if (nargs_equal(args, 2))
	{
		if ((args[0].isHeadNumber() || args[0].isHeadComplex()) && (args[1].isHeadNumber() || args[1].isHeadComplex())) {//if of type real or complex

			if (args[0].isHeadNumber() && args[1].isHeadNumber() && (args[0].head().asNumber() >= 0 || args[1].head().asNumber() >= 1 || args[1].head().asNumber() <= -1)) //|| (1.0/ args[1].head().asNumber()) % 2.0 == 1.0))//checks both numbers are real, or if both negative number and even fraction power exists, (^ (- 1) (/ 1 2)) is essentially sqrt(-1)
			{
				result = pow(args[0].head().asNumber(), args[1].head().asNumber());
			}
			else//all other cases require complex format
			{
				complexed = true;
				if (args[0].isHeadNumber())//base number
				{
					resultI = resultI + args[0].head().asNumber();
				}
				else//power of number
				{
					resultI = resultI + args[0].head().asComplex();
				}
				if (args[1].isHeadNumber())
				{
					resultI = pow(resultI, args[1].head().asNumber());
				}
				else
				{
					resultI = pow(resultI, args[1].head().asComplex());
				}

			}
		}
		else {
			throw SemanticError("Error in call to pow, argument not a number");
		}
	}
	else
	{
		throw SemanticError("Error in call to pow: invalid number of arguments.");
	}
	if (complexed == true)//chooses format
	{
		return Expression(resultI);
	}
	return Expression(result);
};

Expression NLog(const std::vector<Expression> & args) {
	double result = 0;
	if (nargs_equal(args, 1))
	{
		if (args[0].isHeadNumber()) {
			if (args[0].head().asNumber() <= 0)//checks if valid input
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

Expression REAL(const std::vector<Expression> & args) {
	double result = 0;
	if (nargs_equal(args, 1))
	{
		if (args[0].isHeadComplex()) {//checks for complex
			result = real(args[0].head().asComplex());
		}
		else {
			throw SemanticError("Error in call to real, argument not a complex number");
		}
	}
	else
	{
		throw SemanticError("Error in call to real: invalid number of arguments.");
	}
	return Expression(result);
};

Expression IMAG(const std::vector<Expression> & args) {
	double result = 0;
	if (nargs_equal(args, 1))
	{
		if (args[0].isHeadComplex()) {//checks for complex
			result = imag(args[0].head().asComplex());
		}
		else {
			throw SemanticError("Error in call to imag, argument not a complex number");
		}
	}
	else
	{
		throw SemanticError("Error in call to imag: invalid number of arguments.");
	}
	return Expression(result);
};

Expression MAG(const std::vector<Expression> & args) {
	double result = 0;
	if (nargs_equal(args, 1))
	{
		if (args[0].isHeadComplex()) {//checks for complex
			result = sqrt(pow(real(args[0].head().asComplex()), 2) + pow(imag(args[0].head().asComplex()), 2));
		}
		else {
			throw SemanticError("Error in call to mag, argument not a complex number");
		}
	}
	else
	{
		throw SemanticError("Error in call to mag: invalid number of arguments.");
	}
	return Expression(result);
};

Expression ARG(const std::vector<Expression> & args) {
	double result = 0;
	if (nargs_equal(args, 1))
	{
		if (args[0].isHeadComplex()) {//checks for complex
			result = atan(imag(args[0].head().asComplex()) / real(args[0].head().asComplex()));
		}
		else {
			throw SemanticError("Error in call to arg, argument not a complex number");
		}
	}
	else
	{
		throw SemanticError("Error in call to arg: invalid number of arguments.");
	}
	return Expression(result);
};

Expression CONJ(const std::vector<Expression> & args) {
	complex<double> result = 0;
	if (nargs_equal(args, 1))
	{
		if (args[0].isHeadComplex()) {//checks for complex
			result = args[0].head().asComplex() - (imag(args[0].head().asComplex()) * 2 * Im);
		}
		else {
			throw SemanticError("Error in call to conj, argument not a complex number");
		}
	}
	else
	{
		throw SemanticError("Error in call to conj: invalid number of arguments.");
	}
	return Expression(result);
};

Expression LIST(const std::vector<Expression> & args) {

	Expression list;
	list.markList();
	for (auto & a : args) {
		list.append(a);
	}
	return list;

};

Expression FirstLIST(const std::vector<Expression> & args) {

	if (nargs_equal(args, 1))
	{
		if (args[0].isHeadList()) {//checks for complex
			return args[0].tailVector()[0];
		}
		else {
			throw SemanticError("Error in call to first, argument not a list");
		}
	}
	else
	{
		throw SemanticError("Error in call to first: invalid number of arguments.");
	}


};

Expression RestLIST(const std::vector<Expression> & args) {
	Expression list;
	list.markList();
	if (nargs_equal(args, 1))
	{
		if (args[0].isHeadList()) {

			for (int i = 1; i < args[0].tailVector().size(); i++) {
				list.append(args[0].tailVector()[i]);
			}
			return list;
		}
		else {
			throw SemanticError("Error in call to rest, argument not a list");
		}
	}
	else
	{
		throw SemanticError("Error in call to rest: invalid number of arguments.");
	}


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

  //real part of complex
  envmap.emplace("real", EnvResult(ProcedureType, REAL));

  //imag part of complex
  envmap.emplace("imag", EnvResult(ProcedureType, IMAG));

  //magnitude of complex
  envmap.emplace("mag", EnvResult(ProcedureType, MAG));

  //angle of complex
  envmap.emplace("arg", EnvResult(ProcedureType, ARG));

  //conjugate of complex
  envmap.emplace("conj", EnvResult(ProcedureType, CONJ));

  //creates list
  envmap.emplace("list", EnvResult(ProcedureType, LIST));

  //first element in list
  envmap.emplace("first", EnvResult(ProcedureType, FirstLIST));

  //second and beyond elements in list
  envmap.emplace("rest", EnvResult(ProcedureType, RestLIST));
}
