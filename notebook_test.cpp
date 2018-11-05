#include <QTest>
#include "notebook_app.hpp"
#include "output_widget.hpp"
class NotebookTest : public QObject {
  Q_OBJECT

private slots:

  void initTestCase();
  //void defaultTest();
  //void plotscriptTests();
  //void pointTests();
  //void lineTests();
  void textTests();
  void tester();
  //void graphTests();

  // TODO: implement additional tests here
  
private:
	NotebookApp widget;
};

void NotebookTest::initTestCase(){
	widget.show();
}
/*
void NotebookTest::defaultTest() {
	auto inwid = widget.findChild<InputWidget *>("input");
	auto outwid = widget.findChild<OutputWidget *>("output");
	QVERIFY2(inwid, "Could not find input widget with name: 'input'");
	QVERIFY2(outwid, "Could not find out widget with name: 'output'");
}

*/
/* Check location, check default font
*/



void NotebookTest::textTests()
{
	auto inwid = widget.findChild<InputWidget *>("input");
	auto outwid = widget.findChild<OutputWidget *>("output");
	QGraphicsTextItem defaultFontCheck;

	//clears just in case
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);

	//testing (make-text "Hello World!")
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyClicks(inwid, "(make-text \"Hello World!\")");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	auto viewwid = outwid->findChild<QGraphicsView *>();
	QVERIFY2(viewwid, "Could not find view widget");
	QVERIFY2(viewwid->items().size() == 1, "incorrect number of items in QGraphicsView");
	//QGraphicsTextItem * text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[0]);
	auto sceneman = viewwid->scene();
	auto itemR = sceneman->itemAt(0.0, 0.0, QTransform());
	auto text = qgraphicsitem_cast<QGraphicsTextItem *>(itemR);

	QPointF posCheck;
	posCheck.setX(0.0 - (text->boundingRect().width() / 2.0));
	posCheck.setY(0.0 - (text->boundingRect().height() / 2.0));
	QCOMPARE(text->toPlainText(), QString("Hello World!"));
	QCOMPARE(text->pos(), posCheck);
	qDebug() << '(' << text->sceneBoundingRect().center().x() << ", " << text->sceneBoundingRect().center().y() << "); Scale: " << text->scale() << "; Type: " << text->type() << "; Text: " << text->toPlainText();
	//Debug() << "Bounding Rect: TopL" << '(' << text->sceneBoundingRect().topLeft().x() << ", " << text->sceneBoundingRect().center().y() << "); TopR
	//QCOMPARE(text->font(), defaultFontCheck.font());

	

	//(begin
	//(define xloc 0)
	//(define yloc 0)
	//(list
	//(set-property "position" (make-point (+ xloc 20) yloc) (make-text "Hi"))
	//(set-property "position" (make-point (+ xloc 40) yloc) (make-text "Hi"))
	//(set-property "position" (make-point (+ xloc 60) yloc) (make-text "Hi"))
	//(set-property "position" (make-point (+ xloc 80) yloc) (make-text "Hi"))
	//(set-property "position" (make-point (+ xloc 100) yloc) (make-text "Hi"))
	//)
	//)

	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyClicks(inwid, "(begin (define xloc 0) (define yloc 0) (list (set-property \"position\" (make-point (+ xloc 20) yloc) (make-text \"Hi\")) (set-property \"position\" (make-point (+ xloc 40) yloc) (make-text \"Hi\")) (set-property \"position\" (make-point (+ xloc 60) yloc) (make-text \"Hi\")) (set-property \"position\" (make-point (+ xloc 80) yloc) (make-text \"Hi\")) (set-property \"position\" (make-point (+ xloc 100) yloc) (make-text \"Hi\"))))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QVERIFY2(viewwid->items().size() == 5, "incorrect number of items in QGraphicsView");
	for (int i = 0; i < 5; i++)
	{
		text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[(4 - i)]);
		posCheck.setX((i + 1)*20.0 - (text->boundingRect().width() / 2.0));
		posCheck.setY(0.0 - (text->boundingRect().height() / 2.0));
		QCOMPARE(text->toPlainText(), QString("Hi"));
		QCOMPARE(text->pos(), posCheck);
	//	QCOMPARE(text->font(), defaultFontCheck.font());
	}


	//(begin
	//(define xloc 0)
	//(define yloc 0)
	//(list
	//(set-property "position" (make-point xloc (+ yloc 20)) (make-text "Hi"))
	//(set-property "position" (make-point xloc (+ yloc 40)) (make-text "Hi"))
	//(set-property "position" (make-point xloc (+ yloc 60)) (make-text "Hi"))
	//(set-property "position" (make-point xloc (+ yloc 80)) (make-text "Hi"))
	//(set-property "position" (make-point xloc (+ yloc 100)) (make-text "Hi"))
	//)
	//)

	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyClicks(inwid, "(begin (define xloc 0) (define yloc 0) (list (set-property \"position\" (make-point xloc (+ yloc 20)) (make-text \"Hi\")) (set-property \"position\" (make-point xloc (+ yloc 40)) (make-text \"Hi\")) (set-property \"position\" (make-point xloc (+ yloc 60)) (make-text \"Hi\")) (set-property \"position\" (make-point xloc (+ yloc 80)) (make-text \"Hi\")) (set-property \"position\" (make-point xloc (+ yloc 100)) (make-text \"Hi\"))))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QVERIFY2(viewwid->items().size() == 5, "incorrect number of items in QGraphicsView");
	for (int i = 0; i < 5; i++)
	{
		text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[(4 - i)]);
		posCheck.setX(0.0 - (text->boundingRect().width() / 2.0));
		posCheck.setY((i + 1)*20.0 - (text->boundingRect().height() / 2.0));
		QCOMPARE(text->toPlainText(), QString("Hi"));
		QCOMPARE(text->pos(), posCheck);
	//	QCOMPARE(text->font(), defaultFontCheck.font());
	}

}


