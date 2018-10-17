#include "expression.hpp"

#include <sstream>
#include <list>

#include "environment.hpp"
#include "semantic_error.hpp"


Expression::Expression(){}

Expression::Expression(const Atom & a){

  m_head = a;
}

// recursive copy
Expression::Expression(const Expression & a){

  m_head = a.m_head;
  for(auto e : a.m_tail){
    m_tail.push_back(e);
  }
}

Expression & Expression::operator=(const Expression & a){

  // prevent self-assignment
  if(this != &a){
    m_head = a.m_head;
    m_tail.clear();
    for(auto e : a.m_tail){
      m_tail.push_back(e);
    } 
  }
  
  return *this;
}


Atom & Expression::head(){
  return m_head;
}

std::vector<Expression> & Expression::tailVector() {
	return m_tail;
}

const Atom & Expression::head() const{
  return m_head;
}

const  std::vector<Expression> & Expression::tailVector() const
{
	return m_tail;
}

bool Expression::isHeadNumber() const noexcept{
  return m_head.isNumber();
}

bool Expression::isHeadComplex() const noexcept {
	return m_head.isComplex();
}

bool Expression::isHeadSymbol() const noexcept{
  return m_head.isSymbol();
}  

bool Expression::isHeadPString() const noexcept {
	return m_head.isPString();
}

bool Expression::isHeadList() const noexcept {
	return m_head.isList();
}

bool Expression::isHeadProcedure() const noexcept {
	return m_head.isProcedure();
}

bool Expression::isHeadProperty() const noexcept
{
	return m_head.isProperty();
}

void Expression::append(const Atom & a){
  m_tail.emplace_back(a);
}

void Expression::append(const Expression & a) {
	m_tail.emplace_back(a);
}


Expression * Expression::tail(){
  Expression * ptr = nullptr;
  
  if(m_tail.size() > 0){
    ptr = &m_tail.back();
  }

  return ptr;
}

Expression::ConstIteratorType Expression::tailConstBegin() const noexcept{
  return m_tail.cbegin();
}

Expression::ConstIteratorType Expression::tailConstEnd() const noexcept{
  return m_tail.cend();
}

Expression apply(const Atom & op, const std::vector<Expression> & args, const Environment & env){

  // head must be a symbol
  if(!op.isSymbol()){
    throw SemanticError("Error during evaluation: procedure name not symbol");
  }
  
  // must map to a proc
  if(!env.is_proc(op)){
    throw SemanticError("Error during evaluation: symbol does not name a procedure");
  }
  
  // map from symbol to proc
  Procedure proc = env.get_proc(op);
  
  // call proc with args
  return proc(args);
}

Expression Expression::handle_lookup(const Atom & head, const Environment & env){
    if(head.isSymbol()){ // if symbol is in env return value
      if(env.is_exp(head)){
	return env.get_exp(head);
      }
      else{
		  cout << head.asSymbol() << endl;
	throw SemanticError("Error during evaluation: unknown symbol");
      }
    }
    else if(head.isNumber() || head.isPString()){// just (1) or ("one")
      return Expression(head);
    }
    else{
      throw SemanticError("Error during evaluation: Invalid type in terminal expression");
    }
}

Expression Expression::handle_begin(Environment & env){
  
  if(m_tail.size() == 0){
    throw SemanticError("Error during evaluation: zero arguments to begin");
  }

  // evaluate each arg from tail, return the last
  Expression result;
  for(Expression::IteratorType it = m_tail.begin(); it != m_tail.end(); ++it){
    result = it->eval(env);
  }
  
  return result;
}


Expression Expression::handle_define(Environment & env){

  // tail must have size 3 or error
  if(m_tail.size() != 2){
    throw SemanticError("Error during evaluation: invalid number of arguments to define");
  }
  
  // tail[0] must be symbol
  if(!m_tail[0].isHeadSymbol()){
    throw SemanticError("Error during evaluation: first argument to define not symbol");
  }

  // but tail[0] must not be a special-form or procedure
  std::string s = m_tail[0].head().asSymbol();
  if((s == "define") || (s == "begin")){
    throw SemanticError("Error during evaluation: attempt to redefine a special-form");
  }
  
  if(env.is_proc(m_head)){
    throw SemanticError("Error during evaluation: attempt to redefine a built-in procedure");
  }
	
  // eval tail[1]
  Expression result = m_tail[1].eval(env);

  if(env.is_exp(m_head)){
    throw SemanticError("Error during evaluation: attempt to redefine a previously defined symbol");
  }
    
  //and add to env
  env.add_exp(m_tail[0].head(), result);
  
  return result;
}

