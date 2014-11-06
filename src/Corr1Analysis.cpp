#include "interface/Corr1Analysis.hpp"

#include "TMath.h"
#include "TGraph.h"


//#define VERBOSE_CORR1

double Corr1Analysis::CorrType1(double y,TSpline3 *spl,TF1 *line,double min,double max)
{
	#ifdef VERBOSE_CORR1
		cout <<"spl:"<<spl->GetName()<<" ln:"<<line->GetName()<<endl;	
	#endif
	if (y<1) return y; // outside correction range  -- protect against 0 correction
	//if (y>TMath::Exp(9)) return y; // outside correction range
	//Corrections are in Log/Log
	double Y=TMath::Log(y);
	#ifdef VERBOSE_CORR1
		cout <<"    y:" << y<<" Y:"<<Y<<endl;
	#endif
	// find x tc spl(x)==y
	//double x=spl->Eval(Y);
	double x=EvalSpline(Y,spl,min,max);
	#ifdef VERBOSE_CORR1
		cout <<"    x:" << x<<" (spline inversion) "<<endl;
	#endif
	// y'=line(x)
	double Y1=line->Eval(x);
	#ifdef VERBOSE_CORR1
		cout <<"    Y1:" << Y1<<" y'="<<TMath::Exp(Y1)<<" (line direct) "<<endl;
	#endif
	// return y'
	return TMath::Exp(Y1);
}

void Corr1Analysis::ClearEvent()
{
	l->digi_charge_integrated_corr1->clear();
	l->digi_max_amplitude_corr1->clear();
}

void Corr1Analysis::AnalyzeEvent()
{

	for( unsigned int iCh=0;iCh<unsigned(nChannels);++iCh)
	      {
		l->digi_charge_integrated_corr1->push_back(CorrType1( 
			  (*l->digi_charge_integrated_sub)[iCh],
			  chIntSpls[iCh],
			  chIntLines[iCh],
			  chIntSplLimits[iCh].first,// min
		 	  chIntSplLimits[iCh].second //max
			  ));
	#ifdef VERBOSE_CORR1
		cout <<"CHINT Ch :"<<iCh<<" Before:"<< (*l->digi_charge_integrated_sub)[iCh] << " After: "<<(*l->digi_charge_integrated_corr1)[iCh]<<endl;
	#endif
		l->digi_max_amplitude_corr1->push_back(CorrType1(
			(*l->digi_max_amplitude_sub)[iCh],
			maxAmplSpls[iCh],
			maxAmplLines[iCh],
			  maxAmplSplLimits[iCh].first,// min
		 	  maxAmplSplLimits[iCh].second //max
			));
		}

}

void Corr1Analysis::Init(LoopAndFill *l1)
{
	BaseAnalysis::Init(l1);
	// check that these branches are input

	chIntFile=TFile::Open(chIntFileName.c_str());
	maxAmplFile=TFile::Open(maxAmplFileName.c_str());
	double x,y;

	for( unsigned int iCh=0;iCh<unsigned(nChannels);++iCh)
	      {
		string gName=Form("tofit_ch%d",iCh);
		string lName=Form("line_ch%d",iCh);
	
		// Get Graphs and convert to splines	
		TGraph *orig=(TGraph*)maxAmplFile->Get(gName.c_str());
		TGraph *invert=new TGraph();
		double min,max;
			for(int i=0;i<orig->GetN();++i)
				{
				orig->GetPoint(i,x,y);
				invert->SetPoint(i,y,x);
				if (i==0 || y<min) min=y;
				if (i==0 || y>max) max=y;
				}
		maxAmplSplLimits.push_back(pair<float,float>(min,max));
		maxAmplSpls.push_back( new TSpline3(Form("ma_spline_%d",iCh), invert  ) );
		orig=(TGraph*) chIntFile->Get(gName.c_str());
		invert->Delete();
		invert= new TGraph(); // add Delete ? 
			for(int i=0;i<orig->GetN();++i)
				{
				orig->GetPoint(i,x,y);
				invert->SetPoint(i,y,x);
				if (i==0 || y<min) min=y;
				if (i==0 || y>max) max=y;
				}
		chIntSplLimits.push_back(pair<float,float>(min,max));
		chIntSpls.push_back( new TSpline3(Form("ci_spline_%d",iCh), invert   )  );
		//Get Lines
		maxAmplLines.push_back( (TF1*)maxAmplFile->Get(lName.c_str())->Clone( Form("ma_%s",lName.c_str())))  ;
		chIntLines.push_back  ( (TF1*)chIntFile->Get(lName.c_str())->Clone(Form("ci_%s",lName.c_str())))  ;
		
	      }
	
}

void Corr1Analysis::Close(){
	if(maxAmplFile!=NULL)maxAmplFile->Close();
	maxAmplFile=NULL;
	if(chIntFile!=NULL)chIntFile->Close();
	chIntFile=NULL;
}

double Corr1Analysis::EvalSpline(double x, TSpline3 *spl,double min, double max)
{
	if (x<min) // linear extrapolation
		{
		double a=spl->Derivative(min);
		double y1=spl->Eval(min);
		double y=a*(x-min) + y1;
		return y;
		}
	if (x>max) // linear extrapolation
		{
		double a=spl->Derivative(max);
		double y1=spl->Eval(max);
		double y=a*(x-max) + y1;
		return y;
		}
	return spl->Eval(x);
}
