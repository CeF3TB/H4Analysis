#!env python

import os,sys
from glob import glob
from subprocess import call

from optparse import OptionParser

usage = "usage: %prog [options]"
parser=OptionParser(usage=usage)
parser.add_option("-i","--input" ,dest='input',type='string',help="Input Configuration file",default="")
parser.add_option("-d","--dir" ,dest='dir',type='string',help="Directory where to write the configuration",default="submit")
parser.add_option("-v","--debug" ,dest='debug',type='int',help="Debug Verbosity. From 0-3. Default=%default",default=0)
parser.add_option("-n","--njobs" ,dest='njobs',type='int',help="Number of Job to submit",default=50)
parser.add_option("-q","--queue" ,dest='queue',type='string',help="Queue",default="short.q")
parser.add_option("-t","--no-tar" ,dest='tar',action='store_false',help="Do not Make Tar",default=True)
parser.add_option("","--dryrun" ,dest='dryrun',action='store_true',help="Do not Submit",default=False)
(opts,args)=parser.parse_args()

# import Parser
from ParseInputFile import *
config=ParseInputFile(opts.input)

call("[ -d %s ] && rm -r %s"%(opts.dir,opts.dir),shell=True)
call("mkdir -p %s"%opts.dir,shell=True)

if opts.tar:
	cmd=["tar","-czf","%s/package.tar.gz"%opts.dir]
	cmd.extend( glob("bin/*so" ) )
	cmd.extend( glob("data/*dat" ) )
	cmd.extend( glob("data/*sqlite" ) )
	cmd.extend( glob("data/*root" ) )
	cmd.extend( glob("python/*py") )
	cmd.extend( glob("test/*") )
	print " ".join(cmd)
	call(cmd)

## expand *
fileList=[]
for f in config['InputFiles']:
	list=[]
	if 'dcap' in f or 'srm' in f:	
		list=ReadSRM(f)
	else :
		list=glob(f)
		if list == []: ### maybe remote ?
			list=f
	fileList.extend(list)
config['InputFiles']=fileList
	

splittedInput=chunkIt(config['InputFiles'],opts.njobs )

for iJob in range(0,opts.njobs):
	sh=open("%s/sub%d.sh"%(opts.dir,iJob),"w")
	basedir=opts.dir
	if basedir[0] != '/': basedir=os.environ['PWD'] + "/" + opts.dir
	
	sh.write('#!/bin/bash\n')
	sh.write('#$-q %s\n'%opts.queue)
	sh.write('#$-cwd\n')
	sh.write('#$-N %s/Job_%d\n'%(opts.dir,iJob))
	sh.write('#$-o %s/log%d.txt\n'%(basedir,iJob))
	sh.write('#$-e %s/log%d.txt\n'%(basedir,iJob))
	sh.write('source /swshare/ROOT/root_v5.34.18_slc5_amd64_py26_pythia6/bin/thisroot.sh\n')
	if opts.tar:
		sh.write("mkdir -p /scratch/%s_%d\n"%(opts.dir,iJob))
		sh.write("cd /scratch/%s_%d\n"%(opts.dir,iJob))
		sh.write("tar -xzf %s/package.tar.gz\n"%(basedir ))
		sh.write("mkdir -p %s\n"%opts.dir)
		sh.write("cp %s/*dat %s/\n"%(basedir,opts.dir))
	sh.write('touch %s/sub%d.run\n'%(basedir,iJob))
	sh.write('python python/Loop.py --input=%s/input%d.dat --debug 3\n'%(opts.dir,iJob))
	sh.write('EXITCODE=$?\n')
	sh.write('rm %s/sub%d.run\n'%(basedir,iJob))
	sh.write('[ $EXITCODE == 0 ] && touch %s/sub%d.done\n'%(basedir,iJob))
	sh.write('[ $EXITCODE != 0 ] && touch %s/sub%d.fail\n'%(basedir,iJob))
	if opts.tar:
		if basedir != opts.dir : sh.write("mv %s/output%d.root %s/output%d.root\n"%(opts.dir,iJob,basedir,iJob))
	
	dat=open("%s/input%d.dat"%(opts.dir,iJob),"w")
	dat.write("include=%s\n"%opts.input)
	#print "inputFiles=",config['InputFiles'], "Splitted=",splittedInput,"\n\nselected=",','.join(splittedInput[iJob]),"\n\n" ###DEBUG
	dat.write('InputFiles=%s\n'%( ','.join(splittedInput[iJob]) ) )
	dat.write('OutputFile=%s/output%d.root\n'%(opts.dir,iJob) )

	## make the sh file executable	
	call(["chmod","u+x","%s/sub%d.sh"%(opts.dir,iJob)])
	
	## submit
	cmd=["qsub","-b","y","-o","%s/log%d.txt"%(basedir,iJob),"-e","%s/log%d.txt"%(basedir,iJob),"%s/sub%d.sh"%(basedir,iJob)]
	cmdline=' '.join(cmd)
	print cmdline
	if not opts.dryrun: 
		call(cmdline,shell=True)

