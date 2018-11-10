#include "expression.hpp"

#include <sstream>
#include <list>
#include <iostream>
#include <iomanip>
#include <cmath>
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

bool Expression::isHeadNone() const noexcept
{
	return m_head.isNone();
}

void Expression::append(const Atom & a){
  m_tail.emplace_back(a);
}

void Expression::append(const Expression & a) {
	m_tail.emplace_back(a);
}

void Expression::HexpressVisual(Atom headman, std::vector<Expression> tailman, Expression express, int layer)//ONLY WORKS IN HOST, helper function for troubleshooting
{
	Expression holder;
	if (layer == 0)//start of recursion
	{
		holder.m_head = headman;
		holder.m_tail = tailman;
	}
	else
	{
		holder = express;
	}
	int deeper = layer + 1;
	std::cout << '[' << deeper;
	HheadOutputer(holder.head());
	for (unsigned int i = 0; i < holder.m_tail.size(); i++)
	{
		std::cout << ' ';
		HexpressVisual(Atom(), std::vector<Expression>(), holder.m_tail[i], deeper);
	}
	std::cout << ']';
	if (layer == 0)
	{
		std::cout << std::endl;
	}
}

void Expression::HheadOutputer(Atom headman)//helps with HexpressVisual outputting correct type
{
	if (headman.isComplex())
	{
		std::cout << "(COMPL):(" << headman.asComplex() << ')';
	}
	else if (headman.isList())
	{
		std::cout << "(LIST):()";
	}
	else if (headman.isNone())
	{
		std::cout << "(NONE):()";
	}
	else if (headman.isNumber())
	{
		std::cout << "(NUMB):(" << headman.asNumber() << ')';
	}
	else if (headman.isProcedure())
	{
		std::cout << "(PROC):()";
	}
	else if (headman.isProperty())
	{
		std::cout << "(PROP):()";
	}
	else if (headman.isPString())
	{
		std::cout << "(STRI):(" << headman.asPString() << ')';
	}
	else if (headman.isSymbol())
	{
		std::cout << "(SYMB):(" << headman.asSymbol() << ')';
	}
	else
	{
		std::cout << "(unde):()";
	}
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

	//cout << head.asSymbol() << endl;
    if(head.isSymbol()){ // if symbol is in env return value
      if(env.is_exp(head)){
	return env.get_exp(head);
      }
      else{
		 // cout << "error: " << head.asSymbol() << endl;
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

 // Expression result = m_tail[1].eval(env);//remove
  
  Expression result;
  if (env.is_exp(m_tail[1].head()) && env.get_exp(m_tail[1].head()).isHeadProperty())
  {
	 // cout << "here";
	  result = env.get_exp(m_tail[1].head());//gets unevaluated to presereve properties
	  result.m_tail[0] = m_tail[1].eval(env);
  }
  else
  {
	  result = m_tail[1].eval(env);
  }
  
  /*
  if(env.is_exp(m_head)){
    throw SemanticError("Error during evaluation: attempt to redefine a previously defined symbol");
  }
    */
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
	/*
	std::string s = m_tail[0].head().asSymbol();
	if ((s == "define") || (s == "begin")) {
		throw SemanticError("Error during evaluation: attempt to use special forms as arguments");
	}
	*/


	// eval tail[1]
	//Expression result = m_tail[1].eval(env);
	Expression result;
	std::vector<Expression> Variables;


	Variables.emplace_back(m_tail[0].head());
	for (unsigned int i = 0; i < m_tail[0].tailVector().size(); i++) {
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
	Expression lambda;
	//cout << "herebegin" << endl;
	//cout << m_head.asSymbol() << endl;
	if (handle_lookup(m_head, env).isHeadProperty())//unessesary, don't think lambdas themselves have properties
	{
		//cout << "here why?" << endl;
		lambda = handle_lookup(m_head, env).tailVector()[0];
	}
	else
	{
		//cout << m_head.asSymbol()<< endl;
		lambda = handle_lookup(m_head, env);
	}
	//Expression result;
	if (m_tail.size() > 0)
	{
		if (lambda.isHeadProcedure()) {
			if (lambda.tailVector()[0].tailVector().size() == m_tail.size())
			{
				//store old variables
				for (unsigned int i = 0; i < lambda.tailVector()[0].tailVector().size(); i++)
				{
					varStorage.emplace_back(env.get_exp(lambda.tailVector()[0].tailVector()[i].head()));
				}
				//delete old variables
				for (unsigned int i = 0; i < lambda.tailVector()[0].tailVector().size(); i++)
				{
					env.delete_exp(lambda.tailVector()[0].tailVector()[i].head());
				}


				//create new variables
				for (unsigned int i = 0; i < lambda.tailVector()[0].tailVector().size(); i++)
				{
					//cout << lambda.tailVector()[0].tailVector()[i].head().asSymbol() << endl;
					env.add_exp(lambda.tailVector()[0].tailVector()[i].head(), m_tail[i]);
					//cout << lambda.tailVector()[0].tailVector()[i].head().asSymbol() << endl;
					//HexpressVisual(m_tail[i].head(), m_tail[i].tailVector(), Expression(), 0);
				}
				result = lambda.tailVector()[1].eval(env);
				//delete new variables
				for (unsigned int i = 0; i < lambda.tailVector()[0].tailVector().size(); i++)
				{
					env.delete_exp(lambda.tailVector()[0].tailVector()[i].head());
				}
				//add back old variables
				for (unsigned int i = 0; i < lambda.tailVector()[0].tailVector().size(); i++)
				{
					if (!varStorage[i].head().isNone())
					{
						env.add_exp(lambda.tailVector()[0].tailVector()[i].head(), varStorage[i]);
					}
				}
				//cout << "hereend" << endl;
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
	for (unsigned int i = 0; i < list.tailVector().size(); i++)
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
		//HexpressVisual(m_head, m_tail, Expression(), 0);
		throw SemanticError("Error in call to set-property function: incorrect number of arguments");
	}
	if (!m_tail[0].isHeadPString())
	{
		throw SemanticError("Error in call to set-property function: key is not a string");
	}

	Expression newProp;
	newProp.markProperty();
	Expression propertyEx(m_tail[0]);//key
	Expression express;//expression
	if (env.is_exp(m_tail[2].head()))
	{
		//express = env.get_exp(m_tail[2].head());//gets unevaluated property
		express = m_tail[2].eval(env);
	}
	else
	{
		express = m_tail[2].eval(env);
	}
	Expression holder2 = m_tail[1];
	m_head = holder2.head();
	m_tail = holder2.tailVector();

	Expression holder = eval(env);//calculate value (just do .eval(env) next time)
	propertyEx.append(holder);

	
	if (express.isHeadProperty())
	{
		for (unsigned int i = 1; i < express.m_tail.size(); i++)
		{
			if (propertyEx.head().asPString() == express.m_tail[i].head().asPString())
			{
				express.m_tail[i] = propertyEx;
				return express;
			}
		}
		express.append(propertyEx);
		return express;
	}
	else
	{
		newProp.append(express);
		newProp.append(propertyEx);
		return newProp;
	}

	
}



Expression Expression::handle_getProperty(Environment & env) {
	if (m_tail.size() != 2)
	{
		throw SemanticError("Error in call to get-property function: incorrect number of arguments");
	}
	if (!m_tail[0].isHeadPString())
	{
		throw SemanticError("Error in call to get-property function: key is not a string");
	}
	std::string key = m_tail[0].head().asPString();

	Expression propList;
	if (env.is_exp(m_tail[1].head()))
	{
		//propList = env.get_exp(m_tail[1].head());//gets unevaluated property
		propList = m_tail[1].eval(env);
	}
	else if (m_tail[1].isHeadProperty())
	{
		propList = m_tail[1];//in case get and set property are called in the same line, maybe not
	}
	else
	{
		return Expression();
	}


	if (!propList.m_head.isProperty())
	{
		return Expression();
	}
	for (unsigned int i = 1; i < propList.m_tail.size(); i++)
	{
		if (key == propList.m_tail[i].head().asPString())
		{
			return propList.m_tail[i].tailVector()[0];
		}
	}
	return Expression();
}

Expression Expression::handle_discretePlot(Environment & env) {
	Expression graphList;
	Expression textCreate;
	Expression propCreate;
	Expression scaleCreate;
	Expression pointCreate;
	Expression lineCreate;



	graphList.markList();
	double scaleF = 1.0;
	double XScale;
	double YScale;
	double Xmax;
	double Ymax;
	double Xmin;
	double Ymin;



	Xmax = m_tail[0].tailVector()[0].tailVector()[0].head().asNumber();
	Ymax = m_tail[0].tailVector()[0].tailVector()[1].head().asNumber();

	Xmin = m_tail[0].tailVector()[0].tailVector()[0].head().asNumber();
	Ymin = m_tail[0].tailVector()[0].tailVector()[1].head().asNumber();

	for (unsigned int i = 0; i < m_tail[0].tailVector().size(); i++)
	{
		if (m_tail[0].tailVector()[i].tailVector()[0].head().asNumber() > Xmax)
		{
			Xmax = m_tail[0].tailVector()[i].tailVector()[0].head().asNumber();
		}
		if (m_tail[0].tailVector()[i].tailVector()[1].head().asNumber() > Ymax)
		{
			Ymax = m_tail[0].tailVector()[i].tailVector()[1].head().asNumber();
		}

		if (m_tail[0].tailVector()[i].tailVector()[0].head().asNumber() < Xmin)
		{
			Xmin = m_tail[0].tailVector()[i].tailVector()[0].head().asNumber();
		}
		if (m_tail[0].tailVector()[i].tailVector()[1].head().asNumber() < Ymin)
		{
			Ymin = m_tail[0].tailVector()[i].tailVector()[1].head().asNumber();
		}
	}

	XScale = N / (Xmax - Xmin);
	YScale = N / (Ymax - Ymin);

	//double Xshifter = (N + B + B) / 2;
	//double Yshifter = (N + A + A) / 2;
	//plot at (point-Xmin+3.0)*XScale

	for (unsigned int i = 0; i < m_tail[0].tailVector().size(); i++)
	{
		pointCreate = Expression(Atom("make-point"));
		pointCreate.append(Atom((m_tail[0].tailVector()[i].tailVector()[0].head().asNumber())*XScale));
		pointCreate.append(Atom(-(m_tail[0].tailVector()[i].tailVector()[1].head().asNumber())*YScale));//make and scale point

		propCreate = Expression(Atom("set-property"));//set size
		propCreate.append(Atom("\"size\""));
		propCreate.append(Atom(P));
		propCreate.append(pointCreate);

		graphList.append(propCreate.eval(env));

		lineCreate = Expression(Atom("make-line"));
		lineCreate.append(pointCreate);

		pointCreate = Expression(Atom("make-point"));
		pointCreate.append(Atom((m_tail[0].tailVector()[i].tailVector()[0].head().asNumber())*XScale));
		if (Ymin > 0)//line to x axis
		{
			pointCreate.append(Atom(-Ymin * YScale));
		}
		else if (Ymax < 0)
		{
			pointCreate.append(Atom(-Ymax * YScale));
		}
		else
		{
			pointCreate.append(Atom(0.0));
		}
		lineCreate.append(pointCreate);

		propCreate = Expression(Atom("set-property"));
		propCreate.append(Atom("\"thickness\""));
		propCreate.append(Atom(0.0));
		propCreate.append(lineCreate);

		graphList.append(propCreate.eval(env));

	}




	//create graph line boarders (maybe make a helper function?)

	//left
	lineCreate = Expression(Atom("make-line"));

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmin*XScale));
	pointCreate.append(Atom(-Ymax*YScale));

	lineCreate.append(pointCreate);

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmin*XScale));
	pointCreate.append(Atom(-Ymin * YScale));

	lineCreate.append(pointCreate);

	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"thickness\""));
	propCreate.append(Atom(0.0));
	propCreate.append(lineCreate);

	graphList.append(propCreate.eval(env));

	//right
	lineCreate = Expression(Atom("make-line"));

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmax * XScale));
	pointCreate.append(Atom(-Ymax * YScale));

	lineCreate.append(pointCreate);

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmax * XScale));
	pointCreate.append(Atom(-Ymin * YScale));

	lineCreate.append(pointCreate);

	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"thickness\""));
	propCreate.append(Atom(0.0));
	propCreate.append(lineCreate);

	graphList.append(propCreate.eval(env));

	//top
	lineCreate = Expression(Atom("make-line"));

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmin * XScale));
	pointCreate.append(Atom(-Ymax * YScale));

	lineCreate.append(pointCreate);

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmax * XScale));
	pointCreate.append(Atom(-Ymax * YScale));

	lineCreate.append(pointCreate);

	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"thickness\""));
	propCreate.append(Atom(0.0));
	propCreate.append(lineCreate);

	graphList.append(propCreate.eval(env));

	//bottom
	lineCreate = Expression(Atom("make-line"));

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmin * XScale));
	pointCreate.append(Atom(-Ymin * YScale));

	lineCreate.append(pointCreate);

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmax * XScale));
	pointCreate.append(Atom(-Ymin * YScale));

	lineCreate.append(pointCreate);

	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"thickness\""));
	propCreate.append(Atom(0.0));
	propCreate.append(lineCreate);

	graphList.append(propCreate.eval(env));

	//X axis
	if (Ymin <= 0.0 && Ymax >= 0.0)
	{
		lineCreate = Expression(Atom("make-line"));

		pointCreate = Expression(Atom("make-point"));
		pointCreate.append(Atom(Xmin * XScale));
		pointCreate.append(Atom(0.0));

		lineCreate.append(pointCreate);

		pointCreate = Expression(Atom("make-point"));
		pointCreate.append(Atom(Xmax * XScale));
		pointCreate.append(Atom(0.0));

		lineCreate.append(pointCreate);

		propCreate = Expression(Atom("set-property"));
		propCreate.append(Atom("\"thickness\""));
		propCreate.append(Atom(0.0));
		propCreate.append(lineCreate);

		graphList.append(propCreate.eval(env));
	}

	//Y axis
	if (Xmin <= 0.0 && Xmax >= 0.0)
	{
		lineCreate = Expression(Atom("make-line"));

		pointCreate = Expression(Atom("make-point"));
		pointCreate.append(Atom(0.0));
		pointCreate.append(Atom(-Ymin * YScale));

		lineCreate.append(pointCreate);

		pointCreate = Expression(Atom("make-point"));
		pointCreate.append(Atom(0.0));
		pointCreate.append(Atom(-Ymax * YScale));

		lineCreate.append(pointCreate);

		propCreate = Expression(Atom("set-property"));
		propCreate.append(Atom("\"thickness\""));
		propCreate.append(Atom(0.0));
		propCreate.append(lineCreate);

		graphList.append(propCreate.eval(env));
	}
	//


	//create graph labels
	if (m_tail.size() == 2)
	{
		for (unsigned int i = 0; i < m_tail[1].tailVector().size(); i++)
		{
			if (m_tail[1].tailVector()[i].tailVector()[0].head().asPString() == "\"text-scale\"")//should be found first
			{
				scaleF = m_tail[1].tailVector()[i].tailVector()[1].head().asNumber();
			}
		}

		for (unsigned int i = 0; i < m_tail[1].tailVector().size(); i++)
		{

			if (m_tail[1].tailVector()[i].tailVector()[0].head().asPString() == "\"title\"")//(13, 0)
			{
				textCreate = Expression(Atom("make-text"));
				textCreate.append(m_tail[1].tailVector()[i].tailVector()[1]);
				//std::cout << m_tail[1].tailVector()[i].tailVector()[1].head().asPString() << std::endl << std::endl;
				pointCreate = Expression(Atom("make-point"));
				pointCreate.append(Atom((Xmax + Xmin) * XScale/2.0));
				pointCreate.append(Atom((-Ymax * YScale) - A));
				propCreate = Expression(Atom("set-property"));
				propCreate.append(Atom("\"position\""));
				propCreate.append(pointCreate);
				propCreate.append(textCreate);

			}
			else if (m_tail[1].tailVector()[i].tailVector()[0].head().asPString() == "\"abscissa-label\"")//(13, 26)
			{
				textCreate = Expression(Atom("make-text"));
				textCreate.append(m_tail[1].tailVector()[i].tailVector()[1]);

				pointCreate = Expression(Atom("make-point"));
				pointCreate.append(Atom((Xmax + Xmin) * XScale / 2.0));
				pointCreate.append(Atom((-Ymin * YScale) + A));
				propCreate = Expression(Atom("set-property"));
				propCreate.append(Atom("\"position\""));
				propCreate.append(pointCreate);
				propCreate.append(textCreate);
			}
			else if (m_tail[1].tailVector()[i].tailVector()[0].head().asPString() == "\"ordinate-label\"")//(0, 13)
			{
				textCreate = Expression(Atom("make-text"));
				textCreate.append(m_tail[1].tailVector()[i].tailVector()[1]);
				propCreate = Expression(Atom("set-property"));
				propCreate.append(Atom("\"text-rotation\""));
				propCreate.append(Atom(std::atan2(0, -1) / -2.0));
				propCreate.append(textCreate);

				textCreate = propCreate;
				pointCreate = Expression(Atom("make-point"));
				pointCreate.append(Atom((Xmin * XScale) - B));
				pointCreate.append(Atom((-Ymax - Ymin) * YScale / 2.0));
				propCreate = Expression(Atom("set-property"));
				propCreate.append(Atom("\"position\""));
				propCreate.append(pointCreate);
				propCreate.append(textCreate);
			}

			if (!(m_tail[1].tailVector()[i].tailVector()[0].head().asPString() == "\"text-scale\""))//skip
			{
				scaleCreate = Expression(Atom("set-property"));
				scaleCreate.append(Atom("\"text-scale\""));
				scaleCreate.append(scaleF);
				scaleCreate.append(propCreate);
				graphList.append(scaleCreate.eval(env));//evaluate lambda
			}



		}
	}
	//graph limits
	std::stringstream precision;
	precision << std::setprecision(2) << Ymax;
	textCreate = Expression(Atom("make-text"));
	textCreate.append(Atom('\"'+precision.str()+'\"'));
	precision.str(std::string());

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom((Xmin * XScale) - D));
	pointCreate.append(Atom(-Ymax * YScale));
	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"position\""));
	propCreate.append(pointCreate);
	propCreate.append(textCreate);

	scaleCreate = Expression(Atom("set-property"));
	scaleCreate.append(Atom("\"text-scale\""));
	scaleCreate.append(scaleF);
	scaleCreate.append(propCreate);
	graphList.append(scaleCreate.eval(env));//evaluate lambda


	precision << std::setprecision(2) << Ymin;
	textCreate = Expression(Atom("make-text"));
	textCreate.append(Atom('\"' + precision.str() + '\"'));
	precision.str(std::string());

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom((Xmin * XScale) - D));
	pointCreate.append(Atom(-Ymin * YScale));
	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"position\""));
	propCreate.append(pointCreate);
	propCreate.append(textCreate);

	scaleCreate = Expression(Atom("set-property"));
	scaleCreate.append(Atom("\"text-scale\""));
	scaleCreate.append(scaleF);
	scaleCreate.append(propCreate);
	graphList.append(scaleCreate.eval(env));//evaluate lambda

	precision << std::setprecision(2) << Xmax;
	textCreate = Expression(Atom("make-text"));
	textCreate.append(Atom('\"' + precision.str() + '\"'));
	precision.str(std::string());

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmax * XScale));
	pointCreate.append(Atom((-Ymin * YScale) + C));
	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"position\""));
	propCreate.append(pointCreate);
	propCreate.append(textCreate);

	scaleCreate = Expression(Atom("set-property"));
	scaleCreate.append(Atom("\"text-scale\""));
	scaleCreate.append(scaleF);
	scaleCreate.append(propCreate);
	graphList.append(scaleCreate.eval(env));//evaluate lambda

	precision << std::setprecision(2) << Xmin;
	textCreate = Expression(Atom("make-text"));
	textCreate.append(Atom('\"' + precision.str() + '\"'));
	precision.str(std::string());

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmin * XScale));
	pointCreate.append(Atom((-Ymin * YScale) + C));
	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"position\""));
	propCreate.append(pointCreate);
	propCreate.append(textCreate);

	scaleCreate = Expression(Atom("set-property"));
	scaleCreate.append(Atom("\"text-scale\""));
	scaleCreate.append(scaleF);
	scaleCreate.append(propCreate);
	graphList.append(scaleCreate.eval(env));//evaluate lambda
	
	return graphList;
}