Expression Expression::handle_lambda(Environment & env) {

	// tail must have size 3 or error
	if (m_tail.size() != 2) {
		throw SemanticError("Error during evaluation: invalid number of arguments to define");
	}


	// but tail[0] must not be a special-form or procedure
	std::string s = m_tail[0].head().asSymbol();
	if ((s == "define") || (s == "begin")) {
		throw SemanticError("Error during evaluation: attempt to use special forms as arguments");
	}



	// eval tail[1]
	//Expression result = m_tail[1].eval(env);
	Expression result;
	std::vector<Expression> Variables;


	Variables.emplace_back(m_tail[0].head());
	for (int i = 0; i < m_tail[0].tailVector().size(); i++) {
		if (m_tail[0].tailVector()[i].isHeadSymbol())
		{
			Variables.emplace_back(m_tail[0].tailVector()[i]);
		}
		else
		{
			throw SemanticError("Error during evaluation: Arguments are not symbols");
		}
	}

	// map from symbol to proc
	Procedure proc = env.get_proc(Atom("list"));




	result.markProcedure();

	// call proc with Variables
	result.append(proc(Variables));
	result.append(m_tail[1]);

	return result;
}


Expression Expression::handle_lambdaProcedure(Environment & env) {

	std::vector<Expression> varStorage;
	Expression result;
	Expression lambda = handle_lookup(m_head, env);

	//Expression result;
	if (m_tail.size() > 0)
	{
		if (lambda.isHeadProcedure()) {
			if (lambda.tailVector()[0].tailVector().size() == m_tail.size())
			{
				//store old variables
				for (int i = 0; i < lambda.tailVector()[0].tailVector().size(); i++)
				{
					varStorage.emplace_back(env.get_exp(lambda.tailVector()[0].tailVector()[i].head()));
				}
				//delete old variables
				for (int i = 0; i < lambda.tailVector()[0].tailVector().size(); i++)
				{
					env.delete_exp(lambda.tailVector()[0].tailVector()[i].head());
				}


				//create new variables
				for (int i = 0; i < lambda.tailVector()[0].tailVector().size(); i++)
				{
					env.add_exp(lambda.tailVector()[0].tailVector()[i].head(), m_tail[i]);
				}

				result = lambda.tailVector()[1].eval(env);

				//delete new variables
				for (int i = 0; i < lambda.tailVector()[0].tailVector().size(); i++)
				{
					env.delete_exp(lambda.tailVector()[0].tailVector()[i].head());
				}

				//add back old variables
				for (int i = 0; i < lambda.tailVector()[0].tailVector().size(); i++)
				{
					if (!varStorage[i].head().isNone())
					{
						env.add_exp(lambda.tailVector()[0].tailVector()[i].head(), varStorage[i]);
					}
				}

				return result;

			}
			else
			{
				
				throw SemanticError("Error in call to lambda function, invalid number of arguments inside list for selected procedure");
			}
		}
		else {

			throw SemanticError("Error in call to lambda function, argument types are invalid");
		}
	}
	else
	{
		throw SemanticError("Error in call to lambda function: no arguments");
	}
}



Expression Expression::handle_apply(Environment & env) {
	if (m_tail.size() != 2) {
		throw SemanticError("Error call to apply function: invalid number of arguments to define");
	}

	



/*
	Expression checker = handle_lookup(m_tail[0].head(), env);
	if (!checker.isHeadProcedure())
	{
		throw SemanticError("Error in call to apply function: first argument not a procedure");
	}
	*/
	bool procedurecatch;
	if (m_tail[0].tailVector().empty())
	{
		procedurecatch = false;
	}
	else
	{
		procedurecatch = true;
	}
	m_head = m_tail[0].head();
	Expression list = m_tail[1].eval(env);
	if (!list.isHeadList())
	{
		throw SemanticError("Error in call to apply function: second argument not a list");
	}
	m_tail = list.tailVector();

	if (env.is_proc(m_head))
	{
		if (procedurecatch == true)
		{
			throw SemanticError("Error in call to apply function: first argument not a procedure");
		}
		return apply(m_head, m_tail, env);



	}
	else
	{
		if (env.is_exp(m_head))
		{
			return handle_lambdaProcedure(env);
		}
		else
		{
			throw SemanticError("Error in call to apply function: first argument not a procedure");
		}
	}
}

Expression Expression::handle_map(Environment & env) {
	if (m_tail.size() != 2) {
		throw SemanticError("Error call to map function: invalid number of arguments to define");
	}
	m_head = m_tail[0].head();
	std::vector<Expression> Results;
	Expression list = m_tail[1].eval(env);

	if (!list.isHeadList())
	{
		throw SemanticError("Error in call to map function: second argument not a list");
	}

	Expression resultHold;
	for (int i = 0; i < list.tailVector().size(); i++)
	{
		m_tail.clear();
		

		resultHold = list.tailVector()[i];//.eval(env);
		if (resultHold.isHeadList())
		{
			m_tail = resultHold.tailVector();
		}
		else
		{
			m_tail.emplace_back(resultHold.head());
		}
		if (env.is_proc(m_head))
		{
			Results.emplace_back(apply(m_head, m_tail, env));
		}
		else
		{
			if (env.is_exp(m_head))
			{
				Results.emplace_back(handle_lambdaProcedure(env));
			}
			else
			{
				throw SemanticError("Error in call to map function: first argument not a procedure");
			}
		}
	}

	Procedure proc = env.get_proc(Atom("list"));
	return proc(Results);
}

