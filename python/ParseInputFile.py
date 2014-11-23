import re
from os import popen
def ParseInputFile(fileName):
	f=open(fileName,"r");
	R={}
	R['config']={}
	for line in f:
		l3=line.split('\n')[0] ## remove \n, spaces ...
		l2=l3.split('#')[0]
		parts=l2.split('=');
		#INCLUDE
		if parts[0] == 'include':
			print "including File:",parts[1]
			K=ParseInputFile(parts[1])
			#copy and update
			for key in K:
				if key == 'config': continue
				R[key]=K[key];
			for key in K['config']: ## update also config
				R['config'][key]=K['config'][key]
		#VALUES
		elif parts[0] == 'OutputFile' or parts[0]=='chainName' or parts[0]=='outTreeName':
			R[ parts[0] ] = parts[1]
		#VECTORS
		elif parts[0] == 'HV' or \
			parts[0] == 'InputFiles' or  \
			parts[0] == 'Analysis' :
			R[parts[0]]='='.join(parts[1:]).split(',');
		#TABLES
		elif parts[0] == 'ENERGIES' or parts[0] == 'PEDESTAL':
			table=parts[1].split(';'); 
			R[parts[0]]=[]
			for row in table:
				R[parts[0]] += row.split(',')
		# FILES
		elif parts[0]=='InputBranches' or parts[0] == 'OutputBranches':
			R[parts[0]]=[]
			fileList=parts[1].split(',')
			for fName in fileList:
				if len(fName) >0 and fName[0] == '$':
					br=fName[1:]
					R[parts[0]].append(br);
				fIn=open(fName,"r")
				for branch in fIn:
					br= branch.split('#')[0]
					br= re.sub('\n','',br)
					if br == "": continue
					R[parts[0]].append( br )
		###CONFIG ANALYSIS
		elif parts[0]=='config':
			#print "Parsing configuration Line:",line ###DEBUG
			configline='='.join(parts[1:]).split(' ')
			Analysis=configline[0]
			R[ parts[0] ][ Analysis ]=[]
			for i in range(1,len( configline)):
				#print "Config Line for analysis ", Analysis , " is ",configline[i]
				R[ parts[0] ][ Analysis ].append( configline[i] )
			#print "config is now:",R['config'] ### DEBUG
		### INPUT RUNS
		elif parts[0] =='InputRuns':
			print "--- InputRuns ---"	
			R[ 'InputFiles' ] = []
			template=parts[1].split(' ')[0]
			runs=parts[1].split(' ')[1].split(',')
			R[ 'InputFiles' ]= [ re.sub('%%RUN%%',x,template) for x in runs ]
			print R['InputFiles']
			print "----------------"
		else:
			if parts[0] != "" : print "Ignoring line:",line
			
	return R
def PrintConfiguration(config):
	print 
	print "===== CONFIGURATION ====="
	for key in config:
		print "key=",key,"has value:",config[key]	
	print "========================="
	print

def chunkIt(seq, num):
  ''' Take a list seq and return a n lists'''
  R=[]
  for i in range(0,num): R.append([])
  for j in range(0,len(seq) ): 
	R[ j%num ].append(seq[j])
  return R
  ### BUG FOR ROUNDING
  ### avg = len(seq) / float(num)
  ### out = []
  ### last = 0.0

  ### while last < len(seq):
  ###   out.append(seq[int(last):int(last + avg)])
  ###   last += avg

  ### return out

def ReadSRM(url):
    prefix=""
    postfix=""
    if "dcap" in url:
	#srmls srm://t3se01.psi.ch:8443/srm/managerv2?SFN=
	#dcap://t3se01.psi.ch:22125/
	##    aosutheou/prefix*postfix
	if '*' in url:
		star=url.rfind('*')
		prefix=url[:star]
		if '/' in prefix:
			slash=prefix.rfind('/')
			prefix=prefix[slash+1:]
		postfix=url[star+1:]
		slash=url.rfind('/')
		url=url[:slash+1]
	
	pfn=re.sub('t3se01.psi.ch:22125','t3se01.psi.ch:8443/srm/managerv2?SFN=', ### T3. TODO Add T2
		re.sub('dcap://','srm://',url)
		)
    elif "srm" in url:
	pfn=url
    else:	
    	pfn=url
    command = "lcg-ls -b -D srmv2 "+pfn+" 2>/dev/null"
    print "Executing command: " + command
    out_pipe = popen( command )
    #print "lcg-ls -l "+pfn+" 2>/dev/null | awk '{print $5}'"
    #out = [ line  for line in out_pipe.readlines() if ( prefix== "" or re.match(prefix,line) ) and ( postfix=="" or re.match(postfix,line) ) ]
    out = [ re.sub('^','dcap://t3se01.psi.ch:22125/',re.sub('\n','',line)) for line in out_pipe.readlines() if  ( postfix== "" or re.search(postfix,line) ) and ( prefix== "" or re.search(prefix,line) )  ]
    #print "OUT IS ",out
    exit_status=out_pipe.close() 
    if exit_status != None: 
		print "ERROR: not able to read on the storage-element. Try to do:\n voms-proxy-init  -voms cms\n"
		raise Exception('SRM', 'READ')
    return out
	

