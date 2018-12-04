#include "output_widget.hpp"

Expression propFinder(Atom prop, Expression express)
{
	for (unsigned int i = 0; i < express.tailVector().size(); i++)
	{
		if (prop == express.tailVector()[i].head())
		{
			return express.tailVector()[i];
		}
	}
	
	return Expression();

}

OutputWidget::OutputWidget(QWidget * parent) : QWidget(parent) {
	auto layout = new QGridLayout();

	//fitInView(grapher.sceneRect(), Qt::KeepAspectRatio);
	//interpA = new InterpAccesser();
	viewer.setScene(&grapher);
	viewer.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	viewer.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	layout->addWidget(&viewer, 0, 0);
	setLayout(layout);
}

void OutputWidget::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	viewer.fitInView(grapher.itemsBoundingRect(), Qt::KeepAspectRatio);
}

void OutputWidget::startHandle()
{
	handle_Expression(Expression(), false);
}

void OutputWidget::notReady()
{
	ready = false;
}


int OutputWidget::startPressed()
{
	return psEnter("%start");
}

int OutputWidget::stopPressed()
{
	return psEnter("%stop");
}

int OutputWidget::resetPressed()
{
	return psEnter("%reset");
}

int OutputWidget::interruptPressed()
{
	if (interpA.online())
	{
		std::stringstream Qout;
		Qout.str(std::string());
		std::streambuf* old = std::cerr.rdbuf(Qout.rdbuf());
		std::cerr << "Error: interpreter kernel not running.";

		grapher.clear();
		QString qstr = QString::fromStdString(Qout.str());
		auto text = new QGraphicsTextItem(qstr);
		text->setPos(0, 0);
		grapher.addItem(text);

		std::cerr.rdbuf(old);


		interpA.stop();
		interpA.start();
		
	}
	return EXIT_SUCCESS;
	//std::lock_guard<std::mutex> lock(the_mutex);
	//exitkey = true;
}


int OutputWidget::psEnter(QString inputtxt)
{
//	viewer.fitInView(grapher.itemsBoundingRect(), Qt::KeepAspectRatio);
	std::cout << inputtxt.toStdString() << "\n";//TESTING
	std::istringstream stream(inputtxt.toStdString());
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
	std::cout << "HERE!" << "\n";//TESTING

		if (line == "%exit")
		{
			interpA.stop();
			return EXIT_SUCCESS;
		}
		
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
		std::cout << "HERE! before" << "\n";//TESTING
		if (interpA.online() == false)
		{
			std::cout << "HERE! error" << "\n";
			std::stringstream Qout;
			Qout.str(std::string());
			std::streambuf* old = std::cerr.rdbuf(Qout.rdbuf());
			std::cerr << "Error: interpreter kernel not running.";

			grapher.clear();
			QString qstr = QString::fromStdString(Qout.str());
			auto text = new QGraphicsTextItem(qstr);
			text->setPos(0, 0);
			grapher.addItem(text);

			std::cerr.rdbuf(old);

			return EXIT_FAILURE;
		}
		else
		{
			std::cout << "HERE! almost" << "\n";//TESTING
			std::cout << line << "\n";//TESTING
			interpA.push_in(line);
			std::cout << "HERE! DONE" << "\n";//TESTING
		}
			//viewer.fitInView(grapher.itemsBoundingRect(), Qt::KeepAspectRatio);
	return EXIT_SUCCESS;
}

