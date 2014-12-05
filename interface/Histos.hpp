#ifndef HISTOS_H
#define HISTOS_H

#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TChain.h"
#include "TProfile.h"
#include <iostream>
#include "interface/Looper.hpp"
#include "interface/OutFile.hpp"
#include <assert.h>

class Histos : 
	virtual public Looper, 
	virtual public OutFile
{
	map<string,TH1*> histoContainer; // in principle TH2 inheriths from TH1
public:
	//Init & Close inheriths
	int BookHisto(string name, string title, int nBins, double xmin, double xmax,string type="TH1D");	
	int BookHisto(string name, string title, int nBins, double xmin, double xmax, double nBinsY, double ymin, double ymax,string type="TH2D");	
	void FillHisto(string name, double value, double weight=1);
	void FillProfile(string name, double x,double y, double weight=1);
	void FillTH2(string name, double x,double y, double weight=1);
	void Write();
	// TODO, add a check if exist, so it will not be created
	TH1D* GetHisto(string name){
		auto h=histoContainer.find(name);
		if (h == histoContainer.end() ) return NULL;
		return (TH1D*)histoContainer[name];
		}
	void Print();
};

#endif
