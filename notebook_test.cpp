#include <QTest>
#include "notebook_app.hpp"
#include "output_widget.hpp"
class NotebookTest : public QObject {
  Q_OBJECT

private slots:

  void initTestCase();
  void defaultTest();
  void plotscriptTests();
  void pointTests();
  void lineTests();
  void textTests();

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


/* Check location, check default font
*/
void NotebookTest::plotscriptTests()
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
	QTest::keyClicks(inwid, "(get-property \"key\" (3))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	auto viewwid = outwid->findChild<QGraphicsView *>();
	QVERIFY2(viewwid, "Could not find view widget");
	QVERIFY2(viewwid->items().size() == 1, "incorrect number of items in QGraphicsView");
	QGraphicsTextItem * text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[0]);
	QCOMPARE(text->toPlainText(), QString("NONE"));
	QCOMPARE(text->pos(), defaultPos);
	QCOMPARE(text->font(), defaultFontCheck.font());
	

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
	QCOMPARE(text->toPlainText(), QString("(-1)"));
	QCOMPARE(text->pos(), defaultPos);
	QCOMPARE(text->font(), defaultFontCheck.font());

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
	QCOMPARE(text->toPlainText(), QString("(-1,-1.22465e-16)"));
	QCOMPARE(text->pos(), defaultPos);
	QCOMPARE(text->font(), defaultFontCheck.font());

	/*
	(begin
	(define title "The Title")
	(title)
	)
	*/
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
	QCOMPARE(text->toPlainText(), QString("(\"The Title\")"));
	QCOMPARE(text->pos(), defaultPos);
	QCOMPARE(text->font(), defaultFontCheck.font());

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
	QCOMPARE(text->toPlainText(), QString("Error: Invalid Program. Could not parse."));
	QCOMPARE(text->pos(), defaultPos);
	QCOMPARE(text->font(), defaultFontCheck.font());

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
	QCOMPARE(text->toPlainText(), QString("Error in call to first, argument not a list"));
	QCOMPARE(text->pos(), defaultPos);
	QCOMPARE(text->font(), defaultFontCheck.font());
}
//check if filled in
void NotebookTest::pointTests()
{
	auto inwid = widget.findChild<InputWidget *>("input");
	auto outwid = widget.findChild<OutputWidget *>("output");

	//clears just in case
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);


	//testing (make-point 0 0)
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyClicks(inwid, "(make-point 0 0)");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	auto viewwid = outwid->findChild<QGraphicsView *>();
	QVERIFY2(viewwid, "Could not find view widget");
	QVERIFY2(viewwid->items().size() == 1, "incorrect number of items in QGraphicsView");
	QGraphicsEllipseItem * point = qgraphicsitem_cast<QGraphicsEllipseItem *>(viewwid->items()[0]);
	QBrush fillTest;
	fillTest.setColor(Qt::black);
	fillTest.setStyle(Qt::SolidPattern);
	QCOMPARE(point->brush(), fillTest);//check if filled in
	QRectF pos;
	double size = 0.0;
	pos.setRect(0.0-(size/2.0), 0.0-(size/2.0), size, size);
	QCOMPARE(point->rect(), pos);

	//testing (set-property "size" 20 (make-point 0 0))
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
	QCOMPARE(point->brush(), fillTest);//check if filled in
	size = 20;
	pos.setRect(0.0-(size / 2.0), 0.0-(size / 2.0), size, size);
	QCOMPARE(point->rect(), pos);

	/*
	(list
	(set-property "size" 1 (make-point 0 0))
	(set-property "size" 2 (make-point 4 0))
	(set-property "size" 4 (make-point 8 0))
	(set-property "size" 8 (make-point 16 0))
	(set-property "size" 16 (make-point 32 0))
	(set-property "size" 32 (make-point 64 0))
	)
	*/
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
		QCOMPARE(point->brush(), fillTest);//check if filled in
		size = 1.0*(pow(2, i));
		if (i == 0)//starting point at (0, 0)
		{
			pos.setRect(0.0-(size / 2.0), 0.0-(size / 2.0), size, size);
		}
		else
		{
			pos.setRect(1.0*pow(2, i+1)-(size / 2.0), 0.0-(size / 2.0), size, size);
		}
		QCOMPARE(point->rect(), pos);
	}
	
	/*
	(list
	(set-property "size" 1 (make-point 0 0))
	(set-property "size" 2 (make-point 0 4))
	(set-property "size" 4 (make-point 0 8))
	(set-property "size" 8 (make-point 0 16))
	(set-property "size" 16 (make-point 0 32))
	(set-property "size" 32 (make-point 0 64))
	)
	*/
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
		point = qgraphicsitem_cast<QGraphicsEllipseItem *>(viewwid->items()[(5-i)]);
		QCOMPARE(point->brush(), fillTest);//check if filled in
		size = 1.0*(pow(2, i));
		if (i == 0)//starting point at (0, 0)
		{
			pos.setRect(0.0-(size / 2.0), 0.0-(size / 2.0), size, size);
		}
		else
		{
			pos.setRect(0.0-(size / 2.0), 1.0*pow(2, i + 1)-(size / 2.0), size, size);
		}
		QCOMPARE(point->rect(), pos);
	}
}


