#include <QTest>
#include "notebook_app.hpp"
#include "output_widget.hpp"
class NotebookTest : public QObject {
  Q_OBJECT

private slots:
  void initTestCase();
  void defaultTest();
 // void graphTests();
  void testDiscretePlotLayout();
  void testContinuousPlotLayout();
  void plotscriptTests();
  void pointTests();
  void lineTests();
  void textTests();
  void centerTester();
 
  
  // TODO: implement additional tests here
  
private:
	NotebookApp widget;
};

/*
findLines - find lines in a scene contained within a bounding box
with a small margin
*/
int findLines(QGraphicsScene * scene, QRectF bbox, qreal margin) {

	QPainterPath selectPath;

	QMarginsF margins(margin, margin, margin, margin);
	selectPath.addRect(bbox.marginsAdded(margins));
	scene->setSelectionArea(selectPath, Qt::ContainsItemShape);

	int numlines(0);
	foreach(auto item, scene->selectedItems()) {
		if (item->type() == QGraphicsLineItem::Type) {
			numlines += 1;
		}
	}

	return numlines;
}

/*
findPoints - find points in a scene contained within a specified rectangle
*/
int findPoints(QGraphicsScene * scene, QPointF center, qreal radius) {

	QPainterPath selectPath;
	selectPath.addRect(QRectF(center.x() - radius, center.y() - radius, 2 * radius, 2 * radius));
	scene->setSelectionArea(selectPath, Qt::ContainsItemShape);

	int numpoints(0);
	foreach(auto item, scene->selectedItems()) {
		if (item->type() == QGraphicsEllipseItem::Type) {
			numpoints += 1;
		}
	}

	return numpoints;
}

/*
findText - find text in a scene centered at a specified point with a given
rotation and string contents
*/
int findText(QGraphicsScene * scene, QPointF center, qreal rotation, QString contents) {

	int numtext(0);
	foreach(auto item, scene->items(center)) {
		if (item->type() == QGraphicsTextItem::Type) {
			QGraphicsTextItem * text = static_cast<QGraphicsTextItem *>(item);
			if ((text->toPlainText() == contents) &&
				(text->rotation() == rotation) &&
				(text->pos() + text->boundingRect().center() == center)) {
				numtext += 1;
			}
		}
	}

	return numtext;
}

