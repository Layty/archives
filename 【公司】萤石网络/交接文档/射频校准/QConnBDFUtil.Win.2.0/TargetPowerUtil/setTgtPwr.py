############################################
# Description: convert target power txt file to board data text file
# Usage: python setTgtPwr.py <input.csv> <bdata.txt>
############################################

#-------------------------------------------
import sys,re,os
#-------------------------------------------
tgtPwrDict2G = {}
tgtPwrDict5G = {}
tgtpwrNewDict = {}

minTgtPwr2G = {}
minTgtPwr5G = {}
noOfChains = 0
chains = [0,1,2,3]
#-------------------------------------------
def pier(freq):
    # Convert Frequency no to Pier number in BD file
    freq = int(freq)
    if freq < 2500:
        ret = 100 + freq%100
    else:
        ret = 40+(freq-5000)/5
    
    return str(ret)

#-------------------------------------------
def getNoOfChains(bdFileStr):
    searchStr = 'targetPower2GVHT20Base_G_0_0 (.*)'
    m = re.search(searchStr,bdFileStr)
    if m:
        return re.subn('(\d+)','',m.group(1))[1]
    
#-------------------------------------------
def parseTgtPwr(tgtPwrStr):
# Parse Target Power file and feed to Dictionary
    tgtLineArr = re.split('\n',tgtPwrStr)
    
    temp =  re.split(',',tgtLineArr[0])
    freqArr = temp[1:len(temp)]
    
    for idx in range(1,len(tgtLineArr)):
        t = tgtLineArr[idx]
        temp =  re.split(',',t)
        if temp[0]:
            mode,rate = re.split(' ',temp[0])
            for f in freqArr:
                if int(f) < 2500 :
                    if mode in minTgtPwr2G:
                        minTgtPwr2G[mode][f] = 99
                    else:
                        minTgtPwr2G[mode] = {}
                        minTgtPwr2G[mode][f] = 99
                else:
                    if mode in minTgtPwr5G:
                        minTgtPwr5G[mode][f] = 99
                    else:
                        minTgtPwr5G[mode] = {}
                        minTgtPwr5G[mode][f] = 99
                
            if not mode in tgtPwrDict2G:
                tgtPwrDict2G[mode] = {}
            if not rate in tgtPwrDict2G[mode]:
                tgtPwrDict2G[mode][rate] = {} 
                
            if not mode in tgtPwrDict5G:
                tgtPwrDict5G[mode] = {}
            if not rate in tgtPwrDict5G[mode]:
                tgtPwrDict5G[mode][rate] = {}
                
            pwrArr = temp[1:len(temp)]
            
            for pIdx in range(0,len(pwrArr)):
                #print mode,rate,freqArr[pIdx]
                if int(freqArr[pIdx]) < 2500:
                    if mode not in('VHT80', 'VHT160'):
						if not pwrArr[pIdx] in (' NA'):
							tgtPwrDict2G[mode][rate][freqArr[pIdx]] =  pwrArr[pIdx]                      
							if float(pwrArr[pIdx]) < minTgtPwr2G[mode][freqArr[pIdx]] :
								minTgtPwr2G[mode][freqArr[pIdx]]  = float(pwrArr[pIdx])
                else:
                    if mode not in ('11b'):
                        tgtPwrDict5G[mode][rate][freqArr[pIdx]] =  pwrArr[pIdx]
                        if float(pwrArr[pIdx]) < minTgtPwr5G[mode][freqArr[pIdx]] :
                            minTgtPwr5G[mode][freqArr[pIdx]]  = float(pwrArr[pIdx]) 

  
#-------------------------------------------
def tgtPwr2BDfile(bdFileStr):
    print "Parsing 11b/11g 2G.."
    bdFileStr = tgtPwr2BDfileLegacy(bdFileStr,'2G') 
    print "Parsing 11a 5G .."
    bdFileStr = tgtPwr2BDfileLegacy(bdFileStr,'5G')
    print "Parsing 11n/11ac 2G .."
    bdFileStr = tgtPwr2BDfileAC(bdFileStr,'2G')
    print "Parsing 11n/11ac 5G .."
    bdFileStr = tgtPwr2BDfileAC(bdFileStr,'5G')
    return bdFileStr

