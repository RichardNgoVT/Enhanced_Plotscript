#ifndef OUTPUT_WIDGET_H
#define OUTPUT_WIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QGraphicsView>
#include <QGraphicsScene>
//#include <QPen>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include "interpreter.hpp"
#include "semantic_error.hpp"
#include "startup_config.hpp"

class OutputWidget : public QWidget {
	Q_OBJECT

public:
	OutputWidget(QWidget * parent = nullptr);
	
	public slots :
		int psEnter(QString inputtxt);



private:
	QGraphicsView viewer;
	QGraphicsScene grapher;
	
	
};


#endif