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


#include "testextscript.h"

#include <QtTest>

#include "extscript.h"
#include "version.h"


void TestExtScript::initTestCase()
{
    extScript = new ExtScript(nullptr);
    extScript->setInterval(1);
    extScript->setExecutable(QString("hello world"));
    extScript->setNumber(0);
    extScript->setRedirect(ExtScript::Redirect::stderr2stdout);
    extScript->setPrefix(QString("echo"));

    QVariantHash value = extScript->run();
    qDebug() << "Init values in first run" << value;
}


void TestExtScript::cleanupTestCase()
{
    delete extScript;
}


void TestExtScript::test_values()
{
    QCOMPARE(extScript->interval(), 1);
    QCOMPARE(extScript->number(), 0);
    QCOMPARE(extScript->executable(), QString("hello world"));
    QCOMPARE(extScript->strRedirect(), QString("stderr2stdout"));
    QCOMPARE(extScript->prefix(), QString("echo"));
}


void TestExtScript::test_run()
{
    // init spy
    QSignalSpy spy(extScript, SIGNAL(dataReceived(const QVariantHash &)));
    QVariantHash firstValue = extScript->run();
    QCOMPARE(firstValue[extScript->tag(QString("custom"))].toString(),
             QString(""));

    // check values
    QVERIFY(spy.wait(5000));
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(
        arguments.at(0).toHash()[extScript->tag(QString("custom"))].toString(),
        QString("\nhello world"));
}


void TestExtScript::test_filters()
{
    extScript->setFilters(QStringList() << QString("newline"));
    // init spy
    QSignalSpy spy(extScript, SIGNAL(dataReceived(const QVariantHash &)));
    extScript->run();

    // check values
    QVERIFY(spy.wait(5000));
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(
        arguments.at(0).toHash()[extScript->tag(QString("custom"))].toString(),
        QString("<br>hello world"));
}


QTEST_MAIN(TestExtScript);
