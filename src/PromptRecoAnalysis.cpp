#include "interface/PromptRecoAnalysis.hpp"
#include <assert.h>


void PromptRecoAnalysis::AnalyzeEvent()
{

  l->SCINTvalues->clear(); l->SCINTvalues->resize(nSCINTChannels);
  l->BGOvalues->clear(); l->BGOvalues->resize(nBGOChannels);

  for (unsigned int iAdc=0; iAdc<l->nAdcChannels; iAdc++){
    if (l->adcBoard[iAdc]==0x0B030001){ // 11,3,0,1
      unsigned int ch = l->adcChannel[iAdc];
      if (ch<4 || ch>7) continue;
      //      cout << ch-4 << endl;
      l->SCINTvalues->at(ch-4)=l->adcData[iAdc];
    }
    else if (l->adcBoard[iAdc]==0x06030001){ // 6,3,0,1
      unsigned int ch = l->adcChannel[iAdc];
      if (ch==13) continue; // FIX: BGO 14 goes in ADC channel 24 instead of 13
      if (ch==24) ch=13;
      if (ch>=nBGOChannels) continue;
      //      cout << ch << endl;
      l->BGOvalues->at(ch)=l->adcData[iAdc];
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
    for (unsigned int i=0; i<mapdigichannels.size(); i++) waveform.push_back(new Waveform());
    cout << "Found " << mapdigichannels.size() << " digitizer channels" << endl;
  }

  unsigned int nDigiChannels = mapdigichannels.size();

  for (unsigned int i=0; i<nDigiChannels; i++) waveform.at(i)->clear();
  l->digi_pedestal->clear();
  l->digi_pedestal_rms->clear();
  l->digi_max_amplitude->clear();
  l->digi_charge_integrated->clear();
  l->digi_time_at_max->clear();
  l->digi_time_at_frac30->clear();
  l->digi_time_at_frac50->clear();
  l->digi_pedestal->resize(nDigiChannels,-999);
  l->digi_pedestal_rms->resize(nDigiChannels,-999);
  l->digi_max_amplitude->resize(nDigiChannels,-999);
  l->digi_charge_integrated->resize(nDigiChannels,-999);
  l->digi_time_at_max->resize(nDigiChannels,-999);
  l->digi_time_at_frac30->resize(nDigiChannels,-999);
  l->digi_time_at_frac50->resize(nDigiChannels,-999);

  for (unsigned int iS=0; iS < l->nDigiSamples; iS++){
    unsigned int index = mapdigichannels[l->digiGroup[iS]*8+l->digiChannel[iS]];
    //    cout << "debug " << l->digiGroup[iS] << " " << l->digiChannel[iS] << " " << index << endl;
    waveform.at(index)->addTimeAndSample(l->digiSampleIndex[iS]*timeSampleUnit(l->digiFrequency[iS]),l->digiSampleValue[iS]);
    //    cout << index << " " << l->digiSampleIndex[iS]*timeSampleUnit(l->digiFrequency[iS]) << " " << l->digiSampleValue[iS] << endl;
  }
  
  for (unsigned int i=0; i<nDigiChannels; i++) {
    //    cout << i << " " << waveform.at(i)->_samples.size() << endl;
    Waveform::baseline_informations wave_pedestal = waveform.at(i)->baseline(5,44);
    waveform.at(i)->offset(wave_pedestal.pedestal);
    waveform.at(i)->rescale(-1);
    Waveform::max_amplitude_informations wave_max = waveform.at(i)->max_amplitude(50,300,5);
    l->digi_pedestal->at(i)=wave_pedestal.pedestal;
    l->digi_pedestal_rms->at(i)=wave_pedestal.rms;
    l->digi_max_amplitude->at(i)=wave_max.max_amplitude;
    l->digi_charge_integrated->at(i)=waveform.at(i)->charge_integrated(4,900);
    l->digi_time_at_max->at(i)=wave_max.time_at_max*1.e9;
    l->digi_time_at_frac30->at(i)=waveform.at(i)->time_at_frac(wave_max.time_at_max-3.e-9,wave_max.time_at_max,0.3,wave_max,7)*1.e9;
    l->digi_time_at_frac50->at(i)=waveform.at(i)->time_at_frac(wave_max.time_at_max-3.e-9,wave_max.time_at_max,0.5,wave_max,7)*1.e9;
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
  
  std::vector<unsigned int> tdc_readings[4];

  float tdc_recox=-999;
  float tdc_recoy=-999;
  for (unsigned int i=0; i<l->nTdcChannels; i++){
    if (l->tdcBoard[i]==0x07030001 && l->tdcChannel[i]<4){
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
    if(l->patternBoard[i]==0x08030001 || l->patternBoard[i]==0x08030002){
      // here is where the hodoscope mapping is done
      if (l->patternBoard[i]==0x08030001){
	if (l->patternChannel[i]<2) hodo=l->HODOY2;
	else hodo=l->HODOX2;
      }
      else if (l->patternBoard[i]==0x08030002){
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
