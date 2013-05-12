# -*- coding: utf-8 -*-

from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyKDE4.kdecore import *
from PyKDE4.kdeui import *
from PyKDE4.kio import *
from PyKDE4.plasma import Plasma
from PyKDE4 import plasmascript
from PyQt4 import QtCore
from configwindow import *
from config import *
from util import *
import commands, os, time



class pyTextWidget(plasmascript.Applet):
    def __init__(self, parent, args=None):
        """widget definition"""
        plasmascript.Applet.__init__(self,parent)

    def init(self):
        """function to initializate widget"""
        self._name = str(self.package().metadata().pluginName())
        self.layout = QGraphicsLinearLayout(Qt.Horizontal, self.applet)
        self.setupCores()
        self.reinit()
        self.setHasConfigurationInterface(True)
     
    def configAccepted(self):
        """function to accept settings"""
        # update local variables
        self.interval = int(self.configpage.ui.spinBox_interval.value())
        self.settings.set('interval', self.interval)
        self.font_family = str(self.configpage.ui.fontComboBox.currentFont().family())
        self.settings.set('font_family', self.font_family)
        self.font_size = int(self.configpage.ui.spinBox_fontSize.value())
        self.settings.set('font_size', self.font_size)
        self.font_color = str(self.configpage.ui.kcolorcombo.color().name())
        self.settings.set('font_color', self.font_color)
        if (self.configpage.ui.comboBox_style.currentIndex() == 0):
            self.font_style = 'normal'
        else:
            self.font_style = 'italic'
        self.settings.set('font_style', self.font_style)
        self.font_weight = int(self.configpage.ui.spinBox_weight.value())
        self.settings.set('font_weight', self.font_weight)
        
        # disconnecting from source and clear layout
        if (self.uptimeBool == 1):
            self.systemmonitor.disconnectSource("system/uptime",  self)
            self.label_uptime.setText('')
            self.layout.removeItem(self.label_uptime)
        if (self.cpuBool == 1):
            self.systemmonitor.disconnectSource("cpu/system/TotalLoad", self)
            if (self.cpuFormat.split('$ccpu')[0] != self.cpuFormat):
                self.label_cpu0.setText('')
                self.layout.removeItem(self.label_cpu0)
                self.label_cpu1.setText('')
                self.layout.removeItem(self.label_cpu1)
                for core in range(self.numCores):
                    self.systemmonitor.disconnectSource("cpu/cpu"+str(core)+"/TotalLoad", self)
                    exec ("self.label_coreCpu" + str(core) + ".setText('')")
                    exec ("self.layout.removeItem(self.label_coreCpu" + str(core) + ")")
            else:
                self.label_cpu.setText('')
                self.layout.removeItem(self.label_cpu)
        if (self.cpuclockBool == 1):
            self.systemmonitor.disconnectSource("cpu/system/AverageClock", self)
            self.label_cpuclock.setText('')
            self.layout.removeItem(self.label_cpuclock)
        if (self.tempBool == 1):
            self.systemmonitor.disconnectSource(self.tempdev, self)
            self.label_temp.setText('')
            self.layout.removeItem(self.label_temp)
        if (self.memBool == 1):
            self.systemmonitor.disconnectSource("mem/physical/application", self)
            if (self.memInMb == False):
                self.systemmonitor.disconnectSource("mem/physical/free", self)
                self.systemmonitor.disconnectSource("mem/physical/used", self)
            self.label_mem.setText('')
            self.layout.removeItem(self.label_mem)
        if (self.swapBool == 1):
            self.systemmonitor.disconnectSource("mem/swap/used", self)
            if (self.swapInMb == False):
                self.systemmonitor.disconnectSource("mem/swap/free", self)
            self.label_swap.setText('')
            self.layout.removeItem(self.label_swap)
        if (self.netBool == 1):
            self.systemmonitor.disconnectSource("network/interfaces/"+self.netdev+"/transmitter/data", self)
            self.systemmonitor.disconnectSource("network/interfaces/"+self.netdev+"/receiver/data", self)
            self.label_netDown.setText('')
            self.label_netUp.setText('')
            self.layout.removeItem(self.label_netUp)
            self.layout.removeItem(self.label_netDown)
        if (self.batBool == 1):
            self.label_bat.setText('')
            self.layout.removeItem(self.label_bat)
        
        self.label_order = "------------"
        
        if (self.configpage.ui.checkBox_uptime.checkState() == 2):
            self.uptimeBool = 1
            pos = self.configpage.ui.slider_uptime.value() - 1
            self.label_order = self.label_order[:pos] + "8" + self.label_order[pos+1:]
        else:
            self.uptimeBool = 0
        self.uptimeFormat = str(self.configpage.ui.lineEdit_uptime.text())
        self.settings.set('uptimeBool', self.uptimeBool)
        self.settings.set('uptimeFormat', self.uptimeFormat)
        
        if (self.configpage.ui.checkBox_cpu.checkState() == 2):
            self.cpuBool = 1
            pos = self.configpage.ui.slider_cpu.value() - 1
            self.label_order = self.label_order[:pos] + "1" + self.label_order[pos+1:]
        else:
            self.cpuBool = 0
        self.cpuFormat = str(self.configpage.ui.lineEdit_cpu.text())
        self.settings.set('cpuBool', self.cpuBool)
        self.settings.set('cpuFormat', self.cpuFormat)
        
        if (self.configpage.ui.checkBox_cpuclock.checkState() == 2):
            self.cpuclockBool = 1
            pos = self.configpage.ui.slider_cpuclock.value() - 1
            self.label_order = self.label_order[:pos] + "7" + self.label_order[pos+1:]
        else:
            self.cpuclockBool = 0
        self.cpuclockFormat = str(self.configpage.ui.lineEdit_cpuclock.text())
        self.settings.set('cpuclockBool', self.cpuclockBool)
        self.settings.set('cpuclockFormat', self.cpuclockFormat)
        
        if (self.configpage.ui.checkBox_temp.checkState() == 2):
            self.tempBool = 1
            pos = self.configpage.ui.slider_temp.value() - 1
            self.label_order = self.label_order[:pos] + "2" + self.label_order[pos+1:]
        else:
            self.tempBool = 0
        self.tempFormat = str(self.configpage.ui.lineEdit_temp.text())
        self.settings.set('tempBool', self.tempBool)
        self.settings.set('tempFormat', self.tempFormat)
        
        if (self.configpage.ui.checkBox_gpuMem.checkState() == 2):
            self.gpuMemBool = 1
            pos = self.configpage.ui.slider_gpuMem.value() - 1
            self.label_order = self.label_order[:pos] + "9" + self.label_order[pos+1:]
        else:
            self.gpuMemBool = 0
        self.gpuMemFormat = str(self.configpage.ui.lineEdit_gpuMem.text())
        self.settings.set('gpuMemBool', self.gpuMemBool)
        self.settings.set('gpuMemFormat', self.gpuMemFormat)
        
        if (self.configpage.ui.checkBox_gpuTemp.checkState() == 2):
            self.gpuTempBool = 1
            pos = self.configpage.ui.slider_gpuTemp.value() - 1
            self.label_order = self.label_order[:pos] + "a" + self.label_order[pos+1:]
        else:
            self.gpuTempBool = 0
        self.gpuTempFormat = str(self.configpage.ui.lineEdit_gpuTemp.text())
        self.settings.set('gpuTempBool', self.gpuTempBool)
        self.settings.set('gpuTempFormat', self.gpuTempFormat)
        
        if (self.configpage.ui.checkBox_mem.checkState() == 2):
            self.memBool = 1
            pos = self.configpage.ui.slider_mem.value() - 1
            self.label_order = self.label_order[:pos] + "3" + self.label_order[pos+1:]
        else:
            self.memBool = 0
        self.memFormat = str(self.configpage.ui.lineEdit_mem.text())
        self.settings.set('memBool', self.memBool)
        self.settings.set('memFormat', self.memFormat)
        
        if (self.configpage.ui.checkBox_swap.checkState() == 2):
            self.swapBool = 1
            pos = self.configpage.ui.slider_swap.value() - 1
            self.label_order = self.label_order[:pos] + "4" + self.label_order[pos+1:]
        else:
            self.swapBool = 0
        self.swapFormat = str(self.configpage.ui.lineEdit_swap.text())
        self.settings.set('swapBool', self.swapBool)
        self.settings.set('swapFormat', self.swapFormat)
        
        if (self.configpage.ui.checkBox_hdd.checkState() == 2):
            self.hddBool = 1
            pos = self.configpage.ui.slider_hdd.value() - 1
            self.label_order = self.label_order[:pos] + "b" + self.label_order[pos+1:]
        else:
            self.hddBool = 0
        self.hddFormat = str(self.configpage.ui.lineEdit_hdd.text())
        self.settings.set('hddBool', self.hddBool)
        self.settings.set('hddFormat', self.hddFormat)
        
        if (self.configpage.ui.checkBox_hddTemp.checkState() == 2):
            self.hddTempBool = 1
            pos = self.configpage.ui.slider_hddTemp.value() - 1
            self.label_order = self.label_order[:pos] + "c" + self.label_order[pos+1:]
        else:
            self.hddTempBool = 0
        self.hddTempFormat = str(self.configpage.ui.lineEdit_hddTemp.text())
        self.settings.set('hddTempBool', self.hddTempBool)
        self.settings.set('hddTempFormat', self.hddTempFormat)
        
        if (self.configpage.ui.checkBox_net.checkState() == 2):
            self.netBool = 1
            pos = self.configpage.ui.slider_net.value() - 1
            self.label_order = self.label_order[:pos] + "5" + self.label_order[pos+1:] 
        else:
            self.netBool = 0
        self.netNonFormat = str(self.configpage.ui.lineEdit_net.text())
        self.num_dev = int(self.configpage.ui.comboBox_numNet.currentIndex())
        self.settings.set('netBool', self.netBool)
        self.settings.set('netNonFormat', self.netNonFormat)
        self.settings.set('num_dev', self.num_dev)
        
        if (self.configpage.ui.checkBox_bat.checkState() == 2):
            self.batBool = 1
            pos = self.configpage.ui.slider_bat.value() - 1
            self.label_order = self.label_order[:pos] + "6" + self.label_order[pos+1:]
        else:
            self.batBool = 0
        self.batFormat = str(self.configpage.ui.lineEdit_bat.text())
        self.battery_device = str(self.configpage.ui.lineEdit_batdev.text())
        self.ac_device = str(self.configpage.ui.lineEdit_acdev.text())
        self.settings.set('batBool', self.batBool)
        self.settings.set('batFormat', self.batFormat)
        self.settings.set('battery_device', self.battery_device)
        self.settings.set('ac_device', self.ac_device)
        
        self.label_order = ''.join(self.label_order.split('-'))
        self.settings.set('label_order', self.label_order)
        
        # reinitializate
        self.reinit()

    def createConfigurationInterface(self, parent):
        """function to setup configuration window"""
        self.configpage = ConfigWindow(self, self.settings)
        
        font = QFont(str(self.settings.get('font_family', 'Terminus')), int(self.settings.get('font_size', 12)), 50)
        self.configpage.ui.spinBox_interval.setValue(int(self.settings.get('interval', 2000)))
        self.configpage.ui.fontComboBox.setCurrentFont(font)
        self.configpage.ui.spinBox_fontSize.setValue(int(self.settings.get('font_size', 12)))
        self.configpage.ui.kcolorcombo.setColor(QColor(str(self.settings.get('font_color', '#000000'))))
        font = str(self.settings.get('font_style', 'normal'))
        if (font == 'normal'):
            self.configpage.ui.comboBox_style.setCurrentIndex(0)
        else:
            self.configpage.ui.comboBox_style.setCurrentIndex(1)
        self.configpage.ui.spinBox_weight.setValue(int(self.settings.get('font_weight', 400)))
        
        if (self.uptimeBool == 1):
            self.configpage.ui.checkBox_uptime.setCheckState(2)
            self.configpage.ui.slider_uptime.setMaximum(len(self.label_order))
            self.configpage.ui.slider_uptime.setValue(self.label_order.find("8")+1)
            self.configpage.ui.slider_uptime.setEnabled(True)
            self.configpage.ui.lineEdit_uptime.setEnabled(True)
        else:
            self.configpage.ui.checkBox_uptime.setCheckState(0)
            self.configpage.ui.slider_uptime.setDisabled(True)
            self.configpage.ui.lineEdit_uptime.setDisabled(True)
        self.configpage.ui.lineEdit_uptime.setText(str(self.settings.get('uptimeFormat', '[uptime: $uptime]')))
        
        if (self.cpuBool == 1):
            self.configpage.ui.checkBox_cpu.setCheckState(2)
            self.configpage.ui.slider_cpu.setMaximum(len(self.label_order))
            self.configpage.ui.slider_cpu.setValue(self.label_order.find("1")+1)
            self.configpage.ui.slider_cpu.setEnabled(True)
            self.configpage.ui.lineEdit_cpu.setEnabled(True)
        else:
            self.configpage.ui.checkBox_cpu.setCheckState(0)
            self.configpage.ui.slider_cpu.setDisabled(True)
            self.configpage.ui.lineEdit_cpu.setDisabled(True)
        self.configpage.ui.lineEdit_cpu.setText(str(self.settings.get('cpuFormat', '[cpu: $cpu%]')))
        
        if (self.cpuclockBool == 1):
            self.configpage.ui.checkBox_cpuclock.setCheckState(2)
            self.configpage.ui.slider_cpuclock.setMaximum(len(self.label_order))
            self.configpage.ui.slider_cpuclock.setValue(self.label_order.find("7")+1)
            self.configpage.ui.slider_cpuclock.setEnabled(True)
            self.configpage.ui.lineEdit_cpuclock.setEnabled(True)
        else:
            self.configpage.ui.checkBox_cpuclock.setCheckState(0)
            self.configpage.ui.slider_cpuclock.setDisabled(True)
            self.configpage.ui.lineEdit_cpuclock.setDisabled(True)
        self.configpage.ui.lineEdit_cpuclock.setText(str(self.settings.get('cpuclockFormat', '[mhz: $cpucl]')))
        
        if (self.tempBool == 1):
            self.configpage.ui.checkBox_temp.setCheckState(2)
            self.configpage.ui.slider_temp.setMaximum(len(self.label_order))
            self.configpage.ui.slider_temp.setValue(self.label_order.find("2")+1)
            self.configpage.ui.slider_temp.setEnabled(True)
            self.configpage.ui.lineEdit_temp.setEnabled(True)
        else:
            self.configpage.ui.checkBox_temp.setCheckState(0)
            self.configpage.ui.slider_temp.setDisabled(True)
            self.configpage.ui.lineEdit_temp.setDisabled(True)
        self.configpage.ui.lineEdit_temp.setText(str(self.settings.get('tempFormat', '[temp: $temp&deg;C]')))
        
        if (self.gpuMemBool == 1):
            self.configpage.ui.checkBox_gpuMem.setCheckState(2)
            self.configpage.ui.slider_gpuMem.setMaximum(len(self.label_order))
            self.configpage.ui.slider_gpuMem.setValue(self.label_order.find("9")+1)
            self.configpage.ui.slider_gpuMem.setEnabled(True)
            self.configpage.ui.lineEdit_gpuMem.setEnabled(True)
        else:
            self.configpage.ui.checkBox_gpuMem.setCheckState(0)
            self.configpage.ui.slider_gpuMem.setDisabled(True)
            self.configpage.ui.lineEdit_gpuMem.setDisabled(True)
        self.configpage.ui.lineEdit_gpuMem.setText(str(self.settings.get('gpuMemFormat', '[gpu mem: $gpumem%]')))
        
        if (self.gpuTempBool == 1):
            self.configpage.ui.checkBox_gpuTemp.setCheckState(2)
            self.configpage.ui.slider_gpuTemp.setMaximum(len(self.label_order))
            self.configpage.ui.slider_gpuTemp.setValue(self.label_order.find("a")+1)
            self.configpage.ui.slider_gpuTemp.setEnabled(True)
            self.configpage.ui.lineEdit_gpuTemp.setEnabled(True)
        else:
            self.configpage.ui.checkBox_gpuTemp.setCheckState(0)
            self.configpage.ui.slider_gpuTemp.setDisabled(True)
            self.configpage.ui.lineEdit_gpuTemp.setDisabled(True)
        self.configpage.ui.lineEdit_gpuTemp.setText(str(self.settings.get('gpuTempFormat', '[gpu temp: $gputemp&deg;C]')))
        
        if (self.memBool == 1):
            self.configpage.ui.checkBox_mem.setCheckState(2)
            self.configpage.ui.slider_mem.setMaximum(len(self.label_order))
            self.configpage.ui.slider_mem.setValue(self.label_order.find("3")+1)
            self.configpage.ui.slider_mem.setEnabled(True)
            self.configpage.ui.lineEdit_mem.setEnabled(True)
        else:
            self.configpage.ui.checkBox_mem.setCheckState(0)
            self.configpage.ui.slider_mem.setDisabled(True)
            self.configpage.ui.lineEdit_mem.setDisabled(True)
        self.configpage.ui.lineEdit_mem.setText(str(self.settings.get('memFormat', '[mem: $mem%]')))
        
        if (self.swapBool == 1):
            self.configpage.ui.checkBox_swap.setCheckState(2)
            self.configpage.ui.slider_swap.setMaximum(len(self.label_order))
            self.configpage.ui.slider_swap.setValue(self.label_order.find("4")+1)
            self.configpage.ui.slider_swap.setEnabled(True)
            self.configpage.ui.lineEdit_swap.setEnabled(True)
        else:
            self.configpage.ui.checkBox_swap.setCheckState(0)
            self.configpage.ui.slider_swap.setDisabled(True)
            self.configpage.ui.lineEdit_swap.setDisabled(True)
        self.configpage.ui.lineEdit_swap.setText(str(self.settings.get('swapFormat', '[swap: $swap%]')))
        
        if (self.hddBool == 1):
            self.configpage.ui.checkBox_hdd.setCheckState(2)
            self.configpage.ui.slider_hdd.setMaximum(len(self.label_order))
            self.configpage.ui.slider_hdd.setValue(self.label_order.find("b")+1)
            self.configpage.ui.slider_hdd.setEnabled(True)
            self.configpage.ui.lineEdit_hdd.setEnabled(True)
        else:
            self.configpage.ui.checkBox_hdd.setCheckState(0)
            self.configpage.ui.slider_hdd.setDisabled(True)
            self.configpage.ui.lineEdit_hdd.setDisabled(True)
        self.configpage.ui.lineEdit_hdd.setText(str(self.settings.get('hddFormat', '[hdd: @@/;@@%]')))
        
        if (self.hddTempBool == 1):
            self.configpage.ui.checkBox_hddTemp.setCheckState(2)
            self.configpage.ui.slider_hddTemp.setMaximum(len(self.label_order))
            self.configpage.ui.slider_hddTemp.setValue(self.label_order.find("c")+1)
            self.configpage.ui.slider_hddTemp.setEnabled(True)
            self.configpage.ui.lineEdit_hddTemp.setEnabled(True)
        else:
            self.configpage.ui.checkBox_hddTemp.setCheckState(0)
            self.configpage.ui.slider_hddTemp.setDisabled(True)
            self.configpage.ui.lineEdit_hddTemp.setDisabled(True)
        self.configpage.ui.lineEdit_hddTemp.setText(str(self.settings.get('hddTempFormat', '[hdd temp: @@/dev/sda@@&deg;C]')))
        
        if (self.netBool == 1):
            self.configpage.ui.checkBox_net.setCheckState(2)
            self.configpage.ui.slider_net.setMaximum(len(self.label_order))
            self.configpage.ui.slider_net.setValue(self.label_order.find("5")+1)
            self.configpage.ui.slider_net.setEnabled(True)
            self.configpage.ui.lineEdit_net.setEnabled(True)
            self.configpage.ui.comboBox_numNet.setEnabled(True)
        else:
            self.configpage.ui.checkBox_net.setCheckState(0)
            self.configpage.ui.slider_net.setDisabled(True)
            self.configpage.ui.comboBox_numNet.setDisabled(True)
            self.configpage.ui.lineEdit_net.setDisabled(True)
        self.configpage.ui.comboBox_numNet.setCurrentIndex(int(self.settings.get('num_dev', 0)))
        self.configpage.ui.lineEdit_net.setText(str(self.settings.get('netNonFormat', '[net: $netKB/s]')))
        
        if (self.batBool == 1):
            self.configpage.ui.checkBox_bat.setCheckState(2)
            self.configpage.ui.slider_bat.setMaximum(len(self.label_order))
            self.configpage.ui.slider_bat.setValue(self.label_order.find("6")+1)
            self.configpage.ui.slider_bat.setEnabled(True)
            self.configpage.ui.lineEdit_bat.setEnabled(True)
            self.configpage.ui.lineEdit_acdev.setEnabled(True)
            self.configpage.ui.lineEdit_batdev.setEnabled(True)
        else:
            self.configpage.ui.checkBox_bat.setCheckState(0)
            self.configpage.ui.slider_bat.setDisabled(True)
            self.configpage.ui.lineEdit_bat.setDisabled(True)
            self.configpage.ui.lineEdit_acdev.setDisabled(True)
            self.configpage.ui.lineEdit_batdev.setDisabled(True)
        self.configpage.ui.lineEdit_bat.setText(str(self.settings.get('batFormat', '[bat: $bat%$ac]')))
        self.configpage.ui.lineEdit_batdev.setText(str(self.settings.get('battery_device', '/sys/class/power_supply/BAT0/capacity')))
        self.configpage.ui.lineEdit_acdev.setText(str(self.settings.get('ac_device', '/sys/class/power_supply/AC/online')))
        
        # add config page
        page = parent.addPage(self.configpage, i18n(self.name()))
        page.setIcon(KIcon(self.icon()))
        
        parent.okClicked.connect(self.configAccepted)
    
    def reinit(self):
        """function to reinitializate widget"""
        self.settings = Config(self)
        self.interval = int(self.settings.get('interval', 2000))
        self.font_family = str(self.settings.get('font_family', 'Terminus'))
        self.font_size = int(self.settings.get('font_size', 12))
        self.font_color = str(self.settings.get('font_color', '#000000'))
        self.font_style = str(self.settings.get('font_style', 'normal'))
        self.font_weight = int(self.settings.get('font_weight', 400))
        self.formatLine = "<pre><p align=\"center\"><span style=\" font-family:'" + self.font_family + "'; font-style:" + self.font_style
        self.formatLine = self.formatLine + "; font-size:" + str(self.font_size) + "pt; font-weight:" + str(self.font_weight)
        self.formatLine = self.formatLine + "; color:" + self.font_color + ";\">$LINE</span></p></pre>"
        self.label_order = str(self.settings.get('label_order', '81729a34bc56'))
        self.uptimeBool = int(self.settings.get('uptimeBool',  1))
        self.cpuBool = int(self.settings.get('cpuBool', 1))
        self.cpuclockBool = int(self.settings.get('cpuclockBool', 1))
        self.tempBool = int(self.settings.get('tempBool', 1))
        self.gpuMemBool = int(self.settings.get('gpuMemBool', 1))
        self.gpuTempBool = int(self.settings.get('gpuTempBool', 1))
        self.memBool = int(self.settings.get('memBool', 1))
        self.swapBool = int(self.settings.get('swapBool', 1))
        self.hddBool = int(self.settings.get('hddBool', 1))
        self.hddTempBool = int(self.settings.get('hddTempBool', 1))
        self.netBool = int(self.settings.get('netBool', 1))
        self.batBool = int(self.settings.get('batBool', 1))
        # small function for update if errors exist
        if (len(self.label_order) != sum([self.uptimeBool,  self.cpuBool, self.cpuclockBool, self.tempBool, self.gpuMemBool,  self.gpuTempBool,  self.memBool, self.swapBool, self.hddBool, self.hddTempBool, self.netBool, self.batBool])):
            self.uptimeBool = 1
            self.settings.set('uptimeBool', self.uptimeBool)
            self.cpuBool = 1
            self.settings.set('cpuBool', self.cpuBool)
            self.cpuclockBool = 1
            self.settings.set('cpuclockBool', self.cpuclockBool)
            self.tempBool = 1
            self.settings.set('tempBool', self.tempBool)
            self.gpuMemBool = 1
            self.settings.set('gpuMemBool', self.gpuMemBool)
            self.gpuTempBool = 1
            self.settings.set('gpuTempBool', self.gpuTempBool)
            self.memBool = 1
            self.settings.set('memBool', self.memBool)
            self.swapBool = 1
            self.settings.set('swapBool', self.swapBool)
            self.hddBool = 1
            self.settings.set('hddBool', self.hddBool)
            self.hddTempBool = 1
            self.settings.set('hddTempBool', self.hddTempBool)
            self.netBool = 1
            self.settings.set('netBool', self.netBool)
            self.batBool = 1
            self.settings.set('batBool', self.batBool)
            self.label_order = '81729a34bc56'
            self.settings.set('label_order', self.label_order)
        
        for order in self.label_order:
            if (order == "1"):
                if (self.cpuBool == 1):
                    self.cpuFormat = str(self.settings.get('cpuFormat', '[cpu: $cpu%]'))
                    if (self.cpuFormat.split('$ccpu')[0] != self.cpuFormat):
                        self.label_cpu0 = Plasma.Label(self.applet)
                        self.label_cpu1 = Plasma.Label(self.applet)
                        if (self.cpuFormat.split('$ccpu')[0].split('$cpu')[0] != self.cpuFormat.split('$ccpu')[0]):
                            line = self.cpuFormat.split('$ccpu')[0].split('$cpu')[0] + '-----' + self.cpuFormat.split('$ccpu')[0].split('$cpu')[1]
                        else:
                            line = self.cpuFormat.split('$ccpu')[0]
                        text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                        self.label_cpu0.setText(text)
                        self.layout.addItem(self.label_cpu0)
                        text = self.formatLine.split('$LINE')[0] + "----- " + self.formatLine.split('$LINE')[1]
                        for core in range(self.numCores):
                            exec ('self.label_coreCpu' + str(core) + ' = Plasma.Label(self.applet)')
                            exec ('self.label_coreCpu' + str(core) + '.setText(text)')
                            exec ('self.layout.addItem(self.label_coreCpu' + str(core) + ')')
                        if (self.cpuFormat.split('$ccpu')[1].split('$cpu')[0] != self.cpuFormat.split('$ccpu')[1]):
                            line = self.cpuFormat.split('$ccpu')[1].split('$cpu')[0] + '-----' + self.cpuFormat.split('$ccpu')[1].split('$cpu')[1]
                        else:
                            line = self.cpuFormat.split('$ccpu')[1]
                        text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                        self.label_cpu1.setText(text)
                        self.layout.addItem(self.label_cpu1)
                    else:
                        self.label_cpu = Plasma.Label(self.applet)
                        if (self.cpuFormat.split('$cpu')[0] != self.cpuFormat):
                            line = self.cpuFormat.split('$cpu')[0] + '-----' + self.cpuFormat.split('$cpu')[1]
                        else:
                            line = self.cpuFormat
                        text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                        self.label_cpu.setText(text)
                        self.layout.addItem(self.label_cpu)
            elif (order == "2"):
                if (self.tempBool == 1):
                    self.tempFormat = str(self.settings.get('tempFormat', '[temp: $temp&deg;C]'))
                    self.setupTemp()
                    self.label_temp = Plasma.Label(self.applet)
                    if (self.tempFormat.split('$temp')[0] != self.tempFormat):
                        line = self.tempFormat.split('$temp')[0] + '----' + self.tempFormat.split('$temp')[1]
                    else:
                        line = self.tempFormat
                    text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                    self.label_temp.setText(text)
                    self.layout.addItem(self.label_temp)
            elif (order == "3"):
                if (self.memBool == 1):
                    self.memFormat = str(self.settings.get('memFormat', '[mem: $mem%]'))
                    if (self.memFormat.split('$memmb')[0] != self.memFormat):
                        self.memInMb = True
                        text = self.formatLine.split('$LINE')[0] + self.memFormat.split('$memmb')[0] + '-----' + self.memFormat.split('$memmb')[1] + self.formatLine.split('$LINE')[1]
                    elif (self.memFormat.split('$mem')[0] != self.memFormat):
                        self.memInMb = False
                        self.mem_used = 0.0
                        self.mem_free = 1.0
                        self.mem_uf = 0.0
                        line = self.memFormat.split('$mem')[0] + '-----' + self.memFormat.split('$mem')[1]
                    else:
                        line = self.memFormat
                    text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                    self.label_mem = Plasma.Label(self.applet)
                    self.label_mem.setText(text)
                    self.layout.addItem(self.label_mem)
            elif (order == "4"):
                if (self.swapBool == 1):
                    self.swapFormat = str(self.settings.get('swapFormat', '[swap: $swap%]'))
                    if (self.swapFormat.split('$swapmb')[0] != self.swapFormat):
                        self.swapInMb = True
                        text = self.formatLine.split('$LINE')[0] + self.swapFormat.split('$swapmb')[0] + '-----' + self.swapFormat.split('$swapmb')[1] + self.formatLine.split('$LINE')[1]
                    elif (self.swapFormat.split('$swap')[0] != self.swapFormat):
                        self.swapInMb = False
                        self.swap_free = 1.0
                        self.swap_used = 0.0
                        line = self.swapFormat.split('$swap')[0] + '-----' + self.swapFormat.split('$swap')[1]
                    else:
                        line = self.swapFormat
                    text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                    self.label_swap = Plasma.Label(self.applet)
                    self.label_swap.setText(text)
                    self.layout.addItem(self.label_swap)
            elif (order == "5"):
                if (self.netBool == 1):
                    self.netNonFormat = str(self.settings.get('netNonFormat', '[net: $netKB/s]'))
                    if (self.netNonFormat.split('@@')[0] != self.netNonFormat):
                        self.netdev = self.netNonFormat.split('@@')[1]
                        self.netNonFormat = self.netNonFormat.split('@@')[0] + self.netNonFormat.split('@@')[2]
                    else:
                        self.num_dev = int(self.settings.get('num_dev', 0))
                        self.setupNetdev()
                    if (self.netNonFormat.split('$netdev')[0] != self.netNonFormat):
                        self.netFormat = self.netNonFormat.split('$netdev')[0] + self.netdev + self.netNonFormat.split('$netdev')[1]
                    else:
                        self.netFormat = self.netNonFormat
                    self.label_netDown = Plasma.Label(self.applet)
                    if (self.netFormat.split('$net')[0] != self.netFormat):
                        line = self.netFormat.split('$net')[0] + '----'
                    else:
                        line = self.netFormat
                    text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                    self.label_netDown.setText(text)
                    self.layout.addItem(self.label_netDown)
                    self.label_netUp = Plasma.Label(self.applet)
                    if (self.netFormat.split('$net')[0] != self.netFormat):
                        line = '/----' + self.netFormat.split('$net')[1]
                    else:
                        line = ''
                    text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                    self.label_netUp.setText(text)
                    self.layout.addItem(self.label_netUp)
            elif (order == "6"):
                if (self.batBool == 1):
                    self.batFormat = str(self.settings.get('batFormat', '[bat: $bat%$ac]'))
                    self.battery_device= str(self.settings.get('battery_device', '/sys/class/power_supply/BAT0/capacity'))
                    self.ac_device = str(self.settings.get('ac_device', '/sys/class/power_supply/AC/online'))
                    self.label_bat = Plasma.Label(self.applet)
                    line = self.batFormat
                    if (line.split('$ac')[0] != line):
                        line = line.split('$ac')[0] + '(-)' + line.split('$ac')[1]
                    if (line.split('$bat')[0] != line):
                        line = line.split('$bat')[0] + '---' + line.split('$bat')[1]
                    text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                    self.label_bat.setText(text)
                    self.layout.addItem(self.label_bat)
            elif (order == "7"):
                if (self.cpuclockBool == 1):
                    self.cpuclockFormat = str(self.settings.get('cpuclockFormat', '[mhz: $cpucl]'))
                    self.label_cpuclock = Plasma.Label(self.applet)
                    if (self.cpuclockFormat.split('$cpucl')[0] != self.cpuclockFormat):
                        line = self.cpuclockFormat.split('$cpucl')[0] + '----' + self.cpuclockFormat.split('$cpucl')[1]
                    else:
                        line = self.cpuclockFormat
                    text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                    self.label_cpuclock.setText(text)
                    self.layout.addItem(self.label_cpuclock)
            elif (order == "8"):
                if (self.uptimeBool == 1):
                    self.uptimeFormat = str(self.settings.get('uptimeFormat', '[uptime: $uptime]'))
                    self.label_uptime = Plasma.Label(self.applet)
                    if (self.uptimeFormat.split('$uptime')[0] != self.uptimeFormat):
                        line = self.uptimeFormat.split('$uptime')[0] + '---d--h--m' + self.uptimeFormat.split('$uptime')[1]
                    else:
                        line = self.uptimeFormat
                    text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                    self.label_uptime.setText(text)
                    self.layout.addItem(self.label_uptime)
            elif (order == "9"):
                if (self.gpuMemBool == 1):
                    self.gpuMemFormat = str(self.settings.get('gpuMemFormat', '[gpu mem: $gpumem%]'))
                    if (self.gpuMemFormat.split('$gpumemmb')[0] != self.gpuMemFormat):
                        text = self.formatLine.split('$LINE')[0] + self.gpuMemFormat.split('$gpumemmb')[0] + '-----' + self.gpuMemFormat.split('$gpumemmb')[1] + self.formatLine.split('$LINE')[1]
                    elif (self.gpuMemFormat.split('$gpumem')[0] != self.gpuMemFormat):
                        line = self.gpuMemFormat.split('$gpumem')[0] + '-----' + self.gpuMemFormat.split('$gpumem')[1]
                    else:
                        line = self.gpuMemFormat
                    text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                    self.label_gpuMem = Plasma.Label(self.applet)
                    self.label_gpuMem.setText(text)
                    self.layout.addItem(self.label_gpuMem)
            elif (order == "a"):
                if (self.gpuTempBool == 1):
                    self.gpuTempFormat = str(self.settings.get('gpuTempFormat', '[gpu temp: $gputemp&deg;C]'))
                    self.label_gpuTemp = Plasma.Label(self.applet)
                    if (self.gpuTempFormat.split('$gputemp')[0] != self.gpuTempFormat):
                        line = self.gpuTempFormat.split('$gputemp')[0] + '----' + self.gpuTempFormat.split('$gputemp')[1]
                    else:
                        line = self.gpuTempFormat
                    text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                    self.label_gpuTemp.setText(text)
                    self.layout.addItem(self.label_gpuTemp)
            elif (order == "b"):
                if (self.hddBool == 1):
                    self.hddFormat = str(self.settings.get('hddFormat', '[hdd: @@/@@%]'))
                    self.label_hdd = Plasma.Label(self.applet)
                    if (self.hddFormat.split('@@')[0] != self.hddFormat):
                        self.mountPoints = self.hddFormat.split('@@')[1].split(';')
                        line = self.hddFormat.split('@@')[0]
                        for items in self.mountPoints:
                            line = line + '-----'
                        line = line + self.hddFormat.split('@@')[2]
                    else:
                        line = self.hddFormat
                    text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                    self.label_hdd.setText(text)
                    self.layout.addItem(self.label_hdd)
            elif (order == "c"):
                if (self.hddTempBool == 1):
                    self.hddTempFormat = str(self.settings.get('hddTempFormat', '[hdd temp: @@/dev/sda@@&deg;C]'))
                    self.label_hddTemp = Plasma.Label(self.applet)
                    if (self.hddTempFormat.split('@@')[0] != self.hddTempFormat):
                        line = self.hddTempFormat.split('@@')[0] + '----' + self.hddTempFormat.split('@@')[2]
                    else:
                        line = self.hddTempFormat
                    text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                    self.label_hddTemp.setText(text)
                    self.layout.addItem(self.label_hddTemp)
        self.applet.setLayout(self.layout)        
        self.theme = Plasma.Svg(self)
        self.theme.setImagePath("widgets/background")
        self.setBackgroundHints(Plasma.Applet.DefaultBackground)
        self.resize(10,10)
        
        # start timer
        self.connectToEngine()
        self.timer = QtCore.QTimer()
        self.timer.setInterval(self.interval)
        self.startPolling()
    
    def setupCores(self):
        """function tp setup number of cores"""
        commandOut = commands.getoutput("grep -c '^processor' /proc/cpuinfo")
        self.numCores = int(commandOut)
    
    def setupNetdev(self):
        """function to setup network device"""
        if (self.num_dev == 0):
            for line in commands.getoutput("ifconfig -a").split("\n"):
                if (line != ''):
                    if ((line[0] != ' ') and (line[0:3] != "lo:")):
                        self.netdev = line.split()[0][:-1]
        else:
            interfaces = []
            for line in commands.getoutput("ifconfig -a").split("\n"):
                if (line != ''):
                    if ((line[0] != ' ') and (line[0:3] != "lo:")):
                        interfaces.append(line.split()[0][:-1])
                    
            command_line = "if ! (ifconfig "+ interfaces[1] + " | grep 'inet ' > /dev/null); then "
            command_line = command_line + "if ! (ifconfig "+ interfaces[0] + " | grep 'inet ' > /dev/null); then echo lo; "
            command_line = command_line + "else echo "+ interfaces[0] + "; fi; else echo "+ interfaces[1] + "; fi"
            self.netdev = commands.getoutput(command_line)
        
    def setupTemp(self):
        """function to setup temp device"""
        commandOut = commands.getoutput("sensors | grep Physical -B2")
        self.tempdev = "lmsensors/"+commandOut.split("\n")[0]+"/"+'_'.join(commandOut.split("\n")[2].split(":")[0].split())
    
    def showConfigurationInterface(self):
        """function to show configuration window"""
        plasmascript.Applet.showConfigurationInterface(self)
    
    def showTooltip(self, text):
        """function to create and set tooltip"""
        tooltip = Plasma.ToolTipContent()
        tooltip.setImage(KIcon(self.icon()))
        tooltip.setSubText(text)
        tooltip.setAutohide(False)
        Plasma.ToolTipManager.self().setContent(self.applet, tooltip)
        Plasma.ToolTipManager.self().registerWidget(self.applet)
      
    def startPolling(self):
        try:
            self.timer.start()
            QtCore.QObject.connect(self.timer, QtCore.SIGNAL("timeout()"), self.updateLabel)
            
            self.updateLabel()
            self.showTooltip('')
        except Exception as (strerror):
            self.showTooltip(str(strerror))
            self.label_error = Plasma.Label(self.applet)
            self.label_error.setText('<font color="red">ERROR</font>')
            self.layout.addItem(self.label_error)
            return
   
    def updateLabel(self):
        """function to update label"""
        if (self.gpuMemBool == 1):
            self.gpuMemText()
        if (self.gpuTempBool == 1):
            self.gpuTempText()
        if ((self.memBool == 1) and (self.memInMb == False)):
            self.memText()
        if ((self.swapBool == 1) and (self.swapInMb == False)):
            self.swapText()
        if (self.hddBool == 1):
            self.hddText()
        if (self.hddTempBool == 1):
            self.hddTempText()
        if (self.batBool == 1):
            self.batText()
    
    def batText(self):
        """function to set battery text"""
        line = self.batFormat
        if (line.split('$bat')[0] != line):
            if os.path.exists(self.battery_device):
                with open (self.battery_device, 'r') as bat_file:
                    bat = bat_file.readline().split('\n')[0]
            else:
                bat = 'off'
            bat = "%3s" % (bat)
            line = line.split('$bat')[0] + bat + line.split('$bat')[1]
        if (line.split('$ac')[0] != line):
            if os.path.exists(self.ac_device):
                with open (self.ac_device, 'r') as bat_file:
                    bat = bat_file.readline().split('\n')[0]
                if (bat == '1'):
                    bat = '(*)'
                else:
                    bat = '( )'
            else:
                bat = '(?)'
            line = line.split('$ac')[0] + bat + line.split('$ac')[1]
        text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
        self.label_bat.setText(text)
    
    def gpuMemText(self):
        """function to set gpu memory text"""
        if (self.gpuMemFormat.split('$gpumem')[0] != self.gpuMemFormat):
            commandOut = commands.getoutput("nvidia-smi -q -d MEMORY | grep Total -A 2")
            gpumem = "%5s" % (str(round(float(commandOut.split()[6]) / float(commandOut.split()[2]), 1)))
            line = self.gpuMemFormat.split('$gpumem')[0] + gpumem + self.gpuMemFormat.split('$gpumem')[1]
        elif (self.gpuMemFormat.split('$gpumemmb')[0] != self.gpuMemFormat):
            commandOut = commands.getoutput("nvidia-smi -q -d MEMORY | grep Used")
            gpumem = "%5s" % (str(round(float(commandOut.split()[2])), 1))
            line = self.gpuMemFormat.split('$gpumem')[0] + gpumem + self.gpuMemFormat.split('$gpumem')[1]
        else:
            line = self.gpuMemFormat
        text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
        self.label_gpuMem.setText(text)
    
    def gpuTempText(self):
        """function to set gpu temperature text"""
        commandOut = commands.getoutput("nvidia-smi -q -d TEMPERATURE | grep Gpu")
        gputemp = "%4s" % (str(round(float(commandOut.split()[2]), 1)))
        if (self.gpuTempFormat.split('$gputemp')[0] != self.gpuTempFormat):
            line = self.gpuTempFormat.split('$gputemp')[0] + gputemp + self.gpuTempFormat.split('$gputemp')[1]
        else:
            line = self.gpuTempFormat
        text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
        self.label_gpuTemp.setText(text)
    
    def hddTempText(self):
        """function to set hdd temperature text"""
        commandOut = commands.getoutput("hddtemp "+self.hddTempFormat.split('@@')[1])
        hddtemp = "%4s" % (str(round(float(commandOut.split(':')[2][:-3]), 1)))
        if (self.hddTempFormat.split('@@')[0] != self.hddTempFormat):
            line = self.hddTempFormat.split('@@')[0] + hddtemp + self.hddTempFormat.split('@@')[2]
        else:
            line = self.hddTempFormat
        text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
        self.label_hddTemp.setText(text)
    
    def hddText(self):
        """function to set hdd usage text"""
        commandOut = commands.getoutput("df -h --output='target,pcent'")
        hdd = ''
        for mounts in self.mountPoints:
            for items in commandOut.split('\n')[1:]:
                if (mounts == items.split()[0]):
                    hdd = hdd + "%5s" % (str(round(float(items.split()[1][:-1]), 1)))
        if (self.hddFormat.split('@@')[0] != self.hddFormat):
            line = self.hddFormat.split('@@')[0] + hdd + self.hddFormat.split('@@')[2]
        else:
            line = self.hddFormat
        text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
        self.label_hdd.setText(text)
    
    def memText(self):
        """function to set mem text"""
        full = self.mem_uf + self.mem_free
        mem = 100 * self.mem_used / full
        mem = "%5s" % (str(round(mem, 1)))
        if (self.memFormat.split('$mem')[0] != self.memFormat):
            line = self.memFormat.split('$mem')[0] + mem + self.memFormat.split('$mem')[1]
        else:
            line = self.memFormat
        text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
        self.label_mem.setText(text)

    def swapText(self):
        """function to set swap text"""
        full = self.swap_used + self.swap_free
        mem = 100 * self.swap_used / full
        mem = "%5s" % (str(round(mem, 1)))
        if (self.swapFormat.split('$swap')[0] != self.swapFormat):
            line = self.swapFormat.split('$swap')[0] + mem + self.swapFormat.split('$swap')[1]
        else:
            line = self.swapFormat
        text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
        self.label_swap.setText(text)
    
    def connectToEngine(self):
        """function to initializate engine"""
        self.systemmonitor = self.dataEngine("systemmonitor")
        if (self.uptimeBool == 1):
            self.systemmonitor.connectSource("system/uptime", self, self.interval)
        if (self.cpuBool == 1):
            self.systemmonitor.connectSource("cpu/system/TotalLoad", self, self.interval)
            if (self.cpuFormat.split('$ccpu')[0] != self.cpuFormat):
                for core in range(self.numCores):
                    self.systemmonitor.connectSource("cpu/cpu"+str(core)+"/TotalLoad", self, self.interval)
        if (self.cpuclockBool == 1):
            self.systemmonitor.connectSource("cpu/system/AverageClock", self, self.interval)
        if (self.netBool == 1):
            self.updateNetdev = 0
            self.systemmonitor.connectSource("network/interfaces/"+self.netdev+"/transmitter/data", self, self.interval)
            self.systemmonitor.connectSource("network/interfaces/"+self.netdev+"/receiver/data", self, self.interval)
        if (self.tempBool == 1):
            self.systemmonitor.connectSource(self.tempdev, self, self.interval)
        if (self.memBool == 1):
            if (self.memInMb):
                self.systemmonitor.connectSource("mem/physical/application", self, self.interval)
            else:
                self.systemmonitor.connectSource("mem/physical/free", self, 200)
                self.systemmonitor.connectSource("mem/physical/used", self, 200)
                self.systemmonitor.connectSource("mem/physical/application", self, 200)
        if (self.swapBool == 1):
            if (self.swapInMb):
                self.systemmonitor.connectSource("mem/swap/used", self, self.interval)
            else:
                self.systemmonitor.connectSource("mem/swap/free", self, 200)
                self.systemmonitor.connectSource("mem/swap/used", self, 200)        
    
    @pyqtSignature("dataUpdated(const QString &, const Plasma::DataEngine::Data &)")
    def dataUpdated(self, sourceName, data):
        """function to refresh data"""
        if (sourceName == "system/uptime"):
            value = int(round(float(data[QString(u'value')]), 1))
            uptimeText = '%3sd%2sh%2sm' % (str(int(value/(24*60*60))), int(value/60/60)-int(value/24/60/60)*24, (value-value%60)/60%60)
            if (self.uptimeFormat.split('$uptime')[0] != self.uptimeFormat):
                line = self.uptimeFormat.split('$uptime')[0] + uptimeText + self.uptimeFormat.split('$uptime')[1]
            else:
                line = self.uptimeFormat
            text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
            self.label_uptime.setText(text)
        elif (sourceName == "cpu/system/TotalLoad"):
            value = str(round(float(data[QString(u'value')]), 1))
            cpuText = "%5s" % (value)
            if (self.cpuFormat.split('$ccpu')[0] != self.cpuFormat):
                if (self.cpuFormat.split('$ccpu')[0].split('$cpu')[0] != self.cpuFormat.split('$ccpu')[0]):
                    line = self.cpuFormat.split('$ccpu')[0].split('$cpu')[0] + cpuText + self.cpuFormat.split('$ccpu')[0].split('$cpu')[1]
                else:
                    line = self.cpuFormat.split('$ccpu')[0]
                text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                self.label_cpu0.setText(text)
                if (self.cpuFormat.split('$ccpu')[1].split('$cpu')[0] != self.cpuFormat.split('$ccpu')[1]):
                    line = self.cpuFormat.split('$ccpu')[1].split('$cpu')[0] + cpuText + self.cpuFormat.split('$ccpu')[1].split('$cpu')[1]
                else:
                    line = self.cpuFormat.split('$ccpu')[1]
                text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                self.label_cpu1.setText(text)
            else:
                if (self.cpuFormat.split('$cpu')[0] != self.cpuFormat):
                    line = self.cpuFormat.split('$cpu')[0] + cpuText + self.cpuFormat.split('$cpu')[1]
                else:
                    line = self.cpuFormat
                text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                self.label_cpu.setText(text)
        elif (str(sourceName)[:7] == "cpu/cpu"):
            value = str(round(float(data[QString(u'value')]), 1))
            cpuText = "%5s" % (value)
            text = self.formatLine.split('$LINE')[0] + cpuText + self.formatLine.split('$LINE')[1]
            exec ('self.label_coreCpu' + str(sourceName)[7] + '.setText(text)')
        elif (sourceName == "cpu/system/AverageClock"):
            value = str(data[QString(u'value')]).split('.')[0]
            cpuclockText = "%4s" % (value)
            if (self.cpuclockFormat.split('$cpucl')[0] != self.cpuclockFormat):
                line = self.cpuclockFormat.split('$cpucl')[0] + cpuclockText + self.cpuclockFormat.split('$cpucl')[1]
            else:
                line = self.cpuclockFormat
            text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
            self.label_cpuclock.setText(text)
        elif (sourceName == "network/interfaces/"+self.netdev+"/transmitter/data"):
            value = str(data[QString(u'value')]).split('.')[0]
            up_speed = "%4s" % (value)
            if (self.netFormat.split('$net')[0] != self.netFormat):
                line = '/' + up_speed + self.netFormat.split('$net')[1]
            else:
                line = ''
            text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
            self.label_netUp.setText(text)
        elif (sourceName == "network/interfaces/"+self.netdev+"/receiver/data"):
            value = str(data[QString(u'value')]).split('.')[0]
            down_speed = "%4s" % (value)
            if (self.netFormat.split('$net')[0] != self.netFormat):
                line = self.netFormat.split('$net')[0] + down_speed
            else:
                line = self.netFormat
            text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
            self.label_netDown.setText(text)
            # update network device
            self.updateNetdev = self.updateNetdev + 1
            if (self.updateNetdev == 100):
                self.updateNetdev = 0
                if (self.netNonFormat.split('@@')[0] == self.netNonFormat):
                    self.systemmonitor.disconnectSource("network/interfaces/"+self.netdev+"/transmitter/data", self)
                    self.systemmonitor.disconnectSource("network/interfaces/"+self.netdev+"/receiver/data", self)
                    self.setupNetdev()
                    self.systemmonitor.connectSource("network/interfaces/"+self.netdev+"/transmitter/data", self, self.interval)
                    self.systemmonitor.connectSource("network/interfaces/"+self.netdev+"/receiver/data", self, self.interval)
                if (self.netNonFormat.split('$netdev')[0] != self.netNonFormat):
                    self.netFormat = self.netNonFormat.split('$netdev')[0] + self.netdev + self.netNonFormat.split('$netdev')[1]
                else:
                    self.netFormat = self.netNonFormat
        elif (sourceName == self.tempdev):
            value = str(round(float(data[QString(u'value')]), 1))
            tempText = "%4s" % (value)
            if (self.tempFormat.split('$temp')[0] != self.tempFormat):
                line = self.tempFormat.split('$temp')[0] + tempText + self.tempFormat.split('$temp')[1]
            else:
                line = self.tempFormat
            text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
            self.label_temp.setText(text)
        elif (sourceName == "mem/physical/free"):
            self.mem_free = float(data[QString(u'value')])
        elif (sourceName == "mem/physical/used"):
            self.mem_uf = float(data[QString(u'value')])
        elif (sourceName == "mem/physical/application"):
            if (self.memInMb):
                mem = str(round(float(data[QString(u'value')]) / 1024, 0)).split('.')[0]
                mem = "%5s" % (mem)
                if (self.memFormat.split('$memmb')[0] != self.memFormat):
                    line = self.memFormat.split('$memmb')[0] + mem + self.memFormat.split('$memmb')[1]
                else:
                    line = self.memFormat
                text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                self.label_mem.setText(text)
            else:
                self.mem_used = float(data[QString(u'value')])
        elif (sourceName == "mem/swap/free"):
            self.swap_free = float(data[QString(u'value')])
        elif (sourceName == "mem/swap/used"):
            if (self.swapInMb):
                mem = str(round(float(data[QString(u'value')]) / 1024, 0)).split('.')[0]
                mem = "%5s" % (mem)
                if (self.swapFormat.split('$swapmb')[0] != self.swapFormat):
                    line = self.swapFormat.split('$swapmb')[0] + mem + self.swapFormat.split('$swapmb')[1]
                else:
                    line = self.swapFormat
                text = self.formatLine.split('$LINE')[0] + line + self.formatLine.split('$LINE')[1]
                self.label_swap.setText(text)
            else:
                self.swap_used = float(data[QString(u'value')])
        


def CreateApplet(parent):
	return pyTextWidget(parent)
