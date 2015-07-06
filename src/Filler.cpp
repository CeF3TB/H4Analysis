#define FILLER_CPP

#include "interface/Filler.hpp"
// -------------------------- TEMPLATE SPECIFICATION ----------- 
// --- VECTOR ----
void Filler::SetBranchAddress(string name, vector<unsigned int>** ptr)
{
	if (!isActive(name) ) return;
	cout<<"[Filler]::[SetBranchAddress]Setting address for branch "<<name<<" with type vector<unsigned int>"<<endl;
	//(*ptr) = new vector<unsigned int>();
	fChain->Branch(name.c_str(),"vector<unsigned int>",ptr);	
	out.isFilled[name]=1;
}
void Filler::SetBranchAddress(string name, vector<int>** ptr)
{
	if (!isActive(name) ) return;
	cout<<"[Filler]::[SetBranchAddress]Setting address for branch "<<name<<" with type vector<int>"<<endl;
	//(*ptr) = new vector<int>();
	fChain->Branch(name.c_str(),"vector<int>",ptr);	
	out.isFilled[name]=1;
}
void Filler::SetBranchAddress(string name, vector<float>** ptr)
{
	if (!isActive(name) ) return;
	cout<<"[Filler]::[SetBranchAddress]Setting address for branch "<<name<<" with type vector<float>"<<endl;
	//(*ptr) = new vector<float>();
	fChain->Branch(name.c_str(),"vector<float>",ptr);	
	out.isFilled[name]=1;
}
void Filler::SetBranchAddress(string name, vector<double>** ptr)
{
	if (!isActive(name) ) return;
	cout<<"[Filler]::[SetBranchAddress]Setting address for branch "<<name<<" with type vector<double>"<<endl;
	//(*ptr) = new vector<double>();
	fChain->Branch(name.c_str(),"vector<double>",ptr);	
	out.isFilled[name]=1;
}
void Filler::SetBranchAddress(string name, vector<bool>** ptr)
{
	if (!isActive(name) ) return;
	cout<<"[Filler]::[SetBranchAddress]Setting address for branch "<<name<<" with type vector<bool>"<<endl;
	//(*ptr) = new vector<bool>();
	fChain->Branch(name.c_str(),"vector<bool>",ptr);	
	out.isFilled[name]=1;
}
void Filler::SetBranchAddress(string name, vector<unsigned long long>** ptr)
{
	if (!isActive(name) ) return;
	cout<<"[Filler]::[SetBranchAddress]Setting address for branch "<<name<<" with type vector<unsigned long long>"<<endl;
	//(*ptr) = new vector<unsigned long long>();
	fChain->Branch(name.c_str(),"vector<unsigned long long>",ptr);
	out.isFilled[name]=1;
}
// ---- VALUES ---
void Filler::SetBranchAddress(string name, unsigned int* ptr)
{
	if (!isActive(name) ) return;
	cout<<"[Filler]::[SetBranchAddress]Setting address for branch "<<name<<" with type uint"<<endl;
	fChain->Branch(name.c_str(),ptr,Form("%s/i",name.c_str()));	
	out.isFilled[name]=1;
}
void Filler::SetBranchAddress(string name, unsigned long long* ptr)
{
	if (!isActive(name) ) return;
	cout<<"[Filler]::[SetBranchAddress]Setting address for branch "<<name<<" with type u long long"<<endl;
	fChain->Branch(name.c_str(),ptr,Form("%s/l",name.c_str()));	
	out.isFilled[name]=1;
}
void Filler::SetBranchAddress(string name, int* ptr)
{
	if (!isActive(name) ) return;
	cout<<"[Filler]::[SetBranchAddress]Setting address for branch "<<name<<" with type int"<<endl;
	fChain->Branch(name.c_str(),ptr,Form("%s/I",name.c_str()));	
	out.isFilled[name]=1;
}
void Filler::SetBranchAddress(string name, float* ptr)
{
	if (!isActive(name) ) return;
	cout<<"[Filler]::[SetBranchAddress]Setting address for branch "<<name<<" with type float"<<endl;
	fChain->Branch(name.c_str(),ptr,Form("%s/F",name.c_str()));	
	out.isFilled[name]=1;
}
void Filler::SetBranchAddress(string name, double* ptr)
{
	if (!isActive(name) ) return;
	cout<<"[Filler]::[SetBranchAddress]Setting address for branch "<<name<<" with type double"<<endl;
	fChain->Branch(name.c_str(),ptr,Form("%s/D",name.c_str()));	
	out.isFilled[name]=1;
}
// ------------------------------------------------------------- 
// ---- ARRAYS ---
void Filler::SetBranchArray(string name, unsigned int* ptr,string n)
{
	if (!isActive(name) ) return;
	cout<<"[Filler]::[SetBranchArray]Setting address for branch "<<name<<" with type uint"<<endl;
	fChain->Branch(name.c_str(),ptr,Form("%s[%s]/i",name.c_str(),n.c_str()));	
	out.isFilled[name]=1;
}
void Filler::SetBranchArray(string name, unsigned long long* ptr,string n)
{
	if (!isActive(name) ) return;
	cout<<"[Filler]::[SetBranchArray]Setting address for branch "<<name<<" with type u long long"<<endl;
	fChain->Branch(name.c_str(),ptr,Form("%s[%s]/l",name.c_str(),n.c_str()));	
	out.isFilled[name]=1;
}
void Filler::SetBranchArray(string name, int* ptr,string n)
{
	if (!isActive(name) ) return;
	cout<<"[Filler]::[SetBranchArray]Setting address for branch "<<name<<" with type int"<<endl;
	fChain->Branch(name.c_str(),ptr,Form("%s[%s]/I",name.c_str(),n.c_str()));	
	out.isFilled[name]=1;
}
void Filler::SetBranchArray(string name, float* ptr,string n)
{
	if (!isActive(name) ) return;
	cout<<"[Filler]::[SetBranchArray]Setting address for branch "<<name<<" with type float"<<endl;
	fChain->Branch(name.c_str(),ptr,Form("%s[%s]/F",name.c_str(),n.c_str()));	
	out.isFilled[name]=1;
}
void Filler::SetBranchArray(string name, double* ptr,string n)
{
	if (!isActive(name) ) return;
	cout<<"[Filler]::[SetBranchArray]Setting address for branch "<<name<<" with type double"<<endl;
	fChain->Branch(name.c_str(),ptr,Form("%s[%s]/D",name.c_str(),n.c_str()));	
	out.isFilled[name]=1;
}

