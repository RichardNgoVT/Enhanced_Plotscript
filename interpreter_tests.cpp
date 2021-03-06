#include "catch.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "semantic_error.hpp"
#include "interpreter.hpp"
#include "expression.hpp"

Expression run(const std::string & program){
  
  std::istringstream iss(program);
    
  Interpreter interp;
    
  bool ok = interp.parseStream(iss);
  if(!ok){
    std::cerr << "Failed to parse: " << program << std::endl; 
  }
  REQUIRE(ok == true);

  Expression result;
  REQUIRE_NOTHROW(result = interp.evaluate());

  return result;
}

TEST_CASE( "Test Interpreter parser with expected input", "[interpreter]" ) {

  std::string program = "(begin (define r 10) (* pi (* r r)))";

  std::istringstream iss(program);
 
  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == true);
}

TEST_CASE( "Test Interpreter parser with numerical literals", "[interpreter]" ) {

  std::vector<std::string> programs = {"(1)", "(+1)", "(+1e+0)", "(1e-0)"};
  
  for(auto program : programs){
    std::istringstream iss(program);
 
    Interpreter interp;

    bool ok = interp.parseStream(iss);

    REQUIRE(ok == true);
  }

  {
    std::istringstream iss("(define x 1abc)");
    
    Interpreter interp;

    bool ok = interp.parseStream(iss);

    REQUIRE(ok == false);
  }
}

TEST_CASE( "Test Interpreter parser with truncated input", "[interpreter]" ) {

  {
    std::string program = "(f";
    std::istringstream iss(program);
  
    Interpreter interp;
    bool ok = interp.parseStream(iss);
    REQUIRE(ok == false);
  }
  
  {
    std::string program = "(begin (define r 10) (* pi (* r r";
    std::istringstream iss(program);

    Interpreter interp;
    bool ok = interp.parseStream(iss);
    REQUIRE(ok == false);
  }
}

TEST_CASE( "Test Interpreter parser with extra input", "[interpreter]" ) {

  std::string program = "(begin (define r 10) (* pi (* r r))) )";
  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with single non-keyword", "[interpreter]" ) {

  std::string program = "hello";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with empty input", "[interpreter]" ) {

  std::string program;
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with empty expression", "[interpreter]" ) {

  std::string program = "( )";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with bad number string", "[interpreter]" ) {

  std::string program = "(1abc)";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with incorrect input. Regression Test", "[interpreter]" ) {

  std::string program = "(+ 1 2) (+ 3 4)";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter result with literal expressions", "[interpreter]" ) {
  
  { // Number
    std::string program = "(4)";
    Expression result = run(program);
    REQUIRE(result == Expression(4.));
  }

  { // Symbol
    std::string program = "(pi)";
    Expression result = run(program);
    REQUIRE(result == Expression(atan2(0, -1)));
  }

}

TEST_CASE( "Test Interpreter result with simple procedures (add)", "[interpreter]" ) {

  { // add, binary case
    std::string program = "(+ 1 2)";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(3.));
  }
  
  { // add, 3-ary case
    std::string program = "(+ 1 2 3)";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(6.));
  }

  { // add, 6-ary case
    std::string program = "(+ 1 2 3 4 5 6)";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(21.));
  }
}
  
TEST_CASE( "Test Interpreter special forms: begin and define", "[interpreter]" ) {

  {
    std::string program = "(define answer 42)";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(42.));
  }

  {
    std::string program = "(begin (define answer 42)\n(answer))";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(42.));
  }
  
  {
    std::string program = "(begin (define answer (+ 9 11)) (answer))";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(20.));
  }

  {
    std::string program = "(begin (define a 1) (define b 1) (+ a b))";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(2.));
  }
}

TEST_CASE( "Test a medium-sized expression", "[interpreter]" ) {

  {
    std::string program = "(+ (+ 10 1) (+ 30 (+ 1 1)))";
    Expression result = run(program);
    REQUIRE(result == Expression(43.));
  }
}

TEST_CASE( "Test arithmetic procedures", "[interpreter]" ) {

  {
    std::vector<std::string> programs = {"(+ 1 -2)",
					 "(+ -3 1 1)",
					 "(- 1)",
					 "(- 1 2)",
					 "(* 1 -1)",
					 "(* 1 1 -1)",
					 "(/ -1 1)",
					 "(/ 1 -1)"};

    for(auto s : programs){
      Expression result = run(s);
      REQUIRE(result == Expression(-1.));
    }
  }
}


TEST_CASE( "Test some semantically invalid expresions", "[interpreter]" ) {
  
  std::vector<std::string> programs = {"(@ none)", // so such procedure
				       "(- 1 1 2)", // too many arguments
				       "(define begin 1)", // redefine special form
				       "(define pi 3.14)"}; // redefine builtin symbol
    for(auto s : programs){
      Interpreter interp;

      std::istringstream iss(s);
      
      bool ok = interp.parseStream(iss);
      REQUIRE(ok == true);
      
   //   REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
    }
}

TEST_CASE( "Test for exceptions from semantically incorrect input", "[interpreter]" ) {

  std::string input = R"(
(+ 1 a)
)";

  Interpreter interp;
  
  std::istringstream iss(input);
  
  bool ok = interp.parseStream(iss);
  REQUIRE(ok == true);
  
  REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
}

TEST_CASE( "Test malformed define", "[interpreter]" ) {

    std::string input = R"(
(define a 1 2)
)";

  Interpreter interp;
  
  std::istringstream iss(input);
  
  bool ok = interp.parseStream(iss);
  REQUIRE(ok == true);
  
  REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
}

TEST_CASE( "Test using number as procedure", "[interpreter]" ) {
    std::string input = R"(
(1 2 3)
)";

  Interpreter interp;
  
  std::istringstream iss(input);
  
  bool ok = interp.parseStream(iss);
  REQUIRE(ok == true);
  
  REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
}
///
///



TEST_CASE("Test foo string", "[interpreter]") {
	{
		std::string program = "(\"foo\")";
		Expression result = run(program);
		REQUIRE(result == Expression(Atom("\"foo\"")));
	}
}

TEST_CASE("Test string with spaces", "[interpreter]") {
	{

		std::string program = "(\"a string with spaces\")";
		Expression result = run(program);
		REQUIRE(result == Expression(Atom("\"a string with spaces\"")));
	}
}

TEST_CASE("Test define with string", "[interpreter]") {
	{
		std::string program = "(begin (define x (\"foo\")) (x))";
		INFO(program);
		Expression result = run(program);
		REQUIRE(result == Expression(Atom("\"foo\"")));
	}
}

TEST_CASE("Test list with string", "[interpreter]") {
	{
		std::string program = "(begin (define mylist (list \"eggs\" \"bread\" \"milk\" \"bacon\")) (first mylist))";
		INFO(program);
		Expression result = run(program);
		REQUIRE(result == Expression(Atom("\"eggs\"")));
	}
}

TEST_CASE("Tests with set-property function", "[interpreter]") {
	{
		std::string program = "(set-property \"number\" \"three\" (3))";
		INFO(program);
		Expression result = run(program);
		REQUIRE(result == Expression(3));

		program = "(set-property \"number\" (3) \"three\")";
		INFO(program);
		result = run(program);
		REQUIRE(result == Expression(Atom("\"three\"")));

		Expression copytest(result);
		REQUIRE(copytest.head().isProperty());
		REQUIRE(!result.head().isNone());

		program = "(set-property \"number\" (+ 1 2) \"three\")";
		INFO(program);
		result = run(program);
		REQUIRE(result == Expression(Atom("\"three\"")));

		program = "(set-property \"label\" \"foo\" (lambda (x) (* 3 (+ 2 x))))";
		INFO(program);
		result = run(program);
		std::string program2 = "(lambda (x) (* 3 (+ 2 x)))";
		INFO(program2);
		Expression result2 = run(program2);
		Expression Copytest(result2);
		REQUIRE(!result2.head().isNone());
		REQUIRE(Copytest.head().isProcedure());
		//REQUIRE(result == result2);
		//figure out how to get equal operator to work with lambda
	}
}

