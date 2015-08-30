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

#include "extscript.h"
#include "ui_extscript.h"

#include <KI18n/KLocalizedString>

#include <QDir>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QSettings>
#include <QStandardPaths>
#include <QTextCodec>

#include "awdebug.h"
#include "version.h"


ExtScript::ExtScript(QWidget *parent, const QString scriptName,
                     const QStringList directories)
    : AbstractExtItem(parent, scriptName, directories),
      ui(new Ui::ExtScript)
{
    qCDebug(LOG_LIB);

    readConfiguration();
    readJsonFilters();
    ui->setupUi(this);
    translate();

    value[tag(QString("custom"))] = QString("");

    process = new QProcess(this);
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(updateValue()));
    process->waitForFinished(0);
}


ExtScript::~ExtScript()
{
    qCDebug(LOG_LIB);

    process->kill();
    delete process;
    delete ui;
}


ExtScript *ExtScript::copy(const QString _fileName, const int _number)
{
    qCDebug(LOG_LIB);
    qCDebug(LOG_LIB) << "File" << _fileName;
    qCDebug(LOG_LIB) << "Number" << _number;

    ExtScript *item = new ExtScript(static_cast<QWidget *>(parent()), _fileName, directories());
    item->setActive(isActive());
    item->setApiVersion(apiVersion());
    item->setComment(comment());
    item->setExecutable(executable());
    item->setHasOutput(hasOutput());
    item->setInterval(interval());
    item->setName(name());
    item->setNumber(_number);
    item->setPrefix(prefix());
    item->setRedirect(redirect());

    return item;
}


QString ExtScript::executable() const
{
    qCDebug(LOG_LIB);

    return m_executable;
}


QStringList ExtScript::filters() const
{
    qCDebug(LOG_LIB);

    return m_filters;
}


bool ExtScript::hasOutput() const
{
    qCDebug(LOG_LIB);

    return m_output;
}


QString ExtScript::prefix() const
{
    qCDebug(LOG_LIB);

    return m_prefix;
}


ExtScript::Redirect ExtScript::redirect() const
{
    qCDebug(LOG_LIB);

    return m_redirect;
}


QString ExtScript::uniq() const
{
    qCDebug(LOG_LIB);

    return m_executable;
}


QString ExtScript::strRedirect() const
{
    qCDebug(LOG_LIB);

    QString value;
    switch (m_redirect) {
    case stdout2stderr:
        value = QString("stdout2stderr");
        break;
    case stderr2stdout:
        value = QString("stderr2stdout");
        break;
    case nothing:
    default:
        value = QString("nothing");
        break;
    }

    return value;
}


void ExtScript::setExecutable(const QString _executable)
{
    qCDebug(LOG_LIB);
    qCDebug(LOG_LIB) << "Executable" << _executable;

    m_executable = _executable;
}


void ExtScript::setFilters(const QStringList _filters)
{
    qCDebug(LOG_LIB);
    qCDebug(LOG_LIB) << "Filters" << _filters;

    std::for_each(_filters.cbegin(), _filters.cend(),
                  [this](QString filter) { return updateFilter(filter); });
//     foreach(QString filter, _filters)
//         updateFilter(filter);
}


void ExtScript::setHasOutput(const bool _state)
{
    qCDebug(LOG_LIB);
    qCDebug(LOG_LIB) << "State" << _state;

    m_output = _state;
}


void ExtScript::setPrefix(const QString _prefix)
{
    qCDebug(LOG_LIB);
    qCDebug(LOG_LIB) << "Prefix" << _prefix;

    m_prefix = _prefix;
}


void ExtScript::setRedirect(const Redirect _redirect)
{
    qCDebug(LOG_LIB);
    qCDebug(LOG_LIB) << "Redirect" << _redirect;

    m_redirect = _redirect;
}


void ExtScript::setStrRedirect(const QString _redirect)
{
    qCDebug(LOG_LIB);
    qCDebug(LOG_LIB) << "Redirect" << _redirect;

    if (_redirect == QString("stdout2sdterr"))
        m_redirect = stdout2stderr;
    else if (_redirect == QString("stderr2sdtout"))
        m_redirect = stderr2stdout;
    else
        m_redirect = nothing;
}


