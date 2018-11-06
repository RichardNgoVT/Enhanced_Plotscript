#include "notebook_app.hpp"

NotebookApp::NotebookApp(QWidget * parent) : QWidget(parent) {

	// make some check boxes and give them names
	auto input = new InputWidget();
	input->setObjectName("input");
	//auto input2 = new InputWidget();
	auto output = new OutputWidget();
	output->setObjectName("output");
	//OutputWidget output;// = new OutputWidget();
	QObject::connect(input, &InputWidget::sftent, output, &OutputWidget::psEnter);//connects
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


	

	//layout->addWidget(input);//add input in top half, output in bottem half
	layout->addWidget(input);
	layout->addWidget(output);
	



	setLayout(layout);
	//show();

	
}



void NotebookApp::outputApp() {

	this->show();
}