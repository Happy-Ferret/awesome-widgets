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


#ifndef AWKEYS_H
#define AWKEYS_H

#include <QObject>

#include "extitemaggregator.h"


class AWDataAggregator;
class AWKeysAggregator;
class ExtQuotes;
class ExtScript;
class ExtUpgrade;
class ExtWeather;
class GraphicalItem;
class QThreadPool;

class AWKeys : public QObject
{
    Q_OBJECT

public:
    explicit AWKeys(QObject *parent = nullptr);
    virtual ~AWKeys();

    Q_INVOKABLE void initDataAggregator(const QVariantMap tooltipParams);
    Q_INVOKABLE void initKeys(const QString currentPattern, const int limit);
    Q_INVOKABLE void setAggregatorProperty(const QString key, const QVariant value);
    Q_INVOKABLE void setWrapNewLines(const bool wrap = false);
    // keys
    Q_INVOKABLE void addDevice(const QString source);
    Q_INVOKABLE QStringList dictKeys(const bool sorted = false,
                                     const QString regexp = QString()) const;
    Q_INVOKABLE QStringList getHddDevices() const;
    Q_INVOKABLE void dataUpdateReceived(const QString sourceName, const QVariantMap data);
    // values
    Q_INVOKABLE QString infoByKey(QString key) const;
    Q_INVOKABLE QString valueByKey(QString key) const;
    // configuration
    Q_INVOKABLE void editItem(const QString type);

signals:
    void dropSourceFromDataengine(const QString source);
    void needTextToBeUpdated(const QString newText) const;
    void needToolTipToBeUpdated(const QString newText) const;
    void needToBeUpdated();

private slots:
    void dataUpdate();
    void loadKeysFromCache();
    void reinitKeys();

private:
    // methods
    void addKeyToCache(const QString type, const QString key = QString(""));
    void calculateValues();
    QString parsePattern(QString pattern) const;
    void setDataBySource(const QString sourceName, const QVariantMap data);
    // objects
    AWKeysAggregator *aggregator = nullptr;
    AWDataAggregator *dataAggregator = nullptr;
    ExtItemAggregator<GraphicalItem> *graphicalItems = nullptr;
    ExtItemAggregator<ExtQuotes> *extQuotes = nullptr;
    ExtItemAggregator<ExtScript> *extScripts = nullptr;
    ExtItemAggregator<ExtUpgrade> *extUpgrade = nullptr;
    ExtItemAggregator<ExtWeather> *extWeather = nullptr;
    // variables
    QHash<QString, QStringList> m_devices;
    QStringList m_foundBars, m_foundKeys, m_foundLambdas;
    QString m_pattern;
    QHash<QString, QString> values;
    bool m_wrapNewLines = false;
    // queue and stream lock properties
    bool lock = false;
    QThreadPool *threadPool = nullptr;
    int queueLimit, queue = 0;
};


#endif /* AWKEYS_H */
