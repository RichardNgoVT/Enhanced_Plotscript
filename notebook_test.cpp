#include <QTest>
#include "notebook_app.hpp"
class NotebookTest : public QObject {
  Q_OBJECT

private slots:

  void initTestCase();
  void defaultTest();
  void plotscriptTests();
  void pointTests();

  // TODO: implement additional tests here
  
private:
	NotebookApp widget;
};

void NotebookTest::initTestCase(){
	widget.show();
}

void NotebookTest::defaultTest() {
	auto inwid = widget.findChild<InputWidget *>("input");
	auto outwid = widget.findChild<OutputWidget *>("output");
	QVERIFY2(inwid, "Could not find input widget with name: 'input'");
	QVERIFY2(outwid, "Could not find out widget with name: 'output'");
}


/* Check location
*/
void NotebookTest::plotscriptTests()
{
	//testing (get-property "key" (3))
	auto inwid = widget.findChild<InputWidget *>("input");
	auto outwid = widget.findChild<OutputWidget *>("output");

	//clears just in case
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);

	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyClicks(inwid, "(get-property \"key\" (3))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	auto viewwid = outwid->findChild<QGraphicsView *>();
	QVERIFY2(viewwid, "Could not find view widget");
	QVERIFY2(viewwid->items().size() == 1, "incorrect number of items in QGraphicsView");
	QGraphicsTextItem * text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[0]);
	QCOMPARE(text->toPlainText(), QString("NONE\n"));

	//testing (cos pi)
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyClicks(inwid, "(cos pi)");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QVERIFY2(viewwid->items().size() == 1, "incorrect number of items in QGraphicsView");
	text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[0]);
	QCOMPARE(text->toPlainText(), QString("(-1)\n"));

	//testing (^ e (- (* I pi)))
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyClicks(inwid, "(^ e (- (* I pi)))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QVERIFY2(viewwid->items().size() == 1, "incorrect number of items in QGraphicsView");
	text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[0]);
	QCOMPARE(text->toPlainText(), QString("(-1,-1.22465e-16)\n"));

	//testing (begin (define title "The Title") (title))
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyClicks(inwid, "(begin (define title \"The Title\") (title))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QVERIFY2(viewwid->items().size() == 1, "incorrect number of items in QGraphicsView");
	text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[0]);
	QCOMPARE(text->toPlainText(), QString("(\"The Title\")\n"));

	//testing (define inc (lambda(x) (+ x 1)))
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyClicks(inwid, "(define inc (lambda (x) (+x 1)))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QVERIFY2(viewwid->items().size() == 0, "incorrect number of items in QGraphicsView");

	//testing (begin))
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyClicks(inwid, "(begin))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QVERIFY2(viewwid->items().size() == 1, "incorrect number of items in QGraphicsView");
	text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[0]);
	QCOMPARE(text->toPlainText(), QString("Error: Invalid Program. Could not parse.\n"));

	//testing (begin (define a I) (first a))
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyClicks(inwid, "(begin (define a I) (first a))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QVERIFY2(viewwid->items().size() == 1, "incorrect number of items in QGraphicsView");
	text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[0]);
	QCOMPARE(text->toPlainText(), QString("Error in call to first, argument not a list\n"));
}

void NotebookTest::pointTests()
{
	//testing (make-point 0 0)
	auto inwid = widget.findChild<InputWidget *>("input");
	auto outwid = widget.findChild<OutputWidget *>("output");

	//clears just in case
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);

	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyClicks(inwid, "(make-point 0 0)");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	auto viewwid = outwid->findChild<QGraphicsView *>();
	QVERIFY2(viewwid, "Could not find view widget");
	QVERIFY2(viewwid->items().size() == 1, "incorrect number of items in QGraphicsView");
	QGraphicsEllipseItem * point = qgraphicsitem_cast<QGraphicsEllipseItem *>(viewwid->items()[0]);
	QCOMPARE(point->brush(), Qt::SolidPattern);//check if filled in
	QRectF pos;
	double size = 0.0;
	pos.setRect(0.0-(size/2.0), 0.0-(size/2.0), size, size);
	QCOMPARE(point->rect(), pos);

	//(set-property "size" 20 (make-point 0 0))
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyClicks(inwid, "(set-property \"size\" 20 (make-point 0 0))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QVERIFY2(viewwid->items().size() == 1, "incorrect number of items in QGraphicsView");
	point = qgraphicsitem_cast<QGraphicsEllipseItem *>(viewwid->items()[0]);
	QCOMPARE(point->brush(), Qt::SolidPattern);//check if filled in
	size = 20;
	pos.setRect(0.0 - (size / 2.0), 0.0 - (size / 2.0), size, size);
	QCOMPARE(point->rect(), pos);

	//(list (set-property "size" 1 (make-point 0 0)) (set-property "size" 2 (make-point 4 0)) (set-property "size" 4 (make-point 8 0)) (set-property "size" 8 (make-point 16 0)) (set-property "size" 16 (make-point 32 0)) (set-property "size" 32 (make-point 64 0)))
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyClicks(inwid, "(list (set-property \"size\" 1 (make-point 0 0)) (set-property \"size\" 2 (make-point 4 0)) (set-property \"size\" 4 (make-point 8 0)) (set-property \"size\" 8 (make-point 16 0)) (set-property \"size\" 16 (make-point 32 0)) (set-property \"size\" 32 (make-point 64 0)))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QVERIFY2(viewwid->items().size() == 6, "incorrect number of items in QGraphicsView");
	for (int i = 0; i < 6; i++)
	{
		point = qgraphicsitem_cast<QGraphicsEllipseItem *>(viewwid->items()[(5-i)]);
		QCOMPARE(point->brush(), Qt::SolidPattern);//check if filled in
		size = 1.0*(pow(2, i));
		if (i == 0)//starting point at (0, 0)
		{
			pos.setRect(0.0 - (size / 2.0), 0.0 - (size / 2.0), size, size);
		}
		else
		{
			pos.setRect(1.0*pow(2, i+1) - (size / 2.0), 0.0 - (size / 2.0), size, size);
		}
		QCOMPARE(point->rect(), pos);
	}
	
	//(list (set-property "size" 1 (make-point 0 0)) (set-property "size" 2 (make-point 0 4)) (set-property "size" 4 (make-point 0 8)) (set-property "size" 8 (make-point 0 16)) (set-property "size" 16 (make-point 0 32)) (set-property "size" 32 (make-point 0 64)))
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyClicks(inwid, "(list (set-property \"size\" 1 (make-point 0 0)) (set-property \"size\" 2 (make-point 0 4)) (set-property \"size\" 4 (make-point 0 8)) (set-property \"size\" 8 (make-point 0 16)) (set-property \"size\" 16 (make-point 0 32)) (set-property \"size\" 32 (make-point 0 64)))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QVERIFY2(viewwid->items().size() == 6, "incorrect number of items in QGraphicsView");
	for (int i = 0; i < 6; i++)
	{
		point = qgraphicsitem_cast<QGraphicsEllipseItem *>(viewwid->items()[(5 - i)]);
		QCOMPARE(point->brush(), Qt::SolidPattern);//check if filled in
		size = 1.0*(pow(2, i));
		if (i == 0)//starting point at (0, 0)
		{
			pos.setRect(0.0 - (size / 2.0), 0.0 - (size / 2.0), size, size);
		}
		else
		{
			pos.setRect(0.0 - (size / 2.0), 1.0*pow(2, i + 1) - (size / 2.0), size, size);
		}
		QCOMPARE(point->rect(), pos);
	}
}
QTEST_MAIN(NotebookTest)
#include "notebook_test.moc"
