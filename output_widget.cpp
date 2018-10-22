#include "output_widget.hpp"

OutputWidget::OutputWidget(QWidget * parent) : QWidget(parent) {
	auto layout = new QGridLayout();
	//auto talk = new QGraphicsTextItem("heyy");
	//grapher.addItem(talk);
	viewer.setScene(&grapher);
	layout->addWidget(&viewer, 0, 0);
	setLayout(layout);
}





int OutputWidget::psEnter(QString inputtxt)
{
	std::istringstream stream(inputtxt.toStdString());
	
	Interpreter interp;
	std::ifstream PREifs(STARTUP_FILE);
	//if (!PREifs) {
	//  error("Prelambdas could not be established.");
	// return EXIT_FAILURE;
	//}

	interp.parseStream(PREifs);
	interp.evaluate();

	std::stringstream Qout;

	if (!interp.parseStream(stream)) {
		std::cerr << "Error: Invalid Program. Could not parse." << endl;
		return EXIT_FAILURE;
	}
	else {
		try {//notebookimplmentation
			Expression exp = interp.evaluate();
			//exp.HexpressVisual(exp.head(), exp.tailVector(), Expression(), 0);
			if (exp.isHeadNone() || exp.isHeadNumber() || exp.isHeadComplex() || exp.isHeadSymbol() || exp.isHeadPString())
			{
				grapher.clear();
				Qout << exp << std::endl;
				QString qstr = QString::fromStdString(Qout.str());
				auto text = new QGraphicsTextItem(qstr);
				grapher.addItem(text);
			}
			else if (exp.isHeadProcedure())
			{
				//remove thing from scene
			}
			else if (exp.isHeadProperty())
			{
				for (int i = 1; i < exp.tailVector().size(); i++)//maybe easier way to do this by using get-property somehow...
				{
					//exp.HexpressVisual(exp.tailVector()[i].tailVector()[0].head(), exp.tailVector()[i].tailVector()[0].tailVector(), Expression(), 0);
					//exp.HexpressVisual(exp.tailVector()[i].head(), exp.tailVector()[i].tailVector(), Expression(), 0);
					//cout << "!" <<exp.tailVector()[i].head().asPString() << "! !" << "object-name! " << exp.tailVector()[i].head().asPString().compare("object-name") << endl;// == "object-name"
					//cout << exp.tailVector()[i].head().asPString() << endl;
					if (exp.tailVector()[i].head().asPString() == "\"object-name\"")//for some reason, == doesn't work here
					{
						//cout << "THiS ONE: ";
						//exp.HexpressVisual(exp.tailVector()[i].tailVector()[0].head(), exp.tailVector()[i].tailVector()[0].tailVector(), Expression(), 0);
						if (exp.tailVector()[i].tailVector()[0].head().asPString() == "\"point\"")
						{
							//cout << exp.tailVector()[0].tailVector()[0].head().asNumber() << endl;
							//cout << "\nhere!\n" << exp.tailVector()[i].tailVector()[0].head().asPString();
							auto point = new QGraphicsEllipseItem;//(exp.tailVector()[0].tailVector()[0].head().asNumber(), exp.tailVector()[0].tailVector()[1].head().asNumber());
							point->setRect(exp.tailVector()[0].tailVector()[0].head().asNumber(), exp.tailVector()[0].tailVector()[1].head().asNumber(), exp.tailVector()[2].tailVector()[0].head().asNumber(), exp.tailVector()[2].tailVector()[0].head().asNumber());
							point->setBrush(Qt::SolidPattern);
							grapher.clear();
							grapher.addItem(point);
							//cout << exp.tailVector()[2].tailVector()[0].head().asNumber() << endl;
							return EXIT_SUCCESS;
						}
						else if (exp.tailVector()[i].tailVector()[0].head().asPString() == "\"lineXX\"")
						{
							exp.HexpressVisual(exp.tailVector()[0].tailVector()[0].tailVector()[0].head(), exp.tailVector()[0].tailVector()[0].tailVector()[0].tailVector(), Expression(), 0);
							exp.HexpressVisual(exp.tailVector()[0].tailVector()[0].tailVector()[0].tailVector()[0].head(), exp.tailVector()[0].tailVector()[0].tailVector()[0].tailVector()[0].tailVector(), Expression(), 0);
							exp.HexpressVisual(exp.tailVector()[0].tailVector()[0].tailVector()[0].tailVector()[1].head(), exp.tailVector()[0].tailVector()[0].tailVector()[0].tailVector()[1].tailVector(), Expression(), 0);

							exp.HexpressVisual(exp.tailVector()[0].tailVector()[0].tailVector()[1].head(), exp.tailVector()[0].tailVector()[1].tailVector()[0].tailVector(), Expression(), 0);
							exp.HexpressVisual(exp.tailVector()[0].tailVector()[0].tailVector()[1].tailVector()[0].head(), exp.tailVector()[0].tailVector()[0].tailVector()[1].tailVector()[0].tailVector(), Expression(), 0);
							exp.HexpressVisual(exp.tailVector()[0].tailVector()[0].tailVector()[1].tailVector()[1].head(), exp.tailVector()[0].tailVector()[0].tailVector()[1].tailVector()[1].tailVector(), Expression(), 0);
							auto line = new QGraphicsLineItem;
							//if (exp.tailVector()[0].tailVector()[0].etList() && exp.tailVector()[0].tailVector()[0].head().setList())
							line->setLine(exp.tailVector()[0].tailVector()[0].tailVector()[0].tailVector()[0].head().asNumber(), exp.tailVector()[0].tailVector()[0].tailVector()[0].tailVector()[1].head().asNumber(), exp.tailVector()[0].tailVector()[0].tailVector()[1].tailVector()[0].head().asNumber(), exp.tailVector()[0].tailVector()[0].tailVector()[1].tailVector()[1].head().asNumber());
							auto penMan = new QPen;

							penMan->setWidth(exp.tailVector()[2].tailVector()[0].head().asNumber());

							line->setPen(*penMan);
							grapher.clear();
							grapher.addItem(line);
							//cout << exp.tailVector()[2].tailVector()[0].head().asNumber() << endl;
							return EXIT_SUCCESS;
						}
						else if (exp.tailVector()[i].tailVector()[0].head().asPString() == "\"text\"")
						{

						}
						
					}
				}




				
			}











			
		}
		catch (const SemanticError & ex) {

			Qout.str(std::string());
			std::streambuf* old = std::cerr.rdbuf(Qout.rdbuf());
			std::cerr << ex.what() << std::endl;

			grapher.clear();
			QString qstr = QString::fromStdString(Qout.str());
			auto text = new QGraphicsTextItem(qstr);
			grapher.addItem(text);

			std::cerr.rdbuf(old);
			return EXIT_FAILURE;
		}
	}

	

	
	return EXIT_SUCCESS;
	//viewer.show();
}