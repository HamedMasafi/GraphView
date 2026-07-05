#pragma once

#include <QObject>

class MultiLine : public QObject
{
    Q_OBJECT

public:
    MultiLine();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void test();
};
