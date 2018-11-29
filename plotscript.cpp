#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <thread>
#include "interpreter.hpp"
#include "thread_safe_queue.hpp"
//#include "testing_stuff.hpp"
#include "semantic_error.hpp"
#include "startup_config.hpp"


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

void threadSender(ThreadSafeQueue<std::string> & inputQ, ThreadSafeQueue<Expression> & outputQ)
{

	Interpreter interp;
	std::string line;

	std::ifstream PREifs(STARTUP_FILE);
	interp.parseStream(PREifs);
	interp.evaluate();
	bool running = true;
	while (running == true)
	{

		inputQ.wait_and_pop(line);

		if (line == "%stop")
		{
			running = false;
			break;
		}

		std::istringstream expression(line);

		if (!interp.parseStream(expression)) {
			//Expression ErrorMsg;
			//ErrorMsg.head().setError("Invalid Expression. Could not parse.");
			//outputQ.push(ErrorMsg);
			Expression ErrorMsg;
			ErrorMsg.head().setError();
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
				continue;
			}
		}
	}
}

int eval_from_stream(std::istream & stream){
  //Interpreter interp;
  //std::ifstream PREifs(STARTUP_FILE);
  //if (!PREifs) {
  //  error("Prelambdas could not be established.");
  // return EXIT_FAILURE;
  //}
	//interp.parseStream(PREifs);
	//interp.evaluate();
	ThreadSafeQueue<std::string> inputQ;
	ThreadSafeQueue<Expression> outputQ;
	std::thread th1(threadSender, std::ref(inputQ), std::ref(outputQ));
	Expression exp;
	std::string line{ std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() };

	inputQ.push(line);
	outputQ.wait_and_pop(exp);
	std::cout << exp << std::endl;


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
	bool running = false;
  //Interpreter interp;
  //std::ifstream PREifs(STARTUP_FILE);
 
  //if (!PREifs) {
	//  error("Prelambdas could not be established.");
	 // return EXIT_FAILURE;
  //}
 
  //interp.parseStream(PREifs);
 // interp.evaluate();




	ThreadSafeQueue<std::string> inputQ;
	ThreadSafeQueue<Expression> outputQ;
	std::thread th1;
	//std::thread th1(threadSender, std::ref(inputQ), std::ref(outputQ));

	Expression exp;

  while(!std::cin.eof()){
    
    prompt();
    std::string line = readline();//block    


    if(line.empty()) continue;
	if (line == "%start")
	{
		if (running == true)
		{
			continue;
		}
		running = true;
		th1 = std::thread(threadSender, std::ref(inputQ), std::ref(outputQ));
		continue;
		
	}

	if (line == "%stop")
	{
		if (running == false)
		{
			continue;
		}
		running = false;
		inputQ.push(line);
		th1.join();
		continue;
	}

	if (line == "%reset")
	{
		if (running == true)
		{
			running = false;
			inputQ.push("%stop");
			th1.join();
		}
		running = true;
		th1 = std::thread(threadSender, std::ref(inputQ), std::ref(outputQ));
		continue;

	}

	if (running == false)
	{
		error("Error: interpreter kernel not running");
	}
	else
	{
		inputQ.push(line);

		outputQ.wait_and_pop(exp);
		if (!exp.isHeadError())
		{
			std::cout << exp << std::endl;
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

//std::thread th1(threadSender, script, expr);


int main(int argc, char *argv[])
{  

	//std::thread th1(threadSender, script, expr);

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