/*
intersectsLine - find lines in a scene that intersect a specified rectangle
*/
int intersectsLine(QGraphicsScene * scene, QPointF center, qreal radius) {

	QPainterPath selectPath;
	selectPath.addRect(QRectF(center.x() - radius, center.y() - radius, 2 * radius, 2 * radius));
	scene->setSelectionArea(selectPath, Qt::IntersectsItemShape);

	int numlines(0);
	foreach(auto item, scene->selectedItems()) {
		if (item->type() == QGraphicsLineItem::Type) {
			numlines += 1;
			//qDebug() << "here";
		}
	}

	return numlines;
}


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
	pos.setRect(0.0 - (size / 2.0), 0.0 - (size / 2.0), size, size);
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
	pos.setRect(0.0 - (size / 2.0), 0.0 - (size / 2.0), size, size);
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
		point = qgraphicsitem_cast<QGraphicsEllipseItem *>(viewwid->items()[(5 - i)]);
		QCOMPARE(point->brush(), fillTest);//check if filled in
		size = 1.0*(pow(2, i));
		if (i == 0)//starting point at (0, 0)
		{
			pos.setRect(0.0 - (size / 2.0), 0.0 - (size / 2.0), size, size);
		}
		else
		{
			pos.setRect(1.0*pow(2, i + 1) - (size / 2.0), 0.0 - (size / 2.0), size, size);
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
		point = qgraphicsitem_cast<QGraphicsEllipseItem *>(viewwid->items()[(5 - i)]);
		QCOMPARE(point->brush(), fillTest);//check if filled in
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
	/*
	for (int i = 0; i < viewwid->items().size(); i++)
	{
		if (viewwid->items()[i]->type() == 8)
		{
			auto text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[i]);
			//qDebug() << '(' << viewwid->items()[i]->x() << ", " << viewwid->items()[i]->y() << "); Scale: " << viewwid->items()[i]->scale() << "; Type: " << viewwid->items()[i]->type() << "; Text: " << text->toPlainText();

			qDebug() << '(' << viewwid->items()[i]->sceneBoundingRect().center().x() << ", " << viewwid->items()[i]->sceneBoundingRect().center().y() << "); Scale: " << viewwid->items()[i]->scale() << "; Type: Text; Text: " << text->toPlainText() << "; Rotation: " << text->rotation();


		}
		else if (viewwid->items()[i]->type() == 4)
		{
			qDebug() << '(' << viewwid->items()[i]->sceneBoundingRect().center().x() << ", " << viewwid->items()[i]->sceneBoundingRect().center().y() << "); Scale: " << viewwid->items()[i]->scale() << "; Type: Point" << "; Width: " << viewwid->items()[i]->sceneBoundingRect().width() << "; Height: " << viewwid->items()[i]->sceneBoundingRect().height();
		}
		else
		{
			qDebug() << '(' << viewwid->items()[i]->sceneBoundingRect().center().x() << ", " << viewwid->items()[i]->sceneBoundingRect().center().y() << "); Scale: " << viewwid->items()[i]->scale() << "; Type: Line";
		}
		//qDebug() << viewwid->items()[0]->scale() << endl;
	}
	*/
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
		line = qgraphicsitem_cast<QGraphicsLineItem *>(viewwid->items()[(2 - i)]);
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
	//QGraphicsTextItem * text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[0]);
	auto sceneman = viewwid->scene();
	auto itemR = sceneman->itemAt(0.0, 0.0, QTransform());
	auto text = qgraphicsitem_cast<QGraphicsTextItem *>(itemR);

	QPointF posCheck;
	posCheck.setX(0.0 - (text->boundingRect().width() / 2.0));
	posCheck.setY(0.0 - (text->boundingRect().height() / 2.0));
	QCOMPARE(text->toPlainText(), QString("Hello World!"));
	QCOMPARE(text->pos(), posCheck);
	//qDebug() << '(' << text->sceneBoundingRect().center().x() << ", " << text->sceneBoundingRect().center().y() << "); Scale: " << text->scale() << "; Type: " << text->type() << "; Text: " << text->toPlainText();
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


void NotebookTest::centerTester()
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
	std::string program = R"(
(discrete-plot (list (list -1 -1) (list 1 1)) 
    (list (list "title" "The Title") 
          (list "abscissa-label" "X Label") 
          (list "ordinate-label" "Y Label")))
)";
	inwid->setPlainText(QString::fromStdString(program));
	QTest::keyClick(inwid, Qt::Key_Return, Qt::ShiftModifier);
	
	auto viewwid = outwid->findChild<QGraphicsView *>();
	QVERIFY2(viewwid, "Could not find view widget");
	//QCOMPARE(viewwid->items().size(), 31);
	//QGraphicsLineItem::type
	
	for (int i = 0; i < viewwid->items().size(); i++)
	{
		if (viewwid->items()[i]->type() == 8)
		{
			auto text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[i]);
			qDebug() << '(' << viewwid->items()[i]->x() << ", " << viewwid->items()[i]->y() << "); Scale: " << viewwid->items()[i]->scale() << "; Type: " << viewwid->items()[i]->type() << "; Text: " << text->toPlainText();
			//qDebug() << viewwid->items()[i]->sceneBoundingRect();
			//qDebug() << '(' << viewwid->items()[i]->sceneBoundingRect().center().x() << ", " << viewwid->items()[i]->sceneBoundingRect().center().y() << "); Scale: " << viewwid->items()[i]->scale() << "; Type: " << viewwid->items()[i]->type() << "; Text: " << text->toPlainText();


		}
		else
		{
		//	qDebug() << viewwid->items()[i]->sceneBoundingRect();
			qDebug() << '(' << viewwid->items()[i]->sceneBoundingRect().center().x() << ", " << viewwid->items()[i]->sceneBoundingRect().center().y() << "); Scale: " << viewwid->items()[i]->scale() << "; Type: " << viewwid->items()[i]->type();
		}
		//qDebug() << viewwid->items()[0]->scale() << endl;
	}
	//QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 100000);
	//QVERIFY2(viewwid->items().size() == 31, "incorrect number of items in QGraphicsView");
	// * text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[0]);
	//QCOMPARE(text->toPlainText(), QString("NONE"));
	//QCOMPARE(text->pos(), defaultPos);
	//QCOMPARE(text->font(), defaultFontCheck.font());
}
*/

