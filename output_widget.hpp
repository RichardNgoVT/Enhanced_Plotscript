#ifndef OUTPUT_WIDGET_H
#define OUTPUT_WIDGET_H

#include <QtWidgets>
#include <fstream>

#include "interpreter.hpp"
#include "semantic_error.hpp"
#include "startup_config.hpp"

class OutputWidget : public QWidget {
	Q_OBJECT

public:
	OutputWidget(QWidget * parent = nullptr);
	QGraphicsView viewer;
	QGraphicsScene grapher;
	Interpreter interp;
	int handle_Expression(Expression exp, bool recurs);
	public slots :
		int psEnter(QString inputtxt);
		void resizeEvent(QResizeEvent* event);


private:
	
	
	
};


#endif