void Filler::Init()
{
	//fOut=TFile::Open(outFileName.c_str(),"RECREATE");
	OutFile::Init();
	fOut->cd();
	cout<<"[Filler]::[Init]:Creating tree with name: "<<outTreeName<<endl;
	fChain=new TTree(outTreeName.c_str(),outTreeName.c_str());
	SetBranches();
}

void Filler::Write(){
	cout<<"[Filler]::[Write] Tree "<<endl;
	SetAliases();
	fOut->cd();
	fChain->Print();
	fChain->Write();
	//fOut->Close();
}

void Filler::SetBranches()
{
	if ( !activeBranches.empty() )
		{
		// disable all the branches
		//fChain->SetBranchStatus("*",0);
		}
	// here will be setted if in activeBranches
	SetBranchesCommon();
	SetBranchesH4tree();
	SetBranchesOuttree();
	return;
}

void Filler::SetBranchesH4tree()
{
	//SetBranchAddress("runNumber"	,&out.runNumber );
	//SetBranchAddress("spillNumber"	,&out.spillNumber );
	//SetBranchAddress("evtNumber"	,&out.evtNumber );
	SetBranchAddress("evtTimeDist"	,&out.evtTimeDist );
	SetBranchAddress("evtTimeStart"	,&out.evtTimeStart );
	SetBranchAddress("nEvtTimes"	,&out.nEvtTimes );
	SetBranchArray("evtTime"	,&out.evtTime[0] 	,"nEvtTimes");
	SetBranchArray("evtTimeBoard"	,&out.evtTimeBoard[0]	,"nEvtTimes");

	SetBranchAddress("nAdcChannels"	,&out.nAdcChannels);
	SetBranchArray("adcBoard"	,&out.adcBoard[0]	,"nAdcChannels");
	SetBranchArray("adcChannel"	,&out.adcChannel[0]	,"nAdcChannels");
	SetBranchArray("adcData"	,&out.adcData[0]	,"nAdcChannels");
                                          
	SetBranchAddress("nTdcChannels"	,&out.nTdcChannels);
	SetBranchArray("tdcBoard"	,&out.tdcBoard[0]	,"nTdcChannels");
	SetBranchArray("tdcChannel"	,&out.tdcChannel[0]	,"nTdcChannels");
	SetBranchArray("tdcData"	,&out.tdcData[0]	,"nTdcChannels");
                                         
	SetBranchAddress("nDigiSamples"	,&out.nDigiSamples);
	SetBranchArray("digiFrequency",&out.digiFrequency[0]	,"nDigiSamples");
	SetBranchArray("digiGroup"	,&out.digiGroup[0]	,"nDigiSamples");
	SetBranchArray("digiChannel"	,&out.digiChannel[0]	,"nDigiSamples");
	SetBranchArray("digiSampleIndex"	,&out.digiSampleIndex[0],"nDigiSamples");
	SetBranchArray("digiSampleValue"	,&out.digiSampleValue[0],"nDigiSamples");
	SetBranchArray("digiSampleValueSub"	,&out.digiSampleValueSub[0],"nDigiSamples");
	SetBranchArray("digiBoard"	,&out.digiBoard[0]	,"nDigiSamples");
                                          
	SetBranchAddress("nScalerWords"	,&out.nScalerWords);
	SetBranchArray("scalerWord"	,&out.scalerWord[0]	,"nScalerWords");
	SetBranchArray("scalerBoard"	,&out.scalerBoard[0]	,"nScalerWords");
                                         
	SetBranchAddress("nPatterns"	,&out.nPatterns);
	SetBranchArray("pattern"	,&out.pattern[0]	,"nPatterns");
	SetBranchArray("patternBoard"	,&out.patternBoard[0]	,"nPatterns");
	SetBranchArray("patternChannel",&out.patternChannel[0]	,"nPatterns");
                                               
	SetBranchAddress("nTriggerWords",&out.nTriggerWords);
	SetBranchArray("triggerWords"	,&out.triggerWords[0]	,"nTriggerWords");
	SetBranchArray("triggerWordsBoard",&out.triggerWordsBoard[0],"nTriggerWords");


}

