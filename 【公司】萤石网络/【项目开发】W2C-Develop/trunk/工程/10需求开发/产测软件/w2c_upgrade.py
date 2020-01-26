# -*- coding: utf-8 -*-  
import wx
import subprocess
import string
import sys
import time
import httplib
import urllib
import urllib2
import json
import os
import unicodedata
import serial

from threading import Thread 
from wx.lib.pubsub import setupkwargs
from wx.lib.pubsub import pub

def passed(item):  
     try: 
          return len(item) != 0
     except ValueError:  
          return False 
######################################################################## 
class TestThread(Thread): 
     """Test Worker Thread Class."""
     serialPort = ''
     seriaPasswd = "YSGW@GW"

     routerPassword = "11111111"

     #设置wifi的SSID,测试过程中若想修改，只需修改routerSSID变量的值即可。
     router2gSSID = "ezviz_016234"
     router2gPwd = "1234ABCDEF"

     #用户的pc地址列表
     pcIpaddr = '192.168.1.128'

     #w2c设备地址
     w2cIpaddr = '192.168.7.1'

     #需要写入的硬件参数及能力集参数
     hik_hw_sncode = ''
     hik_hw_checkcode = ''
     hik_hw_mac = ''
     hik_hw_type = ''

     #三个版本文件名字
     ubootName = ""
     kernelName = ""
     appName = ""

     typeList = ['CS-W2C']
   
 
     def __init__(self, com, sn, check, type, mac, ver): 
          """Init Worker Thread Class."""
          self.serialPort = com
          self.hik_hw_sncode = sn
          self.hik_hw_checkcode = check
          self.hik_hw_mac = mac
          self.hik_hw_type = type
          #self.routerBinName = bin

          self.ubootName = "w2c_uboot_v" + ver + ".bin"
          self.kernelName = "w2c_kernel_v" + ver + ".bin"
          self.appName = "w2c_app_v" + ver + ".bin"
          
          
          Thread.__init__(self)
   
          self.start()  # start the thread 

     
     def run(self): 
          """Run Worker Thread."""
          self.postMsg(0, u"开始检查参数扫描参数合法性")

          if self.doCheckPara() == False:
            return False

          #打开串口
          tryTimes = 0
          openFail = True
          while (tryTimes < 40 and openFail): 
               try:
                    tryTimes += 1
                    openFail = False
                    self.postMsg(0, u"开始尝试第%d次打开串口"%tryTimes)              
                    term = serial.Serial(self.serialPort,115200,timeout=30)          
               except Exception, e:
                    openFail = True
                    time.sleep(1)

          if openFail:
               self.postMsg(1, u"打开串口失败，请检查串口是否已经被占用")
               return False
          else:
               self.postMsg(0, u"打开串口成功")

          #连上串口以后可以不断地输入ctrl+c
          tryTimes = 0;        
          while (tryTimes < 60):
               time.sleep(1)
               tryTimes += 1
               self.postMsg(0, u"开始尝试第%d次进入uBoot"%tryTimes)
               term.write('\x03')   
               outNum= term.inWaiting();
               line = term.read(outNum)
               if 'ath>' in line:
                    break
          if (tryTimes >= 40):
               self.postMsg(1, u"进入uBoot失败")
               return False;
          else:
               self.postMsg(0, u"进入uBoot成功")

          time.sleep(0.5)
          
          outNum= term.inWaiting()
          print term.read(outNum)

          """第一步：写加密参数"""
          
          if self.DoWritew2shw(term) == False:
               return False;


          """第二步：升级版本"""
          term.write('set serverip %s\x0D'%self.pcIpaddr)
          time.sleep(0.5)
          print term.read(term.inWaiting())
          

          self.postMsg(0, u"开始升级版本,地址：%s"%self.pcIpaddr)
          '''升级uboot'''
          self.postMsg(0, u"下载uboot......")
          term.write('tftp 0x80060000 %s\x0D'%self.ubootName)
          time.sleep(5)
          line = term.read(term.inWaiting())
          print line
          if 'done' not in line:
               self.postMsg(1, u"升级%s失败，请检查网络连接是否正常或者重试！"%self.ubootName)
               return False

          self.postMsg(0, u"擦除原uboot......")
          term.write('erase 0x9f000000 +0x50000\x0D')
          time.sleep(2)
          line = term.read(term.inWaiting())
          print line
          if 'Erased 5 sectors' not in line:
               self.postMsg(1, u"升级uboot失败， 擦除失败！")
               return False

          self.postMsg(0, u"复制uboot到flash......")
          term.write('cp.b $fileaddr 0x9f000000 $filesize\x0D')
          time.sleep(1.5)
          line = term.read(term.inWaiting())
          print line
          if 'done' not in line:
               self.postMsg(1, u"升级uboot失败，复制失败！")
               return False

          '''升级kernel'''
          self.postMsg(0, u"下载kernel......")
          term.write('tftp 0x80060000 %s\x0D'%self.kernelName)
          time.sleep(4)
          line = term.read(term.inWaiting())
          print line
          if 'done' not in line:
               waitSec = 50
               tempSec = 1
               while tempSec <= waitSec:
                    time.sleep(1)
                    
                    line = term.read(term.inWaiting())
                    print line
                    if 'done' in line:
                         break
                    else:
                         tempSec += 1

               if tempSec > waitSec:
                    self.postMsg(1, u"升级%s失败，请检查网络连接是否正常或者重试！！"%self.kernelName)
                    return False
 

          self.postMsg(0, u"擦除原kernel......")
          term.write('erase 0x9fe80000 +0x170000\x0D')
          time.sleep(4)
          line = term.read(term.inWaiting())
          print line
          if 'Erased 23 sectors' not in line:
               waitSec = 50
               tempSec = 1
               while tempSec <= waitSec:
                    time.sleep(1)
                    
                    line = term.read(term.inWaiting())
                    print line
                    if 'Erased 23 sectors' in line:
                         break
                    else:
                         tempSec += 1

               if tempSec > waitSec:
                    self.postMsg(1, u"升级kerne失败，擦除失败！！")
                    return False

          self.postMsg(0, u"复制kernel到flash......")
          term.write('cp.b $fileaddr 0x9fe80000 $filesize\x0D')
          time.sleep(4)
          line = term.read(term.inWaiting())
          print line
          if 'done' not in line:
               waitSec = 50
               tempSec = 1
               while tempSec <= waitSec:
                    time.sleep(1)
                    
                    line = term.read(term.inWaiting())
                    print line
                    if 'done' in line:
                         break
                    else:
                         tempSec += 1

               if tempSec > waitSec:
                    self.postMsg(1, u"升级kerne失败，复制失败！！")
                    return False


          '''升级rootfs'''
          self.postMsg(0, u"下载rootfs......")
          term.write('tftp 0x80060000 %s\x0D'%self.appName)
          time.sleep(15)
          line = term.read(term.inWaiting())
          print line
          if 'done' not in line:
               waitSec = 50
               tempSec = 1
               while tempSec <= waitSec:
                    time.sleep(1)
                    
                    line = term.read(term.inWaiting())
                    print line
                    if 'done' in line:
                         break
                    else:
                         tempSec += 1

               if tempSec > waitSec:
                    self.postMsg(1, u"升级%s失败，请检查网络连接是否正常或者重试！！"%self.appName)
                    return False

          self.postMsg(0, u"擦除原rootfs......")
          term.write('erase 0x9f050000 +0xe30000\x0D')
          time.sleep(20)
          line = term.read(term.inWaiting())
          print line
          if 'Erased 227 sectors' not in line:
               waitSec = 80
               tempSec = 1
               while tempSec <= waitSec:
                    time.sleep(1)
                    
                    line = term.read(term.inWaiting())
                    print line
                    if 'Erased 227 sectors' in line:
                         break
                    else:
                         tempSec += 1

               if tempSec > waitSec:
                    self.postMsg(1, u"升级rootfs失败，擦除失败！")
                    return False

          self.postMsg(0, u"复制rootfs到flash......")
          term.write('cp.b $fileaddr 0x9f050000 $filesize\x0D')
          time.sleep(20)
          line = term.read(term.inWaiting())
          print line
          if 'done' not in line:
               waitSec = 80
               tempSec = 1
               while tempSec <= waitSec:
                    time.sleep(1)
                    
                    line = term.read(term.inWaiting())
                    print line
                    if 'done' in line:
                         break
                    else:
                         tempSec += 1

               if tempSec > waitSec:
                    self.postMsg(1, u"升级rootfs失败，复制失败！")
                    return False

          """第三步：重启并设置ssid"""
          self.postMsg(0, u"正在重启设备......")
          term.write('boot\x0D')
          time.sleep(30)
          term.read(term.inWaiting())
          time.sleep(40)
          term.read(term.inWaiting())

          waitSec = 60
          tempSec = 1
          while tempSec <= waitSec:
               term.write('\x0D')
               time.sleep(1)
               line = term.read(term.inWaiting())
               print line
               if 'root@OpenWrt:/#' in line:
                    break
               else:
                    tempSec += 1

          if tempSec > waitSec:
               self.postMsg(1, u"设备启动失败！")
               return False

          #解锁串口
          term.write('%s\x0D'%self.seriaPasswd)
          time.sleep(0.2)
          term.write('YSGW@%s\x0D'%self.routerPassword)
          time.sleep(0.2)
          term.write('YSGW@%s\x0D'%self.hik_hw_checkcode)
          time.sleep(0.2)
          line = term.read(term.inWaiting())
          print line
          if 'console unlock success' not in line:
               self.postMsg(1, u'串口解锁失败....')
               return False
          else:
               self.postMsg(0, u'串口解锁成功....')

          self.postMsg(0, u"设备重启完成，开始写入ssid......")

          msg = "uci set wireless.@wifi-iface[0].ssid=\'%s\'"%self.router2gSSID
          self.writeMsg(term, msg)

          msg = "uci set wireless.@wifi-iface[0].disabled=\'0\'"
          self.writeMsg(term, msg)

          msg = "uci set wireless.@wifi-iface[0].encryption=\'psk-mixed\'"
          self.writeMsg(term, msg)

          msg = "uci set wireless.@wifi-iface[0].key=\'%s\'"%self.router2gPwd
          self.writeMsg(term, msg)

          msg = "uci set wireless.@wifi-iface[1].disabled=1"
          self.writeMsg(term, msg)

          #msg = "uci commit"
          #self.writeMsg(term, msg)

          msg = "uci set hik_cfg.admin.passwd=\'%s\'"%self.routerPassword
          self.writeMsg(term, msg)

          msg = "uci set network.lan.ipaddr=\'%s\'"%self.w2cIpaddr
          self.writeMsg(term, msg)
          
          msg = "uci commit"
          self.writeMsg(term, msg)

          #检查配置是否写入成功
          term.write('uci show hik_cfg\x0D')
          time.sleep(0.5)
          term.write('uci show network\x0D')
          time.sleep(0.5)
          term.write('uci show wireless\x0D')
          time.sleep(0.5)
          line = term.read(term.inWaiting())
          print line

          writeSuccess = True
          if self.router2gSSID not in line:
               writeSuccess = False
          elif self.router2gPwd not in line:
               writeSuccess = False
          elif self.w2cIpaddr not in line:
               writeSuccess = False

          if False == writeSuccess:
               self.postMsg(1, u'写参数失败，需要将设备断电重新执行升级加密！')
               return False

          #msg = "uci commit hik_cfg"
          #self.writeMsg(term, msg)
          
          term.write('reboot\x0D')
          
          self.postMsg(100, u'版本升级和写硬件参数执行成....')
          

     def writeMsg(self, term, msg):
          term.write('%s\x0D'%msg)
          time.sleep(1)
          line = term.read(term.inWaiting())
          print line
          while "error" in line:
               term.write('%s\x0D'%msg)
               time.sleep(1)
               line = term.read(term.inWaiting())
               print line
          

     def postMsg(self, code,msg): 
          pub.sendMessage("update", data=code, extra1='%s'%msg)

     def SetWanAndWifi(self, seri):
          '''''设置wan口地址 跟 wifi的ssid'''  
          self.postMsg(0, u"开始设置ssid...")
          seri.write('nvram set wl1_ssid=%s\x0D'%self.router2gSSID)
          time.sleep(0.1)
          print seri.read(seri.inWaiting())
          seri.write('nvram set wl0_ssid=%s\x0D'%self.router5gSSID)
          time.sleep(0.1)
          print seri.read(seri.inWaiting())

          seri.write('nvram set http_passwd=%s\x0D'%self.routerPassword)
          time.sleep(0.1)
          print seri.read(seri.inWaiting())
          self.postMsg(0, u"ssid设置完成...")

          seri.write('nvram set wl0_chanspec=149l\x0D')
          time.sleep(0.1)
          print seri.read(seri.inWaiting())

          seri.write('nvram set time_zone=UTC-8\x0D')
          time.sleep(0.1)
          print seri.read(seri.inWaiting())


          seri.write('nvram set hik_rf_test_mode=1\x0D')
          time.sleep(0.1)
          print seri.read(seri.inWaiting())

          return True
     
     def charIsHex(self, c):
          if (c >= '0' and c <= '9') or (c >= 'a' and c <= 'f') or (c >= 'A' and c <= 'F'):
               return True
          else:
               return False

     def strIsMac(self, macStr):
          macLen = len('11:22:33:44:55:66')
          if len(macStr) != macLen:
               return False

          macList = macStr.split(':')
          if len(macList) != 6:
               return False

          if (macList[0] == '01') or (macList[0] == 'ff') or (macList[0] == 'FF'):
               return False

          for macByte in macList:
               if len(macByte) != 2:
                    return False
               for c in macByte:
                    if self.charIsHex(c) == False:
                         return False


          return True
     
     def doCheckPara(self):

          if len(self.hik_hw_sncode) != 9 or self.hik_hw_sncode.isdigit() != True:
               self.postMsg(1, u"扫描的序列号有问题，必须为9位数纯数字")
               return False

        
          if len(self.hik_hw_checkcode) != 6 or self.hik_hw_checkcode.isalpha() == False or self.hik_hw_checkcode.isupper() == False:
               self.postMsg(1, u"扫描的验证码有问题，必须为6位数纯大写字母")
               return False

          tmp_hw_type=self.hik_hw_type

          first = tmp_hw_type.find("/")
          if first > 0:
               self.hik_hw_type = tmp_hw_type[:first]
          else:
               self.hik_hw_type = tmp_hw_type
               
          typeRight = False
          for hwType in self.typeList:
               if hwType == self.hik_hw_type:
                    typeRight = True
                    break

          if typeRight == False:
               self.postMsg(1, u"扫描的设备类型有问题")
               return False

          if self.strIsMac(self.hik_hw_mac) == False:
               #print self.hik_hw_mac
               self.postMsg(1, u"扫描的设备mac有问题")
               return False

          self.postMsg(0, u"扫描参数检查成功")

          return True

    
     def DoWritew2shw(self, seri):

          self.postMsg(0, u"开始写入扫描参数")

          #eth0_mac
          et0macaddr=self.hik_hw_mac

          for i in range(len(et0macaddr)):
            if et0macaddr[i].isalpha() and et0macaddr[i].islower() == False:
                et0macaddr = et0macaddr[:i] + et0macaddr[i].lower() + et0macaddr[i+1:]

          
          print et0macaddr
          maclist=self.hik_hw_mac.split(':')
           
          mac0=string.atoi(maclist[0],16)
          mac1=string.atoi(maclist[1],16)
          mac2=string.atoi(maclist[2],16)
          mac3=string.atoi(maclist[3],16)
          mac4=string.atoi(maclist[4],16)
          mac5=string.atoi(maclist[5],16)
            
          #eth1_mac
          tmpDigitStr = "%02x%02x%02x%02x"%(mac2,mac3,mac4,mac5)
          tmpDigit = string.atoi(tmpDigitStr,16)
          tmpDigit += 1
          tmpDigitStr1 = "%08x"%tmpDigit
          print tmpDigitStr1
          sbmac = "%02x:%02x:%s:%s:%s:%s"%(mac0, mac1, tmpDigitStr1[0:2], tmpDigitStr1[2:4], tmpDigitStr1[4:6], tmpDigitStr1[6:])
          print sbmac

          #wifi_mac
          tmpDigit += 1
          tmpDigitStr1 = "%08x"%tmpDigit
          print tmpDigitStr1
          mac00 = "%02x:%02x:%s:%s:%s:%s"%(mac0, mac1, tmpDigitStr1[0:2], tmpDigitStr1[2:4], tmpDigitStr1[4:6], tmpDigitStr1[6:])
          print mac00
          '''
          seri.write('nvram set et0macaddr=%s\x0D'%et0macaddr)
          time.sleep(0.1)
          seri.write('nvram set sb/1/macaddr=%s\x0D'%sbmac)
          time.sleep(0.1)
          seri.write('nvram set 0:macaddr=%s\x0D'%mac00)
          time.sleep(0.1)
        
          hik_hw_date=time.strftime('%Y%m%d')
          seri.write('nvram set hik_hw_date=%s\x0D'%hik_hw_date)
          time.sleep(0.1)
          seri.write('nvram commit\x0D') 

          seri.write('nvram show\x0D')
          time.sleep(3)
          line = seri.read(seri.inWaiting())

          writeFailed = 0
          if self.hik_hw_sncode not in line:
               writeFailed = 1
          elif self.hik_hw_checkcode not in line:
               writeFailed = 1
          elif self.hik_hw_type not in line:
               writeFailed = 1
          elif self.hik_hw_mac not in line:
               writeFailed = 1
          elif hik_hw_date not in line:
               writeFailed = 1
            
          if writeFailed != 0:
               self.postMsg(1, u"参数写入失败")
               return False
          '''

          seri.write('progmac2 %s %s %s %s %s\x0D'%(et0macaddr, sbmac, mac00, self.hik_hw_sncode, self.hik_hw_checkcode))
          time.sleep(1)
          print seri.read(seri.inWaiting())

          seri.write('read_hik \x0D')
          time.sleep(0.5)
          line = seri.read(seri.inWaiting())
          print line

          writeFailed = 0
          if et0macaddr not in line:
               writeFailed = 1
          elif sbmac not in line:
               writeFailed = 1
          elif mac00 not in line:
               writeFailed = 1
          elif self.hik_hw_sncode not in line:
               writeFailed = 1
          elif self.hik_hw_checkcode not in line:
               writeFailed = 1
            
          if writeFailed != 0:
               self.postMsg(1, u"参数写入失败")
               return False
               
          
          self.postMsg(0, u"参数写入成功")
          return True


