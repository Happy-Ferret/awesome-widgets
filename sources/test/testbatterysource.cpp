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


#include "testbatterysource.h"

#include <QtTest>

#include "awtestlibrary.h"
#include "batterysource.h"


void TestBatterySource::initTestCase()
{
    AWTestLibrary::init();
    source = new BatterySource(this, QStringList() << acpiPath);
}


void TestBatterySource::cleanupTestCase()
{
    delete source;
}


void TestBatterySource::test_sources()
{
    QVERIFY(source->sources().count() >= 2);
}


void TestBatterySource::test_battery()
{
    if (source->sources().count() == 2)
        QSKIP("No battery found, test will be skipped");

    QStringList batteries = source->sources();
    std::for_each(batteries.begin(), batteries.end(),
                  [this](const QString bat) {
                      QVariant value = source->data(bat);
                      if (bat == QString("battery/ac"))
                          QCOMPARE(value.type(), QVariant::Bool);
                      else
                          QVERIFY((value.toFloat() >= battery.first)
                                  && (value.toFloat() <= battery.second));
                  });
}


QTEST_MAIN(TestBatterySource);
