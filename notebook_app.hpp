#ifndef NOTEBOOK_APP_H
#define NOTEBOOK_APP_H

#include <QWidget>
#include <QApplication>
#include "input_widget.hpp"
#include "output_widget.hpp"

class NotebookApp : public QWidget {
	Q_OBJECT

public:
	NotebookApp(QWidget * parent = nullptr);//notebookapp
	input *InputWidget;
	output *OutputWidget;
	public slots:
	void outputApp();

private:


};

#endif