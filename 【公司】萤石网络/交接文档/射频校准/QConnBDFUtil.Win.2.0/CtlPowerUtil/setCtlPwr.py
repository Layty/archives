############################################
# Description: convert target power txt file to board data text file
# Usage: python setCtlPwr.py <input.csv> <bdata.txt>
############################################

#-------------------------------------------
import sys,re,os
#-------------------------------------------
tgtPwrDict2G = {}
tgtPwrDict5G = {}
minTgtPwr2G = {}
minTgtPwr5G = {}
noOfChains = 0
chains = [0,1,2,3]

modeNameDict2G = {
                '11B' : '11b',
                'LEGACY' : '20Mhz',
                'VHT20' : '20Mhz',
                'VHT40' : '40Mhz',
                'VHT80' : '80Mhz',
                }
modeNameDict5G = {
                '11B' : '11b',
                'LEGACY' : 'legacy',
                'VHT20' : '20Mhz',
                'VHT40' : '40Mhz',
                'VHT80' : '80Mhz',
                'VHT160'         : '160Mhz',
                '80p80 Fp<Fs'    : '80plus80Mhz_Primary_lower_freq',
                '80p80 Fp>Fs'    : '80plus80Mhz_Primary_higher_freq',
                }

regDict = {
      'FCC'      : '1',
      'MKK'      : '4',
      'ETSI'     : '3',
      'SD_NO_CTL': 'E',
      'NO_CTL'   : 'F',
      'None'     : '0',
    }

modeDict = {
    'LEGACY'   : '0',
    '11B'      : '1',
    'VHT20'    : '2',
    'VHT40'    : '3',
    'VHT80'    : '4',
    'NONHT-DUP'      : '5',
    'VHT160'         : '6',
    '80p80 Fp<Fs'    : '8',
    '80p80 Fp>Fs'    : '9',
    }

modeDict_legacy = {
    'A_11A'    : '0',
    'G_11B'    : '1',
    'G_11G'    : '2',
    'G_TURBO'  : '3',
    'A_TURBO'  : '3',
    'G_108G'   : '4',
    'G_HT20' : '5',
    'A_HT20' : '6',
    'G_HT40' : '7',
    'A_HT40' : '8',
    'A_VHT80' : '9',
    'G_VHT20' : 'a',
    'A_VHT20' : 'b',
    'G_VHT40' : 'c',
    'A_VHT40' : 'd',

    }

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
def parseCtlPwr(ctlPwrStr,bdFileStr):
    ctlIdx = 0
    freqIdx = 0
    prevMode = 'x'
    extModeIdx = ''
    extModeArr = []
    
    # Get all CTL indices
    ctlPwrStrArr = re.findall("(\wGHz,.*?)CTLPwr?",ctlPwrStr,re.DOTALL)
    
    for ctlLineArrAll in ctlPwrStrArr:
        ctlLineArr = re.split("\n",ctlLineArrAll)
        pwrArr = []
        
        cArr = re.split(",",ctlLineArr[0])
        modeArr = cArr[0:6]
        reg = modeArr[3]
        band = int(modeArr[0][0])
        modeIdx = modeDict[modeArr[1]]
        
        if band == 2:
            mode = modeNameDict2G[modeArr[1]]
        else:
            mode = modeNameDict5G[modeArr[1]]
            
        
        freqArr = re.findall('\d{4}',ctlLineArr[0])
        bandEdgeArr = re.findall("\d+",ctlLineArr[1])
        pwrArrtemp = re.findall("(.*?),",ctlLineArr[2])
        for p in pwrArrtemp:
            if len(p)>0:
                pwrArr.append(float(p))
            
        
        if not mode == prevMode:
            ctlIdx = 0 
            freqIdx = 0
            extModeArr.append(extModeIdx)
            extModeIdx = mode[12:len(mode)]
            
        if band == 2:
            b = 'G'
            prefix = 'c'
        else:
            b = 'A'
            prefix = '_eepromC'
            if int(modeIdx) >= 7:
                prefix = 'c'
                extModeIdx  = '%s %s' %(extModeIdx,modeIdx)
                modeIdx = 7
            
        # Get the frequency piers from CSV file
        if len(freqArr) > 0:
            freqLineStr = ''
            tag = 'ctlFreqbin%dG_%s_%d_%s_0_0' %(band,mode,freqIdx,b)
            searchStr = '%s(.*)' %(tag)
            for f in freqArr :
                freqLineStr = freqLineStr + pier(f) + ' '
            subStr = '%s %s' %(tag,freqLineStr)
            bdFileStr = re.sub(searchStr,subStr,bdFileStr)
            freqIdx = freqIdx +1

        # Edit CTL indices in bd file            
        tag = '%stlIndex%dG_%s_%d_%s_0_0 ' %(prefix,band,mode,ctlIdx,b)
        searchStr = '%s(.*)' %(tag)
        subStr = "%smode:0x%s beamforming:0x%s regDmn:0x%s numChMask:%s numSSMask:%s " %(tag,modeIdx,modeArr[2],regDict[modeArr[3]],modeArr[4],modeArr[5])
        bdFileStr = re.sub(searchStr,subStr,bdFileStr)
        
        # Edit CTL data in bd file
        ctlDataLineStr = ''
        tag = 'ctlData%dG_%s_%s_%s_0_0' %(band,mode,ctlIdx,b)
        searchStr = '%s(.*)' %tag
        for i in range(0,len(bandEdgeArr)):
            val = int(bandEdgeArr[i])*128 + int(pwrArr[i]*2)
            val = hex(val)
            ctlDataLineStr =  ctlDataLineStr + str(val) + ' '
        subStr = '%s %s' %(tag,ctlDataLineStr)
        bdFileStr = re.sub(searchStr,subStr,bdFileStr)
        
        ctlIdx = ctlIdx + 1
        prevMode = mode
    
    extModeArr.append(extModeIdx)
    
    for e in extModeArr:
        if e:
            m = re.search("(\w+) (.*)",e)
            searchStr = 'ctlModeExt_HT80p80_%s_A_0_0 (.*)' %(m.group(1))
            subStr = 'ctlModeExt_HT80p80_%s_A_0_0 %s' %(m.group(1),m.group(2))
            a = re.search(searchStr, bdFileStr)
            bdFileStr = re.sub(searchStr, subStr, bdFileStr) 
    
    return bdFileStr
        
        
        
