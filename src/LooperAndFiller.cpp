#include "interface/LooperAndFiller.hpp"

// ------------------
void LoopAndFill::CopyArray(bool* src,bool* dest,int n)			{if (src==NULL || dest==NULL ) return ;for(int i=0;i<n;++i) dest[i]=src[i];}
void LoopAndFill::CopyArray(int* src,int* dest,int n)			{if (src==NULL || dest==NULL ) return ;for(int i=0;i<n;++i) dest[i]=src[i];}
void LoopAndFill::CopyArray(unsigned int* src,unsigned int* dest,int n)	{if (src==NULL || dest==NULL ) return ;for(int i=0;i<n;++i) dest[i]=src[i];}
void LoopAndFill::CopyArray(float* src,float* dest,int n)		{if (src==NULL || dest==NULL ) return ;for(int i=0;i<n;++i) dest[i]=src[i];}
void LoopAndFill::CopyArray(double* src,double* dest,int n)		{if (src==NULL || dest==NULL ) return ;for(int i=0;i<n;++i) dest[i]=src[i];}
void LoopAndFill::CopyArray(unsigned long long* src,unsigned long long* dest,int n){if (src==NULL || dest==NULL ) return ;for(int i=0;i<n;++i) dest[i]=src[i];}
// ----------------------
void LoopAndFill::CopyVector(vector<bool> *src,vector<bool> *dest)			{if (src==NULL || dest==NULL ) return ;dest->clear();for(unsigned int i=0;i<src->size() ;++i)dest->push_back( src->at(i) );}
void LoopAndFill::CopyVector(vector<int> *src,vector<int> *dest)			{if (src==NULL || dest==NULL ) return ;dest->clear();for(unsigned int i=0;i<src->size() ;++i)dest->push_back( src->at(i) );}
void LoopAndFill::CopyVector(vector<unsigned int> *src,vector<unsigned int> *dest)	{if (src==NULL || dest==NULL ) return ;dest->clear();for(unsigned int i=0;i<src->size() ;++i)dest->push_back( src->at(i) );}
void LoopAndFill::CopyVector(vector<float> *src,vector<float> *dest)			{if (src==NULL || dest==NULL ) return ;dest->clear();for(unsigned int i=0;i<src->size() ;++i)dest->push_back( src->at(i) );}
void LoopAndFill::CopyVector(vector<double> *src,vector<double> *dest)			{if (src==NULL || dest==NULL ) return ;dest->clear();for(unsigned int i=0;i<src->size() ;++i)dest->push_back( src->at(i) );}
void LoopAndFill::CopyVector(vector<unsigned long long> *src,vector<unsigned long long> *dest){if (src==NULL || dest==NULL ) return ;dest->clear();for(unsigned int i=0;i<src->size() ;++i)dest->push_back( src->at(i) );}
// ----------------------

void LoopAndFill::Init(){
	Looper::Init();
	Filler::Init();
}

void LoopAndFill::Close()
{
	Filler::Close();
}

