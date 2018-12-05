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
	
	public slots :
	void resizeEvent(QResizeEvent* event);
	void startHandle(Expression exp);
	void outputError(std::string error);
signals:
	void waitforme();


private:
	//bool exitkey = false;
	//mutable std::mutex the_mutex;


};


#endif