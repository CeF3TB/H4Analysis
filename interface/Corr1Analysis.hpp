#ifndef PEDSUB_ANALYSIS_H
#define PEDSUB_ANALYSIS_H

#include "interface/BaseAnalysis.hpp"
#include "interface/LooperAndFiller.hpp"

#include "TSpline.h"
#include "TF1.h"

class Corr1Analysis : public BaseAnalysis
{
/* Produce the profile histograms to do the subtraction
 *
 */
TFile *maxAmplFile;
TFile *chIntFile;

// this splines are inverted !!!
vector<TSpline3*> maxAmplSpls;
vector<TSpline3*> chIntSpls;

vector< pair<float,float> > maxAmplSplLimits;
vector< pair<float,float> > chIntSplLimits;

vector<TF1*> maxAmplLines;
vector<TF1*> chIntLines;

public:
	Corr1Analysis(){ 
			maxAmplFile=NULL;
			chIntFile=NULL; 
			chIntFileName="data/chintCorr1.root";
			maxAmplFileName="data/chintCorr1.root";
			inputType=0;
			};
	~Corr1Analysis(){Close();};
	int inputType; // 0 = base; 1=sub
	int nChannels;
	void Init(LoopAndFill *l1); // { l = l1 ; };
	void ClearEvent();
	void AnalyzeEvent();
	void Close();
	string maxAmplFileName;
	string chIntFileName;
	// spl must be inverted
	static double CorrType1(double y,TSpline3 *spl,TF1 *line,double min,double max);
	static double EvalSpline(double x, TSpline3 *spl,double min, double max); // use spline
	static double GetDerivative(TSpline3 *spl, bool min=true);
};

#endif

