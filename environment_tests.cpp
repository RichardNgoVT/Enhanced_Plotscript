#include "catch.hpp"

#include "environment.hpp"
#include "semantic_error.hpp"

#include <cmath>

TEST_CASE( "Test default constructor", "[environment]" ) {

  Environment env;

  REQUIRE(env.is_known(Atom("pi")));
  REQUIRE(env.is_exp(Atom("pi")));

  REQUIRE(!env.is_known(Atom("hi")));
  REQUIRE(!env.is_exp(Atom("hi")));

  REQUIRE(env.is_proc(Atom("+")));
  REQUIRE(env.is_proc(Atom("-")));
  REQUIRE(env.is_proc(Atom("*")));
  REQUIRE(env.is_proc(Atom("/")));
  REQUIRE(!env.is_proc(Atom("op")));
}

TEST_CASE( "Test get expression", "[environment]" ) {
  Environment env;

  REQUIRE(env.get_exp(Atom("pi")) == Expression(std::atan2(0, -1)));
  REQUIRE(env.get_exp(Atom("hi")) == Expression());
}

TEST_CASE( "Test add expression", "[environment]" ) {
  Environment env;

  Expression a(Atom(1.0));
  env.add_exp(Atom("one"), a);
  REQUIRE(env.is_known(Atom("one")));
  REQUIRE(env.is_exp(Atom("one")));
  REQUIRE(env.get_exp(Atom("one")) == a);

  Expression b(Atom("hello"));
  env.add_exp(Atom("hi"), b);
  REQUIRE(env.is_known(Atom("hi")));
  REQUIRE(env.is_exp(Atom("hi")));
  REQUIRE(env.get_exp(Atom("hi")) == b);

  REQUIRE_THROWS_AS(env.add_exp(Atom(1.0), b), SemanticError);
}

