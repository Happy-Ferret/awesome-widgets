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

#include <QNetworkReply>

#include "abstractextitem.h"


class AbstractWeatherProvider;
namespace Ui
{
class ExtWeather;
}

class ExtWeather : public AbstractExtItem
{
    Q_OBJECT
    Q_PROPERTY(QString city READ city WRITE setCity)
    Q_PROPERTY(QString country READ country WRITE setCountry)
    Q_PROPERTY(bool image READ image WRITE setImage)
    Q_PROPERTY(Provider povider READ provider WRITE setProvider)
    Q_PROPERTY(QString strPovider READ strProvider WRITE setStrProvider)
    Q_PROPERTY(int ts READ ts WRITE setTs)

public:
    enum class Provider { OWM = 0, Yahoo = 1 };

    explicit ExtWeather(QWidget *parent, const QString &filePath = QString());
    virtual ~ExtWeather();
    ExtWeather *copy(const QString &_fileName, const int _number);
    QString jsonMapFile() const;
    QString weatherFromInt(const int _id) const;
    // get methods
    QString city() const;
    QString country() const;
    bool image() const;
    Provider provider() const;
    QString strProvider() const;
    int ts() const;
    QString uniq() const;
    // set methods
    void setCity(const QString &_city = QString("London"));
    void setCountry(const QString &_country = QString("uk"));
    void setImage(const bool _image = false);
    void setProvider(const Provider _provider = Provider::OWM);
    void setStrProvider(const QString &_provider = QString("OWM"));
    void setTs(const int _ts = 0);

public slots:
    void readConfiguration();
    void readJsonMap();
    QVariantHash run();
    int showConfiguration(const QVariant &args = QVariant());
    void writeConfiguration() const;

private slots:
    void sendRequest();
    void weatherReplyReceived(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_manager = nullptr;
    AbstractWeatherProvider *m_providerObject = nullptr;
    bool m_isRunning = false;
    Ui::ExtWeather *ui = nullptr;
    void initProvider();
    void translate();
    // properties
    QString m_city = QString("London");
    QString m_country = QString("uk");
    bool m_image = false;
    Provider m_provider = Provider::OWM;
    int m_ts = 0;
    QVariantMap m_jsonMap = QVariantMap();
    // values
    int m_times = 0;
    QVariantHash m_values;
};


#endif /* EXTWEATHER_H */
