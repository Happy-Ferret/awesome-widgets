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

#include <Plasma/ToolTipManager>
#include <QGraphicsLinearLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QNetworkInterface>
#include <QProcessEnvironment>
#include <QTextCodec>
#include <QTimer>

#include "customlabel.h"
#include <pdebug/pdebug.h>
#include <task/taskadds.h>


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


int AwesomeWidget::getNumberCpus()
{
    if (debug) qDebug() << PDEBUG;

    QString cmd = QString("grep -c '^processor' /proc/cpuinfo");
    if (debug) qDebug() << PDEBUG << ":" << "cmd" << cmd;
    TaskResult process = runTask(cmd);
    if (debug) qDebug() << PDEBUG << ":" << "Cmd returns" << process.exitCode;
    if (process.exitCode != 0)
        if (debug) qDebug() << PDEBUG << ":" << "Error" << process.error;
    return QTextCodec::codecForMib(106)->toUnicode(process.output).toInt();
}


float AwesomeWidget::getTemp(const float temp)
{
    if (debug) qDebug() << PDEBUG;

    float convertedTemp = temp;
    if (configuration[QString("tempUnits")] == QString("Celsius"))
        ;
    else if (configuration[QString("tempUnits")] == QString("Fahrenheit"))
        convertedTemp = temp * 9.0 / 5.0 + 32.0;
    else if (configuration[QString("tempUnits")] == QString("Kelvin"))
        convertedTemp = temp + 273.15;
    else if (configuration[QString("tempUnits")] == QString("Reaumur"))
        convertedTemp = temp * 0.8;
    else if (configuration[QString("tempUnits")] == QString("cm^-1"))
        convertedTemp = temp * 0.695;
    else if (configuration[QString("tempUnits")] == QString("kJ/mol"))
        convertedTemp = temp * 8.31;
    else if (configuration[QString("tempUnits")] == QString("kcal/mol"))
        convertedTemp = temp * 1.98;

    return convertedTemp;
}


QStringList AwesomeWidget::getTimeKeys()
{
    if (debug) qDebug() << PDEBUG;

    QStringList timeKeys;
    timeKeys.append(QString("d"));
    timeKeys.append(QString("dd"));
    timeKeys.append(QString("ddd"));
    timeKeys.append(QString("dddd"));
    timeKeys.append(QString("M"));
    timeKeys.append(QString("MM"));
    timeKeys.append(QString("MMM"));
    timeKeys.append(QString("MMMM"));
    timeKeys.append(QString("yy"));
    timeKeys.append(QString("yyyy"));
    timeKeys.append(QString("h"));
    timeKeys.append(QString("hh"));
    timeKeys.append(QString("m"));
    timeKeys.append(QString("mm"));
    timeKeys.append(QString("s"));
    timeKeys.append(QString("ss"));

    return timeKeys;
}


void AwesomeWidget::init()
{
    if (debug) qDebug() << PDEBUG;

    // dataengines
    extsysmonEngine = dataEngine(QString("ext-sysmon"));
    sysmonEngine = dataEngine(QString("systemmonitor"));
    timeEngine = dataEngine(QString("time"));

    // tooltip
    toolTip = Plasma::ToolTipContent();
    toolTip.setMainText(QString("Awesome Widget"));
    toolTip.setSubText(QString(""));
    toolTipScene = new QGraphicsScene();
    toolTipView = new QGraphicsView(toolTipScene);
    toolTipView->setStyleSheet(QString("background: transparent"));
    toolTipView->setContentsMargins(0, 0, 0, 0);
    toolTipView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    toolTipView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    Plasma::ToolTipManager::self()->setContent(this, toolTip);

    // body
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


QStringList AwesomeWidget::findKeys()
{
    if (debug) qDebug() << PDEBUG;

    QStringList selectedKeys;
    for (int i=0; i<keys.count(); i++)
        if (configuration[QString("text")]
                .indexOf(QString("$") + keys[i] + QString("$")) > -1) {
            if (debug) qDebug() << PDEBUG << ":" << "Found key" << keys[i];
            selectedKeys.append(keys[i]);
        }

    return selectedKeys;
}


QStringList AwesomeWidget::getKeys()
{
    if (debug) qDebug() << PDEBUG;

    QStringList allKeys;
    // time
    allKeys.append(QString("time"));
    allKeys.append(QString("isotime"));
    allKeys.append(QString("shorttime"));
    allKeys.append(QString("longtime"));
    allKeys.append(QString("ctime"));
    // uptime
    allKeys.append(QString("uptime"));
    allKeys.append(QString("cuptime"));
    // cpu
    allKeys.append(QString("cpu"));
    for (int i=0; i<counts[QString("cpu")]; i++)
        allKeys.append(QString("cpu") + QString::number(i));
    // cpuclock
    allKeys.append(QString("cpucl"));
    for (int i=0; i<counts[QString("cpu")]; i++)
        allKeys.append(QString("cpucl") + QString::number(i));
    // temperature
    for (int i=0; i<counts[QString("temp")]; i++)
        allKeys.append(QString("temp") + QString::number(i));
    // gpu
    allKeys.append(QString("gpu"));
    // gputemp
    allKeys.append(QString("gputemp"));
    // memory
    allKeys.append(QString("mem"));
    allKeys.append(QString("memmb"));
    allKeys.append(QString("memgb"));
    allKeys.append(QString("memtotmb"));
    allKeys.append(QString("memtotgb"));
    // swap
    allKeys.append(QString("swap"));
    allKeys.append(QString("swapmb"));
    allKeys.append(QString("swapgb"));
    allKeys.append(QString("swaptotmb"));
    allKeys.append(QString("swaptotgb"));
    // hdd
    for (int i=0; i<counts[QString("mount")]; i++) {
        allKeys.append(QString("hdd") + QString::number(i));
        allKeys.append(QString("hddmb") + QString::number(i));
        allKeys.append(QString("hddgb") + QString::number(i));
        allKeys.append(QString("hddfreemb") + QString::number(i));
        allKeys.append(QString("hddfreegb") + QString::number(i));
        allKeys.append(QString("hddtotmb") + QString::number(i));
        allKeys.append(QString("hddtotgb") + QString::number(i));
    }
    // hdd speed
    for (int i=0; i<counts[QString("disk")]; i++) {
        allKeys.append(QString("hddr") + QString::number(i));
        allKeys.append(QString("hddw") + QString::number(i));
    }
    // hdd temp
    for (int i=0; i<counts[QString("hddtemp")]; i++) {
        allKeys.append(QString("hddtemp") + QString::number(i));
        allKeys.append(QString("hddtemp") + QString::number(i));
    }
    // network
    allKeys.append(QString("down"));
    allKeys.append(QString("up"));
    allKeys.append(QString("netdev"));
    // battery
    allKeys.append(QString("bat"));
    allKeys.append(QString("ac"));
    // player
    allKeys.append(QString("album"));
    allKeys.append(QString("artist"));
    allKeys.append(QString("duration"));
    allKeys.append(QString("progress"));
    allKeys.append(QString("title"));
    // ps
    allKeys.append(QString("ps"));
    allKeys.append(QString("pscount"));
    allKeys.append(QString("pstotal"));
    // package manager
    for (int i=0; i<counts[QString("pkg")]; i++)
        allKeys.append(QString("pkgcount") + QString::number(i));
    // custom
    for (int i=0; i<counts[QString("custom")]; i++)
        allKeys.append(QString("custom") + QString::number(i));
    // desktop
    allKeys.append(QString("desktop"));
    allKeys.append(QString("ndesktop"));
    allKeys.append(QString("tdesktops"));

    return allKeys;
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