void Filler::SetBranchesCommon(){
	SetBranchAddress("runNumber"	,&out.runNumber);
	SetBranchAddress("spillNumber"	,&out.spillNumber);
	SetBranchAddress("evtNumber"	,&out.evtNumber);

}

void Filler::SetBranchesOuttree(){
	SetBranchAddress("ADCvalues"	,&out.ADCvalues);
	SetBranchAddress("BGOvalues"	,&out.BGOvalues);
	SetBranchAddress("SCINTvalues"	,&out.SCINTvalues);
	SetBranchAddress("HODOSMALLvalues"	,&out.HODOSMALLvalues);
	SetBranchAddress("TDCreco"	,&out.TDCreco  );
	SetBranchAddress("digi_charge_integrated"	,&out.digi_charge_integrated);
	SetBranchAddress("digi_charge_integrated_frac50"	,&out.digi_charge_integrated_frac50);
	SetBranchAddress("digi_charge_integrated_frac30"	,&out.digi_charge_integrated_frac30);
	SetBranchAddress("digi_charge_integrated_frac10"	,&out.digi_charge_integrated_frac10);
	SetBranchAddress("digi_max_amplitude"		,&out.digi_max_amplitude);
	SetBranchAddress("digi_pedestal"		,&out.digi_pedestal);
	SetBranchAddress("digi_pedestal_rms"		,&out.digi_pedestal_rms);
	SetBranchAddress("digi_pedestal_noise_sub"		,&out.digi_pedestal_noise_sub);
	SetBranchAddress("digi_pedestal_noise_sub_rms"		,&out.digi_pedestal_noise_sub_rms);
	SetBranchAddress("digi_pedestal_bare"		,&out.digi_pedestal_bare);
	SetBranchAddress("digi_pedestal_bare_rms"		,&out.digi_pedestal_bare_rms);
	SetBranchAddress("digi_pedestal_bare_noise_sub"		,&out.digi_pedestal_bare_noise_sub);
	SetBranchAddress("digi_pedestal_bare_noise_sub_rms"		,&out.digi_pedestal_bare_noise_sub_rms);
	SetBranchAddress("digi_time_at_frac30"		,&out.digi_time_at_frac30);
	SetBranchAddress("digi_time_at_frac50"		,&out.digi_time_at_frac50);
	SetBranchAddress("digi_fall_time_at_frac30"		,&out.digi_fall_time_at_frac30);
	SetBranchAddress("digi_fall_time_at_frac50"		,&out.digi_fall_time_at_frac50);
	SetBranchAddress("digi_time_at_max"		,&out.digi_time_at_max);
	SetBranchAddress("HODOX1",&out.HODOX1   );
	SetBranchAddress("HODOX2",&out.HODOX2   );
	SetBranchAddress("HODOY1",&out.HODOY1   );
	SetBranchAddress("HODOY2",&out.HODOY2   );

	SetBranchAddress("TableX",&out.TableX);
	SetBranchAddress("TableY",&out.TableY);
	SetBranchAddress("CeF3HV",&out.CeF3HV);
	SetBranchAddress("BGOHV",&out.BGOHV);
	SetBranchAddress("BeamEnergy",&out.BeamEnergy);
	SetBranchAddress("BeamTilt",&out.BeamTilt);
	SetBranchAddress("IsPhysics",&out.IsPhysics);
	SetBranchAddress("nTdcHits",&out.nTdcHits);

	SetBranchAddress("digi_charge_integrated_sub"	,&out.digi_charge_integrated_sub);
	SetBranchAddress("digi_max_amplitude_sub"	,&out.digi_max_amplitude_sub);
	SetBranchAddress("digi_pedestal_sub"		,&out.digi_pedestal_sub);
	SetBranchAddress("digi_pedestal_rms_sub"	,&out.digi_pedestal_rms_sub);

	SetBranchAddress("digi_charge_integrated_corr1"	,&out.digi_charge_integrated_corr1);
	SetBranchAddress("digi_max_amplitude_corr1"	,&out.digi_max_amplitude_corr1);

	SetBranchAddress("digi_charge_integrated_corr2"	,&out.digi_charge_integrated_corr2);
	SetBranchAddress("digi_max_amplitude_corr2"	,&out.digi_max_amplitude_corr2);

	SetBranchAddress("digi_pedestal_m",&out.digi_pedestal_m);
	SetBranchAddress("digi_pedestal_q",&out.digi_pedestal_q);

	SetBranchAddress("digi_pedestal_m0",&out.digi_pedestal_m0);
	SetBranchAddress("digi_pedestal_q0",&out.digi_pedestal_q0);
	SetBranchAddress("digi_pedestal_m1",&out.digi_pedestal_m1);
	SetBranchAddress("digi_pedestal_q1",&out.digi_pedestal_q1);
	SetBranchAddress("digi_pedestal_m2",&out.digi_pedestal_m2);
	SetBranchAddress("digi_pedestal_q2",&out.digi_pedestal_q2);

	SetBranchAddress("digi_pedestal_mu0"		,&out.digi_pedestal_mu0);
	SetBranchAddress("digi_pedestal_mu1"		,&out.digi_pedestal_mu1);
	SetBranchAddress("digi_pedestal_mu2"		,&out.digi_pedestal_mu2);
	// -- nofft
	SetBranchAddress("digi_max_amplitude_bare"	,&out.digi_max_amplitude_bare       );	
	SetBranchAddress("digi_time_at_max_bare"	,&out.digi_time_at_max_bare         );	
	SetBranchAddress("digi_charge_integrated_bare"	,&out.digi_charge_integrated_bare   );	
	SetBranchAddress("digi_max_amplitude_bare_noise_sub"	,&out.digi_max_amplitude_bare_noise_sub       );	
	SetBranchAddress("digi_time_at_max_bare_noise_sub"	,&out.digi_time_at_max_bare_noise_sub         );	
	SetBranchAddress("digi_charge_integrated_bare_noise_sub"	,&out.digi_charge_integrated_bare_noise_sub   );	
	SetBranchAddress("digi_max_amplitude_noise_sub"	,&out.digi_max_amplitude_noise_sub       );	
	SetBranchAddress("digi_time_at_max_noise_sub"	,&out.digi_time_at_max_noise_sub         );	
	SetBranchAddress("digi_charge_integrated_noise_sub"	,&out.digi_charge_integrated_noise_sub   );	
	SetBranchAddress("digi_time_at_frac30_bare"	,&out.digi_time_at_frac30_bare      );	
	SetBranchAddress("digi_time_at_frac50_bare"	,&out.digi_time_at_frac50_bare      );	
	SetBranchAddress("digi_fall_time_at_frac30_bare",&out.digi_fall_time_at_frac30_bare );	
	SetBranchAddress("digi_fall_time_at_frac50_bare",&out.digi_fall_time_at_frac50_bare );	

	// --- FFT
	SetBranchAddress("digi_value"			,&out.digi_value);
	SetBranchAddress("digi_value_bare"			,&out.digi_value_bare);
	SetBranchAddress("digi_value_ch"		,&out.digi_value_ch);
	SetBranchAddress("digi_value_time"		,&out.digi_value_time);
	SetBranchAddress("digi_value_noise_sub"			,&out.digi_value_noise_sub);
	SetBranchAddress("digi_value_bare_noise_sub"			,&out.digi_value_bare_noise_sub);
	// ---TOY
	SetBranchAddress("ToyResolution",&out.ToyResolution );
	SetBranchAddress("ToyNoise",&out.ToyNoise );
	// --- FIT
	SetBranchAddress("digi_charge_integrated_fit"	,&out.digi_charge_integrated_fit);
}

