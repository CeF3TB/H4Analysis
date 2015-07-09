#include "interface/PromptRecoAnalysis.hpp"
#include <assert.h>


void PromptRecoAnalysis::AnalyzeEvent()
{

  l->SCINTvalues->clear(); l->SCINTvalues->resize(nSCINTChannels);
  l->HODOSMALLvalues->clear(); l->HODOSMALLvalues->resize(nHODOSMALLChannels);
  l->BGOvalues->clear(); l->BGOvalues->resize(nBGOChannels);


  for (unsigned int iAdc=0; iAdc<l->nAdcChannels; iAdc++){
    if (l->adcBoard[iAdc]==0x0B030001){ // 11,3,0,1
      unsigned int ch = l->adcChannel[iAdc];
      if (ch<4 || ch>7) continue;
      //      cout << ch-4 << endl;
      l->SCINTvalues->at(ch-4)=l->adcData[iAdc];
    }
    else if (l->adcBoard[iAdc]==0xB020001){ // 6,3,0,1
      unsigned int ch = l->adcChannel[iAdc];
      //      if (ch==13) continue; // FIX: BGO 14 goes in ADC channel 24 instead of 13
      //      if (ch==24) ch=13;
      if (ch>=nBGOChannels) continue;
      //      cout << ch << " "<< l->adcData[iAdc]<< endl;
      l->BGOvalues->at(ch)=l->adcData[iAdc];
    }
    else if (l->adcBoard[iAdc]==0x6010001){//small hodo for 2015 read out with adc board, channel 2,3,4,5
      unsigned int ch = l->adcChannel[iAdc];
      if(ch<2 || ch>5) continue;
      l->HODOSMALLvalues->at(ch-2)=l->adcData[iAdc];
    }
  }
  
  //  cout << "done adc" << endl;

  FillTdc();

  //  cout << "done tdc" << endl;

  FillHodo();

  //  cout << "done hodo" << endl;


  if (mapdigichannels.size()==0) {
    for (unsigned int iS=0; iS < l->nDigiSamples; iS++){
      unsigned int ch = l->digiChannel[iS];
      unsigned int gr = l->digiGroup[iS];
      unsigned int index = 8*gr+ch;
      if (mapdigichannels.find(index)==mapdigichannels.end()){
	unsigned int my = mapdigichannels.size();
	mapdigichannels.insert(std::make_pair<unsigned int,unsigned int>(index,my));
	cout << "Adding digitizer group="<<gr<<", channel="<<ch<<" as index " << my << endl;
      }
    }
    waveform.clear();
    waveform_noise_sub.clear();
    for (unsigned int i=0; i<mapdigichannels.size(); i++) {
      waveform.push_back(new Waveform());
      waveform_noise_sub.push_back(new Waveform());
    }
    cout << "Found " << mapdigichannels.size() << " digitizer channels" << endl;
  }

  unsigned int nDigiChannels = mapdigichannels.size();

  for (unsigned int i=0; i<nDigiChannels; i++){
    waveform.at(i)->clear();
    waveform_noise_sub.at(i)->clear();
  }
  l->digi_pedestal->clear();
  l->digi_pedestal_rms->clear();
  l->digi_pedestal_noise_sub->clear();
  l->digi_pedestal_noise_sub_rms->clear();
  l->digi_pedestal_bare->clear();
  l->digi_pedestal_bare_rms->clear();
  l->digi_pedestal_bare_noise_sub->clear();
  l->digi_pedestal_bare_noise_sub_rms->clear();
  l->digi_max_amplitude->clear();
  l->digi_charge_integrated->clear();
  l->digi_charge_integrated_frac50->clear();
  l->digi_charge_integrated_frac30->clear();
  l->digi_charge_integrated_frac10->clear();
  l->digi_time_at_max->clear();
  l->digi_time_at_frac30->clear();
  l->digi_time_at_frac50->clear();
  l->digi_fall_time_at_frac30->clear();
  l->digi_fall_time_at_frac50->clear();
  l->digi_value->clear();
  l->digi_value_bare->clear();
  l->digi_value_ch->clear();
  l->digi_value_time->clear();
  l->digi_value_noise_sub->clear();
  l->digi_value_bare_noise_sub->clear();
  l->digi_max_amplitude_bare       ->clear();
  l->digi_time_at_max_bare         ->clear();
  l->digi_charge_integrated_bare   ->clear();
  l->digi_charge_integrated_bare_fast   ->clear();
  l->digi_time_at_frac30_bare      ->clear();
  l->digi_time_at_frac50_bare      ->clear();
  l->digi_fall_time_at_frac30_bare ->clear();
  l->digi_fall_time_at_frac50_bare ->clear();
  l->digi_max_amplitude_bare       ->resize(nDigiChannels,-999); 
  l->digi_time_at_max_bare         ->resize(nDigiChannels,-999); 
  l->digi_charge_integrated_bare   ->resize(nDigiChannels,-999); 
  l->digi_charge_integrated_bare_fast   ->resize(nDigiChannels,-999); 
  l->digi_time_at_frac30_bare      ->resize(nDigiChannels,-999); 
  l->digi_time_at_frac50_bare      ->resize(nDigiChannels,-999); 
  l->digi_fall_time_at_frac30_bare ->resize(nDigiChannels,-999); 
  l->digi_fall_time_at_frac50_bare ->resize(nDigiChannels,-999); 
  l->digi_pedestal->resize(nDigiChannels,-999);
  l->digi_pedestal_rms->resize(nDigiChannels,-999);
  l->digi_pedestal_noise_sub->resize(nDigiChannels,-999);
  l->digi_pedestal_noise_sub_rms->resize(nDigiChannels,-999);
  l->digi_pedestal_bare->resize(nDigiChannels,-999);
  l->digi_pedestal_bare_rms->resize(nDigiChannels,-999);
  l->digi_pedestal_bare_noise_sub->resize(nDigiChannels,-999);
  l->digi_pedestal_bare_noise_sub_rms->resize(nDigiChannels,-999);
  l->digi_max_amplitude->resize(nDigiChannels,-999);
  l->digi_charge_integrated->resize(nDigiChannels,-999);
  l->digi_charge_integrated_frac50->resize(nDigiChannels,-999);
  l->digi_charge_integrated_frac30->resize(nDigiChannels,-999);
  l->digi_charge_integrated_frac10->resize(nDigiChannels,-999);
  l->digi_time_at_max->resize(nDigiChannels,-999);
  l->digi_time_at_frac30->resize(nDigiChannels,-999);
  l->digi_time_at_frac50->resize(nDigiChannels,-999);
  l->digi_fall_time_at_frac30->resize(nDigiChannels,-999);
  l->digi_fall_time_at_frac50->resize(nDigiChannels,-999);

  l->digi_max_amplitude_bare_noise_sub       ->clear();
  l->digi_time_at_max_bare_noise_sub         ->clear();
  l->digi_charge_integrated_bare_noise_sub   ->clear();
  l->digi_max_amplitude_bare_noise_sub       ->resize(nDigiChannels,-999); 
  l->digi_time_at_max_bare_noise_sub         ->resize(nDigiChannels,-999); 
  l->digi_charge_integrated_bare_noise_sub   ->resize(nDigiChannels,-999); 
  l->digi_max_amplitude_noise_sub       ->clear();
  l->digi_time_at_max_noise_sub         ->clear();
  l->digi_charge_integrated_noise_sub   ->clear();
  l->digi_max_amplitude_noise_sub       ->resize(nDigiChannels,-999); 
  l->digi_time_at_max_noise_sub         ->resize(nDigiChannels,-999); 
  l->digi_charge_integrated_noise_sub   ->resize(nDigiChannels,-999); 



  Waveform*  waveform_noise= new Waveform();

  int digiFrequency[nDigiChannels];
  // add informations in the waveform
  for (unsigned int iS=0; iS < l->nDigiSamples; iS++){
    unsigned int index = mapdigichannels[l->digiGroup[iS]*8+l->digiChannel[iS]];
    //    cout << "debug " << l->digiGroup[iS] << " " << l->digiChannel[iS] << " " << index << endl;
    waveform.at(index)->addTimeAndSample(l->digiSampleIndex[iS]*timeSampleUnit(l->digiFrequency[iS]),l->digiSampleValue[iS]);
    //    cout << index << " " << l->digiSampleIndex[iS]*timeSampleUnit(l->digiFrequency[iS]) << " " << l->digiSampleValue[iS] << endl;
    digiFrequency[index] = l->digiFrequency[iS];
    if(index==emptyChannelIndex)waveform_noise->addTimeAndSample(l->digiSampleIndex[iS]*timeSampleUnit(l->digiFrequency[iS]),l->digiSampleValue[iS]);
  }
  l->digi_frequency=l->digiFrequency[0];
  Waveform::baseline_informations wave_pedestal_noise_sub = waveform_noise->baseline(5,34);
  waveform_noise->offset(wave_pedestal_noise_sub.pedestal);
  waveform_noise->rescale(-1);
  
  

  for (unsigned int i=0; i<nDigiChannels; i++) {
    

    //    cout << i << " " << waveform.at(i)->_samples.size() << endl;
    Waveform::baseline_informations wave_pedestal = waveform.at(i)->baseline(5,34);
    waveform.at(i)->offset(wave_pedestal.pedestal);
    waveform.at(i)->rescale(-1);

    l->digi_pedestal_bare->at(i)=wave_pedestal.pedestal;
    l->digi_pedestal_bare_rms->at(i)=wave_pedestal.rms;


    // --------- BARE 
    Waveform::max_amplitude_informations wave_max_bare = waveform.at(i)->max_amplitude(50,900,5);

    l->	digi_max_amplitude_bare->at(i)=wave_max_bare.max_amplitude;
    l-> digi_charge_integrated_bare->at(i)=waveform.at(i)->charge_integrated(4,900);
    l-> digi_charge_integrated_bare_fast->at(i)=waveform.at(i)->charge_integrated(4,230);
    l-> digi_time_at_max_bare->at(i)=wave_max_bare.time_at_max*1.e9;
    l-> digi_time_at_frac30_bare->at(i)=waveform.at(i)->time_at_frac(wave_max_bare.time_at_max-30.e-9,wave_max_bare.time_at_max,0.3,wave_max_bare,7)*1.e9;
    l-> digi_time_at_frac50_bare->at(i)=waveform.at(i)->time_at_frac(wave_max_bare.time_at_max-30.e-9,wave_max_bare.time_at_max,0.5,wave_max_bare,7)*1.e9;
    l->digi_fall_time_at_frac30_bare->at(i)=waveform.at(i)->time_at_frac(wave_max_bare.time_at_max,wave_max_bare.time_at_max+60.e-9,0.3,wave_max_bare,7,false)*1.e9;
    l->digi_fall_time_at_frac50_bare->at(i)=waveform.at(i)->time_at_frac(wave_max_bare.time_at_max,wave_max_bare.time_at_max+60.e-9,0.5,wave_max_bare,7,false)*1.e9;

    for (unsigned int iS=0; iS < 1024; iS++){
      waveform_noise_sub.at(i)->addTimeAndSample(waveform.at(i)->_times[iS],(waveform.at(i)->_samples[iS]-waveform_noise->_samples[iS]));
    }

    Waveform::max_amplitude_informations wave_max_bare_noise_sub = waveform_noise_sub.at(i)->max_amplitude(50,900,5);
    l->	digi_max_amplitude_bare_noise_sub->at(i)=wave_max_bare_noise_sub.max_amplitude;
    l-> digi_charge_integrated_bare_noise_sub->at(i)=waveform_noise_sub.at(i)->charge_integrated(4,900);
    l-> digi_time_at_max_bare_noise_sub->at(i)=wave_max_bare_noise_sub.time_at_max*1.e9;

    Waveform::baseline_informations wave_pedestal_noise_sub = waveform_noise_sub.at(i)->baseline(5,34);
    l->digi_pedestal_bare_noise_sub->at(i)=wave_pedestal_noise_sub.pedestal;
    l->digi_pedestal_bare_noise_sub_rms->at(i)=wave_pedestal_noise_sub.rms;

    // --- SAVE INFO IN OUTFILE
    if (waveform[i]->_samples.size() != waveform[i]->_times.size() )
      {
	cout<<"[PromptRecoAnalysis]::[AnalyzeEvent]::[ERROR] waveform"<<i<<" of event "<<l->runNumber<<"|"<<l->spillNumber<<"|"<<l->evtNumber
	    <<" samples size ("<<waveform[i]->_samples.size()<<") different from times size ("<<waveform[i]->_times.size()<<")"<<endl;
	assert(0);
      }
    for(unsigned int iSample=0;iSample<waveform[i]->_samples.size();++iSample)
      {
	l->digi_value_ch->push_back(i);		
	l->digi_value_bare->push_back(waveform[i]->_samples.at(iSample));		
	l->digi_value_time->push_back(waveform[i]->_times.at(iSample));
	l->digi_value_bare_noise_sub->push_back(waveform_noise_sub[i]->_samples.at(iSample));		
      }
    
    // --------------------- fft!!! --------------------------
    waveform[i]->fft();
    int cut= 60;
    double tau=10;
	if (digiFrequency[i] == 1) { cut=30;tau=5;}
	else {cut=12; tau=2;}
	cut+=12; 

    waveform[i]->inv_fft(cut,tau);
	
    waveform_noise_sub[i]->fft();
    waveform_noise_sub[i]->inv_fft(cut,tau);
   
    // --- end fft ------------------------------------
    
    for(unsigned int iSample=0;iSample<waveform[i]->_samples.size();++iSample)
      {
	l->digi_value->push_back(waveform[i]->_samples.at(iSample));		
	l->digi_value_noise_sub->push_back(waveform_noise_sub[i]->_samples.at(iSample));		
      }
    

    Waveform::max_amplitude_informations wave_max = waveform.at(i)->max_amplitude(50,900,5);
    l->digi_pedestal->at(i)=wave_pedestal.pedestal;
    l->digi_pedestal_rms->at(i)=wave_pedestal.rms;
    l->digi_pedestal_noise_sub->at(i)=wave_pedestal_noise_sub.pedestal;
    l->digi_pedestal_noise_sub_rms->at(i)=wave_pedestal_noise_sub.rms;
    l->digi_max_amplitude->at(i)=wave_max.max_amplitude;
    l->digi_charge_integrated->at(i)=waveform.at(i)->charge_integrated(4,900);
    l->digi_time_at_max->at(i)=wave_max.time_at_max*1.e9;
    l->digi_time_at_frac30->at(i)=waveform.at(i)->time_at_frac(wave_max.time_at_max-30.e-9,wave_max.time_at_max,0.3,wave_max,7)*1.e9;
    l->digi_time_at_frac50->at(i)=waveform.at(i)->time_at_frac(wave_max.time_at_max-30.e-9,wave_max.time_at_max,0.5,wave_max,7)*1.e9;

    l->digi_fall_time_at_frac30->at(i)=waveform.at(i)->time_at_frac(wave_max.time_at_max,wave_max.time_at_max+60.e-9,0.3,wave_max,7,false)*1.e9;
    l->digi_fall_time_at_frac50->at(i)=waveform.at(i)->time_at_frac(wave_max.time_at_max,wave_max.time_at_max+60.e-9,0.5,wave_max,7,false)*1.e9;

    float time_at_frac50	= waveform.at(i)->time_at_frac(wave_max.time_at_max-30.e-9, wave_max.time_at_max ,0.5,wave_max,7 ); 
    float fall_time_at_frac50	= waveform.at(i)->time_at_frac(wave_max.time_at_max , wave_max.time_at_max + 60.e-9 ,0.5,wave_max,7, false);
    float time_at_frac30	= waveform.at(i)->time_at_frac(wave_max.time_at_max-30.e-9, wave_max.time_at_max ,0.3,wave_max,7 ); 
    float fall_time_at_frac30	= waveform.at(i)->time_at_frac(wave_max.time_at_max , wave_max.time_at_max + 60.e-9 ,0.3,wave_max,7, false);
    float time_at_frac10	= waveform.at(i)->time_at_frac(wave_max.time_at_max-60.e-9, wave_max.time_at_max ,0.1,wave_max,7 ); 
    float fall_time_at_frac10	= waveform.at(i)->time_at_frac(wave_max.time_at_max , wave_max.time_at_max + 100.e-9 ,0.1,wave_max,7, false); 

    Waveform::max_amplitude_informations wave_max_noise_sub = waveform_noise_sub.at(i)->max_amplitude(50,900,5);
    l->	digi_max_amplitude_noise_sub->at(i)=wave_max_noise_sub.max_amplitude;
    l-> digi_charge_integrated_noise_sub->at(i)=waveform_noise_sub.at(i)->charge_integrated(4,900);
    l-> digi_time_at_max_noise_sub->at(i)=wave_max_noise_sub.time_at_max*1.e9;

	
    if(time_at_frac50 >-1 && fall_time_at_frac50>-1)
	{
	int t0=waveform.at(i)->find_time_bin(time_at_frac50);
	int t1=waveform.at(i)->find_time_bin(fall_time_at_frac50); 
    	l->digi_charge_integrated_frac50->at(i)=waveform.at(i)->charge_integrated( t0,t1);
	}
    if(time_at_frac30 >-1 && fall_time_at_frac30>-1)
	{
	int t0=waveform.at(i)->find_time_bin(time_at_frac30);
	int t1=waveform.at(i)->find_time_bin(fall_time_at_frac30); 
    	l->digi_charge_integrated_frac30->at(i)=waveform.at(i)->charge_integrated( t0,t1);
	}
    if(time_at_frac10 >-1 && fall_time_at_frac10>-1)
	{
	int t0=waveform.at(i)->find_time_bin(time_at_frac10);
	int t1=waveform.at(i)->find_time_bin(fall_time_at_frac10); 
    	l->digi_charge_integrated_frac10->at(i)=waveform.at(i)->charge_integrated( t0,t1 );
	}
    //    cout << "pedestal " << i << " " << wave_pedestal.pedestal << " " << l->digi_pedestal->at(i) << endl; 
  }
  
  
  return;
}