QString ExtScript::applyFilters(QString _value) const
{
    qCDebug(LOG_LIB);
    qCDebug(LOG_LIB) << "Value" << _value;

    foreach(QString filt, m_filters) {
        qCInfo(LOG_LIB) << "Found filter" << filt;
        QVariantMap filter = jsonFilters[filt].toMap();
        if (filter.isEmpty()) {
            qCWarning(LOG_LIB) << "Could not find filter" << _value << "in the json";
            continue;
        }
        foreach(QString f, filter.keys())
            _value.replace(f, filter[f].toString());
    }

    return _value;
}


void ExtScript::updateFilter(const QString _filter, const bool _add)
{
    qCDebug(LOG_LIB);
    qCDebug(LOG_LIB) << "Filter" << _filter;
    qCDebug(LOG_LIB) << "Should be added" << _add;

    if (_add) {
        if (m_filters.contains(_filter)) return;
        m_filters.append(_filter);
    } else {
        m_filters.removeOne(_filter);
    }
}


void ExtScript::readConfiguration()
{
    qCDebug(LOG_LIB);
    AbstractExtItem::readConfiguration();

    for (int i=directories().count()-1; i>=0; i--) {
        if (!QDir(directories().at(i)).entryList(QDir::Files).contains(fileName())) continue;
        QSettings settings(QString("%1/%2").arg(directories().at(i)).arg(fileName()), QSettings::IniFormat);

        settings.beginGroup(QString("Desktop Entry"));
        setExecutable(settings.value(QString("Exec"), m_executable).toString());
        setPrefix(settings.value(QString("X-AW-Prefix"), m_prefix).toString());
        setHasOutput(settings.value(QString("X-AW-Output"), QVariant(m_output)).toString() == QString("true"));
        setStrRedirect(settings.value(QString("X-AW-Redirect"), strRedirect()).toString());
        // api == 3
        setFilters(settings.value(QString("X-AW-Filters"), m_filters).toString()
                                                                     .split(QChar(','), QString::SkipEmptyParts));
        settings.endGroup();
    }

    if (!m_output)
        setRedirect(stdout2stderr);

    // update for current API
    if ((apiVersion() > 0) && (apiVersion() < AWESAPI)) {
        qCWarning(LOG_LIB) << "Bump API version from" << apiVersion() << "to" << AWESAPI;
        setApiVersion(AWESAPI);
        writeConfiguration();
    }
}


void ExtScript::readJsonFilters()
{
    qCDebug(LOG_LIB);

    QString fileName = QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                              QString("awesomewidgets/scripts/awesomewidgets-extscripts-filters.json"));
    qCInfo(LOG_LIB) << "Filters file" << fileName;
    QFile jsonFile(fileName);
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCWarning(LOG_LIB) << "Could not open" << fileName;
        return;
    }
    QString jsonText = jsonFile.readAll();
    jsonFile.close();

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonText.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
        qCWarning(LOG_LIB) << "Parse error" << error.errorString();
        return;
    }
    jsonFilters = jsonDoc.toVariant().toMap();

    qCInfo(LOG_LIB) << "Filters" << jsonFilters;
}


QVariantHash ExtScript::run()
{
    qCDebug(LOG_LIB);
    if (!isActive()) return value;

    if ((times == 1) && (process->state() == QProcess::NotRunning)) {
        QStringList cmdList;
        if (!m_prefix.isEmpty()) cmdList.append(m_prefix);
        cmdList.append(m_executable);
        qCInfo(LOG_LIB) << "Run cmd" << cmdList.join(QChar(' '));
        process->start(cmdList.join(QChar(' ')));
    } else if (times >= interval())
        times = 0;
    times++;

    return value;
}


