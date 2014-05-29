# -*- coding: utf-8 -*-

############################################################################
#   This file is part of pytextmonitor                                     #
#                                                                          #
#   pytextmonitor is free software: you can redistribute it and/or         #
#   modify it under the terms of the GNU General Public License as         #
#   published by the Free Software Foundation, either version 3 of the     #
#   License, or (at your option) any later version.                        #
#                                                                          #
#   pytextmonitor is distributed in the hope that it will be useful,       #
#   but WITHOUT ANY WARRANTY; without even the implied warranty of         #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          #
#   GNU General Public License for more details.                           #
#                                                                          #
#   You should have received a copy of the GNU General Public License      #
#   along with pytextmonitor. If not, see http://www.gnu.org/licenses/     #
############################################################################

from PyQt4.QtCore import *
from PyQt4.QtGui import *
import config



class Reinit():
    def __init__(self, parent, defaults=None):
        """class definition"""
        self.parent = parent
        self.defaults = defaults
        self.labels = defaults['format'].keys()


    def reinit(self):
        """function to reinitializate widget"""
        settings = config.Config(self.parent)
        ptmVars = {}

        ptmVars['adv'] = {}
        ptmVars['adv']['acDev'] = str(settings.get('ac_device', '/sys/class/power_supply/AC/online'))
        ptmVars['adv']['acOnline'] = str(settings.get('ac_online', '(*)'))
        ptmVars['adv']['acOffline'] = str(settings.get('ac_offline', '( )'))
        ptmVars['adv']['batDev'] = str(settings.get('battery_device', '/sys/class/power_supply/BAT0/capacity'))
        ptmVars['adv']['customTime'] = str(settings.get('custom_time', '$hh:$mm'))
        ptmVars['adv']['customUptime'] = str(settings.get('custom_uptime', '$dd,$hh,$mm'))
        ptmVars['adv']['layout'] = settings.get('layout', 0).toInt()[0]
        ptmVars['adv']['netdevBool'] = settings.get('netdevBool', 0).toInt()[0]
        ptmVars['adv']['netdev'] = str(settings.get('custom_netdev', 'lo'))
        ptmVars['adv']['netDir'] = str(settings.get('netdir', '/sys/class/net'))
        ptmVars['adv']['tempUnits'] = str(settings.get('temp_units', 'Celsius'))
        ptmVars['adv']['popup'] = settings.get('popup', 2).toInt()[0]

        ptmVars['app'] = {}
        ptmVars['app']['format'] = ["<pre><p align=\"center\"><span style=\" font-family:'" + str(settings.get('font_family', 'Terminus')) +\
            "'; font-style:" + str(settings.get('font_style', 'normal')) + "; font-size:" + str(settings.get('font_size', 12)) +\
            "pt; font-weight:" + str(settings.get('font_weight', 400)) + "; color:" + str(settings.get('font_color', '#000000')) +\
            ";\">", "</span></p></pre>"]
        ptmVars['app']['interval'] = settings.get('interval', 2000).toInt()[0]
        ptmVars['app']['order'] = str(settings.get('label_order', '1345'))

        ptmVars['tooltip'] = {}
        ptmVars['tooltip']['colors'] = {}
        ptmVars['tooltip']['colors']['cpu'] = str(settings.get('cpu_color', '#ff0000'))
        ptmVars['tooltip']['colors']['cpuclock'] = str(settings.get('cpuclock_color', '#00ff00'))
        ptmVars['tooltip']['colors']['mem'] = str(settings.get('mem_color', '#0000ff'))
        ptmVars['tooltip']['colors']['swap'] = str(settings.get('swap_color', '#ffff00'))
        ptmVars['tooltip']['colors']['down'] = str(settings.get('down_color', '#00ffff'))
        ptmVars['tooltip']['colors']['up'] = str(settings.get('up_color', '#ff00ff'))
        ptmVars['tooltip']['num'] = settings.get('tooltip_num', 100).toInt()[0]

        ptmVars['bools'] = {}
        for label in self.labels:
            ptmVars['bools'][label] = settings.get(self.defaults['confBool'][label], self.defaults['bool'][label]).toInt()[0]

        ptmNames = {}
        ptmNames['disk'] = str(settings.get('disk', 'disk/sda_(8:0)')).split('@@')
        ptmNames['hdd'] = str(settings.get('mount', '/')).split('@@')
        ptmNames['hddtemp'] = str(settings.get('hdd', '/dev/sda')).split('@@')
        ptmNames['temp'] = str(settings.get('temp_device', '')).split('@@')
        self.parent.applySettings('names', ptmNames)

        self.parent.createLayout(ptmVars['adv']['layout'])
        ptmVars['formats'] = {}
        ptmVars['tooltip']['required'] = []
        for order in ptmVars['app']['order']:
            label = self.defaults['order'][order]
            if (ptmVars['bools'][label] > 0):
                ptmVars['formats'][label] = str(settings.get(self.defaults['confFormat'][label], self.defaults['format'][label]))
                text = ptmVars['app']['format'][0] + ptmVars['formats'][label] + ptmVars['app']['format'][1]
                self.parent.addLabel(label, text, True, ptmVars['adv']['popup'])
                if ((label in ['cpu', 'cpuclock', 'mem', 'net', 'swap']) and (ptmVars['bools'][label] == 2)):
                    if (label == 'net'):
                        ptmVars['tooltip']['required'].append("down")
                        ptmVars['tooltip']['required'].append("up")
                    else:
                        ptmVars['tooltip']['required'].append(label)
        self.parent.applySettings('vars', ptmVars)
