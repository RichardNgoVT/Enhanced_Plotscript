#ifndef INPUT_WIDGET_H
#define INPUT_WIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
//notebook.cpp

class InputWidget : public QPlainTextEdit {
	Q_OBJECT

public:
	//example_widget(QPlainTextEdit * parent = nullptr);
	public slots :
	void keyPressEvent(QKeyEvent *event);//event from key presses
	void keyReleaseEvent(QKeyEvent *event);//event from key release
	void selfDisable();
	void selfEnable();

signals:
	void sftent(QString inputtxt);

private:
	bool pressed = false;
	//input and output widget


};

#endif
