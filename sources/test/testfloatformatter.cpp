/***************************************************************************
 *   This file is part of awesome-widgets                                  *
 *                                                                         *
 *   awesome-widgets is free software: you can redistribute it and/or      *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   awesome-widgets is distributed in the hope that it will be useful,    *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with awesome-widgets. If not, see http://www.gnu.org/licenses/  *
 ***************************************************************************/


#include "testfloatformatter.h"

#include <QtTest>

#include "awfloatformatter.h"
#include "awtestlibrary.h"


void TestAWFloatFormatter::initTestCase()
{
    formatter = new AWFloatFormatter(nullptr);
}


void TestAWFloatFormatter::cleanupTestCase()
{
    delete formatter;
}


void TestAWFloatFormatter::test_values()
{
}


void TestAWFloatFormatter::test_count()
{
    // assign
    int count = 10 + AWTestLibrary::randomInt();
    formatter->setCount(count);
    QCOMPARE(formatter->count(), count);

    // test
    double value = AWTestLibrary::randomDouble();
    QString output = formatter->convert(value);
    QCOMPARE(output.count(), count);

    // reset
    formatter->setCount(0);
}


void TestAWFloatFormatter::test_fillChar()
{
    // assign
    char c = AWTestLibrary::randomChar();
    formatter->setFillChar(QChar(c));
    QCOMPARE(formatter->fillChar(), QChar(c));
    formatter->setCount(101);

    // test
    int value = AWTestLibrary::randomInt();
    QString output = formatter->convert(value);
    QVERIFY(output.startsWith(QChar(c)));

    // reset
    formatter->setFillChar(QChar());
    formatter->setCount(0);
}


void TestAWFloatFormatter::test_format()
{
    // assign
    QWARN("Lets assing 'z' formatter, it should cause a warning");
    formatter->setFormat('z');
    QCOMPARE(formatter->format(), 'f');
    formatter->setFormat('e');
    QCOMPARE(formatter->format(), 'e');

    // test
    double value = AWTestLibrary::randomDouble();
    QString output = formatter->convert(value);
    QVERIFY(output.contains('e'));

    // reset
    formatter->setFormat('f');
}


void TestAWFloatFormatter::test_precision()
{
    // assign
    int precision = 1 + AWTestLibrary::randomInt(5);
    formatter->setPrecision(precision);
    QCOMPARE(formatter->precision(), precision);

    // test
    double value = AWTestLibrary::randomDouble();
    QString output = formatter->convert(value);
    output.remove(QString("0."));
    QCOMPARE(output.count(), precision);

    // reset
    formatter->setPrecision(-1);
}


void TestAWFloatFormatter::test_multiplier()
{
    formatter->setPrecision(6);

    // assign
    double multiplier = AWTestLibrary::randomDouble();
    formatter->setMultiplier(multiplier);
    QCOMPARE(formatter->multiplier(), multiplier);

    // test
    double value = AWTestLibrary::randomDouble();
    QCOMPARE(formatter->convert(value),
             QString::number(value * multiplier, 'f', 6));

    // reset
    formatter->setMultiplier(1.0);
}


void TestAWFloatFormatter::test_summand()
{
    // assign
    double summand = AWTestLibrary::randomDouble();
    formatter->setSummand(summand);
    QCOMPARE(formatter->summand(), summand);

    // test
    double value = AWTestLibrary::randomDouble();
    QCOMPARE(formatter->convert(value),
             QString::number(value + summand, 'f', 6));

    // reset
    formatter->setSummand(1.0);
}


void TestAWFloatFormatter::test_copy()
{
    doRandom();
    AWFloatFormatter *newFormatter = formatter->copy(QString("/dev/null"), 1);

    QCOMPARE(newFormatter->count(), formatter->count());
    QCOMPARE(newFormatter->fillChar(), formatter->fillChar());
    QCOMPARE(newFormatter->format(), formatter->format());
    QCOMPARE(newFormatter->multiplier(), formatter->multiplier());
    QCOMPARE(newFormatter->precision(), formatter->precision());
    QCOMPARE(newFormatter->summand(), formatter->summand());
    QCOMPARE(newFormatter->number(), 1);

    delete newFormatter;
}


void TestAWFloatFormatter::doRandom()
{
    formatter->setCount(AWTestLibrary::randomInt());
    formatter->setFillChar(QChar(AWTestLibrary::randomChar()));
    formatter->setFormat(AWTestLibrary::randomChar());
    formatter->setMultiplier(AWTestLibrary::randomDouble());
    formatter->setPrecision(AWTestLibrary::randomInt());
    formatter->setSummand(AWTestLibrary::randomDouble());
}


QTEST_MAIN(TestAWFloatFormatter);