void NotebookTest::tester()
{
	auto inwid = widget.findChild<InputWidget *>("input");
	auto outwid = widget.findChild<OutputWidget *>("output");


	QString qstr = QString::fromStdString("(make-text \"hello world\")");
	
	inwid->setPlainText(qstr);

	QTest::keyClick(inwid, Qt::Key_Return, Qt::ShiftModifier);

	auto viewwid = outwid->findChild<QGraphicsView *>();

	auto sceneman = viewwid->scene();
	auto itemR = sceneman->itemAt(0.0, 0.0, QTransform());
	auto text = qgraphicsitem_cast<QGraphicsTextItem *>(itemR);
	QCOMPARE(text->toPlainText(), QString("hello world"));
	QCOMPARE(text->type(), 8);



}


/*


void NotebookTest::graphTests()
{
	auto inwid = widget.findChild<InputWidget *>("input");
	auto outwid = widget.findChild<OutputWidget *>("output");
	QGraphicsTextItem defaultFontCheck;
	QPointF defaultPos;
	defaultPos.setX(0.0);
	defaultPos.setY(0.0);


	//clears just in case
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);

	//testing (get-property "key" (3))
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyClicks(inwid, "(begin (define f (lambda(x) (list x (+ (* 2 x) 1)))) (discrete-plot (map f (range -2 2 0.5)) (list (list \"title\" \"The Data\") (list \"abscissa-label\" \"X Label\") (list \"ordinate-label\" \"Y Label\") (list \"text-scale\" 1))))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 5000);
	auto viewwid = outwid->findChild<QGraphicsView *>();
	QVERIFY2(viewwid, "Could not find view widget");
	QCOMPARE(viewwid->items().size(), 31);
	//QGraphicsLineItem::type
	for (int i = 0; i < viewwid->items().size(); i++)
	{
		if (viewwid->items()[i]->type() == 8)
		{
			auto text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[i]);
			//qDebug() << '(' << viewwid->items()[i]->x() << ", " << viewwid->items()[i]->y() << "); Scale: " << viewwid->items()[i]->scale() << "; Type: " << viewwid->items()[i]->type() << "; Text: " << text->toPlainText();
	
			qDebug() << '(' << viewwid->items()[i]->sceneBoundingRect().center().x() << ", " << viewwid->items()[i]->sceneBoundingRect().center().y() << "); Scale: " << viewwid->items()[i]->scale() << "; Type: " << viewwid->items()[i]->type() << "; Text: " << text->toPlainText();


		}
		else
		{
			qDebug() << '(' << viewwid->items()[i]->sceneBoundingRect().center().x() << ", " << viewwid->items()[i]->sceneBoundingRect().center().y() << "); Scale: " << viewwid->items()[i]->scale() << "; Type: " << viewwid->items()[i]->type();
		}
		//qDebug() << viewwid->items()[0]->scale() << endl;
	}
	//QVERIFY2(viewwid->items().size() == 31, "incorrect number of items in QGraphicsView");
	// * text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[0]);
	//QCOMPARE(text->toPlainText(), QString("NONE"));
	//QCOMPARE(text->pos(), defaultPos);
	//QCOMPARE(text->font(), defaultFontCheck.font());
	//qDebug() << "HEY1\n";
}

*/



QTEST_MAIN(NotebookTest)
#include "notebook_test.moc"