bool Filler::isActive(string name)
{
	map<string,int>::iterator it;
	it=activeBranches.find(name);
	if (it == activeBranches.end() ) return false;
	else return activeBranches[name];
}

void Filler::Fill( )
{ 
	//cout<<"[Filler]::[Fill] Fill tree:"<<fChain->GetEntries()<<endl;
	fChain->Fill() ;
}

void Filler::SetAliases(){
	//fChain->SetAlias("posY","TDCreco[1]+TableY");
	//fChain->SetAlias("posX","TDCreco[0]+TableX");
	fChain->SetAlias("posY","-TDCreco[1]+TableY"); // this seems to be the right one
	fChain->SetAlias("posX","-TDCreco[0]+TableX");
	fChain->SetAlias("rho0","TMath::Sqrt(pow(posY-254+12,2) + pow(posX-194+12,2))");
	fChain->SetAlias("phi0","TMath::ATan( (posY-254+12)/(posX-194+12))+((posX-194+12)<0)*3.1415");
	fChain->SetAlias("rho","TMath::Sqrt(pow(posY-254,2) + pow(posX-194,2))");
	fChain->SetAlias("phi","TMath::ATan( (posY-254)/(posX-194))+((posX-194)<0)*3.1415");
	
	fChain->SetAlias("rho1","TMath::Sqrt(pow(posY-254+12,2) + pow(posX-194-12,2))");
	fChain->SetAlias("phi1","TMath::ATan( (posY-254+12)/(posX-194-12))+((posX-194-12)<0)*3.1415");

	fChain->SetAlias("rho2","TMath::Sqrt(pow(posY-254-12,2) + pow(posX-194-12,2))");
	fChain->SetAlias("phi2","TMath::ATan( (posY-254-12)/(posX-194-12))+((posX-194-12)<0)*3.1415");

	fChain->SetAlias("rho3","TMath::Sqrt(pow(posY-254-12,2) + pow(posX-194+12,2))");
	fChain->SetAlias("phi3","TMath::ATan( (posY-254-12)/(posX-194+12))+((posX-194+12)<0)*3.1415");


	fChain->SetAlias("fwhm0","digi_fall_time_at_frac50[0]-digi_time_at_frac50[0]");
	fChain->SetAlias("fwhm1","digi_fall_time_at_frac50[1]-digi_time_at_frac50[1]");
	fChain->SetAlias("fwhm2","digi_fall_time_at_frac50[2]-digi_time_at_frac50[2]");
	fChain->SetAlias("fwhm3","digi_fall_time_at_frac50[3]-digi_time_at_frac50[3]");
}
