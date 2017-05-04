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

#include "extquotes.h"
#include "ui_extquotes.h"

#include <KI18n/KLocalizedString>

#include <QDir>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QSettings>
#include <QUrlQuery>

#include <qreplytimeout/qreplytimeout.h>

#include "awdebug.h"


ExtQuotes::ExtQuotes(QWidget *parent, const QString &filePath)
    : AbstractExtItem(parent, filePath)
    , ui(new Ui::ExtQuotes)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    if (!filePath.isEmpty())
        readConfiguration();
    ui->setupUi(this);
    translate();

    m_values[tag(QString("ask"))] = 0.0;
    m_values[tag(QString("askchg"))] = 0.0;
    m_values[tag(QString("percaskchg"))] = 0.0;
    m_values[tag(QString("bid"))] = 0.0;
    m_values[tag(QString("bidchg"))] = 0.0;
    m_values[tag(QString("percbidchg"))] = 0.0;
    m_values[tag(QString("price"))] = 0.0;
    m_values[tag(QString("pricechg"))] = 0.0;
    m_values[tag(QString("percpricechg"))] = 0.0;

    // HACK declare as child of nullptr to avoid crash with plasmawindowed
    // in the destructor
    m_manager = new QNetworkAccessManager(nullptr);
    connect(m_manager, SIGNAL(finished(QNetworkReply *)), this,
            SLOT(quotesReplyReceived(QNetworkReply *)));

    connect(this, SIGNAL(requestDataUpdate()), this, SLOT(sendRequest()));
}


ExtQuotes::~ExtQuotes()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    disconnect(m_manager, SIGNAL(finished(QNetworkReply *)), this,
               SLOT(quotesReplyReceived(QNetworkReply *)));
    disconnect(this, SIGNAL(requestDataUpdate()), this, SLOT(sendRequest()));

    m_manager->deleteLater();
    delete ui;
}


ExtQuotes *ExtQuotes::copy(const QString &_fileName, const int _number)
{
    qCDebug(LOG_LIB) << "File" << _fileName << "with number" << _number;

    ExtQuotes *item
        = new ExtQuotes(static_cast<QWidget *>(parent()), _fileName);
    copyDefaults(item);
    item->setNumber(_number);
    item->setTicker(ticker());

    return item;
}


QString ExtQuotes::ticker() const
{
    return m_ticker;
}


QString ExtQuotes::uniq() const
{
    return ticker();
}


void ExtQuotes::setTicker(const QString &_ticker)
{
    qCDebug(LOG_LIB) << "Ticker" << _ticker;

    m_ticker = _ticker;
    initUrl();
}


void ExtQuotes::readConfiguration()
{
    AbstractExtItem::readConfiguration();

    QSettings settings(fileName(), QSettings::IniFormat);

    settings.beginGroup(QString("Desktop Entry"));
    setTicker(settings.value(QString("X-AW-Ticker"), ticker()).toString());
    settings.endGroup();

    bumpApi(AW_EXTQUOTES_API);
}


QVariantHash ExtQuotes::run()
{
    if (m_isRunning)
        return m_values;
    startTimer();

    return m_values;
}


int ExtQuotes::showConfiguration(const QVariant &args)
{
    Q_UNUSED(args)

    ui->lineEdit_name->setText(name());
    ui->lineEdit_comment->setText(comment());
    ui->label_numberValue->setText(QString("%1").arg(number()));
    ui->lineEdit_ticker->setText(ticker());
    ui->checkBox_active->setCheckState(isActive() ? Qt::Checked
                                                  : Qt::Unchecked);
    ui->lineEdit_schedule->setText(cron());
    ui->lineEdit_socket->setText(socket());
    ui->spinBox_interval->setValue(interval());

    int ret = exec();
    if (ret != 1)
        return ret;
    setName(ui->lineEdit_name->text());
    setComment(ui->lineEdit_comment->text());
    setNumber(ui->label_numberValue->text().toInt());
    setApiVersion(AW_EXTQUOTES_API);
    setTicker(ui->lineEdit_ticker->text());
    setActive(ui->checkBox_active->checkState() == Qt::Checked);
    setCron(ui->lineEdit_schedule->text());
    setSocket(ui->lineEdit_socket->text());
    setInterval(ui->spinBox_interval->value());

    writeConfiguration();
    return ret;
}


