#include "tst_multiline.h"
#include <QDebug>
#include <QtTest>
#include <widgets/multiline.h>

MultiLine::MultiLine()
{
}

void MultiLine::initTestCase()
{
}

void MultiLine::cleanupTestCase()
{
}

void MultiLine::test()
{
    GraphView::MultiLineF c;
    c.append(QPointF{1, 1});
    c.append(QPointF{2, 2});
    c.append(QPointF{3, 3});

    auto i = c.lineBegin();

    auto line = *i;
    QLineF l1{1, 1, 2, 2};
    QCOMPARE(line, l1);

    ++i;
    line = *i;
    QLineF l2{2, 2, 3, 3};
    QCOMPARE(line, l2);

    ++i;
    QCOMPARE(i.hasNext(), false);
    QCOMPARE(i, c.lineEnd());

    QList<QLineF> lines;

}

QTEST_APPLESS_MAIN(MultiLine)

#include "tst_multiline.moc"
