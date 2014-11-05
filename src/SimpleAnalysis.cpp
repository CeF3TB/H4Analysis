#include "interface/SimpleAnalysis.hpp"

void SimpleAnalysis::Init(LoopAndFill *l1)
{
	BaseAnalysis::Init(l1);
	if( ! ( l1->inputBranches["digiChannel"] and l1->inputBranches["digiSampleIndex"] and l1->inputBranches["digiSampleValue"] ) )
		{
		cout <<"[SimpleAnalysis]::[Init]::[ERROR]: input branches are not active"<<endl;	
		cout<<"digiChannel:"<< l1->inputBranches["digiChannel"] <<endl;
		cout<<"digiSampleIndex:"<< l1->inputBranches["digiSampleIndex"] <<endl;
		cout<<"digiSampleValue:"<< l1->inputBranches["digiSampleValue"] <<endl;
		assert(0);
		}
	waveforms.clear();
	for(int i=0;i<nChannels;++i)
		{
		waveforms.push_back(new Waveform());
		}
}

void SimpleAnalysis::ClearEvent()
{
// Clear Event
for(unsigned int i=0;i<waveforms.size() ;++i)
	{
	waveforms[i]->clear();
	}
if (l->digi_max_amplitude != NULL) 	l->digi_max_amplitude->clear();
if (l->digi_pedestal != NULL) 		l->digi_pedestal->clear();
if (l->digi_charge_integrated != NULL) 	l->digi_charge_integrated->clear();
if (l->digi_pedestal_rms != NULL) 	l->digi_pedestal_rms->clear();

}

void SimpleAnalysis::AnalyzeEvent()
{
//ClearEvent(); //called by the python directly
for (unsigned int iSample=0;iSample< l->nDigiSamples;iSample++)
 {
 //fill waveform
 //waveform->baseline(5,44); //use 40 samples between 5-44 to get pedestal and RMS
 UInt_t digiGroup   = l->digiGroup[iSample];
 UInt_t digiChannel = l->digiChannel[iSample] + 8*digiGroup; // update this in order to consider also the group. for us the group is always 0
 UInt_t digiSampleIndex = l->digiSampleIndex[iSample];
 Float_t digiSampleValue = l->digiSampleValue[iSample];
	if (inputType==1) digiSampleValue=l->digiSampleValueSub[iSample];
 UInt_t digiFrequency = l->digiFrequency[iSample];
 if (digiChannel >= nChannels) continue;
 waveforms[ digiChannel ]-> addTimeAndSample(digiSampleIndex *timeSampleUnit(digiFrequency), digiSampleValue);
 }
 for(unsigned int i=0;i<waveforms.size() ;++i)
	{
	Waveform::baseline_informations ped=waveforms[i]->baseline(minPed,maxPed);
	waveforms[i]->offset(ped.pedestal);
	waveforms[i]->rescale(-1);
	float max_ampl=waveforms[i]->max_amplitude(50,300,5).max_amplitude;
	float chg_int =waveforms[i]->charge_integrated(0,900);
	if (inputType==0){
		if (l->digi_max_amplitude != NULL)	{ l->digi_max_amplitude->push_back(max_ampl); }
		if (l->digi_pedestal != NULL)		{ l->digi_pedestal->push_back(ped.pedestal); }
		if (l->digi_pedestal_rms != NULL)	{ l->digi_pedestal_rms->push_back(ped.rms); }
		if (l->digi_charge_integrated  !=NULL) 	{ l->digi_charge_integrated->push_back(chg_int) ;}
		}
	else if (inputType==1)	{
		if (l->digi_max_amplitude_sub != NULL)	{ l->digi_max_amplitude_sub->push_back(max_ampl); }
		if (l->digi_pedestal_sub != NULL)		{ l->digi_pedestal_sub->push_back(ped.pedestal); }
		if (l->digi_pedestal_rms_sub != NULL)	{ l->digi_pedestal_rms_sub->push_back(ped.rms); }
		if (l->digi_charge_integrated_sub  !=NULL) 	{ l->digi_charge_integrated_sub->push_back(chg_int) ;}
		}
	}	
 return;
}
