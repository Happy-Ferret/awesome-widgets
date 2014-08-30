/***************************************************************************
 *   This file is part of pytextmonitor                                    *
 *                                                                         *
 *   pytextmonitor is free software: you can redistribute it and/or        *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   pytextmonitor is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with pytextmonitor. If not, see http://www.gnu.org/licenses/    *
 ***************************************************************************/

#include "awesome-widget.h"

#include <QGraphicsLinearLayout>
#include <QNetworkInterface>
#include <QProcessEnvironment>
#include <QTimer>

#include "customlabel.h"
#include <pdebug/pdebug.h>


AwesomeWidget::AwesomeWidget(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
{
    // debug
    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
    QString debugEnv = environment.value(QString("PTM_DEBUG"), QString("no"));
    if (debugEnv == QString("yes"))
        debug = true;
    else
        debug = false;

    setHasConfigurationInterface(true);
    // text format init
    formatLine.append(QString(""));
    formatLine.append(QString(""));
}


AwesomeWidget::~AwesomeWidget()
{
}


QString AwesomeWidget::getNetworkDevice()
{
    if (debug) qDebug() << PDEBUG;

    QString device = QString("lo");
    if (configuration[QString("useCustomNetdev")].toInt() == 2)
        device = configuration[QString("customNetdev")];
    else {
        QList<QNetworkInterface> rawInterfaceList = QNetworkInterface::allInterfaces();
        for (int i=0; i<rawInterfaceList.count(); i++)
            if ((rawInterfaceList[i].flags().testFlag(QNetworkInterface::IsUp)) &&
                    (rawInterfaceList[i].flags().testFlag(QNetworkInterface::IsLoopBack)))
                device = rawInterfaceList[i].name();
    }

    return device;
}


void AwesomeWidget::init()
{
    if (debug) qDebug() << PDEBUG;

    extsysmonEngine = dataEngine(QString("ext-sysmon"));
    sysmonEngine = dataEngine(QString("systemmonitor"));
    timeEngine = dataEngine(QString("time"));

    mainLayout = new QGraphicsLinearLayout();
    mainLayout->setContentsMargins(1, 1, 1, 1);
    setLayout(mainLayout);

    textLabel = new CustomLabel(this, debug);

    // read variables
    configChanged();
    timer = new QTimer(this);
    timer->setSingleShot(false);
    timer->setInterval(configuration[QString("interval")].toInt());
    connect(timer, SIGNAL(timeout()), this, SLOT(updateText()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTooltip()));
    timer->start();
}


//QString DesktopPanel::parsePattern(const QString rawLine, const int num)
//{
//    if (debug) qDebug() << PDEBUG;
//    if (debug) qDebug() << PDEBUG << ":" << "Run function with raw line" << rawLine;
//    if (debug) qDebug() << PDEBUG << ":" << "Run function with number" << num;

//    QString line, mark;
//    line = rawLine;
//    if (currentDesktop == num + 1)
//        mark = configuration[QString("mark")];
//    else
//        mark = QString("");
//    if (line.split(QString("$mark"))[0] != line) {
//        if (debug) qDebug() << PDEBUG << ":" << "Found mark";
//        line = line.split(QString("$mark"))[0] + mark + line.split(QString("$mark"))[1];
//    }
//    if (line.split(QString("$name"))[0] != line) {
//        if (debug) qDebug() << PDEBUG << ":" << "Found name";
//        line = line.split(QString("$name"))[0] + desktopNames[num] + line.split(QString("$name"))[1];
//    }
//    if (line.split(QString("$number"))[0] != line) {
//        if (debug) qDebug() << PDEBUG << ":" << "Found number";
//        line = line.split(QString("$number"))[0] + QString::number(num + 1) + line.split(QString("$number"))[1];
//    }
//    if (line.split(QString("$total"))[0] != line) {
//        if (debug) qDebug() << PDEBUG << ":" << "Found total";
//        line = line.split(QString("$total"))[0] + QString::number(desktopNames.count()) + line.split(QString("$total"))[1];
//    }

//    return line;
//}


K_EXPORT_PLASMA_APPLET(ptm-awesome-widget, AwesomeWidget)
