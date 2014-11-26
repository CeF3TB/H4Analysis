#include "interface/Looper.hpp"

void Looper::AddToChain(string fileName)
{
	cout<<"Added "<< fChain->Add(fileName.c_str()) << " elements to the chain:"<<fileName<<endl;
	return;
}

void Looper::Init()
{
	fChain=new TChain(chainName.c_str());
	//for(auto br : inputBranches)
	//for(map<string,int>::iterator br=inputBranches.begin();br!=inputBranches.end();br++){if(br->second)cout<<"[Looper]::[Init] Contains Branch:"<<br->first<<endl;}
	SetBranches(chainName);
}

void Looper::SetBranches(string tree)
{
	if ( !inputBranches.empty() )
		{
		// disable all the branches
		fChain->SetBranchStatus("*",0);
		}
	SetBranchesCommon();
	if (tree== "H4tree")
		SetBranchesH4tree();
	if (tree== "outputTree")
		SetBranchesOuttree();
	return;
}

void Looper::SetBranchesH4tree()
{
	// --- SetBranchAddress("runNumber",&runNumber );
	// --- SetBranchAddress("spillNumber",&spillNumber );
	// --- SetBranchAddress("evtNumber",&evtNumber );
	SetBranchAddress("evtTimeDist",&evtTimeDist );
	SetBranchAddress("evtTimeStart",&evtTimeStart );
	SetBranchAddress("nEvtTimes",&nEvtTimes );
	SetBranchAddress("evtTime",&evtTime[0] );
	SetBranchAddress("evtTimeBoard",&evtTimeBoard[0]);

	SetBranchAddress("nAdcChannels",&nAdcChannels);
	SetBranchAddress("adcBoard",&adcBoard[0]);
	SetBranchAddress("adcChannel",&adcChannel[0]);
	SetBranchAddress("adcData",&adcData[0]);
                                               
	SetBranchAddress("nTdcChannels",&nTdcChannels);
	SetBranchAddress("tdcBoard",&tdcBoard[0]);
	SetBranchAddress("tdcChannel",&tdcChannel[0]);
	SetBranchAddress("tdcData",&tdcData[0]);
                                               
	SetBranchAddress("nDigiSamples",&nDigiSamples);
	SetBranchAddress("digiFrequency",&digiFrequency[0]);
	SetBranchAddress("digiGroup",&digiGroup[0]);
	SetBranchAddress("digiChannel",&digiChannel[0]);
	SetBranchAddress("digiSampleIndex",&digiSampleIndex[0]);
	SetBranchAddress("digiSampleValue",&digiSampleValue[0]);
	SetBranchAddress("digiSampleValueSub",&digiSampleValueSub[0]);
	SetBranchAddress("digiBoard",&digiBoard[0]);
                                               
	SetBranchAddress("nScalerWords",&nScalerWords);
	SetBranchAddress("scalerWord",&scalerWord[0]);
	SetBranchAddress("scalerBoard",&scalerBoard[0]);
                                               
	SetBranchAddress("nPatterns",&nPatterns);
	SetBranchAddress("pattern",&pattern[0]);
	SetBranchAddress("patternBoard",&patternBoard[0]);
	SetBranchAddress("patternChannel",&patternChannel[0]);
                                               
	SetBranchAddress("nTriggerWords",&nTriggerWords);
	SetBranchAddress("triggerWords",&triggerWords[0]);
	SetBranchAddress("triggerWordsBoard",&triggerWordsBoard[0]);

}

void Looper::SetBranchesCommon(){
	SetBranchAddress("runNumber"	,&runNumber);
	SetBranchAddress("spillNumber"	,&spillNumber);
	SetBranchAddress("evtNumber"	,&evtNumber);

}

void Looper::SetBranchesOuttree(){
	SetBranchAddress("ADCvalues"	,&ADCvalues);
	SetBranchAddress("BGOvalues"	,&BGOvalues);
	SetBranchAddress("SCINTvalues"	,&SCINTvalues);
	SetBranchAddress("TDCreco"	,&TDCreco  );
	SetBranchAddress("digi_charge_integrated"	,&digi_charge_integrated);
	SetBranchAddress("digi_max_amplitude"		,&digi_max_amplitude);
	SetBranchAddress("digi_pedestal"		,&digi_pedestal);
	SetBranchAddress("digi_pedestal_rms"		,&digi_pedestal_rms);
	SetBranchAddress("digi_time_at_frac30"		,&digi_time_at_frac30);
	SetBranchAddress("digi_time_at_frac50"		,&digi_time_at_frac50);
	SetBranchAddress("digi_fall_time_at_frac30"		,&digi_fall_time_at_frac30);
	SetBranchAddress("digi_fall_time_at_frac50"		,&digi_fall_time_at_frac50);
	SetBranchAddress("digi_time_at_max"		,&digi_time_at_max);
	SetBranchAddress("HODOX1",&HODOX1   );
	SetBranchAddress("HODOX2",&HODOX2   );
	SetBranchAddress("HODOY1",&HODOY1   );
	SetBranchAddress("HODOY2",&HODOY2   );

	SetBranchAddress("TableX",&TableX);
	SetBranchAddress("TableY",&TableY);
	SetBranchAddress("CeF3HV",&CeF3HV);
	SetBranchAddress("BGOHV",&BGOHV);
	SetBranchAddress("BeamEnergy",&BeamEnergy);
	SetBranchAddress("BeamTilt",&BeamTilt);
	SetBranchAddress("IsPhysics",&IsPhysics);
	SetBranchAddress("nTdcHits",&nTdcHits);
	
	//SUB
	SetBranchAddress("digi_charge_integrated_sub"	,&digi_charge_integrated_sub);
	SetBranchAddress("digi_max_amplitude_sub"	,&digi_max_amplitude_sub);
	SetBranchAddress("digi_pedestal_sub"		,&digi_pedestal_sub);
	SetBranchAddress("digi_pedestal_rms_sub"	,&digi_pedestal_rms_sub);
	// CORR
	SetBranchAddress("digi_charge_integrated_corr1"	,&digi_charge_integrated_corr1);
	SetBranchAddress("digi_max_amplitude_corr1"	,&digi_max_amplitude_corr1);

	SetBranchAddress("digi_charge_integrated_corr2"	,&digi_charge_integrated_corr2);
	SetBranchAddress("digi_max_amplitude_corr2"	,&digi_max_amplitude_corr2);

	SetBranchAddress("digi_pedestal_m"		,&digi_pedestal_m);
	SetBranchAddress("digi_pedestal_q"		,&digi_pedestal_q);

	SetBranchAddress("digi_pedestal_m0"		,&digi_pedestal_m0);
	SetBranchAddress("digi_pedestal_q0"		,&digi_pedestal_q0);
	SetBranchAddress("digi_pedestal_m1"		,&digi_pedestal_m1);
	SetBranchAddress("digi_pedestal_q1"		,&digi_pedestal_q1);
	SetBranchAddress("digi_pedestal_m2"		,&digi_pedestal_m2);
	SetBranchAddress("digi_pedestal_q2"		,&digi_pedestal_q2);

	SetBranchAddress("digi_pedestal_mu0"		,&digi_pedestal_mu0);
	SetBranchAddress("digi_pedestal_mu1"		,&digi_pedestal_mu1);
	SetBranchAddress("digi_pedestal_mu2"		,&digi_pedestal_mu2);
}
