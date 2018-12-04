#ifndef OUTPUT_WIDGET_H
#define OUTPUT_WIDGET_H

#include <QtWidgets>
#include <fstream>

//#include "interpreter.hpp"
#include "interp_accesser.hpp"
#include "semantic_error.hpp"
#include "startup_config.hpp"

class OutputWidget : public QWidget {
	Q_OBJECT

public:
	OutputWidget(QWidget * parent = nullptr);
	QGraphicsView viewer;
	QGraphicsScene grapher;
	
	//Interperter interp;
	int handle_Expression(Expression exp, bool recurs);
	bool outputReady();
	
	public slots :
	int psEnter(QString inputtxt);
	int startPressed();
	int stopPressed();
	int resetPressed();
	int interruptPressed();
	void resizeEvent(QResizeEvent* event);
	void startHandle();
	void notReady();


private:
	//bool exitkey = false;
	mutable std::mutex the_mutex;
	InterpAccesser interpA;
	bool ready = false;
	
};


#endif