void ExtQuotes::writeConfiguration() const
{
    AbstractExtItem::writeConfiguration();

    QSettings settings(writtableConfig(), QSettings::IniFormat);
    qCInfo(LOG_LIB) << "Configuration file" << settings.fileName();

    settings.beginGroup(QString("Desktop Entry"));
    settings.setValue(QString("X-AW-Ticker"), ticker());
    settings.endGroup();

    settings.sync();
}

void ExtQuotes::quotesReplyReceived(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qCWarning(LOG_AW) << "An error occurs" << reply->error()
                          << "with message" << reply->errorString();
        return;
    }

    m_isRunning = false;
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll(), &error);
    reply->deleteLater();
    if (error.error != QJsonParseError::NoError) {
        qCWarning(LOG_LIB) << "Parse error" << error.errorString();
        return;
    }
    QVariantMap jsonQuotes
        = jsonDoc.toVariant().toMap()[QString("query")].toMap();
    jsonQuotes
        = jsonQuotes[QString("results")].toMap()[QString("quote")].toMap();
    double value;

    // ask
    value = jsonQuotes[QString("Ask")].toString().toDouble();
    m_values[tag(QString("askchg"))]
        = m_values[tag(QString("ask"))].toDouble() == 0.0
              ? 0.0
              : value - m_values[tag(QString("ask"))].toDouble();
    m_values[tag(QString("percaskchg"))]
        = 100.0 * m_values[tag(QString("askchg"))].toDouble()
          / m_values[tag(QString("ask"))].toDouble();
    m_values[tag(QString("ask"))] = value;

    // bid
    value = jsonQuotes[QString("Bid")].toString().toDouble();
    m_values[tag(QString("bidchg"))]
        = m_values[tag(QString("bid"))].toDouble() == 0.0
              ? 0.0
              : value - m_values[tag(QString("bid"))].toDouble();
    m_values[tag(QString("percbidchg"))]
        = 100.0 * m_values[tag(QString("bidchg"))].toDouble()
          / m_values[tag(QString("bid"))].toDouble();
    m_values[tag(QString("bid"))] = value;

    // last trade
    value = jsonQuotes[QString("LastTradePriceOnly")].toString().toDouble();
    m_values[tag(QString("pricechg"))]
        = m_values[tag(QString("price"))].toDouble() == 0.0
              ? 0.0
              : value - m_values[tag(QString("price"))].toDouble();
    m_values[tag(QString("percpricechg"))]
        = 100.0 * m_values[tag(QString("pricechg"))].toDouble()
          / m_values[tag(QString("price"))].toDouble();
    m_values[tag(QString("price"))] = value;

    emit(dataReceived(m_values));
}


void ExtQuotes::sendRequest()
{
    m_isRunning = true;
    QNetworkReply *reply = m_manager->get(QNetworkRequest(m_url));
    new QReplyTimeout(reply, REQUEST_TIMEOUT);
}


void ExtQuotes::initUrl()
{
    // init query
    m_url = QUrl(YAHOO_QUOTES_URL);
    QUrlQuery params;
    params.addQueryItem(QString("format"), QString("json"));
    params.addQueryItem(QString("env"),
                        QString("store://datatables.org/alltableswithkeys"));
    params.addQueryItem(QString("q"),
                        QString(YAHOO_QUOTES_QUERY).arg(ticker()));
    m_url.setQuery(params);
}


void ExtQuotes::translate()
{
    ui->label_name->setText(i18n("Name"));
    ui->label_comment->setText(i18n("Comment"));
    ui->label_number->setText(i18n("Tag"));
    ui->label->setText(
        i18n("<html><head/><body><p>Use YAHOO! finance ticker to \
get quotes for the instrument. Refer to <a href=\"http://finance.yahoo.com/\">\
<span style=\" text-decoration: underline; color:#0057ae;\">http://finance.yahoo.com/\
</span></a></p></body></html>"));
    ui->label_ticker->setText(i18n("Ticker"));
    ui->checkBox_active->setText(i18n("Active"));
    ui->label_schedule->setText(i18n("Schedule"));
    ui->label_socket->setText(i18n("Socket"));
    ui->label_interval->setText(i18n("Interval"));
}
