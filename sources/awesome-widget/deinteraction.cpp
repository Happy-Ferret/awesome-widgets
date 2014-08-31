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

#include <QRegExp>

#include <pdebug/pdebug.h>


void AwesomeWidget::connectToEngine()
{
    if (debug) qDebug() << PDEBUG;
    QRegExp regExp;

    // battery
    regExp = QRegExp(QString("(ac|bat)"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->connectSource(QString("battery"),
                                       this, configuration[QString("interval")].toInt());
    // cpu
    regExp = QRegExp(QString("cpu.*"));
    if (foundKeys.indexOf(regExp) > -1) {
        sysmonEngine->connectSource(QString("cpu/system/TotalLoad"),
                                    this, configuration[QString("interval")].toInt());
        for (int i=0; i<counts[QString("cpu")]; i++)
            sysmonEngine->connectSource(QString("cpu/cpu") + QString::number(i) + QString("/TotalLoad"),
                                        this, configuration[QString("interval")].toInt());
    }
    // cpuclock
    regExp = QRegExp(QString("cpucl.*"));
    if (foundKeys.indexOf(regExp) > -1) {
        sysmonEngine->connectSource(QString("cpu/system/AverageClock"),
                                    this, configuration[QString("interval")].toInt());
        for (int i=0; i<counts[QString("cpu")]; i++)
            sysmonEngine->connectSource(QString("cpu/cpu") + QString::number(i) + QString("/clock"),
                                        this, configuration[QString("interval")].toInt());
    }
    // custom command
    regExp = QRegExp(QString("custom.*"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->connectSource(QString("custom"),
                                       this, configuration[QString("interval")].toInt());
    // desktop
    regExp = QRegExp(QString(".*desktop.*"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->connectSource(QString("desktop"),
                                       this, configuration[QString("interval")].toInt());
    // disk speed
    regExp = QRegExp(QString("hdd[rw].*"));
    if (foundKeys.indexOf(regExp) > -1)
        for (int i=0; i<configuration[QString("disk")].split(QString("@@")).count(); i++) {
            sysmonEngine->connectSource(configuration[QString("disk")].split(QString("@@"))[i] + QString("/Rate/rblk"),
                                        this, configuration[QString("interval")].toInt());
            sysmonEngine->connectSource(configuration[QString("disk")].split(QString("@@"))[i] + QString("/Rate/wblk"),
                                        this, configuration[QString("interval")].toInt());
        }
    // gpu
    regExp = QRegExp(QString("gpu"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->connectSource(QString("gpu"),
                                       this, configuration[QString("interval")].toInt());
    // gputemp
    regExp = QRegExp(QString("gputemp"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->connectSource(QString("gputemp"),
                                       this, configuration[QString("interval")].toInt());
    // mount
    regExp = QRegExp(QString("hdd([0-9]|mb|gb|freemb|freegb|totmb|totgb).*"));
    if (foundKeys.indexOf(regExp) > -1)
        for (int i=0; i<configuration[QString("mount")].split(QString("@@")).count(); i++) {
            sysmonEngine->connectSource(QString("partitions") + configuration[QString("mount")].split(QString("@@"))[i] + QString("/filllevel"),
                                        this, configuration[QString("interval")].toInt());
            sysmonEngine->connectSource(QString("partitions") + configuration[QString("mount")].split(QString("@@"))[i] + QString("/freespace"),
                                        this, configuration[QString("interval")].toInt());
            sysmonEngine->connectSource(QString("partitions") + configuration[QString("mount")].split(QString("@@"))[i] + QString("/usedspace"),
                                        this, configuration[QString("interval")].toInt());
        }
    // hddtemp
    regExp = QRegExp(QString("hddtemp.*"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->connectSource(QString("hddtemp"),
                                       this, configuration[QString("interval")].toInt());
    // memory
    regExp = QRegExp(QString("mem.*"));
    if (foundKeys.indexOf(regExp) > -1) {
        sysmonEngine->connectSource(QString("mem/physical/free"),
                                    this, configuration[QString("interval")].toInt());
        sysmonEngine->connectSource(QString("mem/physical/used"),
                                    this, configuration[QString("interval")].toInt());
        sysmonEngine->connectSource(QString("mem/physical/application"),
                                    this, configuration[QString("interval")].toInt());
    }
    // network
    regExp = QRegExp(QString("(down|up|netdev)"));
    if (foundKeys.indexOf(regExp) > -1) {
        sysmonEngine->connectSource(QString("network/interfaces/") + values[QString("netdev")] + QString("/transmitter/data"),
                                    this, configuration[QString("interval")].toInt());
        sysmonEngine->connectSource(QString("network/interfaces/") + values[QString("netdev")] + QString("/receiver/data"),
                                    this, configuration[QString("interval")].toInt());
    }
    // package manager
    regExp = QRegExp(QString("pkgcount.*"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->connectSource(QString("pkg"),
                                       this, 60*60*1000, Plasma::AlignToHour);
    // player
    regExp = QRegExp(QString("(album|artist|duration|progress|title)"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->connectSource(QString("player"),
                                       this, configuration[QString("interval")].toInt());
    // ps
    regExp = QRegExp(QString("ps.*"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->connectSource(QString("ps"),
                                       this, configuration[QString("interval")].toInt());
    // swap
    regExp = QRegExp(QString("swap.*"));
    if (foundKeys.indexOf(regExp) > -1) {
        sysmonEngine->connectSource(QString("mem/swap/free"),
                                    this, configuration[QString("interval")].toInt());
        sysmonEngine->connectSource(QString("mem/swap/used"),
                                    this, configuration[QString("interval")].toInt());
    }
    // temp
    regExp = QRegExp(QString("temp.*"));
    if (foundKeys.indexOf(regExp) > -1)
        for (int i=0; i<configuration[QString("tempDevice")].split(QString("@@")).count(); i++)
            sysmonEngine->connectSource(configuration[QString("tempDevice")].split(QString("@@"))[i],
                                        this, configuration[QString("interval")].toInt());
    // time
    regExp = QRegExp(QString("(^|iso|short|long|c)time"));
    if (foundKeys.indexOf(regExp) > -1)
        timeEngine->connectSource(QString("Local"),
                                  this, 1000);
    // uptime
    regExp = QRegExp(QString("(^|c)uptime"));
    if (foundKeys.indexOf(regExp) > -1)
        sysmonEngine->connectSource(QString("system/uptime"),
                                    this, configuration[QString("interval")].toInt());
}


void AwesomeWidget::dataUpdated(const QString &sourceName, const Plasma::DataEngine::Data &data)
{
    if (debug) qDebug() << PDEBUG;
    if (debug) qDebug() << PDEBUG << ":" << "Source" << sourceName;

    // regular expressions
    QRegExp cpuRegExp = QRegExp(QString("cpu/cpu.*/TotalLoad"));
    QRegExp cpuclRegExp = QRegExp(QString("cpu/cpu.*/clock"));
    QRegExp hddrRegExp = QRegExp(QString("disk/.*/Rate/rblk"));
    QRegExp hddwRegExp = QRegExp(QString("disk/.*/Rate/wblk"));
    QRegExp mountFillRegExp = QRegExp(QString("partitions/.*/filllevel"));
    QRegExp mountFreeRegExp = QRegExp(QString("partitions/.*/freespace"));
    QRegExp mountUsedRegExp = QRegExp(QString("partitions/.*/usedspace"));

    if (data.keys().isEmpty()) return;
    if (sourceName == QString("battery")) {
        if (data[QString("ac")].toBool())
            values[QString("ac")] = configuration[QString("acOnline")];
        else
            values[QString("ac")] = configuration[QString("acOffline")];
        values[QString("bat")] = QString("%3i").arg(data[QString("bat")].toInt());
    } else if (sourceName == QString("cpu/system/TotalLoad")) {
        values[QString("cpu")] = QString("%5.1f").arg(data[QString("value")].toFloat());
        if (configuration[QString("cpuTooltip")].toInt() == 2)
            tooltipValues[QString("cpu")].append(data[QString("value")].toFloat());
    } else if (sourceName.indexOf(cpuRegExp) > -1) {
        QString number = sourceName;
        number.remove(QString("cpu/cpu"));
        number.remove(QString("/TotalLoad"));
        values[QString("cpu") + number] = QString("%5.1f").arg(data[QString("value")].toFloat());
    } else if (sourceName == QString("cpu/system/AverageClock")) {
        values[QString("cpucl")] = QString("%5.1f").arg(data[QString("value")].toFloat());
        if (configuration[QString("cpuclockTooltip")].toInt() == 2)
            tooltipValues[QString("cpucl")].append(data[QString("value")].toFloat());
    } else if (sourceName.indexOf(cpuclRegExp) > -1) {
        QString number = sourceName;
        number.remove(QString("cpu/cpu"));
        number.remove(QString("/clock"));
        values[QString("cpucl") + number] = QString("%5.1f").arg(data[QString("value")].toFloat());
    } else if (sourceName == QString("custom")) {
        for (int i=0; i<data.keys().count(); i++)
            values[data.keys()[i]] = data[data.keys()[i]].toString();
    } else if (sourceName == QString("desktop")) {
        values[QString("desktop")] = data[QString("currentName")].toString();
        values[QString("ndesktop")] = QString("%i").arg(data[QString("currentNumber")].toInt());
        values[QString("tdesktops")] = QString("%i").arg(data[QString("number")].toInt());
    } else if (sourceName.indexOf(hddrRegExp) > -1) {
        QString device = sourceName;
        device.remove(QString("/Rate/rblk"));
        for (int i=0; i<counts[QString("disk")]; i++)
            if (configuration[QString("disk")].split(QString("@@"))[i] == device) {
                values[QString("hddr") + QString::number(i)] = QString("%i").arg(data[QString("value")].toFloat());
                break;
            }
    } else if (sourceName.indexOf(hddwRegExp) > -1) {
        QString device = sourceName;
        device.remove(QString("/Rate/wblk"));
        for (int i=0; i<counts[QString("disk")]; i++)
            if (configuration[QString("disk")].split(QString("@@"))[i] == device) {
                values[QString("hddw") + QString::number(i)] = QString("%i").arg(data[QString("value")].toFloat());
                break;
            }
    } else if (sourceName == QString("gpu")) {
        values[QString("gpu")] = QString("%4.1f").arg(data[QString("GPU")].toFloat());
    } else if (sourceName == QString("gputemp")) {
        values[QString("gputemp")] = QString("%4.1f").arg(data[QString("GPUTemp")].toFloat());
    } else if (sourceName.indexOf(mountFillRegExp) > -1) {
        QString mount = sourceName;
        mount.remove(QString("partitions"));
        mount.remove(QString("/filllevel"));
        for (int i=0; i<counts[QString("mount")]; i++)
            if (configuration[QString("mount")].split(QString("@@"))[i] == mount) {
                values[QString("hdd") + QString::number(i)] = QString("%5.1f").arg(data[QString("value")].toFloat());
                break;
            }
    } else if (sourceName.indexOf(mountFreeRegExp) > -1) {
        QString mount = sourceName;
        mount.remove(QString("partitions"));
        mount.remove(QString("/freespace"));
        for (int i=0; i<counts[QString("mount")]; i++)
            if (configuration[QString("mount")].split(QString("@@"))[i] == mount) {
                values[QString("hddfreemb") + QString::number(i)] = QString("%i").arg(
                            data[QString("value")].toFloat() / 1024.0);
                values[QString("hddfreegb") + QString::number(i)] = QString("%5.1f").arg(
                            data[QString("value")].toFloat() / (1024.0 * 1024.0));
                break;
            }
    } else if (sourceName.indexOf(mountUsedRegExp) > -1) {
        QString mount = sourceName;
        mount.remove(QString("partitions"));
        mount.remove(QString("/usedspace"));
        for (int i=0; i<counts[QString("mount")]; i++)
            if (configuration[QString("mount")].split(QString("@@"))[i] == mount) {
                values[QString("hddmb") + QString::number(i)] = QString("%i").arg(
                            data[QString("value")].toFloat() / 1024.0);
                values[QString("hddgb") + QString::number(i)] = QString("%5.1f").arg(
                            data[QString("value")].toFloat() / (1024.0 * 1024.0));
                // total
                values[QString("hddtotmb") + QString::number(i)] = QString("%i").arg(
                            values[QString("hddfreemb") + QString::number(i)].toInt() +
                            values[QString("hddmb") + QString::number(i)].toInt());
                values[QString("hddtotgb") + QString::number(i)] = QString("%5.1f").arg(
                            values[QString("hddfreegb") + QString::number(i)].toFloat() +
                            values[QString("hddgb") + QString::number(i)].toFloat());
                break;
            }
    } else if (sourceName == QString("hddtemp")) {
        for (int i=0; i<data.keys().count(); i++)
            for (int j=0; j<counts[QString("hddtemp")]; j++)
                if (data.keys()[i] == configuration[QString("hdd")].split(QString("@@"))[j]) {
                    values[QString("hddtemp") + QString::number(j)] = QString("%4.1f").arg(data[data.keys()[i]].toFloat());
                    break;
                }
    } else if (sourceName == QString("mem/physical/application")) {
        values[QString("memappmb")] = QString("%i").arg(data[QString("value")].toFloat() / 1024.0);
        values[QString("memappgb")] = QString("%5.1f").arg(data[QString("value")].toFloat() / (1024.0 * 1024.0));
    } else if (sourceName == QString("mem/physical/free")) {
        values[QString("memfreemb")] = QString("%i").arg(data[QString("value")].toFloat() / 1024.0);
        values[QString("memfreegb")] = QString("%5.1f").arg(data[QString("value")].toFloat() / (1024.0 * 1024.0));
    } else if (sourceName == QString("mem/physical/used")) {
        values[QString("memmb")] = QString("%i").arg(data[QString("value")].toFloat() / 1024.0);
        values[QString("memgb")] = QString("%5.1f").arg(data[QString("value")].toFloat() / (1024.0 * 1024.0));
        // total
        values[QString("memtotmb")] = QString("%i").arg(
                    values[QString("memmb")].toInt() + values[QString("memfreemb")].toInt());
        values[QString("memtotgb")] = QString("%5.1f").arg(
                    values[QString("memgb")].toFloat() + values[QString("memfreegb")].toFloat());
        // percentage
        values[QString("mem")] = QString("%5.1f").arg(100.0 * values[QString("memmb")].toFloat() / values[QString("memtotmb")].toFloat());
    }
}


void AwesomeWidget::disconnectFromEngine()
{
    if (debug) qDebug() << PDEBUG;
    QRegExp regExp;

    // battery
    regExp = QRegExp(QString("(ac|bat)"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->disconnectSource(QString("battery"), this);
    // cpu
    regExp = QRegExp(QString("cpu.*"));
    if (foundKeys.indexOf(regExp) > -1) {
        sysmonEngine->disconnectSource(QString("cpu/system/TotalLoad"), this);
        for (int i=0; i<counts[QString("cpu")]; i++)
            sysmonEngine->disconnectSource(QString("cpu/cpu") + QString::number(i) + QString("/TotalLoad"), this);
    }
    // cpuclock
    regExp = QRegExp(QString("cpucl.*"));
    if (foundKeys.indexOf(regExp) > -1) {
        sysmonEngine->disconnectSource(QString("cpu/system/AverageClock"), this);
        for (int i=0; i<counts[QString("cpu")]; i++)
            sysmonEngine->disconnectSource(QString("cpu/cpu") + QString::number(i) + QString("/clock"), this);
    }
    // custom command
    regExp = QRegExp(QString("custom.*"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->disconnectSource(QString("custom"), this);
    // desktop
    regExp = QRegExp(QString(".*desktop.*"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->disconnectSource(QString("desktop"), this);
    // disk speed
    regExp = QRegExp(QString("hdd[rw].*"));
    if (foundKeys.indexOf(regExp) > -1)
        for (int i=0; i<configuration[QString("disk")].split(QString("@@")).count(); i++) {
            sysmonEngine->disconnectSource(configuration[QString("disk")].split(QString("@@"))[i] + QString("/Rate/rblk"), this);
            sysmonEngine->disconnectSource(configuration[QString("disk")].split(QString("@@"))[i] + QString("/Rate/wblk"), this);
        }
    // gpu
    regExp = QRegExp(QString("gpu"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->disconnectSource(QString("gpu"), this);
    // gputemp
    regExp = QRegExp(QString("gputemp"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->disconnectSource(QString("gputemp"), this);
    // mount
    regExp = QRegExp(QString("hdd([0-9]|mb|gb|freemb|freegb|totmb|totgb).*"));
    if (foundKeys.indexOf(regExp) > -1)
        for (int i=0; i<configuration[QString("mount")].split(QString("@@")).count(); i++) {
            sysmonEngine->disconnectSource(QString("partitions") + configuration[QString("mount")].split(QString("@@"))[i] + QString("/filllevel"), this);
            sysmonEngine->disconnectSource(QString("partitions") + configuration[QString("mount")].split(QString("@@"))[i] + QString("/freespace"), this);
            sysmonEngine->disconnectSource(QString("partitions") + configuration[QString("mount")].split(QString("@@"))[i] + QString("/usedspace"), this);
        }
    // hddtemp
    regExp = QRegExp(QString("hddtemp.*"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->disconnectSource(QString("hddtemp"), this);
    // memory
    regExp = QRegExp(QString("mem.*"));
    if (foundKeys.indexOf(regExp) > -1) {
        sysmonEngine->disconnectSource(QString("mem/physical/free"), this);
        sysmonEngine->disconnectSource(QString("mem/physical/used"), this);
        sysmonEngine->disconnectSource(QString("mem/physical/application"), this);
    }
    // network
    regExp = QRegExp(QString("(down|up|netdev)"));
    if (foundKeys.indexOf(regExp) > -1) {
        sysmonEngine->disconnectSource(QString("network/interfaces/") + values[QString("netdev")] + QString("/transmitter/data"), this);
        sysmonEngine->disconnectSource(QString("network/interfaces/") + values[QString("netdev")] + QString("/receiver/data"), this);
    }
    // package manager
    regExp = QRegExp(QString("pkgcount.*"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->disconnectSource(QString("pkg"), this);
    // player
    regExp = QRegExp(QString("(album|artist|duration|progress|title)"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->disconnectSource(QString("player"), this);
    // ps
    regExp = QRegExp(QString("ps.*"));
    if (foundKeys.indexOf(regExp) > -1)
        extsysmonEngine->disconnectSource(QString("ps"), this);
    // swap
    regExp = QRegExp(QString("swap.*"));
    if (foundKeys.indexOf(regExp) > -1) {
        sysmonEngine->disconnectSource(QString("mem/swap/free"), this);
        sysmonEngine->disconnectSource(QString("mem/swap/used"), this);
    }
    // temp
    regExp = QRegExp(QString("temp.*"));
    if (foundKeys.indexOf(regExp) > -1)
        for (int i=0; i<configuration[QString("tempDevice")].split(QString("@@")).count(); i++)
            sysmonEngine->disconnectSource(configuration[QString("tempDevice")].split(QString("@@"))[i], this);
    // time
    regExp = QRegExp(QString("(^|iso|short|long|c)time"));
    if (foundKeys.indexOf(regExp) > -1)
        timeEngine->disconnectSource(QString("Local"), this);
    // uptime
    regExp = QRegExp(QString("(^|c)uptime"));
    if (foundKeys.indexOf(regExp) > -1)
        sysmonEngine->disconnectSource(QString("system/uptime"), this);
}
