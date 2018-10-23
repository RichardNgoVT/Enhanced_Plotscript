#include <QApplication>

#include "notebook_app.hpp"
#include <iostream>

using namespace std;
//notebook_app.cpp

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

 // InputWidget Input;
 // InputWidget Input2;
 // OutputWidget Output;

 // Output.setOut(&Input);
 // Output.setOut(&Input2);

 
  //Output.viewer.show();




  NotebookApp Widget;

  Widget.show();
  /*
  auto input = new InputWidget();
  QGraphicsView viewer;
  QGraphicsLinearLayout *layer = new QGraphicsLinearLayout;
  QGraphicsScene grapher;

  QGraphicsWidget *inputTxt = grapher.addWidget(input);
  QGraphicsWidget *outputTxt = grapher.addWidget(new QTextEdit);
 // QObject::connect(input, &InputWidget::sftent, output, &OutputWidget::psEnter);
  layer->addItem(inputTxt);
  layer->addItem(outputTxt);
  QGraphicsWidget *form = new QGraphicsWidget;
  form->setLayout(layer);
  grapher.addItem(form);
  viewer.setScene(&grapher);
  viewer.show();
  
 */
 // Notebook.show();
  //output.show();

  return app.exec();
}
