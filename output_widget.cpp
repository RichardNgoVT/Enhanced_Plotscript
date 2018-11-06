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



int OutputWidget::psEnter(QString inputtxt)
{
	std::istringstream stream(inputtxt.toStdString());
	
	
	std::ifstream PREifs(STARTUP_FILE);
	//if (!PREifs) {
	//  error("Prelambdas could not be established.");
	// return EXIT_FAILURE;
	//}

	interp.parseStream(PREifs);
	interp.evaluate();

	

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


			viewer.fitInView(grapher.itemsBoundingRect(), Qt::KeepAspectRatio);


	
	
	



	return EXIT_SUCCESS;
}

int OutputWidget::handle_Expression(Expression exp, bool recurs)
{
	std::stringstream Qout;
	if (recurs == false)
	{
		grapher.clear();
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
						point->setBrush(Qt::SolidPattern);
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
								QTransform transform;
								transform.translate(text->boundingRect().center().x(), text->boundingRect().center().y());
								transform.rotate((propFinder(Atom("\"text-rotation\""), exp).tailVector()[0].head().asNumber() / (std::atan2(0, -1)))*180.0);
								transform.translate(-text->boundingRect().center().x(), -text->boundingRect().center().y());
								text->setTransform(transform);
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

