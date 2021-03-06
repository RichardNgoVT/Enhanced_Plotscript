#include "atom.hpp"

#include <sstream>
#include <cctype>
#include <cmath>
#include <limits>

Atom::Atom(): m_type(NoneKind) {}

Atom::Atom(double value){

  setNumber(value);
}


Atom::Atom(std::complex<double> value) {

	setComplex(value);
}

Atom::Atom(const Token & token): Atom(){
  // is token a number?
  double temp;
  std::istringstream iss(token.asString());
  if(iss >> temp){
    // check for trailing characters if >> succeeds
    if(iss.rdbuf()->in_avail() == 0){
      setNumber(temp);
    }
  }
  else{ // else assume symbol or string
	  //checks if starts and ends with "
	  if (token.asString()[0] == '\"'){// && token.asString()[token.asString().length()-1] == '\"') {
		  setPString(token.asString());
	  }
    // make sure does not start with number
    else if(!std::isdigit(token.asString()[0])){
      setSymbol(token.asString());
    }
  }
}

Atom::Atom(const std::string & value): Atom() {
	if (value[0] == '\"'){// && value[value.length() - 1] == '\"') {
		setPString(value);
	}
	else
	{
		setSymbol(value);
	}
}

Atom::Atom(const Atom & x): Atom(){
  if(x.isNumber()){
    setNumber(x.numberValue);
  }
  else if(x.isSymbol()){
    setSymbol(x.stringValue);
  }
  else if (x.isPString())
  {
	  setPString(x.stringValue);
  }
  else if (x.isComplex()) {
	  setComplex(x.complexValue);
  }
  else if (x.isList()) {
	  setList();
  }
  else if (x.isProcedure())
  {
	  setProcedure();
  }
  else if (x.isProperty())
  {
	  setProperty();
  }
  else if (x.isError())
  {
	  setError(x.stringValue);
  }
}

Atom & Atom::operator=(const Atom & x){

  if(this != &x){
    if(x.m_type == NoneKind){
      m_type = NoneKind;
    }
    else if(x.m_type == NumberKind){
      setNumber(x.numberValue);
    }
    else if(x.m_type == SymbolKind){
      setSymbol(x.stringValue);
    }
	else if (x.m_type == StringKind) {
		setPString(x.stringValue);
	}
	else if (x.m_type == ComplexKind) {
		setComplex(x.complexValue);
	}
	else if (x.m_type == ListKind) {
		setList();
	}
	else if (x.m_type == ProcedureKind) {
		setProcedure();
	}
	else if (x.m_type == PropertyKind) {
		setProperty();
	}
	else if (x.m_type == ErrorKind) {
		setError(x.stringValue);
	}
  }
  return *this;
}
  
Atom::~Atom(){

  // we need to ensure the destructor of the symbol or pstring string is called
  if(m_type == SymbolKind || m_type == StringKind || m_type == ErrorKind){
    stringValue.~basic_string();
  }
}

bool Atom::isNone() const noexcept{
  return m_type == NoneKind;
}

bool Atom::isNumber() const noexcept{
  return m_type == NumberKind;
}

bool Atom::isComplex() const noexcept {
	return m_type == ComplexKind;
}

bool Atom::isSymbol() const noexcept{
  return m_type == SymbolKind;
}  

bool Atom::isPString() const noexcept
{
	return m_type == StringKind;
}

bool Atom::isList() const noexcept {
	return m_type == ListKind;
}

bool Atom::isProcedure() const noexcept
{
	return m_type == ProcedureKind;
}

bool Atom::isProperty() const noexcept
{
	return m_type == PropertyKind;
}

bool Atom::isError() const noexcept
{
	return m_type == ErrorKind;
}


void Atom::setNumber(double value){

  m_type = NumberKind;
  numberValue = value;
}

void Atom::setComplex(std::complex<double> value) {

	m_type = ComplexKind;
	complexValue = value;
}

void Atom::setSymbol(const std::string & value){

  // we need to ensure the destructor of the symbol string is called
  if(m_type == SymbolKind || m_type == StringKind || m_type == ErrorKind){
    stringValue.~basic_string();
  }
    
  m_type = SymbolKind;

  // copy construct in place
  new (&stringValue) std::string(value);
}

void Atom::setPString(const std::string & value) {

	// we need to ensure the destructor of the PString string is called
	if (m_type == SymbolKind || m_type == StringKind || m_type == ErrorKind) {
		stringValue.~basic_string();
	}

	m_type = StringKind;

	// copy construct in place
	new (&stringValue) std::string(value);
}

void Atom::setError(const std::string & value)
{

	if (m_type == SymbolKind || m_type == StringKind || m_type == ErrorKind) {
		stringValue.~basic_string();
	}

	m_type = ErrorKind;

	// copy construct in place
	new (&stringValue) std::string(value);
	
}

void Atom::setList() {

	m_type = ListKind;
}

void Atom::setProcedure() {

	m_type = ProcedureKind;
}

void Atom::setProperty()
{
	m_type = PropertyKind;
}

double Atom::asNumber() const noexcept{

  return (m_type == NumberKind) ? numberValue : 0.0;  
}

std::complex<double> Atom::asComplex() const noexcept {

	return (m_type == ComplexKind) ? complexValue : std::complex<double>(0.0,0.0);
}

std::string Atom::asSymbol() const noexcept{

  std::string result;

  if(m_type == SymbolKind){
    result = stringValue;
  }

  return result;
}

std::string Atom::asPString() const noexcept {

	std::string result;

	if (m_type == StringKind) {
		result = stringValue;
	}

	return result;
}

std::string Atom::asError() const noexcept {

	std::string result;

	if (m_type == ErrorKind) {
		result = stringValue;
	}

	return result;
}

bool Atom::operator==(const Atom & right) const noexcept{
  
  if(m_type != right.m_type) return false;

  switch(m_type){
  case NoneKind:
    if(right.m_type != NoneKind) return false;
    break;
  case NumberKind:
    {
      if(right.m_type != NumberKind) return false;
      double dleft = numberValue;
      double dright = right.numberValue;
      double diff = fabs(dleft - dright);
      if(std::isnan(diff) ||
	 (diff > std::numeric_limits<double>::epsilon())) return false;
    }
    break;
  case SymbolKind:
    {
      if(right.m_type != SymbolKind) return false;

      return stringValue == right.stringValue;
    }
    break;
  case StringKind:
  {
	  if (right.m_type != StringKind) return false;

	  return stringValue == right.stringValue;
  }
  break;
  case ComplexKind:
  {
	  if (right.m_type != ComplexKind) return false;

	  return complexValue == right.complexValue;
  }
  break;
  case ListKind:
  {
	  if (right.m_type != ListKind) return false;
  }
  break;
  case ProcedureKind:
  {
	  if (right.m_type != ProcedureKind) return false;
  }
  break;
  case PropertyKind:
  {
	  if (right.m_type != PropertyKind) return false;
  }
  break;
  case ErrorKind:
  {
	  if (right.m_type != ErrorKind) return false;
	  return stringValue == right.stringValue;
  }
  break;
  default:
    return false;
  }

  return true;
}

bool operator!=(const Atom & left, const Atom & right) noexcept{
  
  return !(left == right);
}


std::ostream & operator<<(std::ostream & out, const Atom & a){

  if(a.isNumber()){
    out << a.asNumber();
  }
  if(a.isSymbol()){
    out << a.asSymbol();
  }
  if (a.isPString()) {
	  out << a.asPString();
  }
  if (a.isComplex()) {
	  out << a.asComplex();
  }
  return out;
}
