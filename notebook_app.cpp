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
	timer->setSingleShot(true);
	//should be unessesary, but somehow interpA goes out of scope when accessed in a helper function
	//startExp = new QTimer(this);
	inEna = new QTimer(this);
	inEna->setSingleShot(true);
	inDis = new QTimer(this);
	inDis->setSingleShot(true);

	QObject::connect(input, &InputWidget::sftent, this, &NotebookApp::psEnter);//connects


	QObject::connect(startB, &QPushButton::pressed, this, &NotebookApp::startPressed);

	QObject::connect(stopB, &QPushButton::pressed, this, &NotebookApp::stopPressed);

	QObject::connect(resetB, &QPushButton::pressed, this, &NotebookApp::resetPressed);



	QObject::connect(interruptB, &QPushButton::pressed, this, &NotebookApp::interruptPressed);



	QObject::connect(timer, &QTimer::timeout, this, &NotebookApp::checkOutput);

	QObject::connect(this, &NotebookApp::expOut, output, &OutputWidget::startHandle);

	QObject::connect(this, &NotebookApp::errorOut, output, &OutputWidget::outputError);

	QObject::connect(inDis, &QTimer::timeout, input, &InputWidget::selfDisable);

	QObject::connect(inEna, &QTimer::timeout, input, &InputWidget::selfEnable);

	QObject::connect(output, &OutputWidget::waitforme, input, &InputWidget::selfEnable);
	


	QHBoxLayout * layoutButts = new QHBoxLayout();
	layoutButts->addWidget(startB);
	layoutButts->addWidget(stopB);
	layoutButts->addWidget(resetB);
	layoutButts->addWidget(interruptB);
	QVBoxLayout * layout = new QVBoxLayout();
	layout->addLayout(layoutButts);
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


int NotebookApp::startPressed()
{
	return psEnter("%start");
}

int NotebookApp::stopPressed()
{
	return psEnter("%stop");
}

int NotebookApp::resetPressed()
{
	return psEnter("%reset");
}

int NotebookApp::interruptPressed()
{
	if (interpA.online())
	{
		emit errorOut("Error: interpreter kernel interrupted");
		interpA.exit();
		inEna->start(0);
	}
	waiting = false;
	return EXIT_FAILURE;
	//std::lock_guard<std::mutex> lock(the_mutex);
	//exitkey = true;
}



void NotebookApp::checkOutput()
{
	Expression exp;
	if (interpA.try_pop_out(exp) && waiting == true)
	{
		if (exp.isHeadError())
		{
			emit errorOut(exp.head().asError());
		}
		else
		{
			emit expOut(exp);
		}
		//inEna->start(0);
		waiting = false;
	}
	else
	{
		timer->start(0);
	}
}

int NotebookApp::psEnter(QString inputtxt)
{
	//	viewer.fitInView(grapher.itemsBoundingRect(), Qt::KeepAspectRatio);
	//std::cout << inputtxt.toStdString() << "\n";//TESTING
	//std::istringstream stream(inputtxt.toStdString());
	//bool why = interpA.online();
	/*
	std::ifstream PREifs(STARTUP_FILE);
	//if (!PREifs) {
	//  error("Prelambdas could not be established.");
	// return EXIT_FAILURE;
	//}

	interp.parseStream(PREifs);
	interp.evaluate();
	*/

	/*
	if (!interp.parseStream(stream)) {
	std::stringstream Qout;
	Qout.str(std::string());
	std::streambuf* old = std::cerr.rdbuf(Qout.rdbuf());
	std::cerr << "Error: Invalid Program. Could not parse.";

	grapher.clear();
	QString qstr = QString::fromStdString(Qout.str());
	auto text = new QGraphicsTextItem(qstr);
	text->setPos(0, 0);
	grapher.addItem(text);

	std::cerr.rdbuf(old);

	return EXIT_FAILURE;
	}
	else {
	try {//notebookimplmentation
	Expression exp = interp.evaluate();
	//exp.HexpressVisual(exp.head(), exp.tailVector(), Expression(), 0);
	handle_Expression(exp, false);

	}
	catch (const SemanticError & ex) {
	std::stringstream Qout;
	Qout.str(std::string());
	std::streambuf* old = std::cerr.rdbuf(Qout.rdbuf());
	std::cerr << ex.what();

	grapher.clear();
	QString qstr = QString::fromStdString(Qout.str());
	auto text = new QGraphicsTextItem(qstr);
	text->setPos(0, 0);
	grapher.addItem(text);

	std::cerr.rdbuf(old);
	return EXIT_FAILURE;
	}
	}
	*/
	std::string line = inputtxt.toStdString();
	//std::stringstream Qout;
	//std::cout << "HERE!" << "\n";//TESTING


	if (line == "%start")
	{
		interpA.start();
		return EXIT_SUCCESS;

	}

	if (line == "%stop")
	{
		interpA.stop();
		return EXIT_SUCCESS;
	}

	if (line == "%reset")
	{
		interpA.stop();
		interpA.start();
		return EXIT_SUCCESS;

	}

	if (interpA.online() == false)
	{
		emit errorOut("Error: interpreter kernel not running");
		return EXIT_FAILURE;
	}
	else
	{
		interpA.push_in(line);
		waiting = true;
		timer->start(0);
		inDis->start(0);
		
	}
	return EXIT_SUCCESS;
}