#include "interface/PedestalAnalysis.hpp"
#include <assert.h>
#include <cmath>
#include "TMath.h"

#define DOUBLE_PEAK

void PedestalAnalysis::ClearEvent(){
	BaseAnalysis::ClearEvent();
	#ifdef DOUBLE_PEAK
	l->digi_pedestal_m->clear();
	l->digi_pedestal_q->clear();
	l->digi_pedestal_m0->clear();
	l->digi_pedestal_q0->clear();
	l->digi_pedestal_m1->clear();
	l->digi_pedestal_q1->clear();
	#endif
	return;
}

void PedestalAnalysis::AnalyzeEvent()
{
	vector<float> pedestalY[nChannels]; // begin
	vector<float> pedestalX[nChannels];
	for(int i=0;i<nChannels;++i)
		{
		pedestalX[i].clear();
		pedestalY[i].clear();
		}
		

	#ifdef DOUBLE_PEAK 
	vector<float> samplesY[nChannels]; // tail
	vector<float> samplesX[nChannels];

	vector<float> baselineY[nChannels]; // begin+tail
	vector<float> baselineX[nChannels];

	for(int i=0;i<nChannels;++i)
		{
		samplesX[i].clear();
		samplesY[i].clear();

		baselineX[i].clear();
		baselineX[i].clear();
		}
	#endif
	// compute m,q for the pedestal subtraction
	for (unsigned int iSample=0;iSample< l->nDigiSamples;iSample++)
	{
 		UInt_t digiGroup   = l->digiGroup[iSample];
 		UInt_t digiChannel = l->digiChannel[iSample] + 8*digiGroup;
 		UInt_t digiSampleIndex = l->digiSampleIndex[iSample];
 		Float_t digiSampleValue = l->digiSampleValue[iSample];
			if( digiSampleIndex >4 && digiSampleIndex<44)
				{
				pedestalX[digiChannel].push_back(digiSampleIndex);
				pedestalY[digiChannel].push_back(digiSampleValue);
				}
	}

	float pedM[nChannels];	
	//float pedQ[nChannels];
	for (int iCh=0;iCh<nChannels ;iCh++)
		{
		pair<float,float> P=regression(pedestalX[iCh],pedestalY[iCh]);
		pedM[iCh]=P.second;//pedQ[iCh]=P.first;
		}

	for (unsigned int iSample=0;iSample< l->nDigiSamples;iSample++)
	{
 		UInt_t digiGroup   = l->digiGroup[iSample];
 		UInt_t digiChannel = l->digiChannel[iSample] + 8*digiGroup;
 		UInt_t digiSampleIndex = l->digiSampleIndex[iSample];
 		Float_t digiSampleValue = l->digiSampleValue[iSample];
		int HV = int(l->CeF3HV);
		if( digiChannel < unsigned(nChannels)) 
			{
			l->FillProfile( Form("tprofile_pedestal_ch%d_HV%d",digiChannel, HV ),digiSampleIndex +1 , digiSampleValue );
			l->FillProfile( Form("tprofile_pedestal_ch%d_HV%d",digiChannel, 0 ),digiSampleIndex +1 , digiSampleValue ); //fill 0 with everything
			float ped= l->digi_pedestal->at(digiChannel);
			// approximate to the closest pedDelta*n+pedMin
			int n= TMath::Floor((ped-pedMin)/pedDelta);
			ped=pedMin+n*ped;
				assert( fabs(ped - l->digi_pedestal->at(digiChannel)) < pedDelta) ;
				assert( l->digi_pedestal->at(digiChannel) >= ped) ;
			if (ped>pedMax ||ped<pedMin) ped=0;
			l->FillProfile( Form("tprofile_pedestal_ch%d_ped%.1f",digiChannel,ped),digiSampleIndex +1 , digiSampleValue );
			if (pedM[digiChannel]>=0)l->FillProfile( Form("tprofile_pedestal_ch%d_ped%.1f_mPlus",digiChannel,ped),digiSampleIndex +1 , digiSampleValue );
			else l->FillProfile( Form("tprofile_pedestal_ch%d_ped%.1f_mMinus",digiChannel,ped),digiSampleIndex +1 , digiSampleValue );



		// DOUBLE PEAK
			#ifdef DOUBLE_PEAK
			//save all the samples
			if( digiSampleIndex >600 && digiSampleIndex<1000)
				{
				samplesX[digiChannel].push_back(digiSampleIndex);
				samplesY[digiChannel].push_back(digiSampleValue);

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
		l->FillHisto(Form("th1d_m1_ch%d_HV%d",iCh,int(l->CeF3HV)),m1[iCh] );
		l->FillHisto(Form("th1d_q1_ch%d_HV%d",iCh,int(l->CeF3HV)),q1[iCh] );

		l->FillHisto(Form("th1d_m0_ch%d_HV%d",iCh,int(l->CeF3HV)),m0[iCh] );
		l->FillHisto(Form("th1d_q0_ch%d_HV%d",iCh,int(l->CeF3HV)),q0[iCh] );
		// baseline
		pair<float,float> B=regression(baselineX[iCh],baselineY[iCh]);
		l->FillHisto(Form("th1d_m2_ch%d_HV%d",iCh,int(l->CeF3HV)),B.second );
		l->FillHisto(Form("th1d_q2_ch%d_HV%d",iCh,int(l->CeF3HV)),B.first );
		l->digi_pedestal_m->push_back(B.second);
		l->digi_pedestal_q->push_back(B.first);

		l->digi_pedestal_m0->push_back(P.second);
		l->digi_pedestal_q0->push_back(P.first);
		l->digi_pedestal_m1->push_back(R.second);
		l->digi_pedestal_q1->push_back(R.first);
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
	// Add 0 
	HV.push_back(0);
	for( unsigned int iHV=0;iHV<HV.size() ;++iHV)
	   {
	   for( unsigned int iCh=0;iCh<unsigned(nChannels);++iCh)
	      {
		cout<<"[PedestalAnalysis]::[Init] Ch="<<iCh<<"/"<<nChannels<<" HV="<<iHV<<"/"<<HV.size() <<endl;
		l->BookHisto(Form("tprofile_pedestal_ch%d_HV%d",iCh,HV[iHV]),"Pedestal",1024,0,1024,"TProfile");
		// DOUBLE PEAK
		#ifdef DOUBLE_PEAK
		l->BookHisto(Form("th1d_m1_ch%d_HV%d",iCh,HV[iHV]),"M TAIL",20000,-.01,.01,"TH1D"); // there is no trend
		l->BookHisto(Form("th1d_q1_ch%d_HV%d",iCh,HV[iHV]),"Q TAIL",20000,3600-100,3600+100,"TH1D"); // there is no trend
		l->BookHisto(Form("th1d_m0_ch%d_HV%d",iCh,HV[iHV]),"M PED",20000,-1.,1.,"TH1D"); // there is no trend
		l->BookHisto(Form("th1d_q0_ch%d_HV%d",iCh,HV[iHV]),"Q PED",20000,3600-100,3600+100,"TH1D"); // there is no trend

		l->BookHisto(Form("th1d_m2_ch%d_HV%d",iCh,HV[iHV]),"M PED",20000,-1.,1.,"TH1D"); // there is no trend
		l->BookHisto(Form("th1d_q2_ch%d_HV%d",iCh,HV[iHV]),"Q PED",20000,3600-300,3600+300,"TH1D"); // there is no trend
		#endif
		//END DOUBLE PEAK
	      }	
	   }
	cout<<"[PedestalAnalisis]::[Init]: Pedestal Histograms vs ped"<<endl;
	for( unsigned int iCh=0;iCh<unsigned(nChannels);++iCh)
	{
		for( float iPed=pedMin;iPed<= pedMax;iPed+=pedDelta)
		{
			l->BookHisto(Form("tprofile_pedestal_ch%d_ped%.1f",iCh,iPed),"Pedestal",1024,0,1024,"TProfile");
			l->BookHisto(Form("tprofile_pedestal_ch%d_ped%.1f_mPlus",iCh,iPed),"Pedestal",1024,0,1024,"TProfile");
			l->BookHisto(Form("tprofile_pedestal_ch%d_ped%.1f_mMinus",iCh,iPed),"Pedestal",1024,0,1024,"TProfile");
		}
	        l->BookHisto(Form("tprofile_pedestal_ch%d_ped0",iCh),"Pedestal",1024,0,1024,"TProfile");
	        l->BookHisto(Form("tprofile_pedestal_ch%d_ped0_mPlus",iCh),"Pedestal",1024,0,1024,"TProfile");
	        l->BookHisto(Form("tprofile_pedestal_ch%d_ped0_mMinus",iCh),"Pedestal",1024,0,1024,"TProfile");
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