void NotebookTest::testDiscretePlotLayout()
{
	auto inputWidget = widget.findChild<InputWidget *>("input");
	auto outputWidget = widget.findChild<OutputWidget *>("output");
	std::string program = R"( 
(discrete-plot (list (list -1 -1) (list 1 1)) 
    (list (list "title" "The Title") 
          (list "abscissa-label" "X Label") 
          (list "ordinate-label" "Y Label") ))
)";

	inputWidget->setPlainText(QString::fromStdString(program));
	QTest::keyClick(inputWidget, Qt::Key_Return, Qt::ShiftModifier);

	auto view = outputWidget->findChild<QGraphicsView *>();
	QVERIFY2(view, "Could not find QGraphicsView as child of OutputWidget");

	auto scene = view->scene();
	/*
	for (int i = 0; i < view->items().size(); i++)
	{
		if (view->items()[i]->type() == 8)
		{
			auto text = qgraphicsitem_cast<QGraphicsTextItem *>(view->items()[i]);
			//qDebug() << '(' << view->items()[i]->x() << ", " << view->items()[i]->y() << "); Scale: " << view->items()[i]->scale() << "; Type: " << view->items()[i]->type() << "; Text: " << text->toPlainText();

			qDebug() << '(' << view->items()[i]->sceneBoundingRect().center().x() << ", " << view->items()[i]->sceneBoundingRect().center().y() << "); Scale: " << view->items()[i]->scale() << "; Type: Text; Text: " << text->toPlainText() << "; Rotation: " << text->rotation();


		}
		else if(view->items()[i]->type() == 4)
		{
			qDebug() << '(' << view->items()[i]->sceneBoundingRect().center().x() << ", " << view->items()[i]->sceneBoundingRect().center().y() << "); Scale: " << view->items()[i]->scale() << "; Type: Point"  << "; Width: " << view->items()[i]->sceneBoundingRect().width() << "; Height: " << view->items()[i]->sceneBoundingRect().height();
		}
		else
		{
			qDebug() << '(' << view->items()[i]->sceneBoundingRect().center().x() << ", " << view->items()[i]->sceneBoundingRect().center().y() << "); Scale: " << view->items()[i]->scale() << "; Type: Line";
		}
		//qDebug() << view->items()[0]->scale() << endl;
	}
	*/
	//QTest::mouseClick(inputWidget, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 10000);
	// first check total number of items
	// 8 lines + 2 points + 7 text = 17
	auto items = scene->items();
	QCOMPARE(items.size(), 17);

	// make them all selectable
	foreach(auto item, items) {
		item->setFlag(QGraphicsItem::ItemIsSelectable);
	}

	double scalex = 20.0 / 2.0;
	double scaley = 20.0 / 2.0;

	double xmin = scalex * -1;
	double xmax = scalex * 1;
	double ymin = scaley * -1;
	double ymax = scaley * 1;
	double xmiddle = (xmax + xmin) / 2;
	double ymiddle = (ymax + ymin) / 2;

	// check title
	QCOMPARE(findText(scene, QPointF(xmiddle, -(ymax + 3)), 0, QString("The Title")), 1);

	// check abscissa label
	QCOMPARE(findText(scene, QPointF(xmiddle, -(ymin - 3)), 0, QString("X Label")), 1);

	// check ordinate label
	QCOMPARE(findText(scene, QPointF(xmin - 3, -ymiddle), -90, QString("Y Label")), 1);

	// check abscissa min label
	QCOMPARE(findText(scene, QPointF(xmin, -(ymin - 2)), 0, QString("-1")), 1);

	// check abscissa max label
	QCOMPARE(findText(scene, QPointF(xmax, -(ymin - 2)), 0, QString("1")), 1);

	// check ordinate min label
	QCOMPARE(findText(scene, QPointF(xmin - 2, -ymin), 0, QString("-1")), 1);

	// check ordinate max label
	QCOMPARE(findText(scene, QPointF(xmin - 2, -ymax), 0, QString("1")), 1);

	// check the bounding box bottom
	QCOMPARE(findLines(scene, QRectF(xmin, -ymin, 20, 0), 0.1), 1);

	// check the bounding box top
	QCOMPARE(findLines(scene, QRectF(xmin, -ymax, 20, 0), 0.1), 1);

	// check the bounding box left and (-1, -1) stem
	QCOMPARE(findLines(scene, QRectF(xmin, -ymax, 0, 20), 0.1), 2);

	// check the bounding box right and (1, 1) stem
	QCOMPARE(findLines(scene, QRectF(xmax, -ymax, 0, 20), 0.1), 2);

	// check the abscissa axis
	QCOMPARE(findLines(scene, QRectF(xmin, 0, 20, 0), 0.1), 1);

	// check the ordinate axis 
	QCOMPARE(findLines(scene, QRectF(0, -ymax, 0, 20), 0.1), 1);

	// check the point at (-1,-1)
	QCOMPARE(findPoints(scene, QPointF(-10, 10), 0.6), 1);

	// check the point at (1,1)
	QCOMPARE(findPoints(scene, QPointF(10, -10), 0.6), 1);
}

void NotebookTest::testContinuousPlotLayout()
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
	std::string program = R"(
