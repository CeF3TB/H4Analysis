#include "interface/ToysAnalysis.hpp"

#include <ctime>
#include <iostream>
using namespace std;

void ToysAnalysis::Init(LoopAndFill*l1)
{
	cout<<"[ToysAnalysis]::[Init]"<<endl;
	BaseAnalysis::Init(l1);
	r=new TRandom3();
	if (seed >0) r->SetSeed(seed);
	else r->SetSeed( (unsigned)time(NULL) ) ;
	
	cout<<"[ToysAnalysis]::[Init] Constructing unsmeared shapes and histos"<<endl;

	fEvent=new TF1("myfunc","(x>-[0])*[2]*TMath::Exp(-0.5*TMath::Power((TMath::Log((  (x+[0]>0.001)*(x+[0]) + (x+[0]<=0.001)*0.001  )/[0])/[1]),2))",-30,100);
	fEvent->SetParameter(0,20);
	fEvent->SetParameter(1,0.6);
	fEvent->SetParameter(2,0.97);
	fEvent->SetRange(-30,100);

	// compute integral only once
	fIntegral=fEvent->Integral(-30,100);
	// Convert the function into an histogram -- unsmeared histo
	// a precise smear can be done through the function event by event to understand also the binning, but probably it is negligible
	hEvent=new TH1D("hEvent","hEvent",1024,0-.5,1024-.5);
	
	for(int i=0;i<hEvent->GetNbinsX();i++)
		{
		float xlow,xhigh,value;
		// -30 ns to center it to roughtly 10 ns = 50 digiSampleIndex
		xlow=  ( hEvent->GetBinLowEdge(i+1)-150 )*.2   ; // .2 = timesampleunit
		xhigh= ( hEvent->GetBinLowEdge(i+2)-150 )*.2    ;
		value=fEvent->Integral(xlow,xhigh);	
		hEvent->SetBinContent(i+1, value);
		}
	hEvent->Scale(1./hEvent->Integral( ) );


	cout<<"---- TOYS ANALYSIS CONFIGURATION ------"<<endl;
	cout<<"Seed="<<r->GetSeed()<<endl;
	cout<<"Noise RMS="<<noise_rms<<endl;
	cout<<"Resolution="<<resolution<<endl;
	cout<<"Using Function to simulate event="<<endl;
	//cout<<""<<endl;
	cout<<"---------------------------------------"<<endl;
	
	cout<<"[ToysAnalysis]::[Init] Faking active branches"<<endl;
	// fake active branches, that this analysis will fill
	l->AddInputBranch("digiChannel");
	l->AddInputBranch("digiSampleIndex");
	l->AddInputBranch("digiSampleValue");
	
	cout<<"[ToysAnalysis]::[Init] Done"<<endl;

}

void ToysAnalysis::ClearEvent()
{
	// event will be overwritten
}

void ToysAnalysis::AnalyzeEvent()
{
	// Simulate only one channel
	int nChannels=1;
	l->nDigiSamples=1024*nChannels;
	float my_charge=r->Gaus(charge_mean,resolution*charge_mean);
	int my_x_shift= int(r->Gaus(0,x_shift_rms));
	float my_pedestal=r->Gaus(40,10);
	for(unsigned int iS=0;iS<l->nDigiSamples;++iS)
		{
		l->digiFrequency[iS]=0;
		l->digiChannel[iS]=iS / 1024; 
		l->digiGroup[iS]=l->digiChannel[iS]/8;
		l->digiChannel[iS]=l->digiChannel[iS]%8;
		l->digiSampleIndex[iS]=iS % 1024; 
		l->digiBoard[iS]=0;
		float value=0;
		//check that with the shift, it is still inside the h boundaries
		if( l->digiSampleIndex[iS]+1 +my_x_shift >0 && l->digiSampleIndex[iS]+1 +my_x_shift< hEvent->GetNbinsX() +1 ) 
			value=hEvent->GetBinContent( l->digiSampleIndex[iS]+1 +my_x_shift );
		// scale to the right "energy"  --- flip it negative!
		value *= -my_charge;
		value += my_pedestal;
		//smear
		value += r->Gaus(0,noise_rms);
		l->digiSampleValue[iS]=value;
		}
	l->nScalerWords=0;
	l->nPatterns=0;
	l->nTriggerWords=0;
	l->nAdcChannels=0;
	l->nTdcChannels=0;
	// --event informations
	l->nEvtTimes=1;
	l->evtTime[0]=(ULong64_t)time(NULL) * 1000000;
	l->runNumber=0;	//in data starts from 1
	l->spillNumber=0;	//in data starts from 1
	l->evtNumber=++iEvent;
	// -- truth
	l->BeamEnergy=charge_mean;
	l->ToyResolution=resolution;
	l->ToyNoise=noise_rms;
	// -- complete the DataBase Informations
	l->CeF3HV=0;
	l->BeamTilt=0;
	l->IsPhysics=0;
	l->TableX=0;
	l->TableY=0;
}
