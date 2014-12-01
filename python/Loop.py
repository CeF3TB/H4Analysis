#!env python

import os,sys
import array
from optparse import OptionParser

usage = "usage: %prog [options]"
parser=OptionParser(usage=usage)
parser.add_option("","--input" ,dest='input',type='string',help="Input Configuration file",default="")
parser.add_option("","--debug" ,dest='debug',type='int',help="Debug Verbosity. From 0-3. Default=%default",default=0)
(opts,args)=parser.parse_args()


if opts.debug >0 : print "-> Importing ROOOT"

sys.argv=[]
import ROOT 
ROOT.gROOT.SetBatch(1);

if opts.debug >0 : print "-> Importing Analysis Library"

ROOT.gSystem.Load("bin/H4Analysis.so")

if opts.debug >0 : print "-> Parse Configuration"

from  ParseInputFile import *
config=ParseInputFile(opts.input)

if opts.debug>0 : PrintConfiguration(config)


if opts.debug >0 : print "-> Init Class"

l=ROOT.LoopAndFill()

## INPUT
l.chainName="H4tree"
if 'chainName' in config:
	l.chainName=config['chainName']
#l.inputBranches[]=1
for b in config['InputBranches']:
	#l.inputBranches[b]=1
	l.AddInputBranch(b);
## OUTPUT
l.outFileName=config['OutputFile']
l.outTreeName="outputTree"
if 'outputTreeName' in config:
	l.outTreeName=config["outputTreeName"]
	
for b in config['OutputBranches']:
	#l.activeBranches[b]=1
	l.AddActiveBranch(b);
l.Init();

## INPUT -- POST INIT
for f in config['InputFiles']:
	print "Adding file '"+f+"'"
	l.AddToChain(f);

if opts.debug >0 : print "-> Init Analysis"
## INIT ANALYSIS
analysis=[]
for name in config['Analysis']:
	print "--- Loading Analysis: '" +name+"' ---"
   	analyzer = ROOT.__getattr__(name)()
	## config
	if name in config['config']:
		for line in config['config'][name]:
			print "Configuring Analysis "+name+"  with line " + line ##DEBUG
			exec("analyzer."+line ) 
	analyzer.Init( l);
	analysis.append(analyzer)
	print "--- Analysis '"+name+"' Loaded ---"

### A=ROOT.PedestalAnalysis()
### A.nChannels=4
### A.HV.push_back(600)
### A.Init(l)
### analysis.append(A)

if opts.debug >0 : print "-> Looping: Entries=",l.GetEntries() 

for iEntry in range(0,l.GetEntries() ) :
	if opts.debug >0  and iEntry % 1000 ==0 : print "--> Entry",iEntry,"of",l.GetEntries()
	l.GetEntry(iEntry)
	## ALL ANALYSIS ANALIZE EVENT
	for idx,A in enumerate(analysis):
	     try:
		A.StartWatch();
		A.ClearEvent()
		A.AnalyzeEvent()
		A.StopWatch();
	     except Exception as e:
		print "Exception occured while processing analysis:",config["Analysis"][idx]
		raise e
	l.Fill()
	
if opts.debug >0 : print "-> Writing"
l.Write();
l.Close();

if opts.debug >0 : print "-> END"

print "******* TIME SUMMARY ********"
print "------------------------"
print "    Looper   " 
print "------------------------"
print "CpuTime:",l.CpuTime()
print "RealTime:",l.RealTime()
print "Nentries:",l.GetEntries()
print "------------------------"
for i in range(0,len(analysis)):
	print "    %s   " %config["Analysis"][i]
	print "------------------------"
	print "CpuTime:",analysis[i].CpuTime()
	print "RealTime:",analysis[i].RealTime()
	print "------------------------"
print "*****************************"

