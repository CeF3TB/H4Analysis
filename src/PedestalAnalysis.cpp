#include "interface/PedestalAnalysis.hpp"
#include <assert.h>
#include <cmath>
#include "TMath.h"

#define DOUBLE_PEAK

void PedestalAnalysis::ClearEvent(){
	BaseAnalysis::ClearEvent();

	return;
}

void PedestalAnalysis::AnalyzeEvent()
{

	// main loop -- Fill and update branches
	for (unsigned int iSample=0;iSample< l->nDigiSamples;iSample++)
	{
 		UInt_t digiGroup   = l->digiGroup[iSample];
 		UInt_t digiChannel = l->digiChannel[iSample] + 8*digiGroup;
 		UInt_t digiSampleIndex = l->digiSampleIndex[iSample];
 		Float_t digiSampleValue = l->digiSampleValue[iSample];
		//int HV = int(l->CeF3HV);

		if( digiChannel < unsigned(nChannels)) 
			{

			l->FillProfile( Form("tprofile_pedestal_ch%d",digiChannel),digiSampleIndex +1 , digiSampleValue );

			}
	}
	return;
}

void PedestalAnalysis::Init(LoopAndFill *l1)
{
	cout<<"[PedestalAnalysis]::[Init]"<<endl;
	BaseAnalysis::Init(l1);
	//l->BookHisto("pedestal_ch0_HV600",1024,0,1024);
	if( ! ( l1->inputBranches["digiChannel"] and 
		l1->inputBranches["digiSampleIndex"] and 
		l1->inputBranches["digiSampleValue"] ) )
		{
		cout <<"[PedestalAnalysis]::[Init]::[ERROR]: input branches are not active"<<endl;	
		cout<<"digiChannel:"<< l1->inputBranches["digiChannel"] <<endl;
		cout<<"digiSampleIndex:"<< l1->inputBranches["digiSampleIndex"] <<endl;
		cout<<"digiSampleValue:"<< l1->inputBranches["digiSampleValue"] <<endl;
		assert(0);
		}
	cout<<"[PedestalAnalisis]::[Init]: Pedestal Histograms vs mu"<<endl;
	for( unsigned int iCh=0;iCh<unsigned(nChannels);++iCh)
	{
		l->BookHisto(Form("tprofile_pedestal_ch%d",iCh),"Pedestal",1024,0,1024,"TProfile");
	       //l->BookHisto(Form("tprofile_pedestal_ch%d",iCh,'A'+iCat),"Pedestal",1024,0,1024,"TProfile");
	}
	cout<<"[PedestalAnalysis]::[Init]::Done"<<endl;
	return ;
}

