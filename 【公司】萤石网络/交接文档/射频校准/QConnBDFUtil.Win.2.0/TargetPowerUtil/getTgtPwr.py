############################################
# Description: Get Target Powers from Board Data file
# Usage: python getTgtPwr.py <bdata.txt> <output.csv> 
############################################

#-------------------------------------------
import sys,re,os
#-------------------------------------------

#CSV_HEADER = '''\
#Data rate,2412,2432,2462,2472,5180,5210,5320,5400,5500,5600,5700,5825
#'''
leglen=0
CSV_HEADER =[]
freq2Gcck_final =[]
freq2Gb_final1 =[]
freq2Gb_final =[]
freq2Gb_temp = []
freq5G_final =[]
#CSV_HEADER_NEW = '''\
#Mode,User,Rate,2412,2432,2462,2472,5180,5210,5320,5400,5500,5600,5700,5825
#'''

modeArr = ['11b','11ag','VHT20','VHT40','VHT80']
modeArr_cck = ['11b']
modeArr_nac = ['11ag','VHT20','VHT40','VHT80']
bRate = ['1Mbps','2Mbps','5.5Mbps','11Mbps']
agRate = ['6-24Mbps','36Mbps','48Mbps','54Mbps']
nacRate = ['MCS0','MCS1-2','MCS3-4',
           'MCS5','MCS6','MCS7','MCS8','MCS9',
           'MCS15','MCS16','MCS17','MCS18','MCS19',
           'MCS25','MCS26','MCS27','MCS28','MCS29',
           'MCS35','MCS36','MCS37','MCS38','MCS39',
           'NA'
               ]

nacRate2ch = ['MCS0','MCS1-2','MCS3-4',
           'MCS5','MCS6','MCS7','MCS8','MCS9',
           'MCS15','MCS16','MCS17','MCS18','MCS19',
               ]

nacRate3ch = ['MCS0','MCS1-2','MCS3-4',
           'MCS5','MCS6','MCS7','MCS8','MCS9',
           'MCS15','MCS16','MCS17','MCS18','MCS19',
           'MCS25','MCS26','MCS27','MCS28','MCS29',
               ]

nacRateLite = ['MCS0','MCS1-2','MCS3-4',
           'MCS5','MCS6','MCS7','MCS8','MCS9',
               ]
tgtPwrDict = {}
tgtPwrDict_cck = {}
noOfFreqs = 12

#-------------------------------------------
def getNoOfChains(bdFileStr):
    searchStr = 'targetPower2GVHT20Base_G_0_0 (.*)'
    m = re.search(searchStr,bdFileStr)
    if m:
        return re.subn('(\d+)','',m.group(1))[1]

#-------------------------------------------


#------------------------------------------------
def getCSV_HEADER(bdFileStr):
  
  search2GCCKStr = 'targetFreqbinCck_G_0_0 (.*\d)'
  search2GbStr1 = 'targetFreqbin2G_G_0_0 (.*\d)'
  search2GbStr = 'targetFreqbin2GVHT40_G_0_0 (.*\d)'
  search5GStr = 'targetFreqbin5GVHT80_A_0_0 (.*\d)'
  freq2Gcck_temp = re.findall(search2GCCKStr,bdFileStr)
  freq2Gb_temp1 = re.findall(search2GbStr1,bdFileStr)
  freq2Gb_temp = re.findall(search2GbStr,bdFileStr)
  freq5G_temp = re.findall(search5GStr,bdFileStr)
  freq2Gcck = re.split('\s+',freq2Gcck_temp[0])
  freq2Gb1 = re.split('\s+',freq2Gb_temp1[0])
  freq2Gb = re.split('\s+',freq2Gb_temp[0])
  freq5G = re.split('\s+',freq5G_temp[0])
  for t in freq2Gcck:
    t = int(t) + 2300
    freq2Gcck_final.append(t)
  for t in freq2Gb1:
	t = int(t) + 2300;
	freq2Gb_final1.append(t)
	
	
  for t in freq2Gb:
    t = int(t) + 2300
    freq2Gb_final.append(t)
	
  for t in freq5G:
    t = (int(t)*5) + 4800
    freq5G_final.append(t)
  
  
  for t in freq2Gcck_final:
	    if (CSV_HEADER.count(t) == 0):
		  CSV_HEADER.append(t)
  for t in freq2Gb_final1:
	    if (CSV_HEADER.count(t) == 0):
		  CSV_HEADER.append(t)	  
 
  for t in freq2Gb_final:
	    if (CSV_HEADER.count(t) == 0):
		  CSV_HEADER.append(t)	  
  for t in freq5G_final:
	    if (CSV_HEADER.count(t) == 0):
		  CSV_HEADER.append(t)
  
