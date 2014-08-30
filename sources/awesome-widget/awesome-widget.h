/***************************************************************************
 *   This file is part of pytextmonitor                                    *
 *                                                                         *
 *   pytextmonitor is free software: you can redistribute it and/or        *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   pytextmonitor is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with pytextmonitor. If not, see http://www.gnu.org/licenses/    *
 ***************************************************************************/


#ifndef AWESOME_WIDGET_H
#define AWESOME_WIDGET_H

#include <Plasma/Applet>
#include <Plasma/DataEngine>
#include <Plasma/Label>

#include <ui_advanced.h>
#include <ui_appearance.h>
#include <ui_deconfig.h>
#include <ui_tooltipconfig.h>
#include <ui_widget.h>


class QGraphicsLinearLayout;

class AwesomeWidget : public Plasma::Applet
{
    Q_OBJECT

public:
    AwesomeWidget(QObject *parent, const QVariantList &args);
    ~AwesomeWidget();
    QString getNetworkDevice();
    void init();
    // de configuration
    QMap<QString, QString> readDataEngineConfiguration();
    void writeDataEngineConfiguration(const QMap<QString, QString> settings);
    QMap<QString, QString> updateDataEngineConfiguration(const QMap<QString, QString> rawConfig);

public slots:
    // dataengine
    void connectToEngine();
    void dataUpdated(const QString &sourceName, const Plasma::DataEngine::Data &data);
    void disconnectFromEngine();
    // configuration interface
    void configAccepted();
    void configChanged();

private slots:
    void reinit();
    // configuration interface
    void setNetworkDevice();
    void setTooltipBackground();

protected:
    void createConfigurationInterface(KConfigDialog *parent);

private:
    // functions
    void updateText();
    // ui
    QGraphicsLinearLayout *layout;
    // debug
    bool debug;
    // data engine
    Plasma::DataEngine *extsysmonEngine;
    Plasma::DataEngine *sysmonEngine;
    Plasma::DataEngine *timeEngine;
    // configuration interface
    Ui::AdvancedWindow uiAdvancedConfig;
    Ui::AppearanceWindow uiAppConfig;
    Ui::ConfigWindow uiWidConfig;
    Ui::DEWindow uiDEConfig;
    Ui::TooltipWindow uiTooltipConfig;
    // configuration
    QMap<QString, QString> configuration;
    QStringList diskDevices;
    QStringList formatLine;
};


#endif /* AWESOME_WIDGET_H */
