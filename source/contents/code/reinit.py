from PyQt4.QtCore import *
from PyKDE4.plasma import Plasma
import config
import gpuchecker
import gputempchecker
import hddtempchecker



class Reinit():
    def __init__(self, parent):
        """class definition"""
        self.parent = parent

    def reinit(self):
        """function to reinitializate widget"""
        self.parent.settings = config.Config(self.parent)
        self.parent.interval = int(self.parent.settings.get('interval', 2000))
        self.parent.font_family = str(self.parent.settings.get('font_family', 'Terminus'))
        self.parent.font_size = int(self.parent.settings.get('font_size', 12))
        self.parent.font_color = str(self.parent.settings.get('font_color', '#000000'))
        self.parent.font_style = str(self.parent.settings.get('font_style', 'normal'))
        self.parent.font_weight = int(self.parent.settings.get('font_weight', 400))
        self.parent.formatLine = "<pre><p align=\"center\"><span style=\" font-family:'" + self.parent.font_family + "'; font-style:" + self.parent.font_style
        self.parent.formatLine = self.parent.formatLine + "; font-size:" + str(self.parent.font_size) + "pt; font-weight:" + str(self.parent.font_weight)
        self.parent.formatLine = self.parent.formatLine + "; color:" + self.parent.font_color + ";\">$LINE</span></p></pre>"
        self.parent.label_order = str(self.parent.settings.get('label_order', '1345'))
        for label in self.parent.dict_orders.values():
            if ((label == 'cpu') or (label == 'mem') or (label == 'swap') or (label == 'net')):
                exec ('self.parent.' + label + 'Bool = int(self.parent.settings.get("' + label + 'Bool",  1))')
            else:
                exec ('self.parent.' + label + 'Bool = int(self.parent.settings.get("' + label + 'Bool",  0))')
        # small function for update if errors exist
        summ = 0
        for label in self.parent.dict_orders.values():
            exec ('summ += self.parent.' + label + 'Bool')
        if (len(self.parent.label_order) != summ):
            for label in self.parent.dict_orders.values():
                if ((label == 'cpu') or (label == 'mem') or (label == 'swap') or (label == 'net')):
                    exec ('self.parent.' + label + 'Bool = 1')
                else:
                    exec ('self.parent.' + label + 'Bool = 0')
                exec ('self.parent.settings.set("' + label + 'Bool", self.parent.' + label + 'Bool)')
            self.parent.label_order = '1345'
            self.parent.settings.set('label_order', self.parent.label_order)
        
        for order in self.parent.label_order:
            if (order == "1"):
                if (self.parent.cpuBool == 1):
                    self.parent.cpuFormat = str(self.parent.settings.get('cpuFormat', '[cpu: $cpu%]'))
                    if (self.parent.cpuFormat.split('$ccpu')[0] != self.parent.cpuFormat):
                        self.parent.label_cpu0 = Plasma.Label(self.parent.applet)
                        self.parent.label_cpu1 = Plasma.Label(self.parent.applet)
                        if (self.parent.cpuFormat.split('$ccpu')[0].split('$cpu')[0] != self.parent.cpuFormat.split('$ccpu')[0]):
                            line = self.parent.cpuFormat.split('$ccpu')[0].split('$cpu')[0] + '-----' + self.parent.cpuFormat.split('$ccpu')[0].split('$cpu')[1]
                        else:
                            line = self.parent.cpuFormat.split('$ccpu')[0]
                        text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                        self.parent.label_cpu0.setText(text)
                        self.parent.layout.addItem(self.parent.label_cpu0)
                        text = self.parent.formatLine.split('$LINE')[0] + "-----" + self.parent.formatLine.split('$LINE')[1]
                        for core in range(self.parent.numCores):
                            exec ('self.parent.label_coreCpu' + str(core) + ' = Plasma.Label(self.parent.applet)')
                            exec ('self.parent.label_coreCpu' + str(core) + '.setText(text)')
                            exec ('self.parent.layout.addItem(self.parent.label_coreCpu' + str(core) + ')')
                        if (self.parent.cpuFormat.split('$ccpu')[1].split('$cpu')[0] != self.parent.cpuFormat.split('$ccpu')[1]):
                            line = self.parent.cpuFormat.split('$ccpu')[1].split('$cpu')[0] + '-----' + self.parent.cpuFormat.split('$ccpu')[1].split('$cpu')[1]
                        else:
                            line = self.parent.cpuFormat.split('$ccpu')[1]
                        text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                        self.parent.label_cpu1.setText(text)
                        self.parent.layout.addItem(self.parent.label_cpu1)
                    else:
                        self.parent.label_cpu = Plasma.Label(self.parent.applet)
                        if (self.parent.cpuFormat.split('$cpu')[0] != self.parent.cpuFormat):
                            line = self.parent.cpuFormat.split('$cpu')[0] + '-----' + self.parent.cpuFormat.split('$cpu')[1]
                        else:
                            line = self.parent.cpuFormat
                        text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                        self.parent.label_cpu.setText(text)
                        self.parent.layout.addItem(self.parent.label_cpu)
            elif (order == "2"):
                if (self.parent.tempBool == 1):
                    self.parent.tempFormat = str(self.parent.settings.get('tempFormat', '[temp: $temp&deg;C]'))
                    self.parent.label_temp = Plasma.Label(self.parent.applet)
                    if (self.parent.tempFormat.split('$temp')[0] != self.parent.tempFormat):
                        line = self.parent.tempFormat.split('$temp')[0] + '----' + self.parent.tempFormat.split('$temp')[1]
                    else:
                        line = self.parent.tempFormat
                    text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                    self.parent.label_temp.setText(text)
                    self.parent.layout.addItem(self.parent.label_temp)
            elif (order == "3"):
                if (self.parent.memBool == 1):
                    self.parent.memFormat = str(self.parent.settings.get('memFormat', '[mem: $mem%]'))
                    if (self.parent.memFormat.split('$memmb')[0] != self.parent.memFormat):
                        self.parent.memInMb = True
                        text = self.parent.formatLine.split('$LINE')[0] + self.parent.memFormat.split('$memmb')[0] + '-----' + self.parent.memFormat.split('$memmb')[1] + self.parent.formatLine.split('$LINE')[1]
                    elif (self.parent.memFormat.split('$mem')[0] != self.parent.memFormat):
                        self.parent.memInMb = False
                        self.parent.mem_used = 0.0
                        self.parent.mem_free = 1.0
                        self.parent.mem_uf = 0.0
                        line = self.parent.memFormat.split('$mem')[0] + '-----' + self.parent.memFormat.split('$mem')[1]
                    else:
                        line = self.parent.memFormat
                    text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                    self.parent.label_mem = Plasma.Label(self.parent.applet)
                    self.parent.label_mem.setText(text)
                    self.parent.layout.addItem(self.parent.label_mem)
            elif (order == "4"):
                if (self.parent.swapBool == 1):
                    self.parent.swapFormat = str(self.parent.settings.get('swapFormat', '[swap: $swap%]'))
                    if (self.parent.swapFormat.split('$swapmb')[0] != self.parent.swapFormat):
                        self.parent.swapInMb = True
                        text = self.parent.formatLine.split('$LINE')[0] + self.parent.swapFormat.split('$swapmb')[0] + '-----' + self.parent.swapFormat.split('$swapmb')[1] + self.parent.formatLine.split('$LINE')[1]
                    elif (self.parent.swapFormat.split('$swap')[0] != self.parent.swapFormat):
                        self.parent.swapInMb = False
                        self.parent.swap_free = 1.0
                        self.parent.swap_used = 0.0
                        line = self.parent.swapFormat.split('$swap')[0] + '-----' + self.parent.swapFormat.split('$swap')[1]
                    else:
                        line = self.parent.swapFormat
                    text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                    self.parent.label_swap = Plasma.Label(self.parent.applet)
                    self.parent.label_swap.setText(text)
                    self.parent.layout.addItem(self.parent.label_swap)
            elif (order == "5"):
                if (self.parent.netBool == 1):
                    self.parent.netNonFormat = str(self.parent.settings.get('netNonFormat', '[net: $netKB/s]'))
                    if (self.parent.netNonFormat.split('@@')[0] != self.parent.netNonFormat):
                        self.parent.netdev = self.parent.netNonFormat.split('@@')[1]
                        self.parent.netNonFormat = self.parent.netNonFormat.split('@@')[0] + self.parent.netNonFormat.split('@@')[2]
                    else:
                        self.parent.num_dev = int(self.parent.settings.get('num_dev', 0))
                        self.parent.setupNetdev()
                    if (self.parent.netNonFormat.split('$netdev')[0] != self.parent.netNonFormat):
                        self.parent.netFormat = self.parent.netNonFormat.split('$netdev')[0] + self.parent.netdev + self.parent.netNonFormat.split('$netdev')[1]
                    else:
                        self.parent.netFormat = self.parent.netNonFormat
                    self.parent.label_netDown = Plasma.Label(self.parent.applet)
                    if (self.parent.netFormat.split('$net')[0] != self.parent.netFormat):
                        line = self.parent.netFormat.split('$net')[0] + '----'
                    else:
                        line = self.parent.netFormat
                    text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                    self.parent.label_netDown.setText(text)
                    self.parent.layout.addItem(self.parent.label_netDown)
                    self.parent.label_netUp = Plasma.Label(self.parent.applet)
                    if (self.parent.netFormat.split('$net')[0] != self.parent.netFormat):
                        line = '/----' + self.parent.netFormat.split('$net')[1]
                    else:
                        line = ''
                    text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                    self.parent.label_netUp.setText(text)
                    self.parent.layout.addItem(self.parent.label_netUp)
            elif (order == "6"):
                if (self.parent.batBool == 1):
                    self.parent.batFormat = str(self.parent.settings.get('batFormat', '[bat: $bat%$ac]'))
                    self.parent.battery_device= str(self.parent.settings.get('battery_device', '/sys/class/power_supply/BAT0/capacity'))
                    self.parent.ac_device = str(self.parent.settings.get('ac_device', '/sys/class/power_supply/AC/online'))
                    self.parent.label_bat = Plasma.Label(self.parent.applet)
                    line = self.parent.batFormat
                    if (line.split('$ac')[0] != line):
                        line = line.split('$ac')[0] + '(-)' + line.split('$ac')[1]
                    if (line.split('$bat')[0] != line):
                        line = line.split('$bat')[0] + '---' + line.split('$bat')[1]
                    text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                    self.parent.label_bat.setText(text)
                    self.parent.layout.addItem(self.parent.label_bat)
            elif (order == "7"):
                if (self.parent.cpuclockBool == 1):
                    self.parent.cpuclockFormat = str(self.parent.settings.get('cpuclockFormat', '[mhz: $cpucl]'))
                    if (self.parent.cpuclockFormat.split('$ccpucl')[0] != self.parent.cpuclockFormat):
                        self.parent.label_cpuclock0 = Plasma.Label(self.parent.applet)
                        self.parent.label_cpuclock1 = Plasma.Label(self.parent.applet)
                        if (self.parent.cpuclockFormat.split('$ccpucl')[0].split('$cpucl')[0] != self.parent.cpuclockFormat.split('$ccpucl')[0]):
                            line = self.parent.cpuclockFormat.split('$ccpucl')[0].split('$cpucl')[0] + '----' + self.parent.cpuclockFormat.split('$ccpucl')[0].split('$cpucl')[1]
                        else:
                            line = self.parent.cpuclockFormat.split('$ccpucl')[0]
                        text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                        self.parent.label_cpuclock0.setText(text)
                        self.parent.layout.addItem(self.parent.label_cpuclock0)
                        text = self.parent.formatLine.split('$LINE')[0] + "----" + self.parent.formatLine.split('$LINE')[1]
                        for core in range(self.parent.numCores):
                            exec ('self.parent.label_coreCpuclock' + str(core) + ' = Plasma.Label(self.parent.applet)')
                            exec ('self.parent.label_coreCpuclock' + str(core) + '.setText(text)')
                            exec ('self.parent.layout.addItem(self.parent.label_coreCpuclock' + str(core) + ')')
                        if (self.parent.cpuclockFormat.split('$ccpucl')[1].split('$cpucl')[0] != self.parent.cpuclockFormat.split('$ccpucl')[1]):
                            line = self.parent.cpuclockFormat.split('$ccpucl')[1].split('$cpucl')[0] + '----' + self.parent.cpuclockFormat.split('$ccpucl')[1].split('$cpucl')[1]
                        else:
                            line = self.parent.cpuclockFormat.split('$ccpucl')[1]
                        text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                        self.parent.label_cpuclock1.setText(text)
                        self.parent.layout.addItem(self.parent.label_cpuclock1)
                    else:
                        self.parent.label_cpuclock = Plasma.Label(self.parent.applet)
                        if (self.parent.cpuclockFormat.split('$cpucl')[0] != self.parent.cpuclockFormat):
                            line = self.parent.cpuclockFormat.split('$cpucl')[0] + '----' + self.parent.cpuclockFormat.split('$cpucl')[1]
                        else:
                            line = self.parent.cpuclockFormat
                        text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                        self.parent.label_cpuclock.setText(text)
                        self.parent.layout.addItem(self.parent.label_cpuclock)
            elif (order == "8"):
                if (self.parent.uptimeBool == 1):
                    self.parent.uptimeFormat = str(self.parent.settings.get('uptimeFormat', '[uptime: $uptime]'))
                    self.parent.label_uptime = Plasma.Label(self.parent.applet)
                    if (self.parent.uptimeFormat.split('$uptime')[0] != self.parent.uptimeFormat):
                        line = self.parent.uptimeFormat.split('$uptime')[0] + '---d--h--m' + self.parent.uptimeFormat.split('$uptime')[1]
                    else:
                        line = self.parent.uptimeFormat
                    text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                    self.parent.label_uptime.setText(text)
                    self.parent.layout.addItem(self.parent.label_uptime)
            elif (order == "9"):
                if (self.parent.gpuBool == 1):
                    self.parent.gpuFormat = str(self.parent.settings.get('gpuFormat', '[gpu: $gpu%]'))
                    self.parent.label_gpu = Plasma.Label(self.parent.applet)
                    if (self.parent.gpuFormat.split('$gpu')[0] != self.parent.gpuFormat):
                        line = self.parent.gpuFormat.split('$gpu')[0] + '-----' + self.parent.gpuFormat.split('$gpu')[1]
                    else:
                        line = self.parent.gpuFormat
                    text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                    self.parent.label_gpu.setText(text)
                    self.parent.layout.addItem(self.parent.label_gpu)
            elif (order == "a"):
                if (self.parent.gputempBool == 1):
                    self.parent.gputempFormat = str(self.parent.settings.get('gputempFormat', '[gpu temp: $gputemp&deg;C]'))
                    self.parent.label_gputemp = Plasma.Label(self.parent.applet)
                    if (self.parent.gputempFormat.split('$gputemp')[0] != self.parent.gputempFormat):
                        line = self.parent.gputempFormat.split('$gputemp')[0] + '----' + self.parent.gputempFormat.split('$gputemp')[1]
                    else:
                        line = self.parent.gputempFormat
                    text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                    self.parent.label_gputemp.setText(text)
                    self.parent.layout.addItem(self.parent.label_gputemp)
            elif (order == "b"):
                if (self.parent.hddBool == 1):
                    self.parent.hddFormat = str(self.parent.settings.get('hddFormat', '[hdd: @@/@@%]'))
                    if (self.parent.hddFormat.split('@@')[0] != self.parent.hddFormat):
                        self.parent.mountPoints = self.parent.hddFormat.split('@@')[1].split(';')
                        line = self.parent.hddFormat.split('@@')[0]
                        text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                        self.parent.label_hdd0 = Plasma.Label(self.parent.applet)
                        self.parent.label_hdd0.setText(text)
                        self.parent.layout.addItem(self.parent.label_hdd0)
                        text = self.parent.formatLine.split('$LINE')[0] + "-----" + self.parent.formatLine.split('$LINE')[1]
                        for mount in self.parent.mountPoints:
                            exec ('self.parent.label_hdd_' + ''.join(mount.split('/')) + ' = Plasma.Label(self.parent.applet)')
                            exec ('self.parent.label_hdd_' + ''.join(mount.split('/')) + '.setText(text)')
                            exec ('self.parent.layout.addItem(self.parent.label_hdd_' + ''.join(mount.split('/')) + ')')
                        line = self.parent.hddFormat.split('@@')[2]
                        text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                        self.parent.label_hdd1 = Plasma.Label(self.parent.applet)
                        self.parent.label_hdd1.setText(text)
                        self.parent.layout.addItem(self.parent.label_hdd1)
                    else:
                        line = self.parent.hddFormat
                        text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                        self.parent.label_hdd0.setText(text)
                        self.parent.layout.addItem(self.parent.label_hdd0)
            elif (order == "c"):
                if (self.parent.hddtempBool == 1):
                    self.parent.hddtempFormat = str(self.parent.settings.get('hddtempFormat', '[hdd temp: @@/dev/sda@@&deg;C]'))
                    self.parent.label_hddtemp = Plasma.Label(self.parent.applet)
                    if (self.parent.hddtempFormat.split('@@')[0] != self.parent.hddtempFormat):
                        line = self.parent.hddtempFormat.split('@@')[0] + '----' + self.parent.hddtempFormat.split('@@')[2]
                    else:
                        line = self.parent.hddtempFormat
                    text = self.parent.formatLine.split('$LINE')[0] + line + self.parent.formatLine.split('$LINE')[1]
                    self.parent.label_hddtemp.setText(text)
                    self.parent.layout.addItem(self.parent.label_hddtemp)
        self.parent.applet.setLayout(self.parent.layout)        
        self.parent.theme = Plasma.Svg(self.parent)
        self.parent.theme.setImagePath("widgets/background")
        self.parent.setBackgroundHints(Plasma.Applet.DefaultBackground)
        self.parent.resize(10,10)
        
        # create threading
        self.parent.dataengine.connectToEngine()
        self.parent.timer = QTimer()
        self.parent.timer.setInterval(self.parent.interval)
        if (self.parent.gpuBool == 1):
            self.parent.gpuChecker = gpuchecker.GpuThread(self.parent.interval, self.parent.gpudev)
            self.parent.gpuChecker.start()
        if (self.parent.gputempBool == 1):
            self.parent.gpuTempChecker = gputempchecker.GpuTempThread(self.parent.interval, self.parent.gpudev)
            self.parent.gpuTempChecker.start()
        if (self.parent.hddtempBool ==1):
            self.parent.hddTempChecker = hddtempchecker.HddTempThread(self.parent.interval, self.parent.hddtempFormat.split('@@')[1])
            self.parent.hddTempChecker.start()
        self.parent.startPolling()