#-------------------------------------------
def tgtPwr2BDfileAC(bdFileStr,band):
# Edit board data file based on target power dictionary for 11N/11AC
    extraBit = ''
    extrBitCount = 0
    extraBitStr = ''
    freqSearchStr = 'targetFreqbin%s%s_%s_0_0 '
    tgtpwrBaseStr = 'targetPower%s%sBase_%s_%d_0 '
    tgtpwrDeltaStr = 'targetPower%s%sDelta_%s_%d_0 '
    modeArr = ['VHT20','VHT40','VHT80','VHT160']
    nacRate = ['MCS0','MCS1-2','MCS3-4',
               'MCS5','MCS6','MCS7','MCS8','MCS9',
               'MCS15','MCS16','MCS17','MCS18','MCS19',
               'MCS25','MCS26','MCS27','MCS28','MCS29',
               'MCS35','MCS36','MCS37','MCS38','MCS39',
               'NA'
               ]
    
    # Set Band
    if band == '2G':
        b = 'G'
        tgtPwrDict = tgtPwrDict2G
    else:
        b = 'A'
        tgtPwrDict = tgtPwrDict5G
    
    # Do for three modes - VHT20/40/80
    for idx in [0,1,2]: 
        mode = modeArr[idx]
        searchStr = freqSearchStr %(band,modeArr[idx],b)
        
        # Get Frequency Array from dictionary and get the minimum target power
        for r in tgtPwrDict[mode]:
            freqArr = sorted(tgtPwrDict[mode][r].keys())
            if b=='G':
                if not mode == 'VHT80':
                    freqArr = [freqArr[0],freqArr[1],freqArr[len(freqArr)-1]]                   
                    minPwrDict = minTgtPwr2G                    
            else:
                if not mode == '11b':
                    freqArr = freqArr
                    minPwrDict = minTgtPwr5G
                   
        # Edit the frequency piers
        subStr = ''
        for freq in freqArr:
            f = pier(freq)
            subStr = subStr + f + ' '
        
        #print searchStr+subStr
        # bdFileStr = re.sub(searchStr+'.*' ,searchStr+subStr , bdFileStr)
        
        # Do for every frequency set the target powers
        for fIdx in range(0,len(freqArr)):
            nibble = []
            freq = freqArr[fIdx]
            searchStrBase = tgtpwrBaseStr %(band,modeArr[idx],b,fIdx)
            minPwr = minPwrDict[mode][freqArr[fIdx]]         
            
            subStr = ''            
            base = ''            
            
            for c in range(0,noOfChains):
                #print subStr
                subStr = '%s%d ' %(subStr,int(minPwr*2))
                #print subStr            
                bdFileStr = re.sub(searchStrBase+'.*' ,searchStrBase+subStr , bdFileStr)
            
            searchStr =  tgtpwrDeltaStr %(band,modeArr[idx],b,fIdx)
            subStr = ''
            
            if noOfChains == 2:
                rateLen = 14
            if noOfChains == 3:
                rateLen = 18
            else:
                rateLen = len(nacRate)
            
            for rIdx in range(0,rateLen):                
                #if b == 'A':
                    #if fIdx == 0:
                     #   Str='targetPower5GVHT20Base_A_0_0(.*\d)'
                     #   temp = re.findall(Str,bdFileStr)
                     #   new = re.split('\s+',temp[0])
                     #   minPwr = (int(temp[0][1]+temp[0][2])/2)                 
                                 
                if not rIdx == rateLen-1:    
                    if( float(tgtPwrDict[mode][nacRate[rIdx]][freq]) >= minPwr):
                        diff = int((float(tgtPwrDict[mode][nacRate[rIdx]][freq]) - minPwr)*2)                        
                                
                    else:
                        print 'BasePower for frequency '+ freq + ' in '+ mode + ' is '
                        print minPwr
                        print 'Target power entered for frequency '+ freq + ' in '+ mode + ' ' + nacRate[rIdx] +' is'
                        print float(tgtPwrDict[mode][nacRate[rIdx]][freq])
                        raise ValueError('The power values entered should be greater than the base power')
                        return bdFileStr
                    nibble.append(diff%16)
                else:
                    #diff = (int(tgtPwrDict[mode]['MCS5'][freq]) - minPwr)*2
                    #nibble.append(diff%16)
                    nibble.append(0)            
              
                if band == '2G': 
                    if not mode == 'VHT80':
                         extraBit = extraBit + str(diff/16)
                else:
                    if not mode == '11b':
                         extraBit = extraBit + str(diff/16)              
                
                #print searchStr+subStr                
                if rIdx%2 == 1:
                        subStr = '%s%s ' %(subStr,hex(nibble[rIdx]*16+nibble[rIdx-1]))                     
                 
            bdFileStr = re.sub(searchStr+'.*' ,searchStr+subStr , bdFileStr)    
    
    if band == '2G':
        searchStr = "extTPow2xDelta2G "
    else:
        searchStr = "extTPow2xDelta5G "
        
    m = re.search(searchStr+'(.*)',bdFileStr)
    m = re.findall('(\d+)',m.group(1))
    extrBitCount = len(m)*8
    
    for c in range(len(extraBit),extrBitCount,1):
          extraBit = extraBit + '0'
    
    for e in range(0,extrBitCount,8):
          binVal = extraBit[e+7]+extraBit[e+6]+extraBit[e+5]+extraBit[e+4] +\
                   extraBit[e+3]+extraBit[e+2]+extraBit[e+1]+extraBit[e]
          extraBitStr =  extraBitStr + str(int(binVal,2)) + ' '
    
    subStr = "%s%s " %(searchStr,extraBitStr)
    bdFileStr = re.sub(searchStr+'.*' ,subStr , bdFileStr)
    return bdFileStr
    
