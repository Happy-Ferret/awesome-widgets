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
import QtQuick.Dialogs 1.1 as QtDialogs

import org.kde.plasma.private.awesomewidget 1.0


Item {
    id: dataenginePage
    // backend
    AWKeys {
        id: awKeys
    }
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

    property variant cfg_dataengine: awConfig.readDataEngineConfiguration()


    Column {
        id: pageColumn
        anchors.fill: parent
        QtControls.GroupBox {
            height: implicitHeight
            width: parent.width
            title: i18n("ACPI")
            LineSelector {
                text: i18n("ACPI path")
                value: cfg_dataengine["ACPIPATH"]
                onValueEdited: cfg_dataengine["ACPIPATH"] = newValue
            }
        }

        QtControls.GroupBox {
            height: implicitHeight
            width: parent.width
            title: i18n("GPU")
            ComboBoxSelector {
                model: [
                    {
                        'label': "auto",
                        'name': "auto"
                    },
                    {
                        'label': "disable",
                        'name': "disable"
                    },
                    {
                        'label': "ati",
                        'name': "ati"
                    },
                    {
                        'label': "nvidia",
                        'name': "nvidia"
                    }
                ]
                text: i18n("GPU device")
                value: cfg_dataengine["GPUDEV"]
                onValueEdited: cfg_dataengine["GPUDEV"] = newValue
            }
        }

        QtControls.GroupBox {
            height: implicitHeight
            width: parent.width
            title: i18n("HDD temperature")
            Column {
                height: implicitHeight
                width: parent.width
                ComboBoxSelector {
                    id: hdd
                    text: i18n("HDD")
                    value: cfg_dataengine["HDDDEV"]
                    onValueEdited: cfg_dataengine["HDDDEV"] = newValue
                }

                LineSelector {
                    text: i18n("hddtemp cmd")
                    value: cfg_dataengine["HDDTEMPCMD"]
                    onValueEdited: cfg_dataengine["HDDTEMPCMD"] = newValue
                }
            }
        }

        QtControls.GroupBox {
            height: implicitHeight
            width: parent.width
            title: i18n("Player")
            Column {
                height: implicitHeight
                width: parent.width
                IntegerSelector {
                    maximumValue: 100
                    minimumValue: 1
                    stepSize: 1
                    text: i18n("Player data symbols")
                    value: cfg_dataengine["PLAYERSYMBOLS"]
                    onValueEdited: cfg_dataengine["PLAYERSYMBOLS"] = newValue
                }

                ComboBoxSelector {
                    model: [
                        {
                            'label': "disable",
                            'name': "disable"
                        },
                        {
                            'label': "mpris",
                            'name': "mpris"
                        },
                        {
                            'label': "mpd",
                            'name': "mpd"
                        }
                    ]
                    text: i18n("Music player")
                    value: cfg_dataengine["PLAYER"]
                    onValueEdited: cfg_dataengine["PLAYER"] = newValue
                }

                ComboBoxSelector {
                    id: mpris
                    editable: true
                    model: [
                        {
                            'label': 'auto',
                            'name': 'auto'
                        },
                        {
                            'label': 'amarok',
                            'name': 'amarok'
                        },
                        {
                            'label': 'audacious',
                            'name': 'audacious'
                        },
                        {
                            'label': 'clementine',
                            'name': 'clementine'
                        },
                        {
                            'label': 'deadbeef',
                            'name': 'deadbeef'
                        },
                        {
                            'label': 'vlc',
                            'name': 'vlc'
                        },
                        {
                            'label': 'qmmp',
                            'name': 'qmmp'
                        },
                        {
                            'label': 'xmms2',
                            'name': 'xmms2'
                        },
                        {
                            'label': cfg_dataengine["MPRIS"],
                            'name': cfg_dataengine["MPRIS"]
                        }
                    ]
                    text: i18n("MPRIS player name")
                    currentIndex: model.length - 1
                }

                LineSelector {
                    text: i18n("MPD address")
                    value: cfg_dataengine["MPDADDRESS"]
                    onValueEdited: cfg_dataengine["MPDADDRESS"] = newValue
                }

                IntegerSelector {
                    maximumValue: 65535
                    minimumValue: 1000
                    stepSize: 1
                    text: i18n("MPD port")
                    value: cfg_dataengine["MPDPORT"]
                    onValueEdited: cfg_dataengine["MPDPORT"] = newValue
                }
            }
        }

        QtControls.GroupBox {
            height: implicitHeight
            width: parent.width
            title: i18n("Extensions")
            Column {
                height: implicitHeight
                width: parent.width
                Row {
                    height: implicitHeight
                    width: parent.width
                    QtControls.Label {
                        height: parent.height
                        width: parent.width * 2 / 5
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        text: i18n("Custom scripts")
                    }
                    QtControls.Button {
                        width: parent.width * 3 / 5
                        text: i18n("Edit scripts")
                        onClicked: awKeys.editItem("extscript")
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
                        text: i18n("Quotes monitor")
                    }
                    QtControls.Button {
                        width: parent.width * 3 / 5
                        text: i18n("Edit tickers")
                        onClicked: awKeys.editItem("extquotes")
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
                        text: i18n("Package manager")
                    }
                    QtControls.Button {
                        width: parent.width * 3 / 5
                        text: i18n("Edit command")
                        onClicked: awKeys.editItem("extupgrade")
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
                        text: i18n("Weather")
                    }
                    QtControls.Button {
                        width: parent.width * 3 / 5
                        text: i18n("Edit weather")
                        onClicked: awKeys.editItem("extweather")
                    }
                }
            }
        }
    }


    Component.onCompleted: {
        if (debug) console.debug()

        // init submodule
        awKeys.updateCache()

        // update hdd model
        hdd.model = awKeys.getHddDevices()
        hdd.onCompleted
    }

    Component.onDestruction: {
        if (debug) console.debug()

        cfg_dataengine["MPRIS"] = mpris.editText
        awConfig.writeDataEngineConfiguration(cfg_dataengine)
    }
}
