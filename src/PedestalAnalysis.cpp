#include "interface/PedestalAnalysis.hpp"
#include <assert.h>
#include <cmath>
#include "TMath.h"

#define DOUBLE_PEAK

void PedestalAnalysis::ClearEvent(){
	BaseAnalysis::ClearEvent();

	l->digi_pedestal_mu0->clear();
	l->digi_pedestal_mu1->clear();
	l->digi_pedestal_mu2->clear();

	#ifdef DOUBLE_PEAK
		l->digi_pedestal_m->clear();
		l->digi_pedestal_q->clear();
		l->digi_pedestal_m0->clear();
		l->digi_pedestal_q0->clear();
		l->digi_pedestal_m1->clear();
		l->digi_pedestal_q1->clear();
		l->digi_pedestal_m2->clear();
		l->digi_pedestal_q2->clear();
	#endif
	return;
}

void PedestalAnalysis::AnalyzeEvent()
{
	vector<float> pedestalY[nChannels]; // begin
	vector<float> pedestalX[nChannels];

	vector<float> samplesY[nChannels]; // tail @900
	vector<float> samplesX[nChannels];

	vector<float> presamplesY[nChannels]; // tail @ 800
	vector<float> presamplesX[nChannels];

	for(int i=0;i<nChannels;++i)
		{
		pedestalX[i].clear();
		pedestalY[i].clear();

		samplesX[i].clear();
		samplesY[i].clear();

		presamplesX[i].clear();
		presamplesY[i].clear();
		}
		

	#ifdef DOUBLE_PEAK 
	vector<float> baselineY[nChannels]; // begin+tail
	vector<float> baselineX[nChannels];

	for(int i=0;i<nChannels;++i)
		{

		baselineX[i].clear();
		baselineX[i].clear();
		}
	#endif
	// compute m,q for the pedestal subtraction, and mu
	for (unsigned int iSample=0;iSample< l->nDigiSamples;iSample++)
	{
 		UInt_t digiGroup   = l->digiGroup[iSample];
 		UInt_t digiChannel = l->digiChannel[iSample] + 8*digiGroup;
 		UInt_t digiSampleIndex = l->digiSampleIndex[iSample];
 		Float_t digiSampleValue = l->digiSampleValue[iSample];
		if (digiChannel >=nChannels) continue;
			if( digiSampleIndex >4 && digiSampleIndex<44)
				{
				pedestalX[digiChannel].push_back(digiSampleIndex);
				pedestalY[digiChannel].push_back(digiSampleValue);
				}
			if( digiSampleIndex >800-30 && digiSampleIndex<800+30)
				{
				presamplesX[digiChannel].push_back(digiSampleIndex);
				presamplesY[digiChannel].push_back(digiSampleValue);
				}
			if( digiSampleIndex >900-30 && digiSampleIndex<900+30)
				{
				samplesX[digiChannel].push_back(digiSampleIndex);
				samplesY[digiChannel].push_back(digiSampleValue);
				}
	}

	//float pedM[nChannels];	
	////float pedQ[nChannels];
	//for (int iCh=0;iCh<nChannels ;iCh++)
	//	{
	//	pair<float,float> P=regression(pedestalX[iCh],pedestalY[iCh]);
	//	pedM[iCh]=P.second;//pedQ[iCh]=P.first;
	//	}
	
	// main loop -- Fill and update branches
	for (unsigned int iSample=0;iSample< l->nDigiSamples;iSample++)
	{
 		UInt_t digiGroup   = l->digiGroup[iSample];
 		UInt_t digiChannel = l->digiChannel[iSample] + 8*digiGroup;
 		UInt_t digiSampleIndex = l->digiSampleIndex[iSample];
 		Float_t digiSampleValue = l->digiSampleValue[iSample];
		//int HV = int(l->CeF3HV);

		if( digiChannel < unsigned(nChannels)) 
			{
			float R_mu=mean(samplesY[digiChannel]);    // 1
			float P_mu=mean(pedestalY[digiChannel]);   // 0
			float A_mu=mean(presamplesY[digiChannel]); //2

			l->digi_pedestal_mu0->push_back(P_mu);
			l->digi_pedestal_mu1->push_back(R_mu);
			l->digi_pedestal_mu2->push_back(A_mu);
			
			// compute categories:
			float dmu2 = R_mu-P_mu;
			float dmu1 = A_mu-P_mu;
			//float dmut = A_mu - R_mu;

			char cat=computeCategory(digiChannel, dmu2, dmu1);
			// this is the variable I'm binning
			float mu= dmu1;
			// approximate to the closest muDelta*n+muMin
			int n= TMath::Floor((mu-muMin)/muDelta);
			mu=muMin+n*muDelta;

			if (mu>muMax || mu<muMin) mu=0;

			l->FillProfile( Form("tprofile_pedestal_ch%d_cat%c_dmu%.1f",digiChannel,cat,mu),digiSampleIndex +1 , digiSampleValue );



		// DOUBLE PEAK
			#ifdef DOUBLE_PEAK
			//save all the samples
			if( digiSampleIndex >900-30 && digiSampleIndex<900+30)
				{
				baselineX[digiChannel].push_back(digiSampleIndex);
				baselineY[digiChannel].push_back(digiSampleValue);
				}
			if( digiSampleIndex >4 && digiSampleIndex<44)
				{
				//pedestalX[digiChannel].push_back(digiSampleIndex);
				//pedestalY[digiChannel].push_back(digiSampleValue);

				baselineX[digiChannel].push_back(digiSampleIndex);
				baselineY[digiChannel].push_back(digiSampleValue);
				}
			#endif
		// END DOUBLE PEAK
			}
	}
	// DOUBLE PEAK
	#ifdef DOUBLE_PEAK
		//perform regression
		float m1[nChannels],q1[nChannels];
		float m0[nChannels],q0[nChannels];
	for (unsigned iCh=0;iCh<unsigned(nChannels) ;++iCh)
		{
		pair<float,float> R=regression(samplesX[iCh],samplesY[iCh]);
		m1[iCh]=R.second; q1[iCh]=R.first;
		pair<float,float> P=regression(pedestalX[iCh],pedestalY[iCh]);
		m0[iCh]=P.second; q0[iCh]=P.first;
		// baseline
		pair<float,float> B=regression(baselineX[iCh],baselineY[iCh]);

		pair<float,float> A=regression(presamplesX[iCh],presamplesY[iCh]);
	
		

		l->digi_pedestal_m->push_back(B.second);
		l->digi_pedestal_q->push_back(B.first);

		l->digi_pedestal_m0->push_back(P.second);
		l->digi_pedestal_q0->push_back(P.first);
		l->digi_pedestal_m1->push_back(R.second);
		l->digi_pedestal_q1->push_back(R.first);
		l->digi_pedestal_m2->push_back(A.second);
		l->digi_pedestal_q2->push_back(A.first);
		
		}
		
		// END DOUBLE PEAK
	#endif
	return;
}