Expression Expression::handle_setProperty(Environment & env) {
	
	if (m_tail.size() != 3)
	{
		throw SemanticError("Error in call to set-property function: incorrect number of arguments");
	}
	if (!m_tail[0].isHeadPString())
	{
		throw SemanticError("Error in call to set-property function: key is not a string");
	}

	Expression newProp;
	newProp.markProperty();
	Expression propertyEx(m_tail[0]);//key
	propertyEx.append(m_tail[1]);//property
	Expression express(m_tail[2]);//expression

	m_head = m_tail[2].head();
	m_tail = m_tail[2].tailVector();

	Expression holder = eval(env);//calculate expression
	
	if (holder.isHeadProperty())
	{
		holder.append(propertyEx);
		return holder;
	}
	else
	{
		newProp.append(holder);
		newProp.append(propertyEx);
		return newProp;
	}

	
}

/*
Expression Expression::handle_getProperty(Environment & env) {

}
*/
// this is a simple recursive version. the iterative version is more
// difficult with the ast data structure used (no parent pointer).
// this limits the practical depth of our ASTdef
Expression Expression::eval(Environment & env){
  
	if (m_head.isProperty())
	{
		if (m_head.isSymbol() && m_head.asSymbol() == "set-property") {
			return handle_setProperty(env);
		}
		else//uness
		{
			m_head = m_tail[0].head();
			m_tail = m_tail[0].tailVector();
			return eval(env);
		}

	}
	else//unessesary
	{
		if (m_tail.empty()) {
			if (m_head.asSymbol() == "list")//empty list
			{
				return apply(m_head, m_tail, env);
			}
			return handle_lookup(m_head, env);
		}
		// handle begin special-form
		else if (m_head.isSymbol() && m_head.asSymbol() == "begin") {
			return handle_begin(env);
		}
		// handle define special-form
		else if (m_head.isSymbol() && m_head.asSymbol() == "define") {
			return handle_define(env);
		}
		else if (m_head.isSymbol() && m_head.asSymbol() == "lambda") {
			return handle_lambda(env);
		}
		//handle "procedure" apply
		else if (m_head.isSymbol() && m_head.asSymbol() == "apply") {
			return handle_apply(env);
		}
		//handle "procedure" map
		else if (m_head.isSymbol() && m_head.asSymbol() == "map") {
			return handle_map(env);
		}
		//handle "procedure" set-property
		else if (m_head.isSymbol() && m_head.asSymbol() == "set-property") {
			return handle_setProperty(env);
		}
		// else attempt to treat as procedure
		else {
			std::vector<Expression> results;
			for (Expression::IteratorType it = m_tail.begin(); it != m_tail.end(); ++it) {
				results.push_back(it->eval(env));
			}
			if (!results.empty() && m_head.isSymbol() && env.is_exp(m_head) && handle_lookup(m_head, env).isHeadProcedure()) {
				m_tail = results;
				return handle_lambdaProcedure(env);
			}
			return apply(m_head, results, env);
		}
	}
}


std::ostream & operator<<(std::ostream & out, const Expression & exp){
	if (exp.isHeadComplex())//checks if outer parentheses are needed
	{
		out << exp.head();

		for (auto e = exp.tailConstBegin(); e != exp.tailConstEnd(); ++e) {
			out << *e;
		}
	}
	else if (exp.isHeadList() || exp.isHeadProcedure())
	{
		out << "(";
		for (auto e = exp.tailConstBegin(); e != exp.tailConstEnd(); ++e) {
			if (e == exp.tailConstBegin())
			{
				out << *e;
			}
			else
			{
				out << " ";
				out << *e;
			}
		}
		out << ")";

	}
	else if (exp.isHeadSymbol())
	{
		out << "(";
		out << exp.head();
		for (auto e = exp.tailConstBegin(); e != exp.tailConstEnd(); ++e) {
			out << " ";
			out << *e;
		}
		out << ")";

	}
	else if (exp.isHeadProperty())
	{
		out << *exp.tailConstBegin();
	}
	else
	{
		out << "(";
		out << exp.head();

		for (auto e = exp.tailConstBegin(); e != exp.tailConstEnd(); ++e) {
			out << *e;
		}

		out << ")";
	}
  return out;
}

bool Expression::operator==(const Expression & exp) const noexcept{

	Expression propertyEx;
	if (m_head.isProperty())
	{
		propertyEx.m_head = m_tail[0].head();
		propertyEx.m_tail = m_tail[0].tailVector();
		return exp == propertyEx;
	}


  bool result = (m_head == exp.m_head);

  result = result && (m_tail.size() == exp.m_tail.size());

  if(result){
    for(auto lefte = m_tail.begin(), righte = exp.m_tail.begin();
	(lefte != m_tail.end()) && (righte != exp.m_tail.end());
	++lefte, ++righte){
      result = result && (*lefte == *righte);
    }
  }

  return result;
}

void Expression::markList()
{
	m_head.setList();
}

void Expression::markProcedure()
{
	m_head.setProcedure();
}

void Expression::markProperty()
{
	m_head.setProperty();
}

bool operator!=(const Expression & left, const Expression & right) noexcept{

  return !(left == right);
}
