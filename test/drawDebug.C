#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TCanvas.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

//root [3] outputTree->Print("*digi*")
// --- digiFrequency 
// --- digiGroup 
// --- digiChannel 
// --- digiSampleIndex 
// --- digiSampleValue 
// --- digiSampleValueSub 
// --- digiBoard 
// --- digi_charge_integrated 
// --- digi_max_amplitude 
// --- digi_pedestal 
// --- digi_pedestal_rms 
// --- digi_time_at_frac30 
// --- digi_time_at_frac50 
// --- digi_time_at_max 
// --- digi_charge_integrated_sub 
// --- digi_max_amplitude_sub 
// --- digi_pedestal_sub 
// --- digi_pedestal_rms_sub 
// --- digi_charge_integrated_corr1 
// --- digi_max_amplitude_corr1 
// --- digi_charge_integrated_corr2 
// --- digi_max_amplitude_corr2 
//
int DrawEvent(TChain *t,string event,string opt="")
{
	cout<< "DEBUG 1: start event "<<event<<endl;
	int digiChannel=0;
	int spill=4;
	int run=398;
	string baseSelection=Form("digiChannel==%d && digiGroup==0 && runNumber==%d && spillNumber==%d",digiChannel,run,spill);
	TCanvas *c;
	if (opt.find("SAME") == string::npos)
		{
		cout<< "DEBUG 2: creating new canvas "<<endl;
		c=new TCanvas("c","c",800,800);
		TH2D *dummy=new TH2D("dummy","dummy",1024,0-.5,1024-.5,1000,-5000,5000);
		dummy->Draw("AXIS");
		dummy->Draw("AXIS X+ Y+ SAME");
		}
	//else c=gPad;

	// ---- GET BASIC SHAPE	
	//
	string gName= "shape_"+event ;
	TH2D *g=new TH2D(gName.c_str(),"shape",1024,0-.5,1024-.5,10000,-5000,5000) ;
	cout<< "DEBUG 3: Drawing Graph "<< gName <<endl;
	t->Draw(		Form("digiSampleValue:digiSampleIndex>>%s",gName.c_str()),
				Form("%s && evtNumber==%s",baseSelection.c_str(),event.c_str()), "goff"
		);	
		g->SetMarkerStyle(23);g->SetMarkerSize(0.8);g->SetMarkerColor(kGreen+2);
	// ---- GET SUB SHAPE
	string gNameSub= "shapeSub_"+event ;
	TH2D *gSub=new TH2D(gNameSub.c_str(),"shape sub",1024,0-.5,1024-.5,10000,-5000,5000) ;
	cout<< "DEBUG 4: Drawing Graph "<< gNameSub <<endl;
	t->Draw(		Form("digiSampleValueSub:digiSampleIndex>>%s",gNameSub.c_str()),
				Form("%s && evtNumber==%s",baseSelection.c_str(),event.c_str()), "goff"
		);	
		gSub->SetMarkerStyle(20);

	// GET PEDESTAL SUB
	vector<float> *digi_pedestal_sub=new vector<float>;
	vector<float> *digi_pedestal_rms_sub=new vector<float>;
	vector<float> *digi_pedestal=new vector<float>;
	vector<float> *digi_pedestal_rms=new vector<float>;
	UInt_t evtNumber;
	UInt_t spillNumber;
	UInt_t runNumber;

	t->SetBranchAddress("digi_pedestal_sub",&digi_pedestal_sub);
	t->SetBranchAddress("digi_pedestal_rms_sub",&digi_pedestal_rms_sub);
	t->SetBranchAddress("digi_pedestal",&digi_pedestal);
	t->SetBranchAddress("digi_pedestal_rms",&digi_pedestal_rms);
	t->SetBranchAddress("evtNumber",&evtNumber);
	t->SetBranchAddress("spillNumber",&spillNumber);
	t->SetBranchAddress("runNumber",&runNumber);


	float ped=-999;
	float pedrms=-999;
	
	float ped0=-999;
	float pedrms0=-999;

	cout<< "DEBUG 5: Getting PED INFO "<< gNameSub <<endl;
	//for(int iEntry=0;iEntry<t->GetEntries();iEntry++)
	//{
	//	t->GetEntry(iEntry);
	//	if( evtNumber != atoi(event.c_str() ) ) continue;	
	//	if( runNumber != run ) continue;
	//	if( spillNumber != spill ) continue;
	//	cout<< "DEBUG 6: ONLY ONE!!!! "<< iEntry <<endl;
	//	ped=digi_pedestal_sub[digiChannel];
	//	pedrms=digi_pedestal_rms_sub[digiChannel];

	//	ped0=digi_pedestal[digiChannel];
	//	pedrms0=digi_pedestal_rms[digiChannel];
	//}
	
	//cout<< "DEBUG 7: Draw " <<endl;
	//TGraph *pedLine=new TGraph(); pedLine->SetName("pedLine");
	//TGraph *pedLineRMS=new TGraph(); pedLine->SetName("pedLineRMS");
	//
	//pedLine->SetPoint(0,0,ped); pedLine->SetPoint(1,1024,ped);
	//	pedLine->SetLineWidth(2);
	//pedLineRMS->SetPoint(0,0,ped-pedrms);pedLineRMS->SetPoint(1,0,ped+pedrms);
	//	pedLineRMS->SetPoint(2,1024,ped+pedrms);pedLineRMS->SetPoint(3,1024,ped-pedrms);
	//	pedLineRMS->SetLineColor(kRed); pedLineRMS->SetFillColor(kRed-7);
	//
	//pedLineRMS->Draw("LF SAME");
	//pedLine->Draw("L SAME");

	if (opt.find("SAME") == string::npos)
		gSub->SetMarkerColor(kMagenta);
		
	gSub->Draw("P SAME");
	g->Draw("P SAME");
	
	//cout<<" -------------------------------"<<endl;
	//cout<<" ped0 :"<<ped0<<" +/- "<<pedrms0<<endl;
	//cout<<" pedsub :"<<ped<<" +/- "<<pedrms<<endl;
	//cout<<" -------------------------------"<<endl;
	
	cout<< "DEBUG 8: Return " <<endl;
	return 0;
	
}

int drawDebug()
{
	TChain *t=new TChain("outputTree");	
	cout<<" Adinnig "<< t->Add("physics_debug/output*.root") <<" files"<<endl;;
	// mean=4000
//	DrawEvent(t,"806",""); // spill=4 run =398
	DrawEvent(t,"826","");
	// mean=7000
	//DrawEvent(t,"550","SAME");
	DrawEvent(t,"589","SAME");
	return 0;
}