Expression Expression::handle_continuousPlot(Environment & env) {
	if (!(m_tail[0].isHeadSymbol() && env.is_exp(m_tail[0].head())))//also check if lambda only has one argument
	{
		throw SemanticError("Error in call to continuous-plot function: first argument not a lambda");
	
	}
	if (!m_tail[1].eval(env).isHeadList())//also check if list contains only numbers, if list is of size 2
	{
		throw SemanticError("Error in call to continuous-plot function: second argument not a list");
	}
	Expression lambda;
	lambda = m_tail[0].head();
	lambda.append(Expression(0));//buffer
	double Xmax;
	double Xmin;
	double Ymax;
	double Ymin;

	//double Xshifter = (N + B + B) / 2;
	//double Yshifter = (N + A + A) / 2;

	Xmax = m_tail[1].eval(env).tailVector()[1].head().asNumber();
	Xmin = m_tail[1].eval(env).tailVector()[0].head().asNumber();

	std::vector<double> Xplot;

	for (int i = 0; i < 51; i++)//points
	{
		Xplot.push_back(i*(Xmax - Xmin) / 50.0 + Xmin);
	}

	double y1;
	double y2;
	double y3;
	//bool iterated = false;
	for (int j = 0; j < 10; j++)//should be 10
	{
		//iterated = false;
		for (unsigned int i = 2; i < Xplot.size(); i+=2)
		{
			lambda.tailVector()[0] = Expression(Xplot[i - 2]);//does this work?
			y1 = lambda.eval(env).head().asNumber();

			lambda.tailVector()[0] = Expression(Xplot[i - 1]);
			y2 = lambda.eval(env).head().asNumber();
			if (i == 2)
			{
				Ymax = y1;
				Ymin = y1;
				if (Ymax < y2)
				{
					Ymax = y2;
				}
				if (Ymin > y2)
				{
					Ymin = y2;
				}
			}
			lambda.tailVector()[0] = Expression(Xplot[i]);
			y3 = lambda.eval(env).head().asNumber();
			if (Ymax < y3)
			{
				Ymax = y3;
			}
			if (Ymin > y3)
			{
				Ymin = y3;
			}
			//if ((180 - (arcCos((x3-x2)/sqrt((x3-x2)^2 + (y3-y2)^2)) - arcCos((x2-x1)/sqrt((x2-x1)^2 + (y2-y1)^2))) < 175) || (-180 - (arcCos((x3-x2)/sqrt((x3-x2)^2 + (y3-y2)^2)) - arcCos((x2-x1)/sqrt((x2-x1)^2 + (y2-y1)^2))) > -175))
			if ((180 - (180.0 / (std::atan2(0, -1))) * (acos((Xplot[i] - Xplot[i - 1]) / sqrt(pow((Xplot[i] - Xplot[i - 1]), 2.0) + pow((y3 - y2), 2.0))) - acos((Xplot[i - 1] - Xplot[i - 2]) / sqrt(pow((Xplot[i - 1] - Xplot[i - 2]), 2) + pow((y2 - y1), 2)))) < 175) || (-180 - (180.0 / (std::atan2(0, -1))) * (acos((Xplot[i] - Xplot[i - 1]) / sqrt(pow((Xplot[i] - Xplot[i - 1]), 2) + pow((y3 - y2), 2))) - acos((Xplot[i - 1] - Xplot[i - 2]) / sqrt(pow((Xplot[i - 1] - Xplot[i - 2]), 2) + pow((y2 - y1), 2)))) > -175))
			{
				Xplot.insert(Xplot.begin() + i, (Xplot[i] - Xplot[i - 1]) / 2 + Xplot[i - 1]);
				Xplot.insert(Xplot.begin() + i - 1, (Xplot[i - 1] - Xplot[i - 2]) / 2 + Xplot[i - 2]);
				i = i + 2;
			}

		}

	}

	//graphing
	Expression graphList;
	Expression textCreate;
	Expression propCreate;
	Expression scaleCreate;
	Expression pointCreate;
	Expression lineCreate;

	graphList.markList();
	double scaleF = 1.0;
	double XScale;
	double YScale;
	

	XScale = N / (Xmax - Xmin);
	YScale = N / (Ymax - Ymin);

	double Yi;

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Expression());
	pointCreate.append(Expression());

	lineCreate = Expression(Atom("make-line"));
	lineCreate.append(Expression());
	lineCreate.append(Expression());

	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"thickness\""));
	propCreate.append(Atom(0.0));
	propCreate.append(Expression());

	Expression holder;
	for (unsigned int i = 0; i < Xplot.size()-1; i++)
	{
		lambda.tailVector()[0] = Expression(Xplot[i]);
		Yi = lambda.eval(env).head().asNumber();
		//pointCreate = Expression(Atom("make-point"));
		pointCreate.tailVector()[0] = (Atom(Xplot[i] * XScale));
		pointCreate.tailVector()[1] = (Atom(-Yi*YScale));

		//lineCreate = Expression(Atom("make-line"));
		lineCreate.tailVector()[0] = (pointCreate);

		lambda.tailVector()[0] = Expression(Xplot[i+1]);
		Yi = lambda.eval(env).head().asNumber();
		//pointCreate = Expression(Atom("make-point"));
		pointCreate.tailVector()[0] = (Atom(Xplot[i + 1] * XScale));
		pointCreate.tailVector()[1] = (Atom(-Yi * YScale));

		lineCreate.tailVector()[1] = (pointCreate);

		//propCreate = Expression(Atom("set-property"));
		//propCreate.append(Atom("\"thickness\""));
		//propCreate.append(Atom(0.0));
		propCreate.tailVector()[2] = (lineCreate);
		holder = propCreate;
		graphList.append(holder.eval(env));//m_tail of holder gets cleared for some reason
		//std::cout << "hey";
	}
	

	//create graph line boarders (maybe make a helper function?)

	//left
	lineCreate = Expression(Atom("make-line"));

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmin*XScale));
	pointCreate.append(Atom(-Ymax * YScale));

	lineCreate.append(pointCreate);

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmin*XScale));
	pointCreate.append(Atom(-Ymin * YScale));

	lineCreate.append(pointCreate);

	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"thickness\""));
	propCreate.append(Atom(0.0));
	propCreate.append(lineCreate);

	graphList.append(propCreate.eval(env));

	//right
	lineCreate = Expression(Atom("make-line"));

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmax * XScale));
	pointCreate.append(Atom(-Ymax * YScale));

	lineCreate.append(pointCreate);

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmax * XScale));
	pointCreate.append(Atom(-Ymin * YScale));

	lineCreate.append(pointCreate);

	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"thickness\""));
	propCreate.append(Atom(0.0));
	propCreate.append(lineCreate);

	graphList.append(propCreate.eval(env));

	//top
	lineCreate = Expression(Atom("make-line"));

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmin * XScale));
	pointCreate.append(Atom(-Ymax * YScale));

	lineCreate.append(pointCreate);

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmax * XScale));
	pointCreate.append(Atom(-Ymax * YScale));

	lineCreate.append(pointCreate);

	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"thickness\""));
	propCreate.append(Atom(0.0));
	propCreate.append(lineCreate);

	graphList.append(propCreate.eval(env));

	//bottom
	lineCreate = Expression(Atom("make-line"));

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmin * XScale));
	pointCreate.append(Atom(-Ymin * YScale));

	lineCreate.append(pointCreate);

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmax * XScale));
	pointCreate.append(Atom(-Ymin * YScale));

	lineCreate.append(pointCreate);

	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"thickness\""));
	propCreate.append(Atom(0.0));
	propCreate.append(lineCreate);

	graphList.append(propCreate.eval(env));

	//X axis
	if (Ymin <= 0.0 && Ymax >= 0.0)
	{
		lineCreate = Expression(Atom("make-line"));

		pointCreate = Expression(Atom("make-point"));
		pointCreate.append(Atom(Xmin * XScale));
		pointCreate.append(Atom(0.0));

		lineCreate.append(pointCreate);

		pointCreate = Expression(Atom("make-point"));
		pointCreate.append(Atom(Xmax * XScale));
		pointCreate.append(Atom(0.0));

		lineCreate.append(pointCreate);

		propCreate = Expression(Atom("set-property"));
		propCreate.append(Atom("\"thickness\""));
		propCreate.append(Atom(0.0));
		propCreate.append(lineCreate);

		graphList.append(propCreate.eval(env));
	}

	//Y axis
	if (Xmin <= 0.0 && Xmax >= 0.0)
	{
		lineCreate = Expression(Atom("make-line"));

		pointCreate = Expression(Atom("make-point"));
		pointCreate.append(Atom(0.0));
		pointCreate.append(Atom(-Ymin * YScale));

		lineCreate.append(pointCreate);

		pointCreate = Expression(Atom("make-point"));
		pointCreate.append(Atom(0.0));
		pointCreate.append(Atom(-Ymax * YScale));

		lineCreate.append(pointCreate);

		propCreate = Expression(Atom("set-property"));
		propCreate.append(Atom("\"thickness\""));
		propCreate.append(Atom(0.0));
		propCreate.append(lineCreate);

		graphList.append(propCreate.eval(env));
	}
	//


	//create graph labels
	if (m_tail.size() == 3)
	{
		for (unsigned int i = 0; i < m_tail[2].tailVector().size(); i++)
		{
			if (m_tail[2].tailVector()[i].tailVector()[0].head().asPString() == "\"text-scale\"")//should be found first
			{
				scaleF = m_tail[2].tailVector()[i].tailVector()[1].head().asNumber();
			}
		}

		for (unsigned int i = 0; i < m_tail[2].tailVector().size(); i++)
		{

			if (m_tail[2].tailVector()[i].tailVector()[0].head().asPString() == "\"title\"")//(13, 0)
			{
				textCreate = Expression(Atom("make-text"));
				textCreate.append(m_tail[2].tailVector()[i].tailVector()[1]);
				//std::cout << m_tail[2].tailVector()[i].tailVector()[1].head().asPString() << std::endl << std::endl;
				pointCreate = Expression(Atom("make-point"));
				pointCreate.append(Atom((Xmax + Xmin) * XScale / 2.0));
				pointCreate.append(Atom((-Ymax * YScale) - A));
				propCreate = Expression(Atom("set-property"));
				propCreate.append(Atom("\"position\""));
				propCreate.append(pointCreate);
				propCreate.append(textCreate);

			}
			else if (m_tail[2].tailVector()[i].tailVector()[0].head().asPString() == "\"abscissa-label\"")//(13, 26)
			{
				textCreate = Expression(Atom("make-text"));
				textCreate.append(m_tail[2].tailVector()[i].tailVector()[1]);

				pointCreate = Expression(Atom("make-point"));
				pointCreate.append(Atom((Xmax + Xmin) * XScale / 2.0));
				pointCreate.append(Atom((-Ymin * YScale) + A));
				propCreate = Expression(Atom("set-property"));
				propCreate.append(Atom("\"position\""));
				propCreate.append(pointCreate);
				propCreate.append(textCreate);
			}
			else if (m_tail[2].tailVector()[i].tailVector()[0].head().asPString() == "\"ordinate-label\"")//(0, 13)
			{
				textCreate = Expression(Atom("make-text"));
				textCreate.append(m_tail[2].tailVector()[i].tailVector()[1]);
				propCreate = Expression(Atom("set-property"));
				propCreate.append(Atom("\"text-rotation\""));
				propCreate.append(Atom(std::atan2(0, -1) / -2.0));
				propCreate.append(textCreate);

				textCreate = propCreate;
				pointCreate = Expression(Atom("make-point"));
				pointCreate.append(Atom((Xmin * XScale) - B));
				pointCreate.append(Atom((-Ymax - Ymin) * YScale / 2.0));
				propCreate = Expression(Atom("set-property"));
				propCreate.append(Atom("\"position\""));
				propCreate.append(pointCreate);
				propCreate.append(textCreate);
			}

			if (!(m_tail[2].tailVector()[i].tailVector()[0].head().asPString() == "\"text-scale\""))//skip
			{
				scaleCreate = Expression(Atom("set-property"));
				scaleCreate.append(Atom("\"text-scale\""));
				scaleCreate.append(scaleF);
				scaleCreate.append(propCreate);
				graphList.append(scaleCreate.eval(env));//evaluate lambda
			}



		}
	}
	//graph limits
	std::stringstream precision;
	precision << std::setprecision(2) << Ymax;
	textCreate = Expression(Atom("make-text"));
	textCreate.append(Atom('\"' + precision.str() + '\"'));
	precision.str(std::string());

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom((Xmin * XScale) - D));
	pointCreate.append(Atom(-Ymax * YScale));
	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"position\""));
	propCreate.append(pointCreate);
	propCreate.append(textCreate);

	scaleCreate = Expression(Atom("set-property"));
	scaleCreate.append(Atom("\"text-scale\""));
	scaleCreate.append(scaleF);
	scaleCreate.append(propCreate);
	graphList.append(scaleCreate.eval(env));//evaluate lambda


	precision << std::setprecision(2) << Ymin;
	textCreate = Expression(Atom("make-text"));
	textCreate.append(Atom('\"' + precision.str() + '\"'));
	precision.str(std::string());

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom((Xmin * XScale) - D));
	pointCreate.append(Atom(-Ymin * YScale));
	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"position\""));
	propCreate.append(pointCreate);
	propCreate.append(textCreate);

	scaleCreate = Expression(Atom("set-property"));
	scaleCreate.append(Atom("\"text-scale\""));
	scaleCreate.append(scaleF);
	scaleCreate.append(propCreate);
	graphList.append(scaleCreate.eval(env));//evaluate lambda

	precision << std::setprecision(2) << Xmax;
	textCreate = Expression(Atom("make-text"));
	textCreate.append(Atom('\"' + precision.str() + '\"'));
	precision.str(std::string());

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmax * XScale));
	pointCreate.append(Atom((-Ymin * YScale) + C));
	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"position\""));
	propCreate.append(pointCreate);
	propCreate.append(textCreate);

	scaleCreate = Expression(Atom("set-property"));
	scaleCreate.append(Atom("\"text-scale\""));
	scaleCreate.append(scaleF);
	scaleCreate.append(propCreate);
	graphList.append(scaleCreate.eval(env));//evaluate lambda

	precision << std::setprecision(2) << Xmin;
	textCreate = Expression(Atom("make-text"));
	textCreate.append(Atom('\"' + precision.str() + '\"'));
	precision.str(std::string());

	pointCreate = Expression(Atom("make-point"));
	pointCreate.append(Atom(Xmin * XScale));
	pointCreate.append(Atom((-Ymin * YScale) + C));
	propCreate = Expression(Atom("set-property"));
	propCreate.append(Atom("\"position\""));
	propCreate.append(pointCreate);
	propCreate.append(textCreate);

	scaleCreate = Expression(Atom("set-property"));
	scaleCreate.append(Atom("\"text-scale\""));
	scaleCreate.append(scaleF);
	scaleCreate.append(propCreate);
	graphList.append(scaleCreate.eval(env));//evaluate lambda

	return graphList;
}


