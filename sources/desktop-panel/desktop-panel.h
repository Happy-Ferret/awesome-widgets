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


#ifndef DESKTOP_PANEL_H
#define DESKTOP_PANEL_H

#include <Plasma/Applet>
#include <Plasma/DataEngine>
#include <QLabel>

#include <ui_appearance.h>
#include <ui_toggle.h>
#include <ui_widget.h>


class QGraphicsGridLayout;
class QGraphicsProxyWidget;
class QTimer;
class DesktopPanel;


class CustomPlasmaLabel : public QLabel
{
    Q_OBJECT

public:
    CustomPlasmaLabel(DesktopPanel *wid,
                      const int num,
                      const bool debugCmd = false);
    ~CustomPlasmaLabel();
    int getNumber();

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    // debug
    bool debug;
    // variables
    int number;
    DesktopPanel *widget;
};


class DesktopPanel : public Plasma::Applet
{
    Q_OBJECT

    typedef struct {
        QRect desktop;
        QList<QRect> windows;
    } DesktopWindowsInfo;

public:
    DesktopPanel(QObject *parent, const QVariantList &args);
    ~DesktopPanel();
    void init();
    DesktopWindowsInfo getInfoByDesktop(const int num);
    QString parsePattern(const QString rawLine, const int num);

public slots:
    // configuration interface
    void configAccepted();
    void configChanged();
    // events
    void changePanelsState();
    void setCurrentDesktop(const int number);

private slots:
    void reinit();
    void paintTooltip();
    void setFontFormating();
    void setFormating();
    void updateText(const int active);
    void updateTooltip();

protected:
    void createConfigurationInterface(KConfigDialog *parent);

private:
    // functions
    QList<Plasma::Containment *> getPanels();
    QString panelLocationToStr(Plasma::Location loc);
    // ui
    QGraphicsGridLayout *layout;
    QList<QGraphicsProxyWidget *> proxyWidgets;
    QList<CustomPlasmaLabel *> labels;
    QTimer *timer;
    // debug
    bool debug;
    int oldState;
    // configuration interface
    Ui::AppearanceWidget uiAppConfig;
    Ui::ConfigWindow uiWidConfig;
    Ui::ToggleWindow uiToggleConfig;
    // configuration
    QMap<QString, QString> configuration;
    QStringList desktopNames, currentFormatLine, formatLine;
};

K_EXPORT_PLASMA_APPLET(desktop-panel, DesktopPanel)


#endif /* DESKTOP_PANEL_H */
