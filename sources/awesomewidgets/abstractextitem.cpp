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

#include "abstractextitem.h"

#include <QDir>
#include <QSettings>
#include <QStandardPaths>
#include <QTime>

#include "abstractextitemaggregator.h"
#include "awdebug.h"


AbstractExtItem::AbstractExtItem(QWidget *parent, const QString filePath)
    : QDialog(parent)
    , m_fileName(filePath)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    qCDebug(LOG_LIB) << "Desktop name" << filePath;

    m_name = m_fileName;
}


AbstractExtItem::~AbstractExtItem()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


void AbstractExtItem::bumpApi(const int _newVer)
{
    qCDebug(LOG_LIB) << "Bump API using new version" << _newVer;

    // update for current API
    if ((apiVersion() > 0) && (apiVersion() < _newVer)) {
        qCWarning(LOG_LIB) << "Bump API version from" << apiVersion() << "to"
                           << _newVer;
        setApiVersion(_newVer);
        writeConfiguration();
    }
}


void AbstractExtItem::copyDefaults(AbstractExtItem *_other) const
{
    _other->setActive(m_active);
    _other->setApiVersion(m_apiVersion);
    _other->setComment(m_comment);
    _other->setInterval(m_interval);
    _other->setName(m_name);
}


QString AbstractExtItem::writtableConfig() const
{
    QString path = m_fileName;
    QString name = QFileInfo(path).fileName();
    path.remove(path.count() - name.count() - 1, name.count() + 1);
    QString dir = QFileInfo(path).fileName();

    return QString("%1/awesomewidgets/%2/%3")
        .arg(QStandardPaths::writableLocation(
            QStandardPaths::GenericDataLocation))
        .arg(dir)
        .arg(name);
}


int AbstractExtItem::apiVersion() const
{
    return m_apiVersion;
}


QString AbstractExtItem::comment() const
{
    return m_comment;
}


QString AbstractExtItem::fileName() const
{
    return m_fileName;
}


int AbstractExtItem::interval() const
{
    return m_interval;
}


bool AbstractExtItem::isActive() const
{
    return m_active;
}


QString AbstractExtItem::name() const
{
    return m_name;
}


int AbstractExtItem::number() const
{
    return m_number;
}


QString AbstractExtItem::tag(const QString _type) const
{
    qCDebug(LOG_LIB) << "Tag type" << _type;

    return QString("%1%2").arg(_type).arg(m_number);
}


void AbstractExtItem::setApiVersion(const int _apiVersion)
{
    qCDebug(LOG_LIB) << "Version" << _apiVersion;

    m_apiVersion = _apiVersion;
}


void AbstractExtItem::setActive(const bool _state)
{
    qCDebug(LOG_LIB) << "State" << _state;

    m_active = _state;
}


void AbstractExtItem::setComment(const QString _comment)
{
    qCDebug(LOG_LIB) << "Comment" << _comment;

    m_comment = _comment;
}


void AbstractExtItem::setInterval(const int _interval)
{
    qCDebug(LOG_LIB) << "Interval" << _interval;
    if (_interval <= 0)
        return;

    m_interval = _interval;
}


void AbstractExtItem::setName(const QString _name)
{
    qCDebug(LOG_LIB) << "Name" << _name;

    m_name = _name;
}


void AbstractExtItem::setNumber(int _number)
{
    qCDebug(LOG_LIB) << "Number" << _number;
    if (_number == -1)
        _number = []() {
            qCWarning(LOG_LIB) << "Number is empty, generate new one";
            qsrand(QTime::currentTime().msec());
            int n = qrand() % 1000;
            qCInfo(LOG_LIB) << "Generated number is" << n;
            return n;
        }();

    m_number = _number;
}


void AbstractExtItem::readConfiguration()
{
    QSettings settings(m_fileName, QSettings::IniFormat);

    settings.beginGroup(QString("Desktop Entry"));
    setName(settings.value(QString("Name"), m_name).toString());
    setComment(settings.value(QString("Comment"), m_comment).toString());
    setApiVersion(
        settings.value(QString("X-AW-ApiVersion"), m_apiVersion).toInt());
    setActive(
        settings.value(QString("X-AW-Active"), QVariant(m_active)).toString()
        == QString("true"));
    setInterval(settings.value(QString("X-AW-Interval"), m_interval).toInt());
    setNumber(settings.value(QString("X-AW-Number"), m_number).toInt());
    settings.endGroup();
}


bool AbstractExtItem::tryDelete() const
{
    bool status = QFile::remove(m_fileName);
    qCInfo(LOG_LIB) << "Remove file" << m_fileName << status;

    return status;
}


void AbstractExtItem::writeConfiguration() const
{
    QSettings settings(writtableConfig(), QSettings::IniFormat);
    qCInfo(LOG_LIB) << "Configuration file" << settings.fileName();

    settings.beginGroup(QString("Desktop Entry"));
    settings.setValue(QString("Encoding"), QString("UTF-8"));
    settings.setValue(QString("Name"), m_name);
    settings.setValue(QString("Comment"), m_comment);
    settings.setValue(QString("X-AW-ApiVersion"), m_apiVersion);
    settings.setValue(QString("X-AW-Active"), QVariant(m_active).toString());
    settings.setValue(QString("X-AW-Interval"), m_interval);
    settings.setValue(QString("X-AW-Number"), m_number);
    settings.endGroup();

    settings.sync();
}
