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
	//QTimer *startExp;
	QTimer *startInter;
	QTimer *resetReady;
	QTimer *inEna;
	QTimer *inDis;

	public slots:
	void outputApp();
	void handleInput(QString inputtxt);
	void checkOutput();
	void canInterrupt();

private:
	bool waiting = false;
	
};

#endif