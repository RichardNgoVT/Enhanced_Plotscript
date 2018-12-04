#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
//#include <thread>

#include <stdio.h>     
#include <stdlib.h>  

#include "interpreter.hpp"
//#include "thread_safe_queue.hpp"
//#include "testing_stuff.hpp"
#include "interp_accesser.hpp"
#include "semantic_error.hpp"
#include "cntlc_tracer.hpp"
//#include "startup_config.hpp"


void prompt(){
  std::cout << "\nplotscript> ";
}

std::string readline(){
  std::string line;
  std::getline(std::cin, line);
 
  return line;
}

void error(const std::string & err_str){
  std::cerr << "Error: " << err_str << std::endl;
}

void info(const std::string & err_str){
  std::cout << "Info: " << err_str << std::endl;
}
/*
void threadSender(ThreadSafeQueue<std::string> & inputQ, ThreadSafeQueue<Expression> & outputQ)
{
	Interpreter interp;
	std::string line;

	std::ifstream PREifs(STARTUP_FILE);
	interp.parseStream(PREifs);
	interp.evaluate();
	bool running = true;

	Expression ErrorMsg;
	ErrorMsg.head().setError();

	while (running == true)
	{

		inputQ.wait_and_pop(line);

		if (line == "%stop")// || exitkey == true)
		{
			running = false;
			break;
		}

		std::istringstream expression(line);

		if (!interp.parseStream(expression)) {
			//Expression ErrorMsg;
			//ErrorMsg.head().setError("Invalid Expression. Could not parse.");
			//outputQ.push(ErrorMsg);
			error("Invalid Expression. Could not parse.");
			outputQ.push(ErrorMsg);
			continue;
		}
		else {
			try {
				Expression exp = interp.evaluate();
				outputQ.push(exp);
				//std::cout << exp << std::endl;
			}
			catch (const SemanticError & ex) {
				std::cerr << ex.what() << std::endl;
				outputQ.push(ErrorMsg);
				continue;
			}
		}
	}
}
*/
int eval_from_stream(std::istream & stream){
  //Interpreter interp;
  //std::ifstream PREifs(STARTUP_FILE);
  //if (!PREifs) {
  //  error("Prelambdas could not be established.");
  // return EXIT_FAILURE;
  //}
	//interp.parseStream(PREifs);
	//interp.evaluate();
	InterpAccesser interpA;
	interpA.start();
	Expression exp;
	std::string line{ std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() };

	interpA.push_in(line);
	interpA.wait_pop_out(exp);

	if (!exp.isHeadError())
	{
		std::cout << exp << std::endl;
	}
	interpA.stop();
	//th1.join();


	/*
  //relocate to thread
  if(!interp.parseStream(stream)){
    error("Invalid Program. Could not parse.");
    return EXIT_FAILURE;
  }
  else{
    try{
      Expression exp = interp.evaluate();
      std::cout << exp << std::endl;
    }
    catch(const SemanticError & ex){
      std::cerr << ex.what() << std::endl;
      return EXIT_FAILURE;
    }	
  }
  */
  return EXIT_SUCCESS;
  
}

int eval_from_file(std::string filename){
      
  std::ifstream ifs(filename);
  
  if(!ifs){
    error("Could not open file for reading.");
    return EXIT_FAILURE;
  }
  
  return eval_from_stream(ifs);
}

int eval_from_command(std::string argexp){
	
  std::istringstream expression(argexp);

 // ThreadSafeQueue();

  return eval_from_stream(expression);
}

// A REPL is a repeated read-eval-print loop
void repl(){
	InterpAccesser interpA;
	//interpA.start();
  //Interpreter interp;
  //std::ifstream PREifs(STARTUP_FILE);
 
  //if (!PREifs) {
	//  error("Prelambdas could not be established.");
	 // return EXIT_FAILURE;
  //}
 
  //interp.parseStream(PREifs);
 // interp.evaluate();


	

	Expression exp;

  while(true){//(!std::cin.eof()){
	  global_status_flag = 0;
    prompt();
    std::string line = readline();//block  
	if (std::cin.fail() || std::cin.eof()) {
		std::cin.clear(); // reset cin state
		line.clear(); //clear input string
	}

	

	if (line == "%exit")
	{
		interpA.stop();
		return;
	}
    if(line.empty()) continue;
	if (line == "%start")
	{
		interpA.start();
		continue;
		
	}

	if (line == "%stop")
	{
		interpA.stop();
		continue;
	}

	if (line == "%reset")
	{
		interpA.stop();
		interpA.start();
		continue;

	}

	if (interpA.online() == false)
	{
		error("interpreter kernel not running");
	}
	else
	{
		interpA.push_in(line);

		//while (interpA.try_pop_out(exp))
		//interpA.wait_pop_out(exp);
		
		while(interpA.empty_out())
		{
			if (global_status_flag > 0)
			{
				//std::cout << "Interrupted eval.\n";
				if (interpA.online())//not nessesary
				{
					error("interpreter kernel interrupted");
					interpA.exit();
				}
				
				break;
			}
		}
		if (global_status_flag == 0)
		{
			interpA.try_pop_out(exp);
			if (!exp.isHeadError())
			{
				std::cout << exp << std::endl;
			}
		}
		
	}

	
	/*
    std::istringstream expression(line);
    
    if(!interp.parseStream(expression)){
      error("Invalid Expression. Could not parse.");
    }
    else{
      try{
	Expression exp = interp.evaluate();
	std::cout << exp << std::endl;
      }
      catch(const SemanticError & ex){
	std::cerr << ex.what() << std::endl;
      }
    }
	*/
  }
}
/*
void goAwayThread(std::thread &th1, ThreadSafeQueue<std::string> &inputQ)
{
	inputQ.push("%stop");
	if (th1.joinable())
	{
		th1.join();
	}
}
*/
//std::thread th1(threadSender, script, expr);


int main(int argc, char *argv[])
{  
	install_handler();
  if(argc == 2){
    return eval_from_file(argv[1]);
  }
  else if(argc == 3){
    if(std::string(argv[1]) == "-e"){
      return eval_from_command(argv[2]);
    }
    else{
      error("Incorrect number of command line arguments.");
    }
  }
  else{
    repl();//do this as a thread?
  }
  return EXIT_SUCCESS;
}
