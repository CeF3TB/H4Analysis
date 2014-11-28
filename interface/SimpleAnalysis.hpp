#ifndef SIMPLE_ANALYSIS_H
#define SIMPLE_ANALYSIS_H

#include "interface/BaseAnalysis.hpp"
#include "interface/LooperAndFiller.hpp"
#include "interface/Waveform.hpp"


class SimpleAnalysis : public BaseAnalysis
{
/* Produce the max-amplitude and 
 * charge integration branches from the bare trees
 */
vector<Waveform*> waveforms;
public:
	int nChannels;
	int minPed;
	int maxPed;
	int inputType; // 0 = base; 1=sub
	SimpleAnalysis(){minPed=5; maxPed=34; nChannels=0; inputType=0;};
	~SimpleAnalysis(){};
	void Init(LoopAndFill *l1); // { l = l1 ; };
	void AnalyzeEvent();
	void ClearEvent();
	inline float timeSampleUnit(int drs4Freq)
  		{
  		  if (drs4Freq == 0)
  		    return 0.2E-9;
  		  else if (drs4Freq == 1)
  		    return 0.4E-9;
  		  else if (drs4Freq == 2)
  		    return 1.E-9;

  		  return -999.;
  		};
};

#endif
