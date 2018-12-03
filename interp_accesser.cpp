#include "interp_accesser.hpp"

void error2(const std::string & err_str) {
	std::cerr << "Error: " << err_str << std::endl;
}


void threadSender(ThreadSafeQueue<std::string> & inputQ, ThreadSafeQueue<Expression> & outputQ, Interpreter & interp)
{
	//std::string STARTUP_FILE;
	//
	
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
			error2("Invalid Expression. Could not parse.");
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


InterpAccesser::InterpAccesser()
{
	running = false;
}

InterpAccesser::~InterpAccesser()
{
	stop();
}

void InterpAccesser::start()
{
	Expression trash;
	if (running == true)
	{
		return;
	}
	
	while (!outputQ.empty())//just in case, get rid for interupt testing
	{
		outputQ.try_pop(trash);
	}
	
	running = true;
	interp.enable();
	th1 = std::thread(threadSender, std::ref(inputQ), std::ref(outputQ), std::ref(interp));
}

void InterpAccesser::stop()
{
	std::string trash;
	if (running == false)
	{
		return;
	}
	while (!inputQ.empty())
	{
		inputQ.try_pop(trash);
	}
	running = false;
	inputQ.push("%stop");
	th1.join();
	interp.reseter();
}

void InterpAccesser::exit()
{
	interp.disable();
	stop();
	start();

}

bool InterpAccesser::online()
{
	return running;
}

bool InterpAccesser::empty_in()
{
	return inputQ.empty();
}

bool InterpAccesser::empty_out()
{
	return outputQ.empty();
}

void InterpAccesser::push_in(const std::string & script)
{
	inputQ.push(script);
}

void InterpAccesser::push_out(const Expression & exp)
{
	outputQ.push(exp);
}

bool InterpAccesser::try_pop_in(std::string & script)
{
	return inputQ.try_pop(script);
}

bool InterpAccesser::try_pop_out(Expression & exp)
{
	return outputQ.try_pop(exp);
}

void InterpAccesser::wait_pop_in(std::string & script)
{
	inputQ.wait_and_pop(script);
}

void InterpAccesser::wait_pop_out(Expression & exp)
{
	outputQ.wait_and_pop(exp);
}