#-------------------------------------------
def parseCtlPwr_legacy(ctlPwrStr,bdFileStr):
    ctlIdx = 0
    ctlIdxStr2G = ''
    ctlIdxStr5G = ''
    
    # Get all CTL indices
    ctlPwrStrArr = re.findall("(\wGHz,.*?)CTLPwr?",ctlPwrStr,re.DOTALL)
    
    for ctlLineArrAll in ctlPwrStrArr:
        ctlLineArr = re.split("\n",ctlLineArrAll)
        pwrArr = []
        
        cArr = re.split(",",ctlLineArr[0])
        modeArr = cArr[0:6]
        reg = modeArr[3]
        band = int(modeArr[0][0])
        
        
        freqArr = re.findall('\d{4}',ctlLineArr[0])
        bandEdgeArr = re.findall("\d+",ctlLineArr[1])
        pwrArrtemp = re.findall("(.*?),",ctlLineArr[2])
        for p in pwrArrtemp:
            if len(p)>0:
                pwrArr.append(float(p))
        
        if band == 2:
            b = 'G'
            modeIdx = modeDict_legacy[b+'_'+modeArr[1]]
            ctlIdxStr2G = ('%s 0x%s%s') %(ctlIdxStr2G,regDict[reg],modeIdx)
        else:
            b = 'A'
            modeIdx = modeDict_legacy[b+'_'+modeArr[1]]
            ctlIdxStr5G = ('%s 0x%s%s') %(ctlIdxStr5G,regDict[reg],modeIdx)
            
        # Get the frequency piers from CSV file
        if len(freqArr) > 0:
            freqLineStr = ''
            tag = 'ctlFreqbin%dG_%d_%s_0_0' %(band,ctlIdx,b)
            searchStr = '%s(.*)' %(tag)
            for f in freqArr :
                freqLineStr = freqLineStr + hex(int(pier(f))) + ' '
            subStr = '%s %s' %(tag,freqLineStr)
            bdFileStr = re.sub(searchStr,subStr,bdFileStr)

        # Edit CTL data in bd file
        ctlDataLineStr = ''
        tag = 'ctlData%dG_%d_%s_0_0' %(band,ctlIdx,b)
        searchStr = '%s(.*)' %tag
        for i in range(0,len(bandEdgeArr)):
            val = int(bandEdgeArr[i])*64 + int(pwrArr[i]*2)
            val = hex(val)
            ctlDataLineStr =  ctlDataLineStr + str(val) + ' '
        subStr = '%s %s' %(tag,ctlDataLineStr)
        bdFileStr = re.sub(searchStr,subStr,bdFileStr)
        
        ctlIdx = (ctlIdx + 1)%18
        
    tag = 'ctlIndex%dG_%s_0_0' %(2,'G')
    searchStr = '%s(.*)' %(tag)
    subStr = '%s %s' %(tag,ctlIdxStr2G)
    bdFileStr = re.sub(searchStr,subStr,bdFileStr)
    
    tag = 'ctlIndex%dG_%s_0_0' %(5,'A')
    searchStr = '%s(.*)' %(tag)
    subStr = '%s %s' %(tag,ctlIdxStr5G)
    bdFileStr = re.sub(searchStr,subStr,bdFileStr)
    
    return bdFileStr
        
        
#-------------------------------------------
# MAIN
if __name__ == '__main__':
    
    if len(sys.argv) !=3:
        print "Wrong Arguements\nUsage:setCtlPwr.py <input.csv> <bdata.txt>\nExiting.."
        sys.exit()
    # Open Target Power csv File
    ctlPwrFilePath = sys.argv[1]
    ctlPwrFile = open(ctlPwrFilePath,'r')
    ctlPwrStr = ctlPwrFile.read()
    
    # Open Board Data Txt File
    bdFilePath = sys.argv[2]
    bdFile = open(bdFilePath,'r')
    bdFileStr = bdFile.read()
    
    # check compatibility
    searchStr = 'flag2__0_0 (.*)'
    m = re.search(searchStr,bdFileStr)
    if m:
        if int(m.group(1),16) < 3:
            # CSV file -> Local Dictionary
            print "Parsing %s.." %ctlPwrFilePath
            bdFileStr = parseCtlPwr_legacy(ctlPwrStr,bdFileStr)
        else:
            # CSV file -> Local Dictionary
            print "Parsing %s.." %ctlPwrFilePath
            bdFileStr = parseCtlPwr(ctlPwrStr,bdFileStr)
    else:
        print "flag2 not found in board data file\nExiting.."
        sys.exit()
    
    
    # Make new Board Data file
    outputFilePath = re.sub('.txt','_edit.txt',bdFilePath)
    outputFile = open(outputFilePath,'w')
    outputFile.write(bdFileStr)
    
    print "Edited file : %s" %outputFilePath
    
    print "Done"

#-------------------------------------------