void PromptRecoAnalysis::Init(LoopAndFill *l1)
{
	cout<<"[PromptRecoAnalysis]::[Init]"<<endl;
	BaseAnalysis::Init(l1);

	FillFiberOrder();

	cout<<"[PromptRecoAnalysis]::[Init]::Done"<<endl;
	return ;
}



void PromptRecoAnalysis::FillTdc(){

  l->TDCreco->clear();
  l->TDCreco->resize(2,-999);
  l->nTdcHits->resize(4,-999);

  std::vector<unsigned int> tdc_readings[4];

  float tdc_recox=-999;
  float tdc_recoy=-999;
  for (unsigned int i=0; i<l->nTdcChannels; i++){
    //    if (l->tdcBoard[i]==0x07030001 && l->tdcChannel[i]<4){
    if (l->tdcBoard[i]==0x07020001 && l->tdcChannel[i]<4){
      tdc_readings[l->tdcChannel[i]].push_back((float)l->tdcData[i]);
    }
  }
  if (tdc_readings[wcXl].size()!=0 && tdc_readings[wcXr].size()!=0){
    float TXl = *std::min_element(tdc_readings[wcXl].begin(),tdc_readings[wcXl].begin()+tdc_readings[wcXl].size());
    float TXr = *std::min_element(tdc_readings[wcXr].begin(),tdc_readings[wcXr].begin()+tdc_readings[wcXr].size());
    float X = (TXr-TXl)*0.005; // = /40./5./10. //position in cm 0.2mm/ns with 25ps LSB TDC
    tdc_recox = X;
  }
  if (tdc_readings[wcYd].size()!=0 && tdc_readings[wcYu].size()!=0){
    float TYd = *std::min_element(tdc_readings[wcYd].begin(),tdc_readings[wcYd].begin()+tdc_readings[wcYd].size());
    float TYu = *std::min_element(tdc_readings[wcYu].begin(),tdc_readings[wcYu].begin()+tdc_readings[wcYu].size());
    float Y = (TYu-TYd)*0.005; // = /40./5./10. //position in cm 0.2mm/ns with 25ps LSB TDC
    tdc_recoy = Y;
  }

  l->nTdcHits->at(0)=tdc_readings[wcXl].size();
  l->nTdcHits->at(1)=tdc_readings[wcXr].size();
  l->nTdcHits->at(2)=tdc_readings[wcYd].size();
  l->nTdcHits->at(3)=tdc_readings[wcYu].size();

  if (tdc_recox>-999 && tdc_recoy>-999){
    l->TDCreco->at(0)=tdc_recox;
    l->TDCreco->at(1)=tdc_recoy;
  }

}


