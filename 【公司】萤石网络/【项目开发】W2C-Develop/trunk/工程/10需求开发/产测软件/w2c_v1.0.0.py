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
import telnetlib
import unicodedata
import serial
import thread
import datetime
import calendar

from threading import Thread 
from wx.lib.pubsub import setupkwargs
from wx.lib.pubsub import pub

def passed(item):  
    try: 
        return len(item) != 0 
    except ValueError:  
        return False  


class TestFrame(wx.Frame):
    
    # 测试持续时间
    TestTime = 6

    # rtc时间检查和补测时间检查偏差值
    rtcTimeOffset = 120
    timeCheckOffset = 5

    #wifi客户端的ip跟mac地址
    wifi2gClientMac = "xx:xx:xx:xx:xx:xx"
    wifi2gClientIp = "192.168.88.109"

    
    #设置wifi的SSID,测试过程中若想修改，只需修改routerSSID变量的值即可。
    router2gSSID = "ezviz_016234"
    router5gSSID = "ezviz_016234_5G"

    #测试目标ip
    wifi2gDstIp = "192.168.88.109"
    
    #测试的门限
    wifiTestLimit = 65
    rssi2glimit = 60

    
    #路由器lan ip地址
    routerHostIp = "192.168.7.1"
    
    #路由器网页登录的用户名跟密码
    routerUserName = "admin"
    routerPassword = "11111111"
    routerAuth = ""

    #支持的设备列表以及各个设备支持的参数,如果需要增加设备类型及参数只需修改这两个数组
    typeList = ['CS-W2C']
    typePara = [
                ['CS-W2C',['web', 'sn', 'check', 'hw type', 'hw mac']],
                ]
    
    #当前选中的设备类型
    hwType = ""

    paraLabels = ['lable1', 'lable2', 'lable3', 'lable4', 'lable5', 'lable6', 'lable7', 'lable8', 'lable9', 'lable10', 'lable11']

    def __init__(self, parent):
        wx.Frame.__init__(self, parent, -1, "W2C Test",
                size=(900, 500))
        self.Center()

        backcorlour = wx.Colour(235, 241, 222)
        self.SetBackgroundColour(backcorlour)
        
        
        panel = wx.Panel(self, -1)

        #wx.StaticText(panel, -1, "selece type",(5, 5),size = (65,20))
        self.lable1 = wx.StaticText(panel, -1, "",(10, 28),size = (60,20))
        self.lable2 = wx.StaticText(panel, -1, "",(10, 46),size = (60,20))
        self.lable3 = wx.StaticText(panel, -1, "",(10, 64),size = (60,20))
        self.lable4 = wx.StaticText(panel, -1, "",(10, 82),size = (60,20))
        self.lable5 = wx.StaticText(panel, -1, "",(10, 100),size = (60,20))
        self.lable6 = wx.StaticText(panel, -1, "",(10, 118),size = (60,20))
        self.lable7 = wx.StaticText(panel, -1, "",(10, 136),size = (60,20))
        self.lable8 = wx.StaticText(panel, -1, "",(10, 154),size = (60,20))
        self.lable9 = wx.StaticText(panel, -1, "",(10, 172),size = (60,20))
        self.lable10 = wx.StaticText(panel, -1, "",(10, 190),size = (60,20))
        self.lable11 = wx.StaticText(panel, -1, "",(10, 208),size = (60,20))
        
        self.text = wx.TextCtrl(panel,-1,value='',pos=(75,30),size=(220,215),style=wx.TE_MULTILINE)
        self.text.Bind(wx.EVT_TEXT, self.OnText, self.text)
        
        self.allButton = wx.Button(panel, -1, "Run All", pos=(400, 30),size = (80,80))
        self.Bind(wx.EVT_BUTTON, self.OnAll, self.allButton)

        self.paraButton = wx.Button(panel, -1, "check para", pos=(400, 130),size = (80,80))
        self.Bind(wx.EVT_BUTTON, self.OnPara, self.paraButton)

        self.wifi2gButton = wx.Button(panel, -1, "Wifi", pos=(510, 130),size = (80,80))
        self.Bind(wx.EVT_BUTTON, self.OnWifi2g, self.wifi2gButton)

        self.rssi2gButton = wx.Button(panel, -1, "RSSI", pos=(620, 130),size = (80,80))
        self.Bind(wx.EVT_BUTTON, self.OnRssi2g, self.rssi2gButton)
        
        self.finishTest = wx.Button(panel, -1, "Finish", pos=(730, 130),size = (80,80))
        self.Bind(wx.EVT_BUTTON, self.OnFinish, self.finishTest)

        #网卡mac输入框
        wx.StaticText(panel, -1, "RSSI MAC:",(10, 310),size = (90,20))
        self.macText = wx.TextCtrl(panel,-1,value='',pos=(100,310),size=(180,20),style=wx.TE_LEFT)
        self.macText.Bind(wx.EVT_TEXT, self.OnMacText, self.macText)

        self.text.SetFocus()
        self.loadDataMac()
        self.DoLoadType()

        # create a pubsub receiver 
        pub.subscribe(self.updateDisplay, "update")

    def WriteLog(self,logvalue):
        logfilehandle = None
        line = unicodedata.normalize('NFKD',self.text.GetValue()).encode('ascii','ignore')
        hw_infos = filter(passed, line.split('\n'))
        if len(hw_infos) < 2:
            return

        filename = hw_infos[1]
        
        if len(filename) > 0 :
            log_date=time.strftime('%Y%m%d')
            if os.path.exists("log/%s"%log_date) == False:
                os.makedirs("log/%s"%log_date);

            logfilehandle = open("log/%s/%s"%(log_date,filename),'a')
            #logfilehandle = open(filename,'a')
        
        if logfilehandle != None:
            logfilehandle.write(logvalue)
            logfilehandle.close()

    def loadDataMac(self):
        if os.path.exists(r'w2c_rssimac.txt') == True:
            file1 = open("w2c_rssimac.txt", "r")
            macStr = file1.readline()
            
            self.macText.SetValue(macStr)
            
            file1.close
            
        return
            

    def OnMacText(self, event):

        macStr = self.macText.GetValue()
        
        file = open("w2c_rssimac.txt", "w+")
        file.truncate()
        file.write(macStr)
        file.close

    def OnText(self, event):
        if len(self.text.GetValue()) > 0:
            self.paraButton.SetBackgroundColour("White")
            self.wifi2gButton.SetBackgroundColour("White")
            self.rssi2gButton.SetBackgroundColour("White")
            self.finishTest.SetBackgroundColour("White")

            line = unicodedata.normalize('NFKD',self.text.GetValue()).encode('ascii','ignore')
            hw_infos = filter(passed, line.split('\n'))
            if len(hw_infos) >= 4:
                tmptype = hw_infos[3]
                first = tmptype.find("/")
                if first > 0:
                    type = tmptype[:first]
                else:
                    type = tmptype
                print type
                for hwtype in self.typeList:
                    if type == hwtype:
                        self.DoLoadTextlabel(self.typePara[self.typeList.index(type)][1], 0)
                        self.hwType = hwtype
                        file = open("hwtype.txt", "w+")
                        file.truncate()
                        file.write(type)
                        file.close
                        return


            return

    def updateDisplay(self, data, extra1, extra2=None):

        if (0 == data):
            exec('self.%s.SetBackgroundColour("Green")'%extra1)
        if 100 == data:
            self.EnableAllBtn()
            #self.text.SetValue("")
            #self.text.SetFocus()
        
    def postMsg(self, code, msg): 
          pub.sendMessage("update", data=code, extra1='%s'%msg)

    def DisableAllBtn(self):
        self.paraButton.SetBackgroundColour("White")
        self.wifi2gButton.SetBackgroundColour("White")
        self.rssi2gButton.SetBackgroundColour("White")
        self.finishTest.SetBackgroundColour("White")

        self.allButton.Disable()
        self.text.Disable()
        self.macText.Disable()
        self.paraButton.Disable()
        self.wifi2gButton.Disable()
        self.rssi2gButton.Disable()
        self.finishTest.Disable()

    def EnableAllBtn(self):
        self.text.Enable()
        self.macText.Enable()
        self.paraButton.Enable()
        self.wifi2gButton.Enable()
        self.rssi2gButton.Enable()
        self.finishTest.Enable()
        self.allButton.Enable()
            
    def AllRun(self, event):
        
        self.DisableAllBtn()
        self.ClearArp()

        if self.OnPara(event) == False:
            self.postMsg(100, '')
            return
        else:
            self.postMsg(0, 'paraButton')
        
        time.sleep(1)
        if self.OnWifi2g(event) == False:
            self.postMsg(100, '')
            return
        else:
            self.postMsg(0, 'wifi2gButton')
            
        time.sleep(1)
        if self.OnRssi2g(event) == False:
            self.postMsg(100, '')
            return
        else:
            self.postMsg(0, 'rssi2gButton')
        
        if self.OnFinish(event) == False:
            self.postMsg(100, '')
            return
        else:
            self.postMsg(0, 'finishTest')
        
        self.text.SetValue("")
        self.text.SetFocus()
        self.postMsg(100, '')
        
        #wx.MessageBox("All Test Succeed", "OK", wx.ICON_QUESTION)

    def OnAll(self, event):
        thread.start_new_thread(self.AllRun, (event, ))

    def DoLoadTextlabel(self, list, firstIndex):

        for i in range(len(self.paraLabels)):
            exec('self.%s.SetLabel("")'%self.paraLabels[i])
              
        num = len(list)
        print num

        index = firstIndex
        i = 0
        while index < num:
            if len(list[index]) > 0:
                #print '%s %s %s'%(index, i, list[index])
                if list[index][-1] == '\n':
                    exec('self.%s.SetLabel("%s")'%(self.paraLabels[i], list[index][:-1]))
                else:
                    exec('self.%s.SetLabel("%s")'%(self.paraLabels[i], list[index]))
                index += 1
                i += 1
            else:
                break

        return
    

    def DoLoadType(self):
        if os.path.exists(r'hwtype.txt') == True:
            file = open("hwtype.txt", "r")
            line = file.readline()
            for hwtype in self.typeList:
                if line == hwtype:
                    self.DoLoadTextlabel(self.typePara[self.typeList.index(hwtype)][1], 0)
                    return
        
        self.DoLoadTextlabel(self.typePara[self.typeList.index('CS-W2C')][1], 0)
                    
        return

    def IsDigit(self,x):
        try:
            x=int(x)
            return isinstance(x,int)
        except ValueError:
            return False

    def strIsDate(self, dateDtr):
        try:
            time.strptime(dateDtr, "%Y-%m-%d")
            return True
        except:
             return False

    def TestWifi1(self,dst,limit):
        cmd = ""
        print sys.platform
        if sys.platform == "win32":
            cmd = "iperf3.exe -c %s -t %d -P 8 -f m"%(dst,self.TestTime)
        else:
            cmd = "iperf3 -c %s -t %d -P 8 -f m"%(dst,self.TestTime)
        print cmd

        count = 3
        while count > 0:
            data = subprocess.Popen(cmd,stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True)
            li = data.stdout.readline()
            while li:
                print li
                self.WriteLog(li)
                first = li.find("Mbits/sec")
                
                sumpos = li.find("SUM")
                if sumpos < 0:
                    li = data.stdout.readline()
                    continue
                    
                roler = li.find("receiver")
                print roler
                if roler < 0:
                    li = data.stdout.readline()
                    continue

                if first > 0:
                    rate = string.atof(li[first-6:first])
                    if rate >= limit:
                        return True
                    else:
                        if count > 1:
                            count -= 1
                            break;
                        else:
                            result = u"wifi下载速度测试失败:%d"%rate
                            wx.MessageBox(result, "Error", wx.ICON_QUESTION)
                            return False
                        
                li = data.stdout.readline()
                
    def TestWifi2(self,dst,limit):
        cmd = ""
        print sys.platform
        if sys.platform == "win32":
            cmd = "iperf3.exe -c %s -t %d -R -P 8 -f m"%(dst,self.TestTime)
        else:
            cmd = "iperf3 -c %s -t %d -R -P 8 -f m"%(dst,self.TestTime)
        print cmd

        count = 3
        while count > 0:
            data = subprocess.Popen(cmd,stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True)
            li = data.stdout.readline()
            while li:
                print li
                self.WriteLog(li)
                first = li.find("Mbits/sec")
                
                sumpos = li.find("SUM")
                if sumpos < 0:
                    li = data.stdout.readline()
                    continue
                    
                roler = li.find("sender")
                print roler
                if roler < 0:
                    li = data.stdout.readline()
                    continue

                if first > 0:
                    rate = string.atof(li[first-6:first])
                    if rate >= limit:
                        return True
                    else:
                        if count > 1:
                            count -= 1
                            break;
                        else:
                            result = u"wifi上传速度测试失败:%d"%rate
                            wx.MessageBox(result, "Error", wx.ICON_QUESTION)
                            return False
                        
                li = data.stdout.readline()
                
                
    def TestWifi(self,dst,limit):
        if self.TestWifi1(dst,limit) == False:
            return False
        
        if self.TestWifi2(dst,limit) == False:
            return False
            
        return True
                

    def OnWifi2g(self, event): 
        self.WriteLog("\n\n Test 2g Wifi: \n")
        if self.TestWifi(self.wifi2gDstIp,self.wifiTestLimit):
            self.wifi2gButton.SetBackgroundColour("Green")
            return True

        self.wifi2gButton.SetBackgroundColour("Red")
        return False


    def checkTelnet(self):

        telnetOpen = True
        
        try:
            tn = telnetlib.Telnet(self.routerHostIp, port=23, timeout=10)        
        except Exception, e:
            telnetOpen = False

        return telnetOpen


    def allParaCheck(self, event):
        self.WriteLog("\n\n allParaCheck: \n")
        if self.OnLogin() != True:
            self.diskButton.SetBackgroundColour("Red")
            self.rssi2gButton.SetBackgroundColour("Red")
            self.rssi5gButton.SetBackgroundColour("Red")
            self.checkVerHwButton.SetBackgroundColour("Red")
            return False

        httpClient = None
        ret = False

        try:
            headers = {"Content-type": "application/json",
            "Accept": "application/json", "Authorization":self.routerAuth}
            httpClient = httplib.HTTPConnection(self.routerHostIp, 80, timeout=30)
            httpClient.request('GET', '/get_para_factory.json',None, headers)
            response = httpClient.getresponse()
            #print response.read()
            if response.status != 200 :
                ret = False
            else:
                jsonret = json.loads(response.read(), encoding="utf-8")
                retcode = jsonret["status"]["ret_code"]
                if retcode != 0 :
                    print jsonret["status"]
                    ret = False
                else:
                    result = jsonret["result"]
                    ret = self.paraParse(result)
			
        except Exception, e:
            ret = False;
            print e
        finally:
            if httpClient:
                httpClient.close()

        return ret
            
    
    def OnFinish(self, event):
        try:
            self.OnHttpRestore(self.routerHostIp)
            self.finishTest.SetBackgroundColour("Green")
            return True
        except:
            self.finishTest.SetBackgroundColour("Red")
            return False

    def macCompare(self, mac1, mac2):
        for i in range(len(mac1)):
            if mac1[i].isalpha() and mac1[i].islower():
                mac1 = mac1[:i] + mac1[i].upper() + mac1[i+1:]

        for i in range(len(mac2)):
            if mac2[i].isalpha() and mac2[i].islower():
                mac2 = mac2[:i] + mac2[i].upper() + mac2[i+1:]

        print mac1
        print mac2
        if mac1 in mac2:
            return True
        else:
            return False
    
    def DoRssi(self, clientMac, rssilimit):  
       
        self.WriteLog("\n\n test RSSI: \n")
        if self.TestNetPro(self.routerHostIp) == False:
            wx.MessageBox(u"ping不通w2c设备", "Error", wx.ICON_QUESTION)
            return 1

        httpClient = None
        ret = 5
		
        try:
            #params = {}
            headers = {"Content-type": "application/json",
            "Accept": "application/json", "Authorization":self.routerAuth}
            httpClient = httplib.HTTPConnection(self.routerHostIp, 80, timeout=30)
            httpClient.request('GET', '/get_para_factory.json','',headers)
            response = httpClient.getresponse()
            print response
            if response.status != 200 :
                ret = 1
            else:
                jsonret = json.loads(response.read(), encoding="utf-8")
                retcode = jsonret["status"]["ret_code"]
                if retcode != 0 :
                    print jsonret["status"]
                    ret = 1
                else:
                    staList = jsonret["result"]["sta_list"]
                    for sta in staList:
                        if self.macCompare(sta["sta_mac"], clientMac) == True:
                            print 'rssi1:%d, rssi2:%d'%(sta["rssi1"], sta["rssi2"])
                            self.WriteLog('rssi1:%d, rssi2:%d'%(sta["rssi1"], sta["rssi2"]))
                            if sta["rssi1"] < rssilimit:
                                ret = 0
                            else:
                                ret = 3
                                
                            break;
        except Exception, e:
            ret = 1;
            print e
        finally:
            if httpClient:
                httpClient.close()
                
        #if ret == 0:
            #self.rssiButton.SetBackgroundColour("Green")

        #print ret
        return ret

    def OnRssi2g(self,event):  
        self.WriteLog("\n\n Test wifi 2g: \n")
        ret = 0

        macAddr = str(self.macText.GetValue())
        if self.strIsMac(macAddr) == False:
            wx.MessageBox(u"2g rssi mac地址有误，请输入正确的地址","Error", wx.ICON_QUESTION)
            self.rssi2gButton.SetBackgroundColour("Red")
            self.WriteLog("test 2g RSSI failed bad address")
            return False
        '''
        for i in range(len(macAddr)):
            if macAddr[i].isalpha() and macAddr[i].islower() == False:
                macAddr = macAddr[:i] + macAddr[i].lower() + macAddr[i+1:]
        '''
        
        self.wifi2gClientMac = macAddr
        
        if self.OnLogin() != True:
            return False
        
        for i in range(1,5):
            
            self.TestNet(self.wifi2gDstIp)
            ret = self.DoRssi(self.wifi2gClientMac, self.rssi2glimit)

            if ret == 0:
                self.rssi2gButton.SetBackgroundColour("Green")
                return True
            else:
                time.sleep(2)
        
        if ret == 1:
            wx.MessageBox(u"网络不通","Error", wx.ICON_QUESTION)    
        elif ret == 2:
            wx.MessageBox(u"2g rssi测试登陆失败","Error", wx.ICON_QUESTION)
        elif ret == 3:
            wx.MessageBox(u"2g rssi检查失败","Error", wx.ICON_QUESTION)
        #elif ret == 4:
        #    wx.MessageBox(u"2g rssi检查失败，ANT1天线有问题","Error", wx.ICON_QUESTION)  
        elif ret == 5:
            wx.MessageBox(u"没有找到此终端的rssi信息，请检查输入的2g rssi mac是否正确!", "Error", wx.ICON_QUESTION)
            
        self.rssi2gButton.SetBackgroundColour("Red")
        return False


    def charIsHex(self, c):
        if (c >= '0' and c <= '9') or (c >= 'a' and c <= 'f') or (c >= 'A' and c <= 'F'):
            return True
        else:
            return False
            
    def strIsMac(self, macStr):
        macLen = len('11:22:33:44:55:66')
        if len(macStr) != macLen:
            return False

        if macStr == '00:00:00:00:00:00':
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
        line = unicodedata.normalize('NFKD',self.text.GetValue()).encode('ascii','ignore')
        hw_infos = filter(passed, line.split('\n'))
        if len(hw_infos) < 5:
            wx.MessageBox(u"扫描的信息不全", "Error", wx.ICON_QUESTION)
            return False
         
        hik_hw_sncode=hw_infos[1]
        hik_hw_checkcode=hw_infos[2]
        #hik_hw_type=hw_infos[3]
        hik_hw_mac=hw_infos[4]

        if len(hik_hw_sncode) != 9 or hik_hw_sncode.isdigit() != True:
            wx.MessageBox(u"扫描的序列号有问题，必须为9位数纯数字", "Error", wx.ICON_QUESTION)
            return False

        
        if len(hik_hw_checkcode) != 6 or hik_hw_checkcode.isalpha() == False or hik_hw_checkcode.isupper() == False:
            wx.MessageBox(u"扫描的验证码有问题，必须为6位数纯大写字母", "Error", wx.ICON_QUESTION)
            return False

        tmp_hw_type=hw_infos[3]
        
        first = tmp_hw_type.find("/")
        if first > 0:
            hik_hw_type = tmp_hw_type[:first]
        else:
            hik_hw_type = tmp_hw_type
        typeRight = False
        for hwType in self.typeList:
            if hwType == hik_hw_type:
                typeRight = True
                break
        
        if typeRight == False:
            wx.MessageBox(u"扫描的设备类型有问题", "Error", wx.ICON_QUESTION)
            return False

        if self.strIsMac(hik_hw_mac) == False:
            wx.MessageBox(u"扫描的设备mac有问题", "Error", wx.ICON_QUESTION)
            return False

        return True

    def OnPara(self, event):

        self.WriteLog("Check para: \n")

        if self.OnLogin() != True:
            wx.MessageBox(u"登陆失败！", "Error", wx.ICON_QUESTION)
            self.paraButton.SetBackgroundColour("Red")
            return False

        if self.doCheckPara() == False:
            wx.MessageBox(u"扫描参数检查失败", "Error", wx.ICON_QUESTION)
            self.paraButton.SetBackgroundColour("Red")
            return False
        elif self.checkDevPara(event) == False:
            self.paraButton.SetBackgroundColour("Red")
            return False
        else:
            self.WriteLog("para check success")
            self.paraButton.SetBackgroundColour("Green")
            return True
        
    
    def ClearArp(self):
        cmd = ""
        if sys.platform == "win32":
            cmd = "arp -d %s"%self.routerHostIp
            #192.168.7.1"
        else:
            cmd = "ping -d %s"%self.routerHostIp
        
        print cmd
        data = os.system(cmd)
        print "GW arp remove"
        print data
        
        return False
    
    def TestNet(self,dst):
        cmd = ""
        if sys.platform == "win32":
            cmd = "ping -n 30 %s"%dst
        else:
            cmd = "ping -c 30 %s"%dst
        
        data = subprocess.Popen(cmd,stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True)
        li = data.stdout.readline()
        while li:
            print li
            self.WriteLog(li)
            first = li.find("ttl")
            last = li.find("TTL")
            if first > 0 or last > 0:
                return True
            li = data.stdout.readline()
        
        return False
        
    def TestNetPro(self,dst):
        cmd = ""
        if sys.platform == "win32":
            cmd = "ping -n 30 %s"%dst
        else:
            cmd = "ping -c 30 %s"%dst
        
        data = subprocess.Popen(cmd,stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True)
        li = data.stdout.readline()
        while li:
            print li
            self.WriteLog(li)
            first = li.find("ttl")
            last = li.find("TTL")
            print first
            print last
            if first > 0:
                ttl = string.atoi(li[first+4:])
                print ttl
                if ttl == 64:
                    return True
            elif last > 0:
                ttl = string.atoi(li[last+4:])
                print ttl
                if ttl == 64:
                    return True
            li = data.stdout.readline()
        
        return False

    def handleHttpResponse(self,response):
        jsonret = json.loads(response, encoding="utf-8")
        retcode = jsonret["status"]["ret_code"]
        if retcode == 0 :
            print response
            return True
        else:
            return False
    
    def OnLogin(self):
        if len(self.routerPassword) == 0 :
            return True
            
        httpClient = None
        success = False
        try:
            params = urllib.urlencode({'username': self.routerUserName, 'userpwd': self.routerPassword})
            headers = {"Content-type": "application/json",
            "Accept": "application/json",}
            httpClient = httplib.HTTPConnection(self.routerHostIp, 80, timeout=30)
            httpClient.request('POST', '/login.cgi',params,headers)
            response = httpClient.getresponse()
            #print response
            if response.status != 200 :
                success = False
            else:
                jsonret = json.loads(response.read(), encoding="utf-8")
                retcode = jsonret["status"]["ret_code"]
                if retcode != 0 :
                    print jsonret["status"]
                    success = False
                else:
                    #print jsonret["result"]
                    self.routerAuth = jsonret["result"]["auth"];
                    print self.routerAuth
                    success = True

        except Exception, e:
            success = False;
            print e
        finally:
            if httpClient:
                httpClient.close()

        if success == False :
             wx.MessageBox(u"登陆失败，请检查是否没有设置密码或者密码不正确", "Error", wx.ICON_QUESTION)

        return success


    def OnHttpRestore(self,event):        
        
        for i in range(10):
            if self.OnLogin() != True:
                return False
            
        httpClient = None
        success = False
        try:
            headers = {"Content-type": "application/json",
            "Accept": "application/json","Authorization":self.routerAuth}
            httpClient = httplib.HTTPConnection(self.routerHostIp, 80, timeout=30)
            httpClient.request('POST', '/restore_def_para_factory.cgi',None,headers)
            response = httpClient.getresponse()
            print response
            if response.status != 200 :
                success = False
            else:
                success = self.handleHttpResponse(response.read())

        except Exception, e:
            success = False;
            print e
        finally:
            if httpClient:
                httpClient.close()

        if success == False :
            self.WriteLog("restore default failed")
            wx.MessageBox(u"恢复出厂设置失败", "Error", wx.ICON_QUESTION)
        else:
            self.WriteLog("restore default success")
            #wx.MessageBox("succeed to restore factory", "Info", wx.ICON_QUESTION)
        return success

    def checkDevPara(self, event):  

        self.WriteLog("Check dev para: \n")
        if self.TestNetPro(self.routerHostIp) == False:
            wx.MessageBox(u"ping不通w2c设备", "Error", wx.ICON_QUESTION)
            return 1

        

        snCode = ""
        checkCode = ""
        eth0_mac = ""
        httpClient = None
        ret = False
        try:
            #params = {}
            headers = {"Content-type": "application/json",
            "Accept": "application/json", "Authorization":self.routerAuth}
            httpClient = httplib.HTTPConnection(self.routerHostIp, 80, timeout=30)
            httpClient.request('GET', '/get_para_factory.json','',headers)
            response = httpClient.getresponse()
            print response
            if response.status != 200 :
                ret = False
            else:
                jsonret = json.loads(response.read(), encoding="utf-8")
                retcode = jsonret["status"]["ret_code"]
                if retcode != 0 :
                    print jsonret["status"]
                    ret = False
                else:
                    para = jsonret["result"]["para"]
                    snCode = para["sncode"]
                    checkCode = para["checkcode"]
                    eth0_mac = para["wan_mac"]
                    ret = True
        except Exception, e:
            ret = False;
            print e
        finally:
            if httpClient:
                httpClient.close()
                
        print "dev para: %s %s %s\n"%(snCode, checkCode, eth0_mac)
        self.WriteLog("dev para: %s %s %s\n"%(snCode, checkCode, eth0_mac))

        line = unicodedata.normalize('NFKD',self.text.GetValue()).encode('ascii','ignore')
        hw_infos = filter(passed, line.split('\n'))
        if len(hw_infos) < 5:
            wx.MessageBox(u"扫描的信息不全", "Error", wx.ICON_QUESTION)
            return False
         
        hik_hw_sncode=hw_infos[1]
        hik_hw_checkcode=hw_infos[2]
        hik_hw_mac=hw_infos[4]
        if True == ret:
            if snCode not in hik_hw_sncode or checkCode not in hik_hw_checkcode or self.macCompare(eth0_mac, hik_hw_mac) == False:
                self.WriteLog("dev para failed: %s %s %s %s %s %s\n"%(snCode, checkCode, eth0_mac, hik_hw_sncode, hik_hw_checkcode, hik_hw_mac))
                wx.MessageBox(u"设备参数检查失败\n扫描参数为:%s %s %s\n设备参数为:%s %s %s"%( hik_hw_sncode, hik_hw_checkcode, hik_hw_mac,snCode, checkCode, eth0_mac), "Error", wx.ICON_QUESTION)
                ret = False
        else:
            self.WriteLog("get device para fialed\n")
            wx.MessageBox(u"获取设备参数失败", "Error", wx.ICON_QUESTION)
        
        return ret


if __name__ == '__main__':
    app = wx.App()
    frame = TestFrame(None)
    frame.Show(True)
    app.MainLoop()