void NotebookTest::lineTests()
{

	auto inwid = widget.findChild<InputWidget *>("input");
	auto outwid = widget.findChild<OutputWidget *>("output");

	//clears just in case
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);


	/*
	(make-line
	(make-point 0 0)
	(make-point 20 20))
	*/
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyClicks(inwid, "(make-line (make-point 0 0) (make-point 20 20))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	auto viewwid = outwid->findChild<QGraphicsView *>();
	QVERIFY2(viewwid, "Could not find view widget");
	QVERIFY2(viewwid->items().size() == 1, "incorrect number of items in QGraphicsView");
	QGraphicsLineItem * line = qgraphicsitem_cast<QGraphicsLineItem *>(viewwid->items()[0]);
	QLineF testLine;
	testLine.setLine(0.0, 0.0, 20.0, 20.0);
	QPen testPen;
	testPen.setColor(Qt::black);
	testPen.setWidth(1);
	QCOMPARE(line->line(), testLine);
	QCOMPARE(line->pen(), testPen);

	/*
	(set-property "thickness" (4)
	(make-line
		(make-point 0 0)
		(make-point 20 20))
	)
	*/
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyClicks(inwid, "(set-property \"thickness\" (4) (make-line (make-point 0 0) (make-point 20 20)))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QVERIFY2(viewwid->items().size() == 1, "incorrect number of items in QGraphicsView");
	line = qgraphicsitem_cast<QGraphicsLineItem *>(viewwid->items()[0]);
	testLine.setLine(0.0, 0.0, 20.0, 20.0);
	testPen.setWidth(4);
	QCOMPARE(line->line(), testLine);
	QCOMPARE(line->pen(), testPen);

	/*
	(list
	(make-line
		(make-point 0 0)
		(make-point 0 20))
	(make-line
		(make-point 10 0)
		(make-point 10 20))
	(make-line
		(make-point 20 0)
		(make-point 20 20))
	)
	*/
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyClicks(inwid, "(list (make-line (make-point 0 0) (make-point 0 20)) (make-line (make-point 10 0) (make-point 10 20)) (make-line (make-point 20 0) (make-point 20 20)))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QVERIFY2(viewwid->items().size() == 3, "incorrect number of items in QGraphicsView");
	for (int i = 0; i < 3; i++)
	{
		line = qgraphicsitem_cast<QGraphicsLineItem *>(viewwid->items()[(2-i)]);
		testLine.setLine(i * 10.0, 0.0, i * 10.0, 20.0);
		testPen.setWidth(1);
		QCOMPARE(line->line(), testLine);
		QCOMPARE(line->pen(), testPen);
	}

	/*
	(list
	(make-line
		(make-point 0 0)
		(make-point 20 0))
	(make-line
		(make-point 0 10)
		(make-point 20 10))
	(make-line
		(make-point 0 20)
		(make-point 20 20))
	)
	*/
	QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_A, Qt::ControlModifier, 10);
	QTest::keyPress(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Backspace, Qt::NoModifier, 10);
	QTest::keyClicks(inwid, "(list (make-line (make-point 0 0) (make-point 20 0)) (make-line (make-point 0 10) (make-point 20 10)) (make-line (make-point 0 20) (make-point 20 20)))");
	QTest::keyPress(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QTest::keyRelease(inwid, Qt::Key_Return, Qt::ShiftModifier, 10);
	QVERIFY2(viewwid->items().size() == 3, "incorrect number of items in QGraphicsView");
	for (int i = 0; i < 3; i++)
	{
		line = qgraphicsitem_cast<QGraphicsLineItem *>(viewwid->items()[(2 - i)]);
		testLine.setLine(0.0, i * 10.0, 20.0, i * 10.0);
		testPen.setWidth(1);
		QCOMPARE(line->line(), testLine);
		QCOMPARE(line->pen(), testPen);
	}
	
}

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
	QGraphicsTextItem * text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[0]);
	QPointF posCheck;
	posCheck.setX(0.0);
	posCheck.setY(0.0);
	QCOMPARE(text->toPlainText(), QString("Hello World!"));
	QCOMPARE(text->pos(), posCheck);
	QCOMPARE(text->font(), defaultFontCheck.font());

	/*
	(begin
	(define xloc 0)
	(define yloc 0)
	(list
	(set-property "position" (make-point (+ xloc 20) yloc) (make-text "Hi"))
	(set-property "position" (make-point (+ xloc 40) yloc) (make-text "Hi"))
	(set-property "position" (make-point (+ xloc 60) yloc) (make-text "Hi"))
	(set-property "position" (make-point (+ xloc 80) yloc) (make-text "Hi"))
	(set-property "position" (make-point (+ xloc 100) yloc) (make-text "Hi"))
	)
	)
	*/
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
		posCheck.setX((i + 1)*20.0);
		posCheck.setY(0.0);
		QCOMPARE(text->toPlainText(), QString("Hi"));
		QCOMPARE(text->pos(), posCheck);
		QCOMPARE(text->font(), defaultFontCheck.font());
	}

	/*
	(begin
	(define xloc 0)
	(define yloc 0)
	(list
	(set-property "position" (make-point xloc (+ yloc 20)) (make-text "Hi"))
	(set-property "position" (make-point xloc (+ yloc 40)) (make-text "Hi"))
	(set-property "position" (make-point xloc (+ yloc 60)) (make-text "Hi"))
	(set-property "position" (make-point xloc (+ yloc 80)) (make-text "Hi"))
	(set-property "position" (make-point xloc (+ yloc 100)) (make-text "Hi"))
	)
	)
	*/
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
		posCheck.setX(0.0);
		posCheck.setY((i + 1)*20.0);
		QCOMPARE(text->toPlainText(), QString("Hi"));
		QCOMPARE(text->pos(), posCheck);
		QCOMPARE(text->font(), defaultFontCheck.font());
	}

}
QTEST_MAIN(NotebookTest)
#include "notebook_test.moc"
