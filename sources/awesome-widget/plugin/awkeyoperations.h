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


#ifndef AWKEYOPERATIONS_H
#define AWKEYOPERATIONS_H

#include <Plasma/DataEngine>

#include <QMutex>
#include <QObject>

#include "extitemaggregator.h"


class AWDataAggregator;
class AWDataEngineAggregator;
class AWKeysAggregator;
class ExtQuotes;
class ExtScript;
class ExtUpgrade;
class ExtWeather;
class GraphicalItem;
class QThreadPool;

class AWKeyOperations : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString pattern READ pattern WRITE setPattern)

public:
    explicit AWKeyOperations(QObject *parent = nullptr);
    virtual ~AWKeyOperations();
    void addDevice(const QString source);
    QStringList devices(const QString type) const;
    QHash<QString, QStringList> devices() const;
    void updateCache();
    // keys
    QStringList dictKeys() const;
    GraphicalItem *giByKey(const QString key) const;
    // values
    QString infoByKey(QString key) const;
    QString pattern() const;
    void setPattern(const QString currentPattern);
    // configuration
    void editItem(const QString type);

signals:
    void updateKeys(const QStringList currentKeys);

private:
    // methods
    void addKeyToCache(const QString type, const QString key = QString(""));
    void reinitKeys();
    // objects
    ExtItemAggregator<GraphicalItem> *graphicalItems = nullptr;
    ExtItemAggregator<ExtQuotes> *extQuotes = nullptr;
    ExtItemAggregator<ExtScript> *extScripts = nullptr;
    ExtItemAggregator<ExtUpgrade> *extUpgrade = nullptr;
    ExtItemAggregator<ExtWeather> *extWeather = nullptr;
    // variables
    QHash<QString, QStringList> m_devices;
    QString m_pattern;
};


#endif /* AWKEYOPERATIONS_H */