void LoopAndFill::Fill(){
	StartWatch(); 
	// I cannot use virtual Inheritance because we have vectors
	// otherwise I would have done it ;-)
	//copy from this to out->
	//
	out.runNumber	=runNumber;
	out.spillNumber	=spillNumber;
	out.evtNumber	=evtNumber;
	// ---- H4tree ---
	out.evtTimeDist = evtTimeDist;
	out.evtTimeStart = evtTimeStart;
	out.nEvtTimes = nEvtTimes;
	if(activeBranches["evtTime"]) 	CopyArray(evtTime,out.evtTime,nEvtTimes);
	if(activeBranches["evtTimeBoard"])CopyArray(evtTimeBoard,out.evtTimeBoard,nEvtTimes);

	out.nAdcChannels = nAdcChannels;
	out.nTdcChannels= nTdcChannels;
	out.nDigiSamples= nDigiSamples;
	out.nScalerWords= nScalerWords;
	out.nPatterns=    nPatterns;
	out.nTriggerWords=nTriggerWords;

	if(activeBranches["adcBoard"])CopyArray(adcBoard,out.adcBoard,nAdcChannels);
	if(activeBranches["adcChannel"])CopyArray(adcChannel,out.adcChannel,nAdcChannels);
	if(activeBranches["adcData"])CopyArray(adcData,out.adcData,nAdcChannels);

	if(activeBranches["tdcBoard"])CopyArray(tdcBoard,out.tdcBoard,nTdcChannels);
	if(activeBranches["tdcChannel"])CopyArray(tdcChannel,out.tdcChannel,nTdcChannels);
	if(activeBranches["tdcData"])CopyArray(tdcData,out.tdcData,nTdcChannels);

	if(activeBranches["digiFrequency"])CopyArray(digiFrequency,out.digiFrequency,nDigiSamples);
	if(activeBranches["digiGroup"])CopyArray(digiGroup,out.digiGroup,nDigiSamples);
	if(activeBranches["digiChannel"])CopyArray(digiChannel,out.digiChannel,nDigiSamples);
	if(activeBranches["digiSampleIndex"])CopyArray(digiSampleIndex,out.digiSampleIndex,nDigiSamples);
	if(activeBranches["digiSampleValue"])CopyArray(digiSampleValue,out.digiSampleValue,nDigiSamples);
	if(activeBranches["digiSampleValueSub"])CopyArray(digiSampleValueSub,out.digiSampleValueSub,nDigiSamples);
	if(activeBranches["digiBoard"])CopyArray(digiBoard,out.digiBoard,nDigiSamples);

	if(activeBranches["scalerWord"])CopyArray(scalerWord,out.scalerWord,nScalerWords);
	if(activeBranches["scalerBoard"])CopyArray(scalerBoard,out.scalerBoard,nScalerWords);

	if(activeBranches["pattern"])CopyArray(pattern,out.pattern,nPatterns);
	if(activeBranches["patternBoard"])CopyArray(patternBoard,out.patternBoard,nPatterns);
	if(activeBranches["patternChannel"])CopyArray(patternChannel,out.patternChannel,nPatterns);

	if(activeBranches["triggerWords"])CopyArray(triggerWords,out.triggerWords,nTriggerWords);
	if(activeBranches["triggerWordsBoard"])CopyArray(triggerWordsBoard,out.triggerWordsBoard,nTriggerWords);
	// ---- dqmTree ----
 	CopyVector(ADCvalues             ,out.ADCvalues);
 	CopyVector(BGOvalues             ,out.BGOvalues);
 	CopyVector(SCINTvalues           ,out.SCINTvalues);
 	CopyVector(TDCreco               ,out.TDCreco);
 	CopyVector(digi_charge_integrated,out.digi_charge_integrated);
 	CopyVector(digi_max_amplitude    ,out.digi_max_amplitude);
 	CopyVector(digi_pedestal         ,out.digi_pedestal);
 	CopyVector(digi_pedestal_rms     ,out.digi_pedestal_rms);
 	CopyVector(digi_time_at_frac30   ,out.digi_time_at_frac30);
 	CopyVector(digi_time_at_frac50   ,out.digi_time_at_frac50);
 	CopyVector(digi_time_at_max      ,out.digi_time_at_max);
 	CopyVector(HODOX1                ,out.HODOX1);
 	CopyVector(HODOX2                ,out.HODOX2);
 	CopyVector(HODOY1                ,out.HODOY1);
 	CopyVector(HODOY2                ,out.HODOY2);

	out.TableX = TableX;
	out.TableY = TableY;
	out.CeF3HV = CeF3HV;
	out.BGOHV = BGOHV;
	out.BeamEnergy = BeamEnergy;
	out.BeamTilt = BeamTilt;
	out.IsPhysics = IsPhysics;
	
	//  Sub
 	CopyVector(digi_charge_integrated_sub,out.digi_charge_integrated_sub);
 	CopyVector(digi_max_amplitude_sub    ,out.digi_max_amplitude_sub);
 	CopyVector(digi_pedestal_sub         ,out.digi_pedestal_sub);
 	CopyVector(digi_pedestal_rms_sub     ,out.digi_pedestal_rms_sub);

 	CopyVector(digi_charge_integrated_corr1,out.digi_charge_integrated_corr1);
 	CopyVector(digi_max_amplitude_corr1    ,out.digi_max_amplitude_corr1);
 	CopyVector(digi_charge_integrated_corr2,out.digi_charge_integrated_corr2);
 	CopyVector(digi_max_amplitude_corr2    ,out.digi_max_amplitude_corr2);
	// DEBUG
 	CopyVector(digi_pedestal_m         ,out.digi_pedestal_m);
 	CopyVector(digi_pedestal_q         ,out.digi_pedestal_q);

 	CopyVector(digi_pedestal_m0        ,out.digi_pedestal_m0);
 	CopyVector(digi_pedestal_q0        ,out.digi_pedestal_q0);
 	CopyVector(digi_pedestal_m1        ,out.digi_pedestal_m1);
 	CopyVector(digi_pedestal_q1        ,out.digi_pedestal_q1);
 	CopyVector(digi_pedestal_m2        ,out.digi_pedestal_m2);
 	CopyVector(digi_pedestal_q2        ,out.digi_pedestal_q2);
 	CopyVector(digi_pedestal_mu2        ,out.digi_pedestal_mu2);
 	CopyVector(digi_pedestal_mu1        ,out.digi_pedestal_mu1);
 	CopyVector(digi_pedestal_mu0        ,out.digi_pedestal_mu0);
	
	Filler::Fill();	
	StopWatch();
}

void LoopAndFill::Write(){
	Histos::Write();
	Filler::Write();
}