TEST_CASE( "Test get built-in procedure", "[environment]" ) {
  Environment env;

  INFO("default procedure")
  Procedure p1 = env.get_proc(Atom("doesnotexist"));
  Procedure p2 = env.get_proc(Atom("alsodoesnotexist"));
  REQUIRE(p1 == p2);
  std::vector<Expression> args;
  REQUIRE(p1(args) == Expression());
  REQUIRE(p2(args) == Expression());

  INFO("test add procedure (normal + normal)")
  Procedure padd = env.get_proc(Atom("+"));
  args.emplace_back(1.0);
  args.emplace_back(2.0);
  REQUIRE(padd(args) == Expression(3.0));

  INFO("test add procedure (normal + complex)")
  args.clear();
  complex<double> Imag(0.0, 1.0);
  complex<double> imagTest(0.0, 0.0);
  imagTest = 2.0 * Imag + 1.0;
  args.emplace_back(imagTest);
  args.emplace_back(3.0);
  imagTest = 2.0 * Imag + 4.0;

  REQUIRE(padd(args) == Expression(imagTest));

  INFO("test add procedure (complex + complex)")
  args.clear();
  args.emplace_back(imagTest);
  imagTest = Imag + 3.0;
  args.emplace_back(imagTest);
  imagTest = 3.0*Imag + 7.0;
  REQUIRE(padd(args) == Expression(imagTest));
  args.emplace_back(Atom("invalid"));
  REQUIRE_THROWS_AS(padd(args), SemanticError);
  args.clear();


  INFO("test mul procedure (normal * normal)")
	  padd = env.get_proc(Atom("*"));
  args.emplace_back(1.0);
  args.emplace_back(2.0);
  REQUIRE(padd(args) == Expression(2.0));

  INFO("test mul procedure (normal * complex)")
	  args.clear();
  imagTest = 2.0 * Imag + 1.0;
  args.emplace_back(imagTest);
  args.emplace_back(3.0);
  imagTest = 6.0 * Imag + 3.0;

  REQUIRE(padd(args) == Expression(imagTest));

  INFO("test mul procedure (complex * complex)")
	  args.clear();
  args.emplace_back(imagTest);
  imagTest = Imag + 3.0;
  args.emplace_back(imagTest);
  imagTest = 21.0*Imag + 3.0;
  REQUIRE(padd(args) == Expression(imagTest));

  INFO("test mul errors")
  args.emplace_back(Atom("invalid"));
  REQUIRE_THROWS_AS(padd(args), SemanticError);
  args.clear();

  INFO("test subneg procedure (normal - normal)")
	  padd = env.get_proc(Atom("-"));
  args.emplace_back(1.0);
  args.emplace_back(2.0);
  REQUIRE(padd(args) == Expression(-1.0));

  INFO("test subneg procedure (normal - complex)")
	  args.clear();
  imagTest = 2.0 * Imag + 1.0;
  args.emplace_back(imagTest);
  args.emplace_back(3.0);
  imagTest = 2.0 * Imag - 2.0;

  REQUIRE(padd(args) == Expression(imagTest));

  INFO("test subneg procedure (complex - complex)")
	  args.clear();
  args.emplace_back(imagTest);
  imagTest = Imag + 3.0;
  args.emplace_back(imagTest);
  imagTest = Imag - 5.0;
  REQUIRE(padd(args) == Expression(imagTest));

  INFO("test subneg procedure (- normal)")
	  args.clear();
  args.emplace_back(5.0);
  REQUIRE(padd(args) == Expression(-5.0));

  INFO("test subneg procedure (- complex)")
	  args.clear();
  imagTest = 2.0 * Imag;
  args.emplace_back(imagTest);
  imagTest = -2.0 * Imag;
  REQUIRE(padd(args) == Expression(imagTest));

  INFO("test subneg errors")
	  args.clear();
	args.emplace_back(Atom(1));
	  args.emplace_back(Atom("invalid"));
  REQUIRE_THROWS_AS(padd(args), SemanticError);
  args[0] = (Atom("invalid"));
  REQUIRE_THROWS_AS(padd(args), SemanticError);
  args.clear();
  args.emplace_back(Atom(1));
  args.emplace_back(Atom(1));
  args.emplace_back(Atom(1));
  REQUIRE_THROWS_AS(padd(args), SemanticError);
  args.clear();
  args.emplace_back(Atom("invalid"));
  REQUIRE_THROWS_AS(padd(args), SemanticError);
  args.clear();

  INFO("test div procedure (normal / normal)")
	  padd = env.get_proc(Atom("/"));
  args.emplace_back(1.0);
  args.emplace_back(2.0);
  REQUIRE(padd(args) == Expression(0.5));

  INFO("test div procedure (normal / complex)")
	  args.clear();
  imagTest = 2.0 * Imag + 1.0;
  args.emplace_back(imagTest);
  args.emplace_back(3.0);
  imagTest = (2.0 * Imag + 1.0)/3.0;

  REQUIRE(padd(args) == Expression(imagTest));

  INFO("test div procedure (complex / complex)")
	  args.clear();
  args.emplace_back(imagTest);
  imagTest = Imag + 3.0;
  args.emplace_back(imagTest);
  imagTest = ((2.0 * Imag + 1.0) / 3.0 )/(Imag + 3.0);
  REQUIRE(padd(args) == Expression(imagTest));

  /*
  INFO("test div procedure (- normal)")
	  args.clear();
  args.emplace_back(5.0);
  REQUIRE(padd(args) == Expression(-5.0));

  INFO("test div procedure (- complex)")
	  args.clear();
  imagTest = 2.0 * Imag;
  args.emplace_back(imagTest);
  imagTest = -2.0 * Imag;
  REQUIRE(padd(args) == Expression(imagTest));

  */
  INFO("test div errors")
	  args.clear();
  args.emplace_back(Atom(1));
  args.emplace_back(Atom("invalid"));
  REQUIRE_THROWS_AS(padd(args), SemanticError);
  args[0] = (Atom("invalid"));
  REQUIRE_THROWS_AS(padd(args), SemanticError);
  args.clear();
  args.emplace_back(Atom(1));
  args.emplace_back(Atom(1));
  args.emplace_back(Atom(1));
  REQUIRE_THROWS_AS(padd(args), SemanticError);
  args.clear();
  args.emplace_back(Atom("invalid"));
  REQUIRE_THROWS_AS(padd(args), SemanticError);
  args.clear();


  INFO("test sqrt procedure (sqrt normal)")
	  padd = env.get_proc(Atom("sqrt"));
  args.emplace_back(4.0);
  REQUIRE(padd(args) == Expression(2.0));

  INFO("test sqrt procedure (sqrt -normal)")
	  args.clear();
  imagTest = -4.0;
  args.emplace_back(imagTest);
  imagTest = (2.0 * Imag);

  REQUIRE(padd(args) == Expression(imagTest));

  INFO("test sqrt procedure (sqrt complex)")
	  args.clear();
  imagTest = Imag;
  args.emplace_back(imagTest);
  imagTest = (sqrt(Imag));
  REQUIRE(padd(args) == Expression(imagTest));

  INFO("test sqrt errors")
	  args.clear();
  args.emplace_back(Atom("invalid"));
  REQUIRE_THROWS_AS(padd(args), SemanticError);
  args.clear();
  args.emplace_back(Atom(1));
  args.emplace_back(Atom(1));
  args.emplace_back(Atom(1));
  REQUIRE_THROWS_AS(padd(args), SemanticError);
  args.clear();
}

TEST_CASE( "Test reset", "[environment]" ) {
  Environment env;

  Expression a(Atom(1.0));
  env.add_exp(Atom("one"), a);
  Expression b(Atom("hello"));
  env.add_exp(Atom("hi"), b);

  env.reset();
  REQUIRE(!env.is_known(Atom("one")));
  REQUIRE(!env.is_exp(Atom("one")));
  REQUIRE(env.get_exp(Atom("one")) == Expression());
  REQUIRE(!env.is_known(Atom("hi")));
  REQUIRE(!env.is_exp(Atom("hi")));
  REQUIRE(env.get_exp(Atom("hi")) == Expression());
}

TEST_CASE( "Test semeantic errors", "[environment]" ) {

  Environment env;

  {
    Expression exp(Atom("begin"));
    
    REQUIRE_THROWS_AS(exp.eval(env), SemanticError);
  }
}

