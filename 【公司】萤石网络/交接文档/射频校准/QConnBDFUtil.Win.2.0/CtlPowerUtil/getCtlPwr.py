############################################
# Description: Get Target Powers from Board Data file
# Usage: python getCtlPwr.py <bdata.txt> <output.csv> 
############################################

#-------------------------------------------
import sys,re,os
#-------------------------------------------

CSV_HEADER = '''\
Band,Mode,Beam Forming,Reg Domain,Chain Mask,Stream Mask,Frequencies
'''
emptyCells = ',,,,,,'

regDict = {
      'FCC'      : '1',
      'MKK'      : '4',
      'ETSI'     : '3',
      'SD_NO_CTL': 'E',
      'NO_CTL'   : 'F',
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
    '11A'    : '0',
    '11B'    : '1',
    '11G'    : '2',
    'TURBO'  : '3',
    '108G'   : '4',
    'HT20' : '5',
#    '11A_HT20' : '6',
    'HT40' : '7',
#    '11A_HT40' : '8',
    'VHT80' : '9',
    'VHT20' : 'a',
#    '11A_VHT20' : 'b',
    'VHT40' : 'c',
#    '11A_VHT40' : 'd',

    }

modeAdjust = {
              '6' : '5',
              '8' : '7',
              'b' : 'a',
              'd' : 'c',
    }

def getKey(dictn,val):
    for k in dictn:
        if (val == dictn[k]):
            return k

#-------------------------------------------
def pier2freq(band,pier):
    pier = int(pier)
    if band == 2:
        return str(2300 + pier)
    else:
        return str(5000 + (pier-40)*5)
#-------------------------------------------
def getNoOfChains(bdFileStr):
    searchStr = 'targetPower2GVHT20Base_G_0_0 (.*)'
    m = re.search(searchStr,bdFileStr)
    if m:
        return re.subn('(\d+)','',m.group(1))[1]

#-------------------------------------------
def parseBoardDataCtlIdx(bdFileStr,band):
    ctlIdxStr = ''
    prevRegDmn = 'x'
    prevMode = 'x'
    regDmnIdx = -1
    printFreq = False
    retStr = ''
    
    if (band == 2):
        b = 'G'
    elif (band == 5):
        b = 'A'
    elif (band == 80):
        b = 'A'

    # Find all CTL indices    
    if band == 2:
        searchStr = 'ctlIndex2G_(\w+)_(\d+)_\w_0_0 mode:0x(\w) beamforming:0x(\w) regDmn:0x(\w) numChMask:0x(\w) numSSMask:0x(\w) '
    elif band == 5:
        searchStr = '_eepromCtlIndex5G_(\w+)_(\d+)_\w_0_0 mode:0x(\w) beamforming:0x(\w) regDmn:0x(\w) numChMask:0x(\w) numSSMask:0x(\w) '
    elif band == 80:
        band = 5
        searchStr = 'ctlIndex5G_(.*?)_(\d+)_\w_0_0 mode:0x(\w) beamforming:0x(\w) regDmn:0x(\w) numChMask:0x(\w) numSSMask:0x(\w) '
    
    ctlIdxArr = re.findall(searchStr, bdFileStr)
    
    for c in ctlIdxArr:
        bandEdgeStr = ''
        powerValStr = ''
        freqStr = ''
        
        mode = c[2]
        bf = c[3]
        reg = c[4]
        cmask = c[5] 
        smask = c[6]
        modeName = c[0]
        idx = c[1]
        
        if int(mode)>=7:
            priMode = modeName[12:len(modeName)]
            searchStr = 'ctlModeExt_HT80p80_%s_A_0_0 (.*)' %priMode
            extBitsArr = re.findall('(\d)',re.findall(searchStr,bdFileStr)[0])
            mode = extBitsArr[int(idx)]
        
        if (int(mode) == 0) and int(band) == 5:
        	if (modeName.find('legacy')<0):
        		continue
        	
        ctlIdxStr = '%sGHz,%s,%s,%s,0x%s,0x%s,' \
        %(band,getKey(modeDict,mode),bf,getKey(regDict,reg),cmask,smask)
        
        if(prevMode == 'x'):
            prevMode = mode
        
        # check if reg domain has changed from previous    
        if not (prevRegDmn == reg):
            regDmnIdx = regDmnIdx+1
            printFreq = True
        else:
            printFreq = False
        
        if not (prevMode == mode):
            regDmnIdx = 0
            prevMode = 'x'
        
        prevRegDmn = reg
        prevMode = mode    
        
        # search freqyency bins
        searchStr = r'ctlFreqbin%dG_%s_%s_%s_0_0 (.*)' %(band,modeName,regDmnIdx,b)
        
        freqStrArr = re.findall(searchStr, bdFileStr,re.I)[0]
        for f in re.findall("\d+",freqStrArr):
            if printFreq : 
                freqStr = freqStr + pier2freq(band,f) + ','
            else:
                freqStr = freqStr + ','
                
        # Add mode and frequency in same line
        lineStr = ctlIdxStr + freqStr + '\n'
        
        searchStr = 'ctlData%dG_%s_%s_%s_0_0 (.*)' %(band,modeName,idx,b)
        ctlDataStr = re.findall(searchStr, bdFileStr)[0]
        
        # Search for CTL data line in bd file
        for c in re.findall("(\w+)",ctlDataStr):
            power = 0
            val = int(c,16) 
            bit = (val>>7)%2
            bandEdgeStr = bandEdgeStr + str(bit) + ','
            
            if bit:
                power = (float(val) - 128)/2
            else:
                power = float(val)/2
                
            powerValStr = powerValStr + str(power) + ','
            
        # print all data
        lineStr = lineStr + emptyCells +  bandEdgeStr + 'bandEdgeBit,' +'\n'
        lineStr = lineStr + emptyCells + powerValStr + 'CTLPwr,' + '\n'
        retStr = retStr + lineStr
    
    return retStr

