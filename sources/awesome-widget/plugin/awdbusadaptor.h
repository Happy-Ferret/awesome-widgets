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


#ifndef AWDBUSADAPTOR_H
#define AWDBUSADAPTOR_H

#include <QDBusAbstractAdaptor>

#include "version.h"


class AWKeys;

class AWDBusAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", AWDBUS_SERVICE_NAME)

public:
    explicit AWDBusAdaptor(AWKeys *_parent = nullptr);
    virtual ~AWDBusAdaptor();

public slots:
    // get methods
    QStringList ActiveServices() const;
    QString Info(const QString &key) const;
    QStringList Keys(const QString &regexp) const;
    QString Value(const QString &key) const;
    qlonglong WhoAmI() const;
    // set methods
    void SetLogLevel(const QString &what, const int level);
    void SetLogLevel(const QString &what, const QString &level, const bool enabled);

private:
    AWKeys *m_plugin = nullptr;
    QStringList m_logLevels = {"debug", "info", "warning", "critical"};
};


#endif /* AWDBUSADAPTOR_H */