#----------------------------------------------------------------------		  
    	 

#-------------------------------------------

def parseBoardDataCCK(bdFileStr):
    # Parse CCK section of board data file
    line = re.findall("targetPowerCck.*_G_(\d)_0 (.*)",bdFileStr)
    addline = line[1]
    ccklen = len(freq2Gcck_final)
    global leglen
    leglen = len(freq2Gb_final)
    count = 0
    for f in freq2Gb_final1:
		if f in freq2Gb_final: 
			count = count + 1
    if count == 0:
		leglen = len(freq2Gb_final)+len(freq2Gb_final1) 
		
    while ccklen < leglen:
        line.insert((ccklen),addline)
        ccklen = ccklen + 1

    keyTag = "%s %s" %(modeArr[0],'%s')
	
    for l in line:
        tgt = re.findall("(\d+)",l[1])
       
        for t in range(0,len(tgt)):
            key = keyTag %bRate[t]
            
                     
            # Add target powers to Dictionary
            if key not in tgtPwrDict:
                tgtPwrDict[key] = []
                
            tgtPwrDict[key].append(float(tgt[t])/2)
   
    return tgtPwrDict

#-------------------------------------------

def parseBoardDataLeg(bdFileStr):
    lines = re.findall("targetPower(\d)G_(\w)_(\d)_0 (.*)",bdFileStr)
    test = re.findall("targetPower2G_(\w)_3_0 (.*)",bdFileStr)
    
    # ADjust for 4 freq piers
    addLine = lines[2]
    
    
    if len(test) == 1:
        lines.insert(3,addLine)

    for l in lines:
        keyTag = "%s %s" %(modeArr[1],'%s')
        tgt = re.findall("(\d+)",l[3])
        for t in range(0,len(tgt)):
            key = keyTag %agRate[t]
            
            # Add target power to dicitonary
            if key not in tgtPwrDict:
                tgtPwrDict[key] = []
            tgtPwrDict[key].append(float(tgt[t])/2)
            
    return tgtPwrDict

#-------------------------------------------
def getExtBitStr(bdFileStr):
    # Parse the extra bit string and return
    retStrArr=[]
    searchArr = re.findall("extTPow2xDelta\dG (.*)",bdFileStr)
    for s in searchArr:
        retStr = ''
        bArr = re.findall("(\d+)",s)
        for b in bArr:
            b = int(b)
            revBinStr = bin(b)[2:].zfill(8)
            binStr = revBinStr[::-1]
            retStr = retStr+binStr
        retStrArr.append(retStr)
    return retStrArr
    
#-------------------------------------------
def parseBoardDataHT(bdFileStr):
    # Parse HT/VHT section of board data file
    tgtBase = {}
    tgtBase['2'] = 0
    tgtBase['5'] = 0
    
    # Prepare the extra bits
    extBitStrArr = getExtBitStr(bdFileStr)
    extBitIdx2G = 0
    extBitIdx5G = 0
    extBitIdx = 0
    
    # Get Base Powers
    lines = re.findall("targetPower(\d)GVHT(\d)0Base_\w_(\d)_0 (.*)",bdFileStr)
    for l in lines:
        nIdx = 0
        
        band = l[0]
        bw = l[1]
        freq = l[2]
        
        if band == '2':
            extBitStr = extBitStrArr[0]
        else:
            extBitStr = extBitStrArr[1]
        
      
        base = re.findall("(\d+)",l[3])
        tgtBase[band] = int(base[0])
    
        
        searchStr = "targetPower%sGVHT%s0Delta_\\w_%s_0" %(band,bw,freq)
        lines = re.findall(searchStr+" (.*)" ,bdFileStr)
        for l in lines:
            nibble = []
            tgt = re.findall("0x(\w+)",l) 
            # Extract Deltas
            for tIdx in range(0,len(tgt)):
                if len(tgt[tIdx]) < 2:
                    nibble.append(int("0x"+tgt[tIdx][0],16))
                    nibble.append(0)
                else:
                    nibble.append(int("0x"+tgt[tIdx][1],16))
                    nibble.append(int("0x"+tgt[tIdx][0],16))
            
            
        for rIdx in range(0,len(nacRate)):
            r = nacRate[rIdx]
            
            key = "VHT%s0 %s" %(bw,r)            
            
            basePwr = int(tgtBase[band])
            deltaPwr = int(nibble[nIdx]) + 16*int(extBitStr[extBitIdx])
            
            
            if band == '2':
                extBitIdx2G = extBitIdx2G + 1
                extBitIdx = extBitIdx2G
            else:
                extBitIdx5G = extBitIdx5G + 1
                extBitIdx = extBitIdx5G
            
                
            tgtPwr = (float(basePwr) + float(deltaPwr))/2
            nIdx = nIdx+1
            if key not in tgtPwrDict:
                tgtPwrDict[key] = []
            tgtPwrDict[key].append(tgtPwr)
    
    return tgtPwrDict

