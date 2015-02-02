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
import QtQuick.Controls.Styles 1.3 as QtStyles
import QtQuick.Dialogs 1.1 as QtDialogs

import org.kde.plasma.private.awesomewidget 1.0
import "."


Item {
    id: tooltipPage
    width: childrenRect.width
    height: childrenRect.height
    implicitWidth: pageColumn.implicitWidth
    implicitHeight: pageColumn.implicitHeight

    property bool debug: AWKeys.isDebugEnabled()

    property alias cfg_tooltipNumber: tooltipNumber.value
    property alias cfg_useTooltipBackground: useTooltipBackground.checked
    property alias cfg_tooltipBackground: tooltipBackground.text
    property alias cfg_cpuTooltip: cpuTooltip.checked
    property alias cfg_cpuTooltipColor: cpuTooltipColor.text
    property alias cfg_cpuclTooltip: cpuclTooltip.checked
    property alias cfg_cpuclTooltipColor: cpuclTooltipColor.text
    property alias cfg_memTooltip: memTooltip.checked
    property alias cfg_memTooltipColor: memTooltipColor.text
    property alias cfg_swapTooltip: swapTooltip.checked
    property alias cfg_swapTooltipColor: swapTooltipColor.text
    property alias cfg_downTooltip: downTooltip.checked
    property alias cfg_downTooltipColor: downTooltipColor.text
    property alias cfg_upTooltipColor: upTooltipColor.text
    property alias cfg_batteryTooltip: batteryTooltip.checked
    property alias cfg_batteryTooltipColor: batteryTooltipColor.text
    property alias cfg_batteryInTooltipColor: batteryInTooltipColor.text


    Column {
        id: pageColumn
        width: units.gridUnit * 25
        QtControls.Label {
          width: parent.width
          horizontalAlignment: Text.AlignHCenter
          verticalAlignment: Text.AlignVCenter
          wrapMode: Text.WordWrap
          text: i18n("CPU, CPU clock, memory, swap and network labels support graphical tooltip. To enable them just make needed checkbox fully checked.")
        }

        Row {
            height: implicitHeight
            width: parent.width
            QtControls.Label {
                height: parent.height
                width: parent.width * 2 / 5
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                text: i18n("Number of values for tooltips")
            }
            QtControls.SpinBox {
                id: tooltipNumber
                width: parent.width * 3 / 5
                minimumValue: 50
                maximumValue: 1000
                stepSize: 25
                value: plasmoid.configuration.tooltipNumber
            }
        }

        Row {
            height: implicitHeight
            width: parent.width
            QtControls.Label {
                height: parent.height
                width: parent.width * 2 / 5 - useTooltipBackground.width
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                text: i18n("Background")
            }
            QtControls.CheckBox {
                id: useTooltipBackground
                height: parent.height
                width: implicitWidth
                style: QtStyles.CheckBoxStyle {
                    indicator: Rectangle {
                        implicitWidth: 16
                        implicitHeight: 16
                        radius: 3
                        border.width: 1
                        border.color: control.activeFocus ? "darkblue" : "gray"
                        Rectangle {
                            visible: control.checked
                            radius: 1
                            anchors.fill: parent
                            anchors.margins: 4
                            color: "#555555"
                            border.color: "#333333"
                        }
                    }
                }
            }
            QtControls.Button {
                id: tooltipBackground
                width: parent.width * 3 / 5
                style: QtStyles.ButtonStyle {
                    background: Rectangle {
                        color: plasmoid.configuration.tooltipBackground
                    }
                }
                text: plasmoid.configuration.tooltipBackground
                onClicked: tooltipBackgroundDialog.visible = true
            }

            QtDialogs.ColorDialog {
                id: tooltipBackgroundDialog
                title: i18n("Select a color")
                color: tooltipBackground.text
                onAccepted: tooltipBackground.text = tooltipBackgroundDialog.color
            }
        }

        Row {
            height: implicitHeight
            width: parent.width
            QtControls.Label {
                height: parent.height
                width: parent.width * 2 / 5 - cpuTooltip.width
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                text: i18n("CPU color")
            }
            QtControls.CheckBox {
                id: cpuTooltip
                height: parent.height
                width: implicitWidth
                style: QtStyles.CheckBoxStyle {
                    indicator: Rectangle {
                        implicitWidth: 16
                        implicitHeight: 16
                        radius: 3
                        border.width: 1
                        border.color: control.activeFocus ? "darkblue" : "gray"
                        Rectangle {
                            visible: control.checked
                            radius: 1
                            anchors.fill: parent
                            anchors.margins: 4
                            color: "#555555"
                            border.color: "#333333"
                        }
                    }
                }
            }
            QtControls.Button {
                id: cpuTooltipColor
                width: parent.width * 3 / 5
                style: QtStyles.ButtonStyle {
                    background: Rectangle {
                        color: plasmoid.configuration.cpuTooltipColor
                    }
                }
                text: plasmoid.configuration.cpuTooltipColor
                onClicked: cpuTooltipColorDialog.visible = true
            }

            QtDialogs.ColorDialog {
                id: cpuTooltipColorDialog
                title: i18n("Select a color")
                color: cpuTooltipColor.text
                onAccepted: cpuTooltipColor.text = cpuTooltipColorDialog.color
            }
        }

        Row {
            height: implicitHeight
            width: parent.width
            QtControls.Label {
                height: parent.height
                width: parent.width * 2 / 5 - cpuclTooltip.width
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                text: i18n("CPU clock color")
            }
            QtControls.CheckBox {
                id: cpuclTooltip
                height: parent.height
                width: implicitWidth
                style: QtStyles.CheckBoxStyle {
                    indicator: Rectangle {
                        implicitWidth: 16
                        implicitHeight: 16
                        radius: 3
                        border.width: 1
                        border.color: control.activeFocus ? "darkblue" : "gray"
                        Rectangle {
                            visible: control.checked
                            radius: 1
                            anchors.fill: parent
                            anchors.margins: 4
                            color: "#555555"
                            border.color: "#333333"
                        }
                    }
                }
            }
            QtControls.Button {
                id: cpuclTooltipColor
                width: parent.width * 3 / 5
                style: QtStyles.ButtonStyle {
                    background: Rectangle {
                        color: plasmoid.configuration.cpuclTooltipColor
                    }
                }
                text: plasmoid.configuration.cpuclTooltipColor
                onClicked: cpuclTooltipColorDialog.visible = true
            }

            QtDialogs.ColorDialog {
                id: cpuclTooltipColorDialog
                title: i18n("Select a color")
                color: cpuclTooltipColor.text
                onAccepted: cpuclTooltipColor.text = cpuclTooltipColorDialog.color
            }
        }

        Row {
            height: implicitHeight
            width: parent.width
            QtControls.Label {
                height: parent.height
                width: parent.width * 2 / 5 - memTooltip.width
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                text: i18n("Memory color")
            }
            QtControls.CheckBox {
                id: memTooltip
                height: parent.height
                width: implicitWidth
                style: QtStyles.CheckBoxStyle {
                    indicator: Rectangle {
                        implicitWidth: 16
                        implicitHeight: 16
                        radius: 3
                        border.width: 1
                        border.color: control.activeFocus ? "darkblue" : "gray"
                        Rectangle {
                            visible: control.checked
                            radius: 1
                            anchors.fill: parent
                            anchors.margins: 4
                            color: "#555555"
                            border.color: "#333333"
                        }
                    }
                }
            }
            QtControls.Button {
                id: memTooltipColor
                width: parent.width * 3 / 5
                style: QtStyles.ButtonStyle {
                    background: Rectangle {
                        color: plasmoid.configuration.memTooltipColor
                    }
                }
                text: plasmoid.configuration.memTooltipColor
                onClicked: memTooltipColorDialog.visible = true
            }

            QtDialogs.ColorDialog {
                id: memTooltipColorDialog
                title: i18n("Select a color")
                color: memTooltipColor.text
                onAccepted: memTooltipColor.text = memTooltipColorDialog.color
            }
        }

        Row {
            height: implicitHeight
            width: parent.width
            QtControls.Label {
                height: parent.height
                width: parent.width * 2 / 5 - swapTooltip.width
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                text: i18n("Swap color")
            }
            QtControls.CheckBox {
                id: swapTooltip
                height: parent.height
                width: implicitWidth
                style: QtStyles.CheckBoxStyle {
                    indicator: Rectangle {
                        implicitWidth: 16
                        implicitHeight: 16
                        radius: 3
                        border.width: 1
                        border.color: control.activeFocus ? "darkblue" : "gray"
                        Rectangle {
                            visible: control.checked
                            radius: 1
                            anchors.fill: parent
                            anchors.margins: 4
                            color: "#555555"
                            border.color: "#333333"
                        }
                    }
                }
            }
            QtControls.Button {
                id: swapTooltipColor
                width: parent.width * 3 / 5
                style: QtStyles.ButtonStyle {
                    background: Rectangle {
                        color: plasmoid.configuration.swapTooltipColor
                    }
                }
                text: plasmoid.configuration.swapTooltipColor
                onClicked: swapTooltipColorDialog.visible = true
            }

            QtDialogs.ColorDialog {
                id: swapTooltipColorDialog
                title: i18n("Select a color")
                color: swapTooltipColor.text
                onAccepted: swapTooltipColor.text = swapTooltipColorDialog.color
            }
        }

        Row {
            height: implicitHeight
            width: parent.width
            QtControls.Label {
                height: parent.height
                width: parent.width * 2 / 5 - downTooltip.width
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                text: i18n("Download speed color")
            }
            QtControls.CheckBox {
                id: downTooltip
                height: parent.height
                width: implicitWidth
                style: QtStyles.CheckBoxStyle {
                    indicator: Rectangle {
                        implicitWidth: 16
                        implicitHeight: 16
                        radius: 3
                        border.width: 1
                        border.color: control.activeFocus ? "darkblue" : "gray"
                        Rectangle {
                            visible: control.checked
                            radius: 1
                            anchors.fill: parent
                            anchors.margins: 4
                            color: "#555555"
                            border.color: "#333333"
                        }
                    }
                }
            }
            QtControls.Button {
                id: downTooltipColor
                width: parent.width * 3 / 5
                style: QtStyles.ButtonStyle {
                    background: Rectangle {
                        color: plasmoid.configuration.downTooltipColor
                    }
                }
                text: plasmoid.configuration.downTooltipColor
                onClicked: downTooltipColorDialog.visible = true
            }

            QtDialogs.ColorDialog {
                id: downTooltipColorDialog
                title: i18n("Select a color")
                color: downTooltipColor.text
                onAccepted: downTooltipColor.text = downTooltipColorDialog.color
            }
        }

        Row {
            height: implicitHeight
            width: parent.width
            QtControls.Label {
                height: parent.height
                width: parent.width * 2 / 5
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                text: i18n("Upload speed color")
            }
            QtControls.Button {
                id: upTooltipColor
                width: parent.width * 3 / 5
                style: QtStyles.ButtonStyle {
                    background: Rectangle {
                        color: plasmoid.configuration.upTooltipColor
                    }
                }
                text: plasmoid.configuration.upTooltipColor
                onClicked: upTooltipColorDialog.visible = true
            }

            QtDialogs.ColorDialog {
                id: upTooltipColorDialog
                title: i18n("Select a color")
                color: upTooltipColor.text
                onAccepted: upTooltipColor.text = upTooltipColorDialog.color
            }
        }

        Row {
            height: implicitHeight
            width: parent.width
            QtControls.Label {
                height: parent.height
                width: parent.width * 2 / 5 - batteryTooltip.width
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                text: i18n("Battery active color")
            }
            QtControls.CheckBox {
                id: batteryTooltip
                height: parent.height
                width: implicitWidth
                style: QtStyles.CheckBoxStyle {
                    indicator: Rectangle {
                        implicitWidth: 16
                        implicitHeight: 16
                        radius: 3
                        border.width: 1
                        border.color: control.activeFocus ? "darkblue" : "gray"
                        Rectangle {
                            visible: control.checked
                            radius: 1
                            anchors.fill: parent
                            anchors.margins: 4
                            color: "#555555"
                            border.color: "#333333"
                        }
                    }
                }
            }
            QtControls.Button {
                id: batteryTooltipColor
                width: parent.width * 3 / 5
                style: QtStyles.ButtonStyle {
                    background: Rectangle {
                        color: plasmoid.configuration.batteryTooltipColor
                    }
                }
                text: plasmoid.configuration.batteryTooltipColor
                onClicked: batteryTooltipColorDialog.visible = true
            }

            QtDialogs.ColorDialog {
                id: batteryTooltipColorDialog
                title: i18n("Select a color")
                color: batteryTooltipColor.text
                onAccepted: batteryTooltipColor.text = batteryTooltipColorDialog.color
            }
        }

        Row {
            height: implicitHeight
            width: parent.width
            QtControls.Label {
                height: parent.height
                width: parent.width * 2 / 5
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                text: i18n("Battery inactive color")
            }
            QtControls.Button {
                id: batteryInTooltipColor
                width: parent.width * 3 / 5
                style: QtStyles.ButtonStyle {
                    background: Rectangle {
                        color: plasmoid.configuration.batteryInTooltipColor
                    }
                }
                text: plasmoid.configuration.batteryInTooltipColor
                onClicked: batteryInTooltipColorDialog.visible = true
            }

            QtDialogs.ColorDialog {
                id: batteryInTooltipColorDialog
                title: i18n("Select a color")
                color: batteryInTooltipColor.text
                onAccepted: batteryInTooltipColor.text = batteryInTooltipColorDialog.color
            }
        }
    }

    Component.onCompleted: {
        if (debug) console.log("[tooltip::onCompleted]")
    }
}