######################################################################## 

class TestFrame(wx.Frame):

     #用户选择的地址
     serialPort="COM1"

     #支持的设备列表以及各个设备支持的参数,如果需要增加设备类型及参数只需修改这两个数组
     seriList = ['COM1', 'COM2', 'COM3', 'COM4', 'COM5', 'COM6', 'COM7', 'COM8', 'COM9', 'COM10']
     corlourList = [
                    ['COM1',[217, 217, 217]],
                    ['COM2',[128, 128, 128]],
                    ['COM3',[114, 159, 220]],
                    ['COM4',[200, 214, 230]],
                    ['COM5',[237, 205, 203]],
                    ['COM6',[235, 241, 222]],
                    ['COM7',[241, 238, 244]],
                    ['COM8',[209, 235, 241]],
                    ['COM9',[253, 239, 227]],
                    ['COM10',[146, 122, 173]],
               ]

     

     def __init__(self, parent):
          wx.Frame.__init__(self, parent, -1, u"W2C版本升级&加密",
                size=(280, 420))
          self.Center()
          color1 = wx.Colour(217, 217, 217)
          self.SetBackgroundColour(color1)
        
          panel = wx.Panel(self, -1)

          self.lable1 = wx.StaticText(panel, -1, "web",(10, 8),size = (60,20))
          self.lable2 = wx.StaticText(panel, -1, "sn",(10, 26),size = (60,20))
          self.lable3 = wx.StaticText(panel, -1, "check",(10, 44),size = (60,20))
          self.lable4 = wx.StaticText(panel, -1, "hw type",(10, 62),size = (60,20))
          self.lable5 = wx.StaticText(panel, -1, "hw mac",(10, 80),size = (60,20))

          self.text = wx.TextCtrl(panel,-1,value='',pos=(75,8),size=(180,110),style=wx.TE_MULTILINE)
          self.text.Bind(wx.EVT_TEXT, self.OnText, self.text)
          
          self.lable8 = wx.StaticText(panel, -1, u"请输入升级包版本\n(x.y.z),例如：3.1.1",(10, 130),size = (100,40))
          self.binText = wx.TextCtrl(panel, -1, value='', pos=(118,140),size=(120,20),style=wx.TE_LEFT)
          self.binText.Bind(wx.EVT_TEXT, self.OnBinText, self.binText)
          
          self.lable6 = wx.StaticText(panel, -1, u"请选择串口",(10, 190),size = (80,20))
          self.seriComboBox = wx.ComboBox(panel, -1, value="COM1", pos=(100, 185), size=(100,30), choices=self.seriList, style=wx.CB_READONLY)
          self.Bind(wx.EVT_COMBOBOX, self.OnSeleSeri, self.seriComboBox)

          self.upgButton = wx.Button(panel, -1, u"升级版本\n写硬件参数", pos=(70, 225),size = (140,100))
          self.Bind(wx.EVT_BUTTON, self.OnUpg, self.upgButton)

          self.lable7 = wx.StaticText(panel, -1, u"",(10, 345),size = (250,20))

          self.loadBinName()
          self.text.SetFocus()
          # create a pubsub receiver 
          pub.subscribe(self.updateDisplay, "update")
     
     def OnBinText(self, event):

        binName = self.binText.GetValue()
        
        file = open("w2c_ver.txt", "w+")
        file.truncate()
        file.write(binName)
        file.close
     
     def loadBinName(self):
        if os.path.exists(r'w2c_ver.txt') == True:
            file = open("w2c_ver.txt", "r")
            binName = file.readline()
            
            self.binText.SetValue(binName)
            
            file.close
     
     def updateDisplay(self, data, extra1, extra2=None):
          if (1 == data):
               self.upgButton.SetBackgroundColour("Red")
               self.upgButton.Enable()
               self.seriComboBox.Enable()
               #self.binText.Enable()
               #self.text.SetValue("")
               self.text.SetFocus()
          if 100 == data:
               self.upgButton.SetBackgroundColour("Green")
               self.upgButton.Enable()
               self.seriComboBox.Enable()
               #self.binText.Enable()
               self.text.SetValue("")
               self.text.SetFocus()
          if 0 == data:
               self.upgButton.SetBackgroundColour("White")
               self.upgButton.Disable()
               self.seriComboBox.Disable()
               #self.binText.Disable()
          self.lable7.SetLabel("%s"%extra1)
     
     def OnSeleSeri(self,event):
          
          self.serialPort = self.seriComboBox.GetValue()
          print self.serialPort

          #self.label2.SetLabel("")
          #self.formatButton.SetBackgroundColour("White")
          colourPara = self.corlourList[self.seriList.index(self.serialPort)]
          #print "%d %d %d"%(colourPara[1][0], colourPara[1][1], colourPara[1][2])
          bckColour = wx.Colour(colourPara[1][0], colourPara[1][1], colourPara[1][2])
          self.SetBackgroundColour(bckColour)
          self.Refresh()

     def OnText(self, event):
          if len(self.text.GetValue()) > 0:
               self.upgButton.SetBackgroundColour("White")
               self.upgButton.Enable()
               self.seriComboBox.Enable()
               self.lable7.SetLabel("")
          return



     def OnUpg(self, event):
          line = unicodedata.normalize('NFKD',self.text.GetValue()).encode('ascii','ignore')
          hw_infos = filter(passed, line.split('\n'))
          if len(hw_infos) < 5:
               self.lable7.SetLabel(u"扫描的参数不全，请检查")
               self.upgButton.SetBackgroundColour("Red")
               return False
          
          binName = self.binText.GetValue()
          ver = ""
          if len(binName) == 0:
               self.lable7.SetLabel(u"请输入升级包版本号,例如:3.1.1")
               self.upgButton.SetBackgroundColour("Red")
               return False
          else:
               verLine = str(binName).split('.')
               if len(verLine) != 3:
                    self.lable7.SetLabel(u"版本号格式不对，必须类似于:3.1.1")
                    self.upgButton.SetBackgroundColour("Red")
                    return False

               if verLine[0].isdigit() == False or verLine[1].isdigit() == False or verLine[2].isdigit() == False:
                    self.lable7.SetLabel(u"版本号格式不对，必须都是数字")
                    self.upgButton.SetBackgroundColour("Red")
                    return False

               ver = verLine[0] + verLine[1] +verLine[2]
          
          
                           
          TestThread(self.serialPort, hw_infos[1], hw_infos[2], hw_infos[3], hw_infos[4], ver)
     

if __name__ == '__main__':
     app = wx.App()
     frame = TestFrame(None)
     frame.Show(True)
     app.MainLoop()
    