#-------------------------------------------
def dict2Txt(tgtDict,noOfFreqs):

	retStr = '';index_2g=[]
	csvLine = '';index_2g_HT40=[]
	tgtPwrStr = '';index_cck=[]
	idx = 0
	flag = 0
	for f in freq2Gcck_final:
			idx = freq2Gb_temp.index(f)
			if(index_cck.count(f) == 0):
				index_cck.append(idx)
	if leglen != len(freq2Gb_final):
		flag = 1
		for f in freq2Gb_final1:
			idx = freq2Gb_temp.index(f)
			if(index_2g.count(f)==0):
				index_2g.append(idx)
		for f in freq2Gb_final:
			idx = freq2Gb_temp.index(f)
			if(index_2g_HT40.count(f)==0):
				index_2g_HT40.append(idx)
							
	
	for rate in bRate:
		lineStr = ''
		i=0;j=0;
		key = "11b %s" %(rate);
		if key in tgtDict:
			lineStr = "%s%s" %(lineStr,key)
			while i < noOfFreqs:
				if i in index_cck and i < leglen and j < len(tgtDict[key]):
					lineStr = "%s,%s" %(lineStr, tgtDict[key][j])
					j = j+1;i=i+1;
				else :
					lineStr = "%s, NA" %(lineStr)
					i=i+1;
		retStr = retStr + lineStr + "\n"
	for mode in modeArr_nac:
            
    
            for rate in agRate+nacRate:
                lineStr = ''
                extStr = ''
                i=0;j=0;
                key = "%s %s" %(mode,rate)
            
                if key in tgtDict:
					lineStr = "%s%s" %(lineStr,key)
					if flag == 0:
						for tIdx in range(0,len(tgtDict[key])):    
							lineStr = "%s,%s" %(lineStr,tgtDict[key][tIdx])
						for eIdx in range(tIdx+1,noOfFreqs):
							extStr = ", NA, NA, NA"
                        
						if mode == "VHT80":
							lineStr = re.sub(key+',',key+extStr+',',lineStr) + "\n"
                        
						else:
							lineStr = lineStr + extStr + "\n"
                    
						retStr = retStr + lineStr           
					else:
						if (mode == "11ag") or (mode == "VHT20"):
							while i < noOfFreqs and j < len(tgtDict[key]) :
								
								if (i in index_2g) or (i >= leglen):
									lineStr = "%s,%s" %(lineStr, tgtDict[key][j])
									j = j+1;i=i+1;
								else :
									lineStr = "%s, NA" %(lineStr)
									i=i+1;
						if mode == "VHT40":
							while i < noOfFreqs and j < len(tgtDict[key]) :
								
								if (i in index_2g_HT40) or (i >= leglen):
									lineStr = "%s,%s" %(lineStr, tgtDict[key][j])
									j = j+1;i=i+1;
								else :
									lineStr = "%s, NA" %(lineStr)
									i=i+1;
						if mode == "VHT80":
							while i < noOfFreqs and j < len(tgtDict[key]) :
								
								if i >= leglen:
									lineStr = "%s,%s" %(lineStr, tgtDict[key][j])
									j = j+1;i=i+1;
								else :
									lineStr = "%s, NA" %(lineStr)
									i=i+1;
						
						retStr = retStr + lineStr + "\n"
	return retStr
				
                    
 
#-------------------------------------------
def parseBoardDataHT_newScheme(fileStr):
    tag = 'tgtPow2xNss'
    searchStr = '%s_SU_(\w+)_(\w)_(\d)_0 (.*)' %tag
    tgtPwrStrArr = re.findall(searchStr,fileStr)
    
    # get SU powers in bd file
    for t in tgtPwrStrArr:
        tgtPwrArray = re.findall('\d+',t[3])
            
        for mcsIdx in range(0,len(tgtPwrArray)):
            key = '%s_%s_%s' %(t[0],'SU',mcsIdx)
            if not key in tgtPwrDict:
                tgtPwrDict[key] = []
            tgtPwr = float(tgtPwrArray[mcsIdx])/2
            tgtPwrDict[key].append(tgtPwr)
            
    searchStr = '%s_MU_(\w+)_(\w)_(\d)_0 (.*)' %tag
    tgtPwrStrArr = re.findall(searchStr,fileStr)
    
    # get MU powers in bd file
    for t in tgtPwrStrArr:
        tgtPwrArray = re.findall('\d+',t[3])
            
        for mcsIdx in range(0,len(tgtPwrArray)):
            key = '%s_%s_%s' %(t[0],'MU',mcsIdx)
            if not key in tgtPwrDict:
                tgtPwrDict[key] = []
            tgtPwr = float(tgtPwrArray[mcsIdx])/2
            tgtPwrDict[key].append(tgtPwr)
    

