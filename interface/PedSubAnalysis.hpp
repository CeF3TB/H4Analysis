#ifndef PEDSUB_ANALYSIS_H
#define PEDSUB_ANALYSIS_H

#include "interface/BaseAnalysis.hpp"
#include "interface/LooperAndFiller.hpp"

class PedSubAnalysis : public BaseAnalysis
{
/* Produce the profile histograms to do the subtraction
 *
 */
TFile *pedFile;
//map<string,TProfile*> pedHisto;
map<string,TProfile*> pedHisto; 
public:
	PedSubAnalysis(){pedFile=NULL;muMin=-2;muMax=6;muDelta=0.2;nCat=3;};
	~PedSubAnalysis(){Close();};
	int nChannels;
	//vector<int> HV;
	float muMin;
	float muMax;
	float muDelta;
	int nCat;
	void Init(LoopAndFill *l1); // { l = l1 ; };
	void ClearEvent();
	void AnalyzeEvent();
	void Close();
	string pedestalFileName;
};

#endif

