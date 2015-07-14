#ifndef PEDESTAL_ANALYSIS_H
#define PEDESTAL_ANALYSIS_H

#include "interface/BaseAnalysis.hpp"
#include "interface/LooperAndFiller.hpp"
#include "interface/Waveform.hpp"
#include <algorithm>
#include <map>

#define wcXl 0
#define wcXr 1
#define wcYd 3
#define wcYu 2

#define nFibersHodo 64
#define nBGOChannels 8
#define nSCINTChannels 0
#define nHODOSMALLChannels 8
#define emptyChannelIndex 4

class PromptRecoAnalysis : public BaseAnalysis
{

public:
	PromptRecoAnalysis(){};
	~PromptRecoAnalysis(){};
	void Init(LoopAndFill *l1); // { l = l1 ; };
	void AnalyzeEvent();
  void FillTdc();
  std::vector<int> fiberOrderA;
  std::vector<int> fiberOrderB;
  void FillHodo();
  void FillFiberOrder();

  std::map<unsigned int,unsigned int> mapdigichannels;
  std::vector<Waveform*> waveform;
  std::vector<Waveform*> waveform_noise_sub;
  std::vector<Waveform*> waveform_noise_bare_sub;
  float timeSampleUnit(int drs4Freq);

};

#endif

