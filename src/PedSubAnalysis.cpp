#include "interface/PedSubAnalysis.hpp"
//#include "interface/PedestalAnalysis.hpp"
#include "TMath.h"

void PedSubAnalysis::ClearEvent()
{
	for(unsigned int i=0;i<MAX_DIGI_SAMPLES;++i)
		l->digiSampleValueSub[i]=0;
}

void PedSubAnalysis::AnalyzeEvent()
{
	for (unsigned int iSample=0;iSample< l->nDigiSamples;iSample++)
	{
 		UInt_t digiGroup   = l->digiGroup[iSample];
 		UInt_t digiChannel = l->digiChannel[iSample] + 8*digiGroup;
 		UInt_t digiSampleIndex = l->digiSampleIndex[iSample];
 		Float_t digiSampleValue = l->digiSampleValue[iSample];
		//int hv = int(l->CeF3HV);
		Float_t valSub=0;
		if( digiChannel <unsigned(nChannels)) 
		{

		string name=Form("tprofile_pedestal_ch%d",digiChannel);
		float sub=0;
		sub=pedHisto[digiChannel]->GetBinContent( digiSampleIndex +1 );
		valSub=digiSampleValue-sub;
		}
		l->digiSampleValueSub[iSample]=valSub;
	}


}

void PedSubAnalysis::Init(LoopAndFill *l1)
{
	BaseAnalysis::Init(l1);
	// check that these branches are input
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
	pedHisto.clear();
	pedFile=TFile::Open(pedestalFileName.c_str());
	for( unsigned int iCh=0;iCh<unsigned(nChannels);++iCh)
	   {
		string name=Form("tprofile_pedestal_ch%d",iCh);
		pedHisto[iCh] = (TProfile*)pedFile->Get(name.c_str());
	   }
	
}

void PedSubAnalysis::Close(){
	if(pedFile!=NULL)pedFile->Close();
	pedFile=NULL;
}