// this is a simple recursive version. the iterative version is more
// difficult with the ast data structure used (no parent pointer).
// this limits the practical depth of our ASTdef
Expression Expression::eval(Environment & env){
	//cout << endl << "EVAL: ";
	//HexpressVisual(m_head, m_tail, Expression(), 0);
	//cout << endl;
	Expression holder;
	/*
	if (m_head.isProperty())//if commented out, expressions with property lists can't be used normally by other functions
	{
		m_head = m_tail[0].head();
		m_tail = m_tail[0].tailVector();
		return eval(env);

	}
	*/
		if (m_tail.empty()) {
			if (m_head.asSymbol() == "list")//empty list
			{
				return apply(m_head, m_tail, env);
			}
			//cout << "justincase\n";
			//HexpressVisual(m_head, m_tail, Expression(), 0);
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
		else if (m_head.isSymbol() && m_head.asSymbol() == "get-property") {
			return handle_getProperty(env);
		}
		else if (m_head.isSymbol() && m_head.asSymbol() == "continuous-plot") {
			return handle_continuousPlot(env);
		}
		// else attempt to treat as procedure
		else {
			std::vector<Expression> results;
			
			for (Expression::IteratorType it = m_tail.begin(); it != m_tail.end(); ++it) {
				results.push_back(it->eval(env));
			}
			//cout << "BEGINforloop" << endl;
			/*
			for (unsigned int i = 0; i < m_tail.size(); i++)
			{
				//cout << m_tail[i].head().asSymbol() << endl;
				//HexpressVisual(m_tail[i].head(), m_tail[i].tailVector(), Expression(), 0);
				results.push_back(m_tail[i].eval(env));
			}
			*/
			//cout << "ENDforloop" << endl;
			if (m_head.isSymbol() && m_head.asSymbol() == "discrete-plot") {
				m_tail = results;
				return handle_discretePlot(env);
			}
			if (!results.empty() && m_head.isSymbol() && env.is_exp(m_head) && (handle_lookup(m_head, env).isHeadProcedure())){
			//	cout <<"procedure: " << m_head.asSymbol() << endl;
				m_tail = results;
				return handle_lambdaProcedure(env);
			}
			return apply(m_head, results, env);
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
	else if (exp.isHeadNone())
	{
		out << "NONE";
	}
	else
	{
		out << "(";
		out << exp.head();

		for (auto e = exp.tailConstBegin(); e != exp.tailConstEnd(); ++e) {
			out << *e;//in case of none, no spaces: ()
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
