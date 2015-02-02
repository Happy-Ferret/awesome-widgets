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

#include "awtooltip.h"

#include <QDebug>
#include <QProcessEnvironment>

#include <pdebug/pdebug.h>


AWToolTip::AWToolTip(QObject *parent,
                     QMap<QString, QVariant> settings)
    : QObject(parent),
      configuration(settings)
{
    // debug
    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
    QString debugEnv = environment.value(QString("DEBUG"), QString("no"));
    debug = (debugEnv == QString("yes"));

    toolTipScene = new QGraphicsScene();
    toolTipView = new QGraphicsView(toolTipScene);
    toolTipView->setStyleSheet(QString("background: transparent"));
    toolTipView->setContentsMargins(0, 0, 0, 0);
    toolTipView->setFrameShape(QFrame::NoFrame);
    toolTipView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    toolTipView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    counts += configuration[QString("cpuTooltip")].toInt();
    counts += configuration[QString("cpuclTooltip")].toInt();
    counts += configuration[QString("memTooltip")].toInt();
    counts += configuration[QString("swapTooltip")].toInt();
    counts += configuration[QString("downTooltip")].toInt();
    counts += configuration[QString("batteryTooltip")].toInt();

    boundaries[QString("cpu")] = 100.0;
    boundaries[QString("cpucl")] = 4000.0;
    boundaries[QString("mem")] = 100.0;
    boundaries[QString("swap")] = 100.0;
    boundaries[QString("down")] = 1.0;
    boundaries[QString("up")] = 1.0;
    boundaries[QString("bat")] = 100.0;

    if (configuration[QString("cpuTooltip")].toBool()) requiredKeys.append(QString("cpu"));
    if (configuration[QString("cpuclTooltip")].toBool()) requiredKeys.append(QString("cpucl"));
    if (configuration[QString("memTooltip")].toBool()) requiredKeys.append(QString("mem"));
    if (configuration[QString("swapTooltip")].toBool()) requiredKeys.append(QString("swap"));
    if (configuration[QString("downTooltip")].toBool()) requiredKeys.append(QString("down"));
    if (configuration[QString("upTooltip")].toBool()) requiredKeys.append(QString("up"));
    if (configuration[QString("batTooltip")].toBool()) requiredKeys.append(QString("bat"));
}


AWToolTip::~AWToolTip()
{
    if (debug) qDebug() << PDEBUG;

    delete toolTipView;
    delete toolTipScene;
}


QPixmap AWToolTip::image()
{
    if (debug) qDebug() << PDEBUG;

    toolTipView->resize(100.0 * counts, 105.0);
    // create image
    toolTipScene->clear();
    QPen pen = QPen();
    // background
    if (configuration[QString("useTooltipBackground")].toBool())
        toolTipScene->setBackgroundBrush(QColor(configuration[QString("tooltipBackground")].toString()));
    else
        toolTipScene->setBackgroundBrush(QBrush(Qt::NoBrush));
    bool down = false;
    bool isBattery = false;
    for (int i=0; i<requiredKeys.count(); i++) {
        float normX = 100.0 / static_cast<float>(data[requiredKeys[i]].count());
        float normY = 100.0 / (1.5 * boundaries[requiredKeys[i]]);
        if (requiredKeys[i] == QString("bat"))
            isBattery = true;
        else
            isBattery = false;
        if (!isBattery) pen.setColor(QColor(configuration[requiredKeys[i] + QString("Color")].toString()));
        float shift = i * 100.0;
        if (down) shift -= 100.0;
        for (int j=0; j<data[requiredKeys[i]].count()-1; j++) {
            float x1 = j * normX + shift;
            float y1 = - fabs(data[requiredKeys[i]][j]) * normY + 5.0;
            float x2 = (j + 1) * normX + shift;
            float y2 = - fabs(data[requiredKeys[i]][j+1]) * normY + 5.0;
            if (isBattery) {
                if (data[requiredKeys[i]][j+1] > 0)
                    pen.setColor(QColor(configuration[QString("batteryColor")].toString()));
                else
                    pen.setColor(QColor(configuration[QString("batteryInColor")].toString()));
            }
            toolTipScene->addLine(x1, y1, x2, y2, pen);
        }
        if (requiredKeys[i] == QString("down")) down = true;
    }

    return toolTipView->grab();
}


void AWToolTip::setData (const QString source, const float value, const bool ac)
{
    if (debug) qDebug() << PDEBUG;

    if (data[source].count() == 0)
        data[source].append(0.0);
    else if (data[source].count() > configuration[QString("tooltipNumber")].toInt())
        data[source].takeFirst();

    if (ac)
        data[source].append(value);
    else
        data[source].append(-value);

    if ((source == QString("down")) || (source == QString("up"))) {
        for (int i=0; i<data[QString("down")].count(); i++)
            if (boundaries[QString("down")] < data[QString("down")][i])
                boundaries[QString("down")] = data[QString("down")][i];
        for (int i=0; i<data[QString("up")].count(); i++)
            if (boundaries[QString("down")] < data[QString("up")][i])
                boundaries[QString("down")] = data[QString("up")][i];
        boundaries[QString("down")] *= 1.2;
        boundaries[QString("up")] = boundaries[QString("down")];
    }
}
