#include "interface/PlotAnalysis.hpp"
#include <exception>

void PlotAnalysis::Init(LoopAndFill *l1)
{
	cout<<"[PlotAnalysis]::[Init]"<<endl;
	BaseAnalysis::Init(l1);
	for(unsigned int i=0;i<E.size() ;++i)
	   for( unsigned int iCh=0;iCh<unsigned(nChannels);++iCh)
		{
		cout<<"[PlotAnalysis]::[Init] Histo ch="<<iCh<<"/"<<nChannels<<" E="<<E[i]<<endl;
		// ---- BOOKING THE PROFILE HISTOS ---
		l->BookHisto(Form("tprofile_chint_sub_ch%d_E%d",iCh,E[i]),"ch_int_sub vs HV",2000,-0.5,1999.5,"TProfile");
		l->BookHisto(Form("tprofile_maxampl_sub_ch%d_E%d",iCh,E[i]),"max_ampl_sub vs HV",2000,-0.5,1999.5,"TProfile");
		l->BookHisto(Form("tprofile_chint_ch%d_E%d",iCh,E[i]),"ch_int vs HV",2000,-0.5,1999.5,"TProfile");
		l->BookHisto(Form("tprofile_maxampl_ch%d_E%d",iCh,E[i]),"max_ampl vs HV",2000,-0.5,1999.5,"TProfile");
		// --- BOOKING THE HISTO THEMSELVEL TO HAVE BETTER RESULTS
		l->BookHisto(Form("th2d_chint_sub_ch%d_E%d",iCh,E[i])," ch_int_sub vs HV",	40,0.-20.,2000-20,	5000,0-50.,500000-50.	,"TH2D");
		l->BookHisto(Form("th2d_maxampl_sub_ch%d_E%d",iCh,E[i])," max_ampl_sub vs HV",	40,0.-20.,2000-20,	4000,-0.5,3999.5	,"TH2D");
		l->BookHisto(Form("th2d_chint_ch%d_E%d",iCh,E[i])," ch_int vs HV",	40,0.-20.,2000-20,	5000,0-50.,500000-50.	,"TH2D");
		l->BookHisto(Form("th2d_maxampl_ch%d_E%d",iCh,E[i])," max_ampl vs HV",	40,0.-20.,2000-20,	4000,-0.5,3999.5	,"TH2D");
		}
	cout<<"[PlotAnalysis]::[Init] Done"<<endl;
}

bool PlotAnalysis::isActiveRun(int run){
	//cache
	static int lastRun=-1;
	static bool lastValue;
	if (run == lastRun) return lastValue;

	auto it=runs.find(run);
	if (it==runs.end() ){ lastValue=false;return false;}
	if (it->second) {lastValue=true ;return true;}
	else {lastValue=false;return false;}
}

void PlotAnalysis::AnalyzeEvent(){
	if (!runs.empty()  && !isActiveRun(l->runNumber)){return;}
	// table selection
	if (int(l->TableX) !=194 || int(l->TableY) !=254 ) {return;} //center table position

	try{
	for( unsigned int iCh=0;iCh<unsigned(nChannels);++iCh)
		{
		//---- FILL PROFILE ---------
		if ( (*l->digi_charge_integrated_sub)[iCh]>10)
			l->FillProfile(Form("tprofile_chint_sub_ch%d_E%d",iCh,int(l->BeamEnergy)),
				l->CeF3HV,//x
				(*l->digi_charge_integrated_sub)[iCh]//y
				);
		if ( (*l->digi_max_amplitude_sub)[iCh]>10)
			l->FillProfile(Form("tprofile_maxampl_sub_ch%d_E%d",iCh,int(l->BeamEnergy)),
				l->CeF3HV,//x
				(*l->digi_max_amplitude_sub)[iCh]//y
				);
		if ( (*l->digi_charge_integrated)[iCh]>10)
			l->FillProfile(Form("tprofile_chint_ch%d_E%d",iCh,int(l->BeamEnergy)),
				l->CeF3HV,//x
				(*l->digi_charge_integrated)[iCh]//y
				);
		if ( (*l->digi_max_amplitude)[iCh]>10)
			l->FillProfile(Form("tprofile_maxampl_ch%d_E%d",iCh,int(l->BeamEnergy)),
				l->CeF3HV,//x
				(*l->digi_max_amplitude)[iCh]//y
				);
		//---- FILL HISTOS ---------
		l->FillTH2(Form("th2d_chint_sub_ch%d_E%d",iCh,int(l->BeamEnergy)),
				l->CeF3HV,//x
				(*l->digi_charge_integrated_sub)[iCh]//y
				);
		l->FillTH2(Form("th2d_chint_ch%d_E%d",iCh,int(l->BeamEnergy)),
				l->CeF3HV,//x
				(*l->digi_charge_integrated)[iCh]//y
				);
		l->FillTH2(Form("th2d_maxampl_sub_ch%d_E%d",iCh,int(l->BeamEnergy)),
				l->CeF3HV,//x
				(*l->digi_max_amplitude_sub)[iCh]//y
				);
		l->FillTH2(Form("th2d_maxampl_ch%d_E%d",iCh,int(l->BeamEnergy)),
				l->CeF3HV,//x
				(*l->digi_max_amplitude_sub)[iCh]//y
				);
		}
	}
	catch(std::exception &e)
		{
		cout <<"***********************************"<<endl;
		cout <<" Catched "<<e.what()<<endl;
		cout <<" nCh"<<nChannels<<endl;
		cout <<" ma "<< l->digi_max_amplitude->size();
		cout <<" ch "<< l->digi_charge_integrated->size();
		cout <<" ma sub "<< l->digi_max_amplitude_sub->size();
		cout <<" ch sub "<< l->digi_charge_integrated_sub->size();
		cout <<"***********************************"<<endl;
		throw e;
		}
}
