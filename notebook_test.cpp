#include <QTest>
#include "notebook_app.hpp"
class NotebookTest : public QObject {
  Q_OBJECT

private slots:

  void initTestCase();
  void defaultTest();
  void plotscriptTests();

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

void NotebookTest::plotscriptTests()
{
	//testing (get-property "key" (3))
	auto inwid = widget.findChild<InputWidget *>("input");
	auto outwid = widget.findChild<OutputWidget *>("output");
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


QTEST_MAIN(NotebookTest)
#include "notebook_test.moc"