int ExtScript::showConfiguration(const QVariant args)
{
    Q_UNUSED(args)
    qCDebug(LOG_LIB);

    ui->lineEdit_name->setText(name());
    ui->lineEdit_comment->setText(comment());
    ui->label_numberValue->setText(QString("%1").arg(number()));
    ui->lineEdit_command->setText(m_executable);
    ui->lineEdit_prefix->setText(m_prefix);
    ui->checkBox_active->setCheckState(isActive() ? Qt::Checked : Qt::Unchecked);
    ui->checkBox_output->setCheckState(m_output ? Qt::Checked : Qt::Unchecked);
    ui->comboBox_redirect->setCurrentIndex(static_cast<int>(m_redirect));
    ui->spinBox_interval->setValue(interval());
    // filters
    ui->checkBox_colorFilter->setCheckState(m_filters.contains(QString("color")) ? Qt::Checked : Qt::Unchecked);
    ui->checkBox_linesFilter->setCheckState(m_filters.contains(QString("newline")) ? Qt::Checked : Qt::Unchecked);
    ui->checkBox_spaceFilter->setCheckState(m_filters.contains(QString("space")) ? Qt::Checked : Qt::Unchecked);

    int ret = exec();
    if (ret != 1) return ret;
    setName(ui->lineEdit_name->text());
    setComment(ui->lineEdit_comment->text());
    setNumber(ui->label_numberValue->text().toInt());
    setApiVersion(AWESAPI);
    setExecutable(ui->lineEdit_command->text());
    setPrefix(ui->lineEdit_prefix->text());
    setActive(ui->checkBox_active->checkState() == Qt::Checked);
    setHasOutput(ui->checkBox_output->checkState() == Qt::Checked);
    setStrRedirect(ui->comboBox_redirect->currentText());
    setInterval(ui->spinBox_interval->value());
    // filters
    updateFilter(QString("color"), ui->checkBox_colorFilter->checkState() == Qt::Checked);
    updateFilter(QString("newline"), ui->checkBox_linesFilter->checkState() == Qt::Checked);
    updateFilter(QString("space"), ui->checkBox_spaceFilter->checkState() == Qt::Checked);

    writeConfiguration();
    return ret;
}


void ExtScript::writeConfiguration() const
{
    qCDebug(LOG_LIB);
    AbstractExtItem::writeConfiguration();

    QSettings settings(QString("%1/%2").arg(directories().first()).arg(fileName()), QSettings::IniFormat);
    qCInfo(LOG_LIB) << "Configuration file" << settings.fileName();

    settings.beginGroup(QString("Desktop Entry"));
    settings.setValue(QString("Exec"), m_executable);
    settings.setValue(QString("X-AW-Prefix"), m_prefix);
    settings.setValue(QString("X-AW-Output"), QVariant(m_output).toString());
    settings.setValue(QString("X-AW-Redirect"), strRedirect());
    settings.setValue(QString("X-AW-Filters"), m_filters.join(QChar(',')));
    settings.endGroup();

    settings.sync();
}


void ExtScript::updateValue()
{
    qCDebug(LOG_LIB);

    qCInfo(LOG_LIB) << "Cmd returns" << process->exitCode();
    QString qdebug = QTextCodec::codecForMib(106)->toUnicode(process->readAllStandardError()).trimmed();
    qCInfo(LOG_LIB) << "Error" << qdebug;
    QString qoutput = QTextCodec::codecForMib(106)->toUnicode(process->readAllStandardOutput()).trimmed();
    qCInfo(LOG_LIB) << "Error" << qoutput;
    QString strValue;

    switch (m_redirect) {
    case stdout2stderr:
        break;
    case stderr2stdout:
        strValue = QString("%1\n%2").arg(qdebug).arg(qoutput);
        break;
    case nothing:
    default:
        strValue = qoutput;
        break;
    }

    // filters
    value[tag(QString("custom"))] = applyFilters(strValue);
}


void ExtScript::translate()
{
    qCDebug(LOG_LIB);

    ui->label_name->setText(i18n("Name"));
    ui->label_comment->setText(i18n("Comment"));
    ui->label_number->setText(i18n("Tag"));
    ui->label_command->setText(i18n("Command"));
    ui->label_prefix->setText(i18n("Prefix"));
    ui->checkBox_active->setText(i18n("Active"));
    ui->checkBox_output->setText(i18n("Has output"));
    ui->label_redirect->setText(i18n("Redirect"));
    ui->label_interval->setText(i18n("Interval"));
    ui->groupBox_filters->setTitle(i18n("Additional filters"));
    ui->checkBox_colorFilter->setText(i18n("Wrap colors"));
    ui->checkBox_linesFilter->setText(i18n("Wrap new lines"));
    ui->checkBox_spaceFilter->setText(i18n("Wrap spaces"));
}
