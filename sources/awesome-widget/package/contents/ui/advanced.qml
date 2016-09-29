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

import QtQuick 2.0
import QtQuick.Controls 1.3 as QtControls
import QtQuick.Dialogs 1.2 as QtDialogs

import org.kde.plasma.private.awesomewidget 1.0


Item {
    id: advancedPage
    // backend
    AWActions {
        id: awActions
    }
    AWConfigHelper {
        id: awConfig
    }

    width: childrenRect.width
    height: childrenRect.height
    implicitWidth: pageColumn.implicitWidth
    implicitHeight: pageColumn.implicitHeight

    property bool debug: awActions.isDebugEnabled()

    property alias cfg_background: background.checked
    property alias cfg_translateStrings: translate.checked
    property alias cfg_wrapNewLines: wrapNewLines.checked
    property alias cfg_wrapText: wordWrap.checked
    property alias cfg_notify: notify.checked
    property alias cfg_checkUpdates: updates.checked
    property alias cfg_optimize: optimize.checked
    property alias cfg_height: widgetHeight.value
    property alias cfg_width: widgetWidth.value
    property alias cfg_interval: update.value
    property alias cfg_queueLimit: queueLimit.value
    property string cfg_tempUnits: tempUnits.value
    property alias cfg_customTime: customTime.value
    property alias cfg_customUptime: customUptime.value
    property alias cfg_acOnline: acOnline.value
    property alias cfg_acOffline: acOffline.value


    Column {
        id: pageColumn
        anchors.fill: parent

        CheckBoxSelector {
            id: background
            text: i18n("Enable background")
        }

        CheckBoxSelector {
            id: translate
            text: i18n("Translate strings")
        }

        CheckBoxSelector {
            id: wrapNewLines
            text: i18n("Wrap new lines")
        }

        CheckBoxSelector {
            id: wordWrap
            text: i18n("Enable word wrap")
        }

        CheckBoxSelector {
            id: notify
            text: i18n("Enable notifications")
        }

        CheckBoxSelector {
            id: updates
            text: i18n("Check updates on startup")
        }

        CheckBoxSelector {
            id: optimize
            text: i18n("Optimize subscription")
        }

        IntegerSelector {
            id: widgetHeight
            maximumValue: 4096
            minimumValue: 0
            stepSize: 50
            text: i18n("Widget height, px")
            value: plasmoid.configuration.height
        }

        IntegerSelector {
            id: widgetWidth
            maximumValue: 4096
            minimumValue: 0
            stepSize: 50
            text: i18n("Widget width, px")
            value: plasmoid.configuration.width
        }

        IntegerSelector {
            id: update
            maximumValue: 10000
            minimumValue: 1000
            stepSize: 500
            text: i18n("Time interval")
            value: plasmoid.configuration.interval
        }

        IntegerSelector {
            id: queueLimit
            maximumValue: 99
            minimumValue: 0
            stepSize: 1
            text: i18n("Messages queue limit")
            value: plasmoid.configuration.queueLimit
        }

        ComboBoxSelector {
            id: tempUnits
            model: [
                {
                    'label': i18n("Celsius"),
                    'name': "Celsius"
                },
                {
                    'label': i18n("Fahrenheit"),
                    'name': "Fahrenheit"
                },
                {
                    'label': i18n("Kelvin"),
                    'name': "Kelvin"
                },
                {
                    'label': i18n("Reaumur"),
                    'name': "Reaumur"
                },
                {
                    'label': i18n("cm^-1"),
                    'name': "cm^-1"
                },
                {
                    'label': i18n("kJ/mol"),
                    'name': "kJ/mol"
                },
                {
                    'label': i18n("kcal/mol"),
                    'name': "kcal/mol"
                }
            ]
            text: i18n("Temperature units")
            value: plasmoid.configuration.tempUnits
            onValueEdited: cfg_tempUnits = newValue
        }

        LineSelector {
            id: customTime
            text: i18n("Custom time format")
            value: plasmoid.configuration.customTime
        }

        LineSelector {
            id: customUptime
            text: i18n("Custom uptime format")
            value: plasmoid.configuration.customUptime
        }

        LineSelector {
            id: acOnline
            text: i18n("AC online tag")
            value: plasmoid.configuration.acOnline
        }

        LineSelector {
            id: acOffline
            text: i18n("AC offline tag")
            value: plasmoid.configuration.acOffline
        }

        ButtonSelector {
            value: i18n("Drop key cache")
            onButtonActivated: awActions.dropCache()
        }

        Row {
            height: implicitHeight
            width: parent.width
            QtControls.Label {
                height: parent.height
                width: parent.width * 2 / 5
            }
            QtControls.Button {
                width: parent.width * 3 / 5
                text: i18n("Export configuration")
                onClicked: saveConfigAs.open()
            }

            QtDialogs.FileDialog {
                id: saveConfigAs
                selectExisting: false
                title: i18n("Export")
                folder: awConfig.configurationDirectory()
                onAccepted: {
                    var status = awConfig.exportConfiguration(
                        plasmoid.configuration,
                        saveConfigAs.fileUrl.toString().replace("file://", ""))
                    if (status) {
                        messageDialog.title = i18n("Success")
                        messageDialog.text = i18n("Please note that binary files were not copied")
                    } else {
                        messageDialog.title = i18n("Ooops...")
                        messageDialog.text = i18n("Could not save configuration file")
                    }
                    messageDialog.open()
                }
            }

            QtDialogs.MessageDialog {
                id: messageDialog
                standardButtons: QtDialogs.StandardButton.Ok
            }
        }

        Row {
            height: implicitHeight
            width: parent.width
            QtControls.Label {
                height: parent.height
                width: parent.width * 2 / 5
            }
            QtControls.Button {
                width: parent.width * 3 / 5
                text: i18n("Import configuration")
                onClicked: openConfig.open()
            }

            QtDialogs.FileDialog {
                id: openConfig
                title: i18n("Import")
                folder: awConfig.configurationDirectory()
                onAccepted: importSelection.open()
            }

            QtDialogs.Dialog {
                id: importSelection

                Column {
                    QtControls.CheckBox {
                        id: importPlasmoid
                        text: i18n("Import plasmoid settings")
                    }

                    QtControls.CheckBox {
                        id: importExtensions
                        text: i18n("Import extensions")
                    }

                    QtControls.CheckBox {
                        id: importAdds
                        text: i18n("Import additional files")
                    }
                }

                onAccepted: {
                    if (debug) console.debug()
                    var importConfig = awConfig.importConfiguration(
                        openConfig.fileUrl.toString().replace("file://", ""),
                        importPlasmoid.checked, importExtensions.checked,
                        importAdds.checked)
                    for (var key in importConfig)
                        plasmoid.configuration[key] = importConfig[key]
                }
            }
        }
    }


    Component.onCompleted: {
        if (debug) console.debug()
    }
}