#-------------------------------------------
def tgtPwr2BDfileLegacy(bdFileStr,band):
    modeArr = ['11b','11ag']
    freqSearchStr = 'targetFreqbin%s_%s_0_0 '
    tgtpwrSearchStr = 'targetPower%s_%s_%d_0 '
    modeSearchArr2G = ['Cck','2G']
    modeSearchArr5G = [' ','5G']
    bRate = ['1Mbps','2Mbps','5.5Mbps','11Mbps']
    agRate = ['6-24Mbps','36Mbps','48Mbps','54Mbps']
    
    if band == '2G':
        b = 'G'
        tgtPwrDict = tgtPwrDict2G
        modeSearchArr = modeSearchArr2G
    else:
        b = 'A'
        tgtPwrDict = tgtPwrDict5G
        modeSearchArr = modeSearchArr5G
    
    for idx in [0,1]: 
        # Edit the frequency Piers    
        mode = modeArr[idx]
        searchStr = freqSearchStr %(modeSearchArr[idx],b)
        #print searchStr
        
        for r in tgtPwrDict[mode]:
        
            freqArr = sorted(tgtPwrDict[mode][r].keys())
            
            if mode == '11b':
                if b=='G':                    
                    freqArr = [freqArr[0],freqArr[len(freqArr)-1]]                    
            elif mode == '11ag':
                if b=='G':
                    freqArr = [freqArr[0],freqArr[len(freqArr)-2],freqArr[len(freqArr)-1]]
                else:
                    freqArr = freqArr
        
        subStr = ''
        for freq in freqArr:
            f = pier(freq)
            subStr = subStr + f + ' '
        
        # bdFileStr = re.sub(searchStr+'.*' ,searchStr+subStr , bdFileStr)
        
        # Edit the target powers
        searchStr = tgtpwrSearchStr[idx]
        for fIdx in range(0,len(freqArr)):
            searchStr = tgtpwrSearchStr %(modeSearchArr[idx],b,fIdx)
            subStr = ''
            for r in [0,1,2,3]:
                if mode == '11b':
                    if b == 'G':
                        subStr = '%s%d ' %(subStr,int(float(tgtPwrDict[mode][bRate[r]][freqArr[fIdx]])*2))                        
                    
                elif mode == '11ag':
                    subStr = '%s%d ' %(subStr,int(float(tgtPwrDict[mode][agRate[r]][freqArr[fIdx]])*2))
            #print searchStr+subStr
            bdFileStr = re.sub(searchStr+'.*' ,searchStr+subStr , bdFileStr)
        
    return bdFileStr
      
