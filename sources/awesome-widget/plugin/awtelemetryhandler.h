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


#ifndef AWTELEMETRYHANDLER_H
#define AWTELEMETRYHANDLER_H

#include <QObject>
#include <QtCore/QVariant>


class QAbstractButton;
class QNetworkReply;

class AWTelemetryHandler : public QObject
{
    Q_OBJECT

public:
    explicit AWTelemetryHandler(QObject *parent = nullptr);
    virtual ~AWTelemetryHandler();
    Q_INVOKABLE QStringList get(const QString group) const;
    Q_INVOKABLE QString getLast(const QString group) const;
    Q_INVOKABLE bool put(const QString group, const QString value) const;
    Q_INVOKABLE bool rotate() const;

private slots:

private:
    void init();
    bool setConfiguration(const QString key, const QVariant value,
                          const bool override) const;
    QString m_clientId;
    QString m_genericConfig;
    QString m_localFile;
    int m_storeCount = 0;
};


#endif /* AWTELEMETRYHANDLER_H */
