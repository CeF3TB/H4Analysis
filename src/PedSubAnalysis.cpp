#include "interface/PedSubAnalysis.hpp"
#include "interface/PedestalAnalysis.hpp"
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
		float 	dmu2= l->digi_pedestal_mu2->at(digiChannel) - l->digi_pedestal_mu0->at(digiChannel);
		float 	dmu1= l->digi_pedestal_mu1->at(digiChannel) - l->digi_pedestal_mu0->at(digiChannel);
		char 	cat	=PedestalAnalysis::computeCategory(digiChannel,dmu2,dmu1);
		float mu=dmu1;
		int n=  TMath::Floor((mu-muMin)/muDelta); 
		mu=muMin+n*muDelta;
		if (mu>muMax || mu<muMin) mu=0; 

		//find dmu and cat
		string name=Form("tprofile_pedestal_ch%d_cat%c_dmu%.1f",digiChannel,cat,mu);
		float sub=0;
		sub=pedHisto[name]->GetBinContent( digiSampleIndex +1 );
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
	for(unsigned int iCat=0;iCat<unsigned(nCat);++iCat)
	{
	for( unsigned int iCh=0;iCh<unsigned(nChannels);++iCh)
	   {
	   for(float iDMu=muMin;iDMu<=muMax;iDMu+=muDelta)
	      {
		string name=Form("tprofile_pedestal_ch%d_cat%c_dmu%.1f",iCh,'A'+iCat,iDMu);
		pedHisto[name] = (TProfile*)pedFile->Get(name.c_str());
	      }
	    string name=Form("tprofile_pedestal_ch%d_cat%c_dmu0.0",iCh,'A'+iCat);
	    pedHisto[name] = (TProfile*)pedFile->Get(name.c_str());
	   }
	}
	
}

void PedSubAnalysis::Close(){
	if(pedFile!=NULL)pedFile->Close();
	pedFile=NULL;
}
