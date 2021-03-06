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


#ifndef AWACTIONS_H
#define AWACTIONS_H

#include <QMap>
#include <QObject>


class AWUpdateHelper;

class AWActions : public QObject
{
    Q_OBJECT

public:
    explicit AWActions(QObject *_parent = nullptr);
    virtual ~AWActions();
    Q_INVOKABLE void checkUpdates(const bool _showAnyway = false);
    Q_INVOKABLE QString getFileContent(const QString &_path) const;
    Q_INVOKABLE bool isDebugEnabled() const;
    Q_INVOKABLE bool runCmd(const QString &_cmd) const;
    Q_INVOKABLE void showLegacyInfo() const;
    Q_INVOKABLE void showReadme() const;
    // configuration slots
    Q_INVOKABLE QString getAboutText(const QString &_type) const;
    Q_INVOKABLE QVariantMap getFont(const QVariantMap &_defaultFont) const;

public slots:
    Q_INVOKABLE static void sendNotification(const QString &_eventId, const QString &_message);

private:
    AWUpdateHelper *m_updateHelper = nullptr;
};


#endif /* AWACTIONS_H */