(begin (define f (lambda (x) (sin x))) (continuous-plot f (list (- pi) pi) (list (list "title" "A continuous linear function") (list "abscissa-label" "x") (list "ordinate-label" "y"))))

)";
	inwid->setPlainText(QString::fromStdString(program));
	QTest::keyClick(inwid, Qt::Key_Return, Qt::ShiftModifier);

	auto viewwid = outwid->findChild<QGraphicsView *>();
	QVERIFY2(viewwid, "Could not find view widget");
	QCOMPARE(viewwid->items().size(), 67 + 12);
	
	//QGraphicsLineItem::type
	
	//double Ytest;
	//double Yvalue;
	//bool correct;
	//std::vector<double> Ystore;
	//double Yscale = 20 / (1 - (-1));
	//double Xscale = 20 / (std::atan2(0, -1) * 2);
	//auto scene = viewwid->scene();
	
	//for (double i = 0; i < 51; i++)
	//{
		//Ytest = sin(i);
	//	QVERIFY2(intersectsLine(scene, QPointF(i*Xscale, -Ytest * Yscale), .1), "No lines found near expected point");
	//	QCOMPARE(intersectsLine(scene, QPointF(0, 0), 100), 1);//can't find any lines
	//}
	//for (int i = 0; i < viewwid->items().size(); i++)
//	{
		//if (viewwid->items()[i]->type() == 6)
	//	{
			//auto line = qgraphicsitem_cast<QGraphicsLineItem *>(viewwid->items()[i]);//storelastvalue
			//Ytest = sin(line->x1);
			//Ytest = sin(scene->items()[i]->sceneBoundingRect().bottomLeft().x());
			//QVERIFY2(intersectsLine(scene, QPointF(scene->items()[i]->sceneBoundingRect().bottomLeft().x(), -Ytest*Yscale), 10), "No lines found near expected point");
			/*
			qDebug() << "\n" << viewwid->items()[i]->boundingRect().bottomLeft().y() << " " << viewwid->items()[i]->boundingRect().topLeft().y() << " " << Ytest;
			if (Ytest == viewwid->items()[i]->sceneBoundingRect().bottomLeft().y())
			{
				Yvalue = viewwid->items()[i]->sceneBoundingRect().bottomLeft().y();
			}
			else if(Ytest == viewwid->items()[i]->sceneBoundingRect().topLeft().y())
			{
				Yvalue = viewwid->items()[i]->sceneBoundingRect().topLeft().y();
			}
			else
			{
				Yvalue = 2.0;//impossible
			}
			QCOMPARE(Yvalue, Ytest);
			Ystore.push_back(Yvalue);
			*/
	//	}
	//}
	/*
	double angle;
	for (int i = 0; i < viewwid->items().size(); i++)
	{
		if (viewwid->items()[i]->type() == 4)
		{	
			angle = (180.0 / (std::atan2(0, -1))) * (acos((viewwid->items()[i]->sceneBoundingRect().center().x() - viewwid->items()[i - 1]->sceneBoundingRect().center().x()) / sqrt(pow((viewwid->items()[i]->sceneBoundingRect().center().x() - viewwid->items()[i - 1]->sceneBoundingRect().center().x()), 2.0) + pow((viewwid->items()[i]->sceneBoundingRect().center().y() - viewwid->items()[i - 1]->sceneBoundingRect().center().y()), 2.0))) - acos((viewwid->items()[i - 1]->sceneBoundingRect().center().x() - viewwid->items()[i - 2]->sceneBoundingRect().center().x()) / sqrt(pow((viewwid->items()[i - 1]->sceneBoundingRect().center().x() - viewwid->items()[i - 2]->sceneBoundingRect().center().x()), 2) + pow((viewwid->items()[i - 1]->sceneBoundingRect().center().y() - viewwid->items()[i - 2]->sceneBoundingRect().center().y()), 2))));
			if (angle < 0)
			{
				angle = angle * -1.0;
			}
			QVERIFY2(angle >= 175.0, "angle exists thats less than 175 degrees");
		}
	}
	*/
	//QTest::mouseClick(inwid, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0), 100000);
	//QVERIFY2(viewwid->items().size() == 31, "incorrect number of items in QGraphicsView");
	// * text = qgraphicsitem_cast<QGraphicsTextItem *>(viewwid->items()[0]);
	//QCOMPARE(text->toPlainText(), QString("NONE"));
	//QCOMPARE(text->pos(), defaultPos);
	//QCOMPARE(text->font(), defaultFontCheck.font());
}




QTEST_MAIN(NotebookTest)
#include "notebook_test.moc"