void PromptRecoAnalysis::FillHodo(){

  l->HODOX1->clear();  l->HODOX1->resize(nFibersHodo,false);
  l->HODOX2->clear();  l->HODOX2->resize(nFibersHodo,false);
  l->HODOY1->clear();  l->HODOY1->resize(nFibersHodo,false);
  l->HODOY2->clear();  l->HODOY2->resize(nFibersHodo,false);

  std::vector<bool> *hodo;

  for(unsigned int i=0;i<l->nPatterns;++i){
    //    if(l->patternBoard[i]==0x08030001 || l->patternBoard[i]==0x08030002){
    if(l->patternBoard[i]==0x08020001 || l->patternBoard[i]==0x08020002){
      // here is where the hodoscope mapping is done
      if (l->patternBoard[i]==0x08020001){
	if (l->patternChannel[i]<2) hodo=l->HODOY2;
	else hodo=l->HODOX2;
      }
      else if (l->patternBoard[i]==0x08020002){
	if (l->patternChannel[i]<2) hodo=l->HODOY1;
	else hodo=l->HODOX1;
      }
      std::vector<int> *fiberorder =(bool)( l->patternChannel[i]&0b1) ? &fiberOrderB : &fiberOrderA;
      for (unsigned int j=0; j<32; j++){
	bool thisfibon = (l->pattern[i]>>j)&0b1;
	hodo->at(fiberorder->at(j)-1)=thisfibon;
      }

    }
  }

}


