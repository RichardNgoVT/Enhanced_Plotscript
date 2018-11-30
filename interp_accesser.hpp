#ifndef INTERP_ACCESSER_HPP
#define INTERP_ACCESSER_HPP
//#include <thread>
//#include <mutex>
#include <fstream>
#include "thread_safe_queue.hpp"
#include "interpreter.hpp"
#include "semantic_error.hpp"
#include "startup_config.hpp"
class InterpAccesser {
public:
	//InterpAccesser();
	InterpAccesser();
	~InterpAccesser();
	void start();
	void stop();
	bool online();
	void push_in(const std::string & script);
	void push_out(const Expression & exp);
	bool try_pop_in(std::string & script);
	bool try_pop_out(Expression & exp);
	void wait_pop_in(std::string & script);
	void wait_pop_out(Expression & exp);






private:
	ThreadSafeQueue<std::string> inputQ;
	ThreadSafeQueue<Expression> outputQ;
	std::thread th1;
	bool running;
};

#endif