void PedestalAnalysis::Init(LoopAndFill *l1)
{
	cout<<"[PedestalAnalysis]::[Init]"<<endl;
	BaseAnalysis::Init(l1);
	//l->BookHisto("pedestal_ch0_HV600",1024,0,1024);
	if( ! ( l1->inputBranches["digiChannel"] and 
		l1->inputBranches["digiSampleIndex"] and 
		l1->inputBranches["digiSampleValue"] ) )
		{
		cout <<"[PedestalAnalysis]::[Init]::[ERROR]: input branches are not active"<<endl;	
		cout<<"digiChannel:"<< l1->inputBranches["digiChannel"] <<endl;
		cout<<"digiSampleIndex:"<< l1->inputBranches["digiSampleIndex"] <<endl;
		cout<<"digiSampleValue:"<< l1->inputBranches["digiSampleValue"] <<endl;
		assert(0);
		}
	cout<<"[PedestalAnalisis]::[Init]: Pedestal Histograms vs mu"<<endl;
	for (unsigned int iCat=0;iCat<unsigned(nCat);++iCat)
	{
	for( unsigned int iCh=0;iCh<unsigned(nChannels);++iCh)
	{
		for( float iDMu=muMin;iDMu<= muMax;iDMu+=muDelta)
		{
			l->BookHisto(Form("tprofile_pedestal_ch%d_cat%c_dmu%.1f",iCh,'A'+iCat,iDMu),"Pedestal",1024,0,1024,"TProfile");
		}
	        l->BookHisto(Form("tprofile_pedestal_ch%d_cat%c_dmu0.0",iCh,'A'+iCat),"Pedestal",1024,0,1024,"TProfile");
	}
	}
	cout<<"[PedestalAnalysis]::[Init]::Done"<<endl;
	return ;
}

pair<float,float> PedestalAnalysis::regression(vector<float>&a,vector<float>&b) //a=x, b=y
        {
        if(a.size() != b.size()) {printf("ERROR\n");return pair<float,float>(-99,-99);}
        // y= mx+q
        float Sxx=0,Sxy=0;
        float ma=mean(a),mb=mean(b);
        for(int i=0;i<int(a.size());i++) {Sxx+=(a[i]-ma)*(a[i]-ma);Sxy=(a[i]-ma)*(b[i]-mb);}
        Sxx/=a.size();
        Sxy/=a.size();
        float m=Sxy/Sxx;
        float q=mb-(m*ma);
        pair<float,float> R(q,m);
        return R;
        }

float PedestalAnalysis::mean(vector<float> &a )
        {
        float S=0;
        for(int i=0;i<int(a.size());i++) S+=a[i];
        S/=a.size();
        return S;
        }

char PedestalAnalysis::computeCategory(
	const UInt_t digiChannel, 
	const float dmu2, 
	const float dmu1   )
{
	float dmut = dmu2 - dmu1 ;
	char cat='A';
	switch( digiChannel)
	{
		case 0: 
			{
			if (dmut > 3.4 ) cat ='A';
			else 
				{
				if (dmu1 > 1-1.5*dmut) cat='B';
				else cat='C';
				}
			break;
			}
		case 1:
			{
			if ( dmu1 > 1.5- 1.5/2.0*dmut ) cat='A';
			else if (dmut > -1.2 ) cat='B';
			else cat='C';
			break;
			}
		case 2:
			{
			if ( dmu1 > 1.5 - dmut)  cat='A';
			else if (dmut > -1.0) cat='B';
			else cat='C';
			break;
			}
		case 3:
			{
			if ( dmut > -1 && dmu1 >1 ) cat='A';
			else if ( dmu1 > 3.8 + 2* dmut ) cat='C';
			else cat='B';
			break;
			}
	}	
	return cat;
}
