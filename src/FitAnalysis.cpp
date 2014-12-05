#include "interface/FitAnalysis.hpp"
#include "interface/Waveform.hpp"
#include "interface/WaveformFit.hpp"

#include <iostream>
#include <algorithm>

FitAnalysis::FitAnalysis() 
{
/*
 * [0] : raise time
 * [1] : 
 * [2] : amplitude
 * [3] : left bound- shift
 * ----
 * [4] : pedestal
 * ----
 * [5] : cos amplitude
 * [6] : 1/omega
 * [7] : phi
 * ----
 * [8] : Exp amplitude
 * [9] : decay 2
//	string raise="(x>-[3])*[2]*TMath::Exp(-0.5*TMath::Power((TMath::Log(( (x+[3]>0.001)*(x+[3]) + (x+[3]<=0.001)*0.001  )/[0])/[1]),2)) ";
//	string ped="[4]";
//	string cos="[5]*TMath::Cos(x/[6]+[7])";
//	string fall="[8]*TMath::Exp(-x*[9])";
//	
//	//myfunc=new TF1("myfunc", (raise + "+" + ped + "+" cos + "+" + fall).c_str()  ,10,1000);
//	myfunc=new TF1("myfunc", (raise + "+" + ped + "+"+ cos  ).c_str()  ,-30,200);
//	
//	myfunc->SetParameter(0,20); myfunc->SetParLimits(0,15,30);
//	myfunc->SetParameter(1,0.6); myfunc->SetParLimits(1,0.1,10);
//	myfunc->SetParameter(2,0.95); myfunc->SetParLimits(2,0.5,1.5); //max-ampl
//	myfunc->SetParameter(3,20); myfunc->SetParLimits(3,15,30);
//	myfunc->SetParameter(4,0); myfunc->SetParLimits(4,-10,10);
//	myfunc->SetParameter(5,2); myfunc->SetParLimits(5,0,5);
//	myfunc->SetParameter(6,180); myfunc->SetParLimits(6,160,200);
//	myfunc->SetParameter(7,0); myfunc->SetParLimits(7,0,6.29);
 */
	//minimizer=new TMinuitMinimizer(kCombined);	
}

FitAnalysis::~FitAnalysis(){
	//map<pair<int,int>,TProfile*>::iterator it;
	//for(it=histos.begin();it!=histos.end();it++)
	//	{
	//	delete it->second;
	//	it->second=NULL;
	//	}
	//histos.clear();
	//delete minimizer;
}

void FitAnalysis::Init(LoopAndFill*l1) 
	{ 
	BaseAnalysis::Init(l1);
	//--- Load Profiles
	sort(fwhmBins.begin(),fwhmBins.end() ); // make sure they are sorted
	TFile *f= TFile::Open(plotFileName.c_str() );

	for(int iCh=0;iCh<nChannels ;++iCh)	
	for(int index=0;index<fwhmBins.size() ;++index)
	{
		//tprofile_shape_ch3_fwhm52.50
		TProfile *p=(TProfile*)f->Get(Form("tprofile_shape_ch%d_fwhm%.2f",iCh , fwhmBins[index]) ) ->Clone();
		histos[ pair<int,int>(index,iCh) ] = p;
		cout <<" * Loading Profile"<< p->GetName()<<endl;
	}		
	
	//f->Close(); // If you close the file root destroy the profiles :(
	
	}
void FitAnalysis::ClearEvent(){

        l->digi_charge_integrated_fit ->clear() ;
}
void FitAnalysis::AnalyzeEvent(){
	//---- construct waveform --- first loop
	std::vector<Waveform*> waveform; // one for each channel
	waveform.resize(nChannels);
	for(int iCh=0;iCh<nChannels;++iCh) waveform[iCh] = new Waveform();
	
	std::vector<int> vFreq; vFreq.resize(nChannels);

	for (unsigned int iSample=0;iSample< l->nDigiSamples;iSample++)
		{
 		UInt_t digiGroup   = l->digiGroup[iSample];
 		UInt_t digiChannel = l->digiChannel[iSample] + 8*digiGroup;
 		UInt_t digiSampleIndex = l->digiSampleIndex[iSample];
 		Float_t digiSampleValue = l->digiSampleValue[iSample];
		UInt_t digiFrequency= l->digiFrequency[iSample];
		if (digiChannel >= nChannels) continue;
		vFreq[digiChannel] = digiFrequency;	 // save for later
		
		Float_t digiTime=digiSampleIndex;
		if ( digiFrequency == 0 ) digiTime *= .2e-9; // time was in ns
		if ( digiFrequency == 1 ) digiTime *= .4e-9;
		if ( digiFrequency == 2 ) digiTime *= 1.0e-9;

		if ( digiSampleIndex >=1000 ) digiSampleValue= l->digi_pedestal->at(digiChannel);
    		waveform[ digiChannel ]-> addTimeAndSample(digiTime, digiSampleValue);
		}
	// --- loop over the channels, 2nd loop	
	for(int iCh=0;iCh<nChannels;++iCh){
		// get fwhm info
		float fwhm = l->digi_fall_time_at_frac50->at(iCh) - l->digi_time_at_frac50->at(iCh);
		int index=0;
		for(int i=0;i< fwhmBins.size() ;++i) if( fwhm> fwhmBins[i]) index=i;
		
		Waveform::baseline_informations baseline;	
			baseline.pedestal=l->digi_pedestal->at(iCh);
			baseline.rms=l->digi_pedestal_rms->at(iCh);

		waveform[ iCh ] ->offset( baseline.pedestal);
		waveform[ iCh ] ->rescale(-1);

		Waveform::max_amplitude_informations wave_max;	 // AFTER FFT
			wave_max.max_amplitude=l->digi_max_amplitude->at(iCh)*1.e-9;
			wave_max.time_at_max=l->digi_time_at_max->at(iCh) *1.e-9;
			if( vFreq[iCh]==0 ) wave_max.sample_at_max= wave_max.time_at_max /= 0.2;
			if( vFreq[iCh]==1 ) wave_max.sample_at_max= wave_max.time_at_max /= 0.4;
			if( vFreq[iCh]==2 ) wave_max.sample_at_max= wave_max.time_at_max /= 1.0;
		//select Profile
		TProfile *ref=histos[pair<int,int>(index,iCh) ];
		if( ref == NULL ) 
			{
			cout <<"Reference Histos does not exists:"<<index<<" ch="<<iCh<<endl;
			assert(0);
			}
		// FIT
		double fitcharge=-999;	 // default
		if ( wave_max.max_amplitude >10 && wave_max.sample_at_max>0 && wave_max.sample_at_max<1000)
		{
			WaveformFit::fitWaveform(waveform[iCh],ref,//minimizer,
					1000,1000, // nsamples before/after max for the fit
					wave_max, // wavemax info
					baseline, // rms
					minimizer
					);
			// par0: amplitude, par1 deltat
			const double* par=minimizer->X();		


			if (minimizer->Status() == 0 ) 
				{
				fitcharge= ref->Integral() * par[0];
				}
		}
		l->digi_charge_integrated_fit->push_back( fitcharge );

		//delete minimizer;
		
	} // loop over iCh
}

