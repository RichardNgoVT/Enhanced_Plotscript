#ifndef NOTEBOOK_APP_H
#define NOTEBOOK_APP_H

#include <QApplication>
#include "input_widget.hpp"
#include "output_widget.hpp"

class NotebookApp : public QWidget {
	Q_OBJECT

public:
	NotebookApp(QWidget * parent = nullptr);//notebookapp
	InputWidget *input;
	OutputWidget *output;
	QTimer *timer;
	QTimer *inEna;
	QTimer *inDis;

	public slots:
	void checkOutput();
	int psEnter(QString inputtxt);
	int startPressed();
	int stopPressed();
	int resetPressed();
	int interruptPressed();

	signals:
	void expOut(Expression exp);
	signals:
	void errorOut(std::string inputtxt);

private:
	InterpAccesser interpA;
	bool waiting = false;//nessesary to keep track of interupt
	
};

#endif