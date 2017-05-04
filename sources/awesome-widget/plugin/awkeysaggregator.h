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


#ifndef AWKEYSAGGREGATOR_H
#define AWKEYSAGGREGATOR_H

#include <QHash>
#include <QObject>

#include "version.h"


class AWFormatterHelper;

class AWKeysAggregator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString acOffline MEMBER m_acOffline WRITE setAcOffline);
    Q_PROPERTY(QString acOnline MEMBER m_acOnline WRITE setAcOnline);
    Q_PROPERTY(QString customTime MEMBER m_customTime WRITE setCustomTime);
    Q_PROPERTY(
        QString customUptime MEMBER m_customUptime WRITE setCustomUptime);
    Q_PROPERTY(QString tempUnits MEMBER m_tempUnits WRITE setTempUnits);
    Q_PROPERTY(bool translate MEMBER m_translate WRITE setTranslate);

    enum class FormatterType {
        // general formatters
        Custom,
        NoFormat,
        Float,
        FloatTwoSymbols,
        Integer,
        IntegerFive,
        IntegerThree,
        List,
        // unit specific formatters
        ACFormat,
        MemGBFormat,
        MemMBFormat,
        NetSmartFormat,
        NetSmartUnits,
        Quotes,
        Temperature,
        Time,
        TimeCustom,
        TimeISO,
        TimeLong,
        TimeShort,
        Timestamp,
        Uptime,
        UptimeCustom
    };

public:
    explicit AWKeysAggregator(QObject *parent = nullptr);
    virtual ~AWKeysAggregator();
    void initFormatters();
    // get methods
    QString formatter(const QVariant &data, const QString &key) const;
    QStringList keysFromSource(const QString &source) const;
    // set methods
    void setAcOffline(const QString &inactive);
    void setAcOnline(const QString &active);
    void setCustomTime(const QString &customTime);
    void setCustomUptime(const QString &customUptime);
    void setDevices(const QHash<QString, QStringList> &devices);
    void setTempUnits(const QString &units);
    void setTranslate(const bool translate);

public slots:
    QStringList registerSource(const QString &source, const QString &units,
                               const QStringList &keys);

private:
    float temperature(const float temp) const;
    AWFormatterHelper *m_customFormatters = nullptr;
    QStringList m_timeKeys = QString(TIME_KEYS).split(QChar(','));
    // variables
    QString m_acOffline;
    QString m_acOnline;
    QString m_customTime;
    QString m_customUptime;
    QHash<QString, QStringList> m_devices;
    QHash<QString, FormatterType> m_formatter;
    QHash<QString, QString> m_map;
    QString m_tempUnits;
    bool m_translate = false;
};


#endif /* AWKEYSAGGREGATOR_H */
