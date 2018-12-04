#include "notebook_app.hpp"

NotebookApp::NotebookApp(QWidget * parent) : QWidget(parent) {

	// make some check boxes and give them names
	auto input = new InputWidget();
	input->setObjectName("input");
	//auto input2 = new InputWidget();
	auto output = new OutputWidget();
	output->setObjectName("output");
	//OutputWidget output;// = new OutputWidget();

	auto startB = new QPushButton("Start Kernel");
	startB->setObjectName("start");

	auto stopB = new QPushButton("Stop Kernel");
	stopB->setObjectName("stop");

	auto resetB = new QPushButton("Reset Kernel");
	resetB->setObjectName("reset");

	auto interruptB = new QPushButton("Interrupt Kernel");
	interruptB->setObjectName("interrupt");

	timer = new QTimer(this);
	//should be unessesary, but somehow interpA goes out of scope when accessed in a helper function
	//startExp = new QTimer(this);
	startInter = new QTimer(this);
	resetReady = new QTimer(this);
	inEna = new QTimer(this);
	inDis = new QTimer(this);

	QObject::connect(input, &InputWidget::sftent, output, &OutputWidget::psEnter);//connects
	QObject::connect(input, &InputWidget::sftent, this, &NotebookApp::handleInput);


	QObject::connect(startB, &QPushButton::pressed, output, &OutputWidget::startPressed);

	QObject::connect(stopB, &QPushButton::pressed, output, &OutputWidget::stopPressed);

	QObject::connect(resetB, &QPushButton::pressed, output, &OutputWidget::resetPressed);



	QObject::connect(interruptB, &QPushButton::pressed, output, &OutputWidget::interruptPressed);

	QObject::connect(startInter, &QTimer::timeout, output, &OutputWidget::interruptPressed);

	
	
	QObject::connect(timer, &QTimer::timeout, output, &OutputWidget::startHandle);

	QObject::connect(timer, &QTimer::timeout, this, &NotebookApp::checkOutput);

	QObject::connect(resetReady, &QTimer::timeout, output, &OutputWidget::notReady);
	
	//QObject::connect(input, &InputWidget::sftent, this, &NotebookApp::show);//connects
	//QGraphicsScene test;

	// layout the widgets
	//output->setOut();
	//QGraphicsView viewer;
	/*
	static QGraphicsScene scene;
	scene.addText("Hello, world!");

	static QGraphicsView viewer(&scene);
	*/
	/*
	QGraphicsScene* grapher = new QGraphicsScene(0, 0, 300, 300, &viewer);
	grapher->setBackgroundBrush(Qt::yellow);
	viewer.setScene(grapher);

	QGraphicsRectItem* rect = new QGraphicsRectItem(50, 50, 100, 100);
	grapher->addItem(rect);


	grapher->addText("supp");
	*/

	


	QVBoxLayout * layout = new QVBoxLayout();
	layout->addWidget(startB);
	layout->addWidget(stopB);
	layout->addWidget(resetB);
	layout->addWidget(interruptB);
	layout->addWidget(input);//add input in top half, output in bottem half
	layout->addWidget(output);

	/*
	auto layout = new QGridLayout;
	layout->addWidget(startB, 0, 0);
	layout->addWidget(stopB, 0, 1);
	layout->addWidget(resetB, 0, 2);
	layout->addWidget(interruptB, 0, 3);
	layout->addWidget(input, 1, 0, 4, 4);
	layout->addWidget(output, 5, 0, 4, 4);
	*/



	setLayout(layout);
	//show();

	
}



void NotebookApp::outputApp() {

	this->show();
}

void NotebookApp::handleInput(QString inputtxt)
{
	//inDis->start(0);
	timer->start(0);
	waiting = true;
}

void NotebookApp::canInterrupt()
{

	//if (waiting == true)
	//{
		startInter->start(0);
		waiting = false;
		//resetReady->start(0);//justincase
	//}
}



void NotebookApp::checkOutput()
{
	timer->start(0);
	/*
		if (output->outputReady() == false)
		{
			timer->start(0);
			//output->handle_Expression(Expression(), false);


		}
		else
		{
			waiting = false;
			resetReady->start(0);
		}
	*/
}