void PromptRecoAnalysis::FillFiberOrder(){

  fiberOrderA.clear();
  fiberOrderB.clear();

  fiberOrderA.push_back(31);
  fiberOrderA.push_back(29);
  fiberOrderA.push_back(23);
  fiberOrderA.push_back(21);
  fiberOrderA.push_back(5);
  fiberOrderA.push_back(7);
  fiberOrderA.push_back(15);
  fiberOrderA.push_back(13);
  fiberOrderA.push_back(1);
  fiberOrderA.push_back(3);
  fiberOrderA.push_back(11);
  fiberOrderA.push_back(9);
  fiberOrderA.push_back(6);
  fiberOrderA.push_back(8);
  fiberOrderA.push_back(16);
  fiberOrderA.push_back(14);
  fiberOrderA.push_back(17);
  fiberOrderA.push_back(27);
  fiberOrderA.push_back(19);
  fiberOrderA.push_back(25);
  fiberOrderA.push_back(24);
  fiberOrderA.push_back(22);
  fiberOrderA.push_back(32);
  fiberOrderA.push_back(30);
  fiberOrderA.push_back(4);
  fiberOrderA.push_back(2);
  fiberOrderA.push_back(12);
  fiberOrderA.push_back(10);
  fiberOrderA.push_back(20);
  fiberOrderA.push_back(18);
  fiberOrderA.push_back(28);
  fiberOrderA.push_back(26);

  fiberOrderB.push_back(54);
  fiberOrderB.push_back(64);
  fiberOrderB.push_back(56);
  fiberOrderB.push_back(62);
  fiberOrderB.push_back(49);
  fiberOrderB.push_back(51);
  fiberOrderB.push_back(59);
  fiberOrderB.push_back(57);
  fiberOrderB.push_back(53);
  fiberOrderB.push_back(55);
  fiberOrderB.push_back(63);
  fiberOrderB.push_back(61);
  fiberOrderB.push_back(45);
  fiberOrderB.push_back(47);
  fiberOrderB.push_back(37);
  fiberOrderB.push_back(39);
  fiberOrderB.push_back(34);
  fiberOrderB.push_back(42);
  fiberOrderB.push_back(36);
  fiberOrderB.push_back(44);
  fiberOrderB.push_back(50);
  fiberOrderB.push_back(52);
  fiberOrderB.push_back(58);
  fiberOrderB.push_back(60);
  fiberOrderB.push_back(38);
  fiberOrderB.push_back(48);
  fiberOrderB.push_back(40);
  fiberOrderB.push_back(46);
  fiberOrderB.push_back(41);
  fiberOrderB.push_back(43);
  fiberOrderB.push_back(33);
  fiberOrderB.push_back(35);

}

float PromptRecoAnalysis::timeSampleUnit(int drs4Freq)
{
  if (drs4Freq == 0)
    return 0.2E-9;
  else if (drs4Freq == 1)
    return 0.4E-9;
  else if (drs4Freq == 2)
    return 1.E-9;
  return -999.;
}