TEST_CASE("Tests with get-property function", "[interpreter]") {
	{
		std::string program = "(begin (define a (+ 1 I)) (define b (set-property \"note\" \"a complex number\" a)) (get-property \"note\" b))";
		INFO(program);
		Expression result = run(program);
		REQUIRE(result == Expression(Atom("\"a complex number\"")));
		
		program = "(begin (define a (+ 1 I)) (define b (set-property \"note\" \"a complex number\" a)) (get-property \"foo\" b))";
		INFO(program);
		result = run(program);
		REQUIRE(result == Expression());
		
	}
}

TEST_CASE("Tests with Discrete function", "[interpreter]") {
	{
		std::string program;
		std::string initial;
		Expression result;

		initial = R"((begin (define make-point (lambda (x y) (set-property "size" (0) (set-property "object-name" "point" (list x y))))) (define make-line (lambda (p1 p2) (set-property "thickness" (1) (set-property "object-name" "line" (list p1 p2))))) (define make-text (lambda (str) (set-property "text-rotation" (0) (set-property "text-scale" (1.0) (set-property "position" (make-point 0 0) (set-property "object-name" "text" (str))))))) )";
		//INFO(program);
		//result = run(program);

		program = R"((discrete-plot (list (list (- 1) (- 1)) (list 1 1)))))";//no labels
		program = initial + program;
		INFO(program);
		result = run(program);
		
		program = R"((discrete-plot (list (list (- 1) (- 1)) (list 1 1)) (list (list "title" "The Title") (list "abscissa-label" "X Label") (list "ordinate-label" "Y Label")))))";
		program = initial + program;
		INFO(program);
		result = run(program);


		program = R"((begin (define f (lambda (x) (list x (+ (* 2 x) 1)))) (discrete-plot (map f (range -2 2 0.5)) (list (list "title" "The Data") (list "abscissa-label" "X Label") (list "ordinate-label" "Y Label") (list "text-scale" 1))))))";
		program = initial + program;
		INFO(program);
		result = run(program);
		
		REQUIRE(true);//didn't crash
		
	}
}

TEST_CASE("Tests with Continous function", "[interpreter]") {
	{

		std::string program;
		std::string initial;
		Expression result;

		initial = R"((begin (define make-point (lambda (x y) (set-property "size" (0) (set-property "object-name" "point" (list x y))))) (define make-line (lambda (p1 p2) (set-property "thickness" (1) (set-property "object-name" "line" (list p1 p2))))) (define make-text (lambda (str) (set-property "text-rotation" (0) (set-property "text-scale" (1.0) (set-property "position" (make-point 0 0) (set-property "object-name" "text" (str))))))) )";

		program = R"((begin (define f (lambda (x) (+ (* 2 x) 1))) (continuous-plot f (list -2 2) (list (list "title" "A continuous linear function") (list "abscissa-label" "x") (list "ordinate-label" "y"))))))";
		program = initial + program;
		INFO(program);
		result = run(program);
		

		program = R"((begin (define f (lambda (x) (sin x))) (continuous-plot f (list (- pi) pi) (list (list "title" "A continuous linear function") (list "abscissa-label" "x") (list "ordinate-label" "y"))))))";
		program = initial + program;
		INFO(program);
		result = run(program);

		program = R"((begin (define f (lambda (x) (/ 1 (+ 1 (^ e (- (* 20 x))))))) (continuous-plot f (list (- 1) 1)))))";
		program = initial + program;
		INFO(program);//no labels
		result = run(program);




		REQUIRE(true);//didn't crash

	}
}
