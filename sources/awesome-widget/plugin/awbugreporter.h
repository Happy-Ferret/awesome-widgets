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


#ifndef AWBUGREPORTER_H
#define AWBUGREPORTER_H

#include <QObject>


class QAbstractButton;
class QNetworkReply;

class AWBugReporter : public QObject
{
    Q_OBJECT

public:
    explicit AWBugReporter(QObject *parent = nullptr);
    virtual ~AWBugReporter();
    Q_INVOKABLE void doConnect();
    Q_INVOKABLE QString generateText(const QString description,
                                     const QString reproduce,
                                     const QString expected,
                                     const QString logs) const;
    Q_INVOKABLE void sendBugReport(const QString title, const QString body);

signals:
    void replyReceived(const int number, const QString url);

private slots:
    void issueReplyRecieved(QNetworkReply *reply);
    void showInformation(const int number, const QString url);
    void userReplyOnBugReport(QAbstractButton *button);

private:
    QString m_lastBugUrl;
};


#endif /* AWBUGREPORTER_H */
