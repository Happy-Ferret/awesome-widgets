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

#ifndef EXTWEATHER_H
#define EXTWEATHER_H

#include <QMap>
#include <QNetworkReply>

#include "abstractextitem.h"

#define OWM_URL "http://api.openweathermap.org/data/2.5/weather?q=$CITY,$COUNTRY&units=metric"
#define OWM_FORECAST_URL "http://api.openweathermap.org/data/2.5/forecast?q=$CITY,$COUNTRY&units=metric"


namespace Ui {
    class ExtWeather;
}

class ExtWeather : public AbstractExtItem
{
    Q_OBJECT
    Q_PROPERTY(QString city READ city WRITE setCity)
    Q_PROPERTY(QString country READ country WRITE setCountry)
    Q_PROPERTY(int ts READ ts WRITE setTs)

public:
    explicit ExtWeather(QWidget *parent = nullptr, const QString weatherName = QString(),
                        const QStringList directories = QStringList(),
                        const bool debugCmd = false);
    ~ExtWeather();
    ExtWeather *copy(const QString fileName, const int number);
    QString weatherFromInt(const int _id) const;
    // get methods
    QString city() const;
    QString country() const;
    int ts() const;
    QString uniq() const;
    // set methods
    void setCity(const QString _city = QString("London"));
    void setCountry(const QString _country = QString("uk"));
    void setTs(const int _ts = 0);

public slots:
    void readConfiguration();
    QVariantMap run();
    int showConfiguration(const QVariant args = QVariant());
    void writeConfiguration() const;

private slots:
    void weatherReplyReceived(QNetworkReply *reply);

private:
    bool debug;
    QNetworkAccessManager *manager;
    bool isRunning = false;
    Ui::ExtWeather *ui;
    QVariantMap parseSingleJson(const QVariantMap json) const;
    void translate();
    QString url(const bool isForecast = false) const;
    // properties
    QString m_city = QString("London");
    QString m_country = QString("uk");
    int m_ts = 0;
    // values
    int times = 0;
    QVariantMap values;
};


#endif /* EXTWEATHER_H */
