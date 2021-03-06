#include "catch.hpp"

#include "atom.hpp"

TEST_CASE( "Test constructors", "[atom]" ) {

  {
    INFO("Default Constructor");
    Atom a;

    REQUIRE(a.isNone());
    REQUIRE(!a.isNumber());
    REQUIRE(!a.isSymbol());
  }

  {
    INFO("Number Constructor");
    Atom a(1.0);

    REQUIRE(!a.isNone());
    REQUIRE(a.isNumber());
    REQUIRE(!a.isSymbol());
  }

  {
    INFO("Symbol Constructor");
    Atom a("hi");

    REQUIRE(!a.isNone());
    REQUIRE(!a.isNumber());
    REQUIRE(a.isSymbol());
  }


  {
    INFO("Token Constructor");
    Token t("hi");
    Atom a(t);

    REQUIRE(!a.isNone());
    REQUIRE(!a.isNumber());
    REQUIRE(a.isSymbol());
  }

  {
    INFO("Copy Constructor");
    Atom a("hi");
    Atom b(1.0);
	Atom e(std::complex<double>(1, 1));
	Atom f("\"hi\"");
	//Atom b(1.0);
    
    Atom c = a;
    REQUIRE(!a.isNone());
    REQUIRE(!c.isNumber());
    REQUIRE(c.isSymbol());

    Atom d = b;
    REQUIRE(!a.isNone());
    REQUIRE(d.isNumber());
    REQUIRE(!d.isSymbol());

	Atom g = e;
	REQUIRE(!e.isNone());
	REQUIRE(!g.isNumber());
	REQUIRE(!g.isSymbol());
	REQUIRE(g.isComplex());

	Atom h = f;
	REQUIRE(!f.isNone());
	REQUIRE(!h.isNumber());
	REQUIRE(!h.isSymbol());
	REQUIRE(!h.isComplex());
	REQUIRE(h.isPString());
  }
}

TEST_CASE( "Test assignment", "[atom]" ) {

  {
    INFO("default to default");
    Atom a;
    Atom b;
    b = a;
    REQUIRE(b.isNone());
    REQUIRE(!b.isNumber());
    REQUIRE(!b.isSymbol());
  }

  {
    INFO("default to number");
    Atom a;
    Atom b(1.0);
    b = a;
    REQUIRE(b.isNone());
    REQUIRE(!b.isNumber());
    REQUIRE(!b.isSymbol());
  }

  {
    INFO("default to symbol");
    Atom a;
    Atom b("hi");
    b = a;
    REQUIRE(b.isNone());
    REQUIRE(!b.isNumber());
    REQUIRE(!b.isSymbol());
  }

  {
    INFO("number to default");
    Atom a(1.0);
    Atom b;
    b = a;
    REQUIRE(b.isNumber());
    REQUIRE(b.asNumber() == 1.0);
  }

  {
    INFO("number to number");
    Atom a(1.0);
    Atom b(2.0);
    b = a;
    REQUIRE(b.isNumber());
    REQUIRE(b.asNumber() == 1.0);
  }

  {
    INFO("number to symbol");
    Atom a("hi");
    Atom b(1.0);
    b = a;
    REQUIRE(b.isSymbol());
    REQUIRE(b.asSymbol() == "hi");
  }

  {
    INFO("symbol to default");
    Atom a("hi");
    Atom b;
    b = a;
    REQUIRE(b.isSymbol());
    REQUIRE(b.asSymbol() == "hi");
  }

  {
    INFO("symbol to number");
    Atom a("hi");
    Atom b(1.0);
    b = a;
    REQUIRE(b.isSymbol());
    REQUIRE(b.asSymbol() == "hi");
  }

  {
    INFO("symbol to symbol");
    Atom a("hi");
    Atom b("bye");
    b = a;
    REQUIRE(b.isSymbol());
    REQUIRE(b.asSymbol() == "hi");
  }

}

TEST_CASE( "test comparison", "[atom]" ) {

  {
    INFO("compare default to default");
    Atom a;
    Atom b;
    REQUIRE(a == b);
  }

  {
    INFO("compare default to number");
    Atom a;
    Atom b(1.0);
    REQUIRE(a != b);
  }

  {
    INFO("compare default to symbol");
    Atom a;
    Atom b("hi");
    REQUIRE(a != b);
  }

  {
    INFO("compare number to default");
    Atom a(1.0);
    Atom b;
    REQUIRE(a != b);
  }

  {
    INFO("compare number to number");
    Atom a(1.0);
    Atom b(1.0);
    Atom c(2.0);
    REQUIRE(a == b);
    REQUIRE(a != c);
  }

  {
    INFO("compare number to symbol");
    Atom a(1.0);
    Atom b("hi");
    REQUIRE(a != b);
  }

  {
    INFO("compare symbol to default");
    Atom a("hi");
    Atom b;
    REQUIRE(a != b);
  }

  {
    INFO("compare symbol to number");
    Atom a("hi");
    Atom b(1.0);
    REQUIRE(a != b);
  }

  {
    INFO("compare symbol to symbol");
    Atom a("hi");
    Atom b("hi");
    Atom c("bye");
    REQUIRE(a == b);
    REQUIRE(a != c);
  }

  {
	  INFO("compare symbol to list");
	  Atom a;
	  a.setList();
	  Atom b;
	  b.setList();
	  Atom c("bye");
	  Atom d(a);
	  REQUIRE(a == b);
	  REQUIRE(a != c);
	  REQUIRE(a == d);
  }

  {
	  INFO("compare symbol to symbol");
	  Atom a;
	  a.setProcedure();
	  Atom b;
	  b.setProcedure();
	  Atom c("bye");
	  Atom d(a);
	  REQUIRE(a == b);
	  REQUIRE(a != c);
	  REQUIRE(a == d);
  }

}