#-------------------------------------------
def dict2Txt_new(tgtDict):
    # Print targetpower dictionary to CSV file
    
    # CCK
    csvLine = ''
    for cckRate in ['11b 1Mbps', '11b 2Mbps' , '11b 5.5Mbps' , '11b 11Mbps']:
        tgtPwrStr = '%s,%s,%s,%s,NA,NA,NA,NA,NA,NA,NA,NA' \
                    %(tgtDict[cckRate][0],tgtDict[cckRate][0],tgtDict[cckRate][1],tgtDict[cckRate][1],)
        csvLine = '%s%s,%s,%s,%s\n' %(csvLine,'CCK','SU',cckRate[4::],tgtPwrStr)
	
    
    # All other modes
    for mode in ['VHT20','VHT40','VHT80','VHT160']:
        for rate in range(0,10,1):
            for user in ['SU','MU']:
                tgtPwrStr = ''
                key = '%s_%s_%s' %(mode,user,rate)
                if (mode == 'VHT80') or (mode == 'VHT160') :
                    tgtPwrStr = 'NA,NA,NA,NA,'
                    for i in tgtDict[key]:
                        tgtPwrStr = tgtPwrStr + str(i) + ',' 
                else:
                    for i in tgtDict[key]:
                        tgtPwrStr = tgtPwrStr + str(i) + ',' 
                csvLine = '%s%s,%s,MCS%s,%s\n' %(csvLine,mode,user,rate,tgtPwrStr)
    return csvLine


#-------------------------------------------
# MAIN
if __name__ == '__main__':
    
    if len(sys.argv) !=3:
        print "Wrong Arguements\nUsage:getTgtPwr.py <bdata.txt> <output.csv>\nExiting.."
        sys.exit()
        
    # Open Board Data Txt File
    bdFilePath = sys.argv[1]
    bdFile = open(bdFilePath,'r')
    bdFileStr = bdFile.read()
    getCSV_HEADER(bdFileStr)
    noOfFreqs = len(CSV_HEADER)
    CSV_HEADER = sorted(CSV_HEADER);
    freq2Gb_temp = CSV_HEADER[:]
    CSV_HEADER = str(CSV_HEADER)
    CSV_HEADER = CSV_HEADER.replace("[","")
    CSV_HEADER = CSV_HEADER.replace("]","")
    CSV_HEADER = CSV_HEADER.replace("'","")

    # check compatibility
    searchStr = 'flag2__0_0 (.*)'
        
    m = re.search(searchStr,bdFileStr)

    if m:
        if int(m.group(1),16) < 3:
            # If old scheme
            #Detect No Of Chains
            noOfChains = getNoOfChains(bdFileStr)
            print "No Of chains = %d" %noOfChains
            if noOfChains == 2:
                nacRate = nacRate2ch
            if noOfChains == 3:
                nacRate = nacRate3ch
            
			# Board Data file -> CSV file
            print "Parsing CCK.."
            
            parseBoardDataCCK(bdFileStr)
            print "Parsing Legacy.."
            parseBoardDataLeg(bdFileStr)
            print "Parsing HT/VHT.."
            parseBoardDataHT(bdFileStr)

            # Open Target Power csv File
            tgtPwrFilePath = sys.argv[2]
            tgtPwrFile = open(tgtPwrFilePath,'w')
            space = 'DataRate'
            tgtPwrFile.write(space)
            space = ','
            tgtPwrFile.write(space)
            tgtPwrFile.write(CSV_HEADER)
            nextline = '\n'
            tgtPwrFile.write(nextline)
            tgtPwrFile.write(dict2Txt(tgtPwrDict,noOfFreqs))
            
            sys.exit()
        else:
			# new scheme
			print "Parsing CCK.."
			parseBoardDataCCK(bdFileStr)
			print "Parsing HT/VHT.."
			parseBoardDataHT_newScheme(bdFileStr)
            
            
			# Open Target Power csv File
			tgtPwrFilePath = sys.argv[2]
			tgtPwrFile = open(tgtPwrFilePath,'w')
			space="Mode,User,Rate,"
			tgtPwrFile.write(space)
			nextline = '\n'
			tgtPwrFile.write(CSV_HEADER)
			tgtPwrFile.write(nextline)
			tgtPwrFile.write(dict2Txt_new(tgtPwrDict))
    else:
        print "flag2 not found in board data file\nExiting.."
        sys.exit()
    
    print "Created file : %s" %tgtPwrFilePath
    print "Done"