#-------------------------------------------
def parseBoardDataCtlIdx_legacy(bdFileStr,band):
    ctlIdxStr = ''
    prevRegDmn = 'x'
    prevMode = 'x'
    regDmnIdx = -1
    printFreq = False
    retStr = ''
    
    if (band == 2):
        b = 'G'
    elif (band == 5):
        b = 'A'

    # Find all CTL indices    
    searchStr = 'ctlIndex%dG(.*)' %band
    m = re.search(searchStr,bdFileStr)
    ctlIdxArr = re.findall('0x(\w+)', m.group(1))
    
    for c in ctlIdxArr:
        freqStr = ''
        mode = c[1]
        reg = c[0]
        
        bf = 'NA'
        cmask = 'NA' 
        smask = 'NA'
        regDmnIdx = regDmnIdx+1
        
        if mode in modeAdjust:
            mode = modeAdjust[mode]
        
        ctlIdxStr = '%sGHz,%s,%s,%s,%s,%s,' \
        %(band,getKey(modeDict_legacy,mode),bf,getKey(regDict,reg),cmask,smask)
        
        bandEdgeStr = ''
        powerValStr = ''
        
        # search frequency bins
        searchStr = r'ctlFreqbin%dG_%d(.*)' %(band,regDmnIdx)
        m = re.search(searchStr,bdFileStr)
        freqStrArr = re.findall('0x\w+', m.group(1))
        
        for f in freqStrArr:
            freqStr = freqStr + pier2freq(band,int(f,16)) + ','
            
        # Add mode and frequency in same line
        lineStr = ctlIdxStr + freqStr + '\n'
        searchStr = 'ctlData%dG_%d(.*)' %(band,regDmnIdx)
        m = re.search(searchStr,bdFileStr)
        ctlDataArr = re.findall('0x\w+', m.group(1))
        
        # Search for CTL data line in bd file
        for c in ctlDataArr:
            power = 0
            val = int(c,16) 
            bit = val>>6
            bandEdgeStr = bandEdgeStr + str(bit) + ','
            power = float(val & 0x3f)/2
            powerValStr = powerValStr + str(power) + ','
            
        # print all data
        lineStr = lineStr + emptyCells +  bandEdgeStr + 'bandEdgeBit,' +'\n'
        lineStr = lineStr + emptyCells + powerValStr + 'CTLPwr,' + '\n'
        retStr = retStr + lineStr
    return retStr

#-------------------------------------------
# MAIN
if __name__ == '__main__':
    
    ctlIdxStr5G_80p80 = ''
    
    if len(sys.argv) !=3:
        print "Wrong Arguements\nUsage:getCtlPwr.py <bdata.txt> <output.csv>\nExiting.."
        sys.exit()
        
    # Open Board Data Txt File
    bdFilePath = sys.argv[1]
    bdFile = open(bdFilePath,'r')
    bdFileStr = bdFile.read()
    
    # check compatibility
    searchStr = 'flag2__0_0 (.*)'
    m = re.search(searchStr,bdFileStr)
    if m:
        if int(m.group(1),16) < 3:
            # Board Data file -> CSV file
            print "Parsing 2G.."
            ctlIdxStr2G = parseBoardDataCtlIdx_legacy(bdFileStr,2)
            print "Parsing 5G.."
            ctlIdxStr5G = parseBoardDataCtlIdx_legacy(bdFileStr,5)
        else:
            # Board Data file -> CSV file
            print "Parsing 2G.."
            ctlIdxStr2G = parseBoardDataCtlIdx(bdFileStr,2)
            print "Parsing 5G.."
            ctlIdxStr5G = parseBoardDataCtlIdx(bdFileStr,5)
            print "Parsing 80p80.."
            ctlIdxStr5G_80p80 = parseBoardDataCtlIdx(bdFileStr,80)
    else:
        print "flag2 not found in board data file\nExiting.."
        sys.exit()
    
        
    # Open Target Power csv File
    tgtPwrFilePath = sys.argv[2]
    tgtPwrFile = open(tgtPwrFilePath,'w')
    tgtPwrFile.write(CSV_HEADER)
    tgtPwrFile.write(ctlIdxStr2G+ctlIdxStr5G+ctlIdxStr5G_80p80)
#    tgtPwrFile.write((tgtPwrDict))
    
    print "Created file : %s" %tgtPwrFilePath
    print "Done"