#-------------------------------------------
def parseTgtPwr_newScheme(csvStr):
    # convert CSV file to local dictionary
    for csvLine in re.split("\n",csvStr):
        if len(csvLine) > 0:
            csvCell = re.split("[,\n]",csvLine)
            if csvCell[0] == 'CCK':
                key = '%s_%s_%s' %(csvCell[0],csvCell[1],csvCell[2])
            else:
                key = '%s_%s_%s' %(csvCell[0],csvCell[1],csvCell[2][3])
            if key in tgtpwrNewDict:
                tgtpwrNewDict[key] = []
            tgtpwrNewDict[key] = csvCell[3:len(csvCell)]
            
    
#-------------------------------------------
def tgtPwr2BDfile_newScheme(bdFileStr):
    # Print target power dictionary to BD file
    # CCK
    for freqIdx in [0,1]:
        tag = 'targetPowerCck_2G_G_%d_0 ' %freqIdx
        searchStr = '%s(.*)' %tag
        line = ''
        for mode in ['1Mbps','2Mbps','5.5Mbps','11Mbps']:
            key = '%s_%s_%s' %('CCK','SU',mode)
            tgtpwr = float(tgtpwrNewDict[key][freqIdx])*2
            line = '%s%s ' %(line,int(tgtpwr))
        subStr = '%s%s' %(tag,line)
        bdFileStr = re.sub(searchStr,subStr,bdFileStr)
        
    # All other modes  
    for mode in ['VHT20','VHT40','VHT80','VHT160']:
        for user in ['SU','MU']:
            if user == 'SU':
                userTag = 'SU'
            else:
                userTag = 'MU'
            
            # 2G
            for freqIdx in range(0,4):
                tag = 'tgtPow2xNss_%s_%s_G_%d_0 ' %(userTag,mode,freqIdx)
                searchStr = "%s(.*)" %tag
                line = ''
                for mcs in range(0,10,1):
                    key = '%s_%s_%s' %(mode,user,mcs)
                    if not tgtpwrNewDict[key][freqIdx] == 'NA':
                        tgtpwr = float(tgtpwrNewDict[key][freqIdx])*2
                        line = "%s%d " %(line,int(tgtpwr))
                subStr = '%s%s' %(tag,line)
                bdFileStr = re.sub(searchStr,subStr,bdFileStr)
            
            # 5G    
            for freqIdx in range(0,8):
                tag = 'tgtPow2xNss_%s_%s_A_%d_0 ' %(userTag,mode,freqIdx)
                searchStr = "%s(.*)" %tag
                line = ''
                for mcs in range(0,10,1):
                    key = '%s_%s_%s' %(mode,user,mcs)
                    if not tgtpwrNewDict[key][freqIdx+4] == 'NA':
                        tgtpwr = float(tgtpwrNewDict[key][freqIdx+4])*2
                        line = "%s%d " %(line,int(tgtpwr))
                subStr = '%s%s' %(tag,line)
                bdFileStr = re.sub(searchStr,subStr,bdFileStr)
            
    return bdFileStr

#-------------------------------------------
# MAIN
if __name__ == '__main__':
    
    if len(sys.argv) !=3:
        print "Wrong Arguements\nUsage:setTgtPwr.py <input.csv> <bdata.txt>\nExiting.."
        sys.exit()
        
    # Open Target Power csv File
    tgtPwrFilePath = sys.argv[1]
    tgtPwrFile = open(tgtPwrFilePath,'r')
    tgtPwrStr = tgtPwrFile.read()
    
    # Open Board Data Txt File
    bdFilePath = sys.argv[2]
    bdFile = open(bdFilePath,'r')
    bdFileStr = bdFile.read()
    
    if tgtPwrStr.find('Mode,User') > -1:
        # new scheme
        print "Parsing csv file..."
        parseTgtPwr_newScheme(tgtPwrStr)
        print "Editing text file..."
        bdFileStr = tgtPwr2BDfile_newScheme(bdFileStr)
    else:
        # old scheme
        noOfChains = getNoOfChains(bdFileStr)
        print "No Of chains = %d" %noOfChains
        # CSV file -> Local Dictionary
        parseTgtPwr(tgtPwrStr)
        # Local Dictionary -> BD file
        bdFileStr = tgtPwr2BDfile(bdFileStr)
        
    # Make new Board Data file
    outputFilePath = re.sub('.txt','_edit.txt',bdFilePath)
    outputFile = open(outputFilePath,'w')
    outputFile.write(bdFileStr)
    
    print "Edited file : %s" %outputFilePath
    
    print "Done"

#-------------------------------------------
