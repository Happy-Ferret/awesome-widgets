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
#include <math.h>

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
    counts += configuration[QString("batTooltip")].toInt();

    boundaries[QString("cpuTooltip")] = 100.0;
    boundaries[QString("cpuclTooltip")] = 4000.0;
    boundaries[QString("memTooltip")] = 100.0;
    boundaries[QString("swapTooltip")] = 100.0;
    boundaries[QString("downTooltip")] = 1.0;
    boundaries[QString("upTooltip")] = 1.0;
    boundaries[QString("batTooltip")] = 100.0;
    size.setHeight(105.0);
    size.setWidth(100.0 * counts);

    if (configuration[QString("cpuTooltip")].toBool()) requiredKeys.append(QString("cpuTooltip"));
    if (configuration[QString("cpuclTooltip")].toBool()) requiredKeys.append(QString("cpuclTooltip"));
    if (configuration[QString("memTooltip")].toBool()) requiredKeys.append(QString("memTooltip"));
    if (configuration[QString("swapTooltip")].toBool()) requiredKeys.append(QString("swapTooltip"));
    if (configuration[QString("downTooltip")].toBool()) requiredKeys.append(QString("downTooltip"));
    if (configuration[QString("upTooltip")].toBool()) requiredKeys.append(QString("upTooltip"));
    if (configuration[QString("batTooltip")].toBool()) requiredKeys.append(QString("batTooltip"));
}


AWToolTip::~AWToolTip()
{
    if (debug) qDebug() << PDEBUG;

    delete toolTipView;
    delete toolTipScene;
}


QSize AWToolTip::getSize()
{
    if (debug) qDebug() << PDEBUG;

    return size;
}


QPixmap AWToolTip::image()
{
    if (debug) qDebug() << PDEBUG;

    toolTipView->resize(size);
    // create image
    toolTipScene->clear();
    QPen pen = QPen();
    // background
    if (configuration[QString("useTooltipBackground")].toBool())
        toolTipScene->setBackgroundBrush(QBrush(QColor(configuration[QString("tooltipBackground")].toString())));
    else
        toolTipScene->setBackgroundBrush(QBrush(Qt::NoBrush));
    bool down = false;
    for (int i=0; i<requiredKeys.count(); i++) {
        float normX = 100.0 / static_cast<float>(data[requiredKeys[i]].count());
        float normY = 100.0 / (1.5 * boundaries[requiredKeys[i]]);
        if (requiredKeys[i] != QString("batTooltip"))
            pen.setColor(QColor(configuration[requiredKeys[i] + QString("Color")].toString()));
        float shift = i * 100.0;
        if (down) shift -= 100.0;
        for (int j=0; j<data[requiredKeys[i]].count()-1; j++) {
            float x1 = j * normX + shift;
            float y1 = - fabs(data[requiredKeys[i]][j]) * normY + 5.0;
            float x2 = (j + 1) * normX + shift;
            float y2 = - fabs(data[requiredKeys[i]][j+1]) * normY + 5.0;
            if (requiredKeys[i] == QString("batTooltip")) {
                if (data[requiredKeys[i]][j+1] > 0)
                    pen.setColor(QColor(configuration[QString("batTooltipColor")].toString()));
                else
                    pen.setColor(QColor(configuration[QString("batInTooltipColor")].toString()));
            }
            toolTipScene->addLine(x1, y1, x2, y2, pen);
        }
        if (requiredKeys[i] == QString("downTooltip")) down = true;
    }

    return toolTipView->grab();
}


void AWToolTip::setData(const QString source, float value, const bool ac)
{
    if (debug) qDebug() << PDEBUG;

    if (data[source].count() == 0)
        data[source].append(0.0);
    else if (data[source].count() > configuration[QString("tooltipNumber")].toInt())
        data[source].takeFirst();
    if (isnan(value)) value = 0.0;

    if (ac)
        data[source].append(value);
    else
        data[source].append(-value);

    if ((source == QString("downTooltip")) || (source == QString("upTooltip"))) {
        for (int i=0; i<data[QString("downTooltip")].count(); i++)
            if (boundaries[QString("downTooltip")] < data[QString("downTooltip")][i])
                boundaries[QString("downTooltip")] = data[QString("downTooltip")][i];
        for (int i=0; i<data[QString("upTooltip")].count(); i++)
            if (boundaries[QString("downTooltip")] < data[QString("upTooltip")][i])
                boundaries[QString("downTooltip")] = data[QString("upTooltip")][i];
        boundaries[QString("downTooltip")] *= 1.2;
        boundaries[QString("upTooltip")] = boundaries[QString("downTooltip")];
    }
}