int OutputWidget::handle_Expression(Expression expRec, bool recurs)
{
	Expression exp = expRec;
	std::stringstream Qout;
	if (recurs == false)
	{
		if (interpA.try_pop_out(exp))
		{
			ready = true;
			grapher.clear();
		}
		else
		{
			return EXIT_SUCCESS;
		}
		
	}
	if (exp.isHeadList())
	{
		for (unsigned int i = 0; i < exp.tailVector().size(); i++)
		{
			handle_Expression(exp.tailVector()[i], true);
		}
		return EXIT_SUCCESS;
	}
	else if (exp.isHeadProcedure())
	{
		return EXIT_SUCCESS;
		//remove thing from scene if nessesary
	}
	else
	{
		if (exp.isHeadProperty())
		{
			for (unsigned int i = 1; i < exp.tailVector().size(); i++)//might as well just check first property, maybe add helper for loop function later, returns expression
			{
				if (exp.tailVector()[i].head().asPString() == "\"object-name\"")
				{
					if (exp.tailVector()[i].tailVector()[0].head().asPString() == "\"point\"")
					{
						if (exp.tailVector()[2].tailVector()[0].head().asNumber() < 0)
						{
							Qout.str(std::string());
							std::streambuf* old = std::cerr.rdbuf(Qout.rdbuf());
							std::cerr << "Error in point: point's size can't be negative";

							grapher.clear();
							QString qstr = QString::fromStdString(Qout.str());
							auto text = new QGraphicsTextItem(qstr);
							text->setPos(0, 0);
							grapher.addItem(text);
							std::cerr.rdbuf(old);
							return EXIT_FAILURE;
						}
						auto point = new QGraphicsEllipseItem;
						point->setRect(exp.tailVector()[0].tailVector()[0].head().asNumber() - (exp.tailVector()[2].tailVector()[0].head().asNumber() / 2.0), exp.tailVector()[0].tailVector()[1].head().asNumber() - (exp.tailVector()[2].tailVector()[0].head().asNumber() / 2.0), exp.tailVector()[2].tailVector()[0].head().asNumber(), exp.tailVector()[2].tailVector()[0].head().asNumber());
					//	std::cout << exp.tailVector()[2].tailVector()[0].head().asNumber() << std::endl;
						point->setBrush(Qt::SolidPattern);
						auto penMan = new QPen;
						penMan->setWidth(0.0);
						point->setPen(*penMan);
						grapher.addItem(point);
						return EXIT_SUCCESS;
					}
					else if (exp.tailVector()[i].tailVector()[0].head().asPString() == "\"line\"")
					{
						if (exp.tailVector()[2].tailVector()[0].head().asNumber() < 0)
						{
							Qout.str(std::string());
							std::streambuf* old = std::cerr.rdbuf(Qout.rdbuf());
							std::cerr << "Error in line: line's thickness can't be negative";

							grapher.clear();
							QString qstr = QString::fromStdString(Qout.str());
							auto text = new QGraphicsTextItem(qstr);
							text->setPos(0, 0);
							grapher.addItem(text);

							std::cerr.rdbuf(old);
							return EXIT_FAILURE;
						}
						auto line = new QGraphicsLineItem;
						line->setLine(exp.tailVector()[0].tailVector()[0].tailVector()[0].tailVector()[0].head().asNumber(), exp.tailVector()[0].tailVector()[0].tailVector()[0].tailVector()[1].head().asNumber(), exp.tailVector()[0].tailVector()[1].tailVector()[0].tailVector()[0].head().asNumber(), exp.tailVector()[0].tailVector()[1].tailVector()[0].tailVector()[1].head().asNumber());
						auto penMan = new QPen;

						penMan->setWidth(exp.tailVector()[2].tailVector()[0].head().asNumber());

						line->setPen(*penMan);
						grapher.addItem(line);
						return EXIT_SUCCESS;
					}
					else if (exp.tailVector()[i].tailVector()[0].head().asPString() == "\"text\"")
					{
						if (exp.tailVector()[2].tailVector()[0].isHeadProperty() && exp.tailVector()[2].tailVector()[0].tailVector().size() == 3 && exp.tailVector()[2].tailVector()[0].tailVector()[1].head().asPString() == "\"object-name\"" && exp.tailVector()[2].tailVector()[0].tailVector()[1].tailVector().size() == 1 && exp.tailVector()[2].tailVector()[0].tailVector()[1].tailVector()[0].head().asPString() == "\"point\"")
						{
							std::string slice = exp.tailVector()[0].head().asPString().substr(1, exp.tailVector()[0].head().asPString().length() - 2);
							QString qstr = QString::fromStdString(slice);
							auto text = new QGraphicsTextItem(qstr);
							if(propFinder(Atom("\"text-scale\""), exp) == Expression() || !propFinder(Atom("\"text-scale\""), exp).tailVector()[0].isHeadNumber() ||  propFinder(Atom("\"text-scale\""), exp).tailVector()[0].head().asNumber() < 0.0)
							{
								
								text->setScale(1.0);
							}
							else
							{
								text->setScale(propFinder(Atom("\"text-scale\""), exp).tailVector()[0].head().asNumber());
							}
							
							
							text->setFont(QFont("Courier", 1));
							text->setPos(exp.tailVector()[2].tailVector()[0].tailVector()[0].tailVector()[0].head().asNumber()-(text->boundingRect().width() / 2.0), exp.tailVector()[2].tailVector()[0].tailVector()[0].tailVector()[1].head().asNumber()-(text->boundingRect().height() / 2.0));
							
							

							if (propFinder(Atom("\"text-rotation\""), exp) == Expression() || !propFinder(Atom("\"text-rotation\""), exp).tailVector()[0].isHeadNumber())
							{
								text->setRotation(0.0);
							}
							else
							{
								text->setTransformOriginPoint(QPoint(text->boundingRect().center().x(), text->boundingRect().center().y()));
								text->setRotation((propFinder(Atom("\"text-rotation\""), exp).tailVector()[0].head().asNumber() / (std::atan2(0, -1)))*180.0);
								/*
								QTransform transform;
								transform.translate(text->boundingRect().center().x(), text->boundingRect().center().y());
								transform.setRotation((propFinder(Atom("\"text-rotation\""), exp).tailVector()[0].head().asNumber() / (std::atan2(0, -1)))*180.0);
								transform.translate(-text->boundingRect().center().x(), -text->boundingRect().center().y());
								text->setTransform(transform);
								*/
							}


							grapher.addItem(text);

							return EXIT_SUCCESS;
						}
						else
						{
							Qout.str(std::string());
							std::streambuf* old = std::cerr.rdbuf(Qout.rdbuf());
							std::cerr << "Error in text: position is not a point";

							grapher.clear();
							QString qstr = QString::fromStdString(Qout.str());
							auto text = new QGraphicsTextItem(qstr);
							text->setPos(0, 0);
							grapher.addItem(text);

							std::cerr.rdbuf(old);
							return EXIT_FAILURE;
						}
					}

				}
			}
		}
		Qout << exp;
		QString qstr = QString::fromStdString(Qout.str());
		auto text = new QGraphicsTextItem(qstr);
		text->setPos(0, 0);
		grapher.addItem(text);
		return EXIT_SUCCESS;
	}

}

bool OutputWidget::outputReady()
{
	return ready;
	/*
	if (interpA.online())
	{
		return !(interpA.empty_out());
	}
	return false;
	*/
}

