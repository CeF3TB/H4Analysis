#include "interface/PedestalAnalysis.hpp"
#include <assert.h>
#include <cmath>

#define DOUBLE_PEAK

void PedestalAnalysis::AnalyzeEvent()
{
	#ifdef DOUBLE_PEAK 
	vector<float> samplesY[nChannels];
	vector<float> samplesX[nChannels];

	vector<float> pedestalY[nChannels];
	vector<float> pedestalX[nChannels];

	for(int i=0;i<nChannels;++i)
		{
		samplesX[i].clear();
		samplesY[i].clear();

		pedestalX[i].clear();
		pedestalY[i].clear();
		}
	#endif


	for (unsigned int iSample=0;iSample< l->nDigiSamples;iSample++)
	{
 		UInt_t digiGroup   = l->digiGroup[iSample];
 		UInt_t digiChannel = l->digiChannel[iSample] + 8*digiGroup;
 		UInt_t digiSampleIndex = l->digiSampleIndex[iSample];
 		Float_t digiSampleValue = l->digiSampleValue[iSample];
		int HV = int(l->CeF3HV);
		if( digiChannel <nChannels) 
			{
			l->FillProfile( Form("tprofile_pedestal_ch%d_HV%d",digiChannel, HV ),digiSampleIndex +1 , digiSampleValue );
			l->FillProfile( Form("tprofile_pedestal_ch%d_HV%d",digiChannel, 0 ),digiSampleIndex +1 , digiSampleValue ); //fill 0 with everything

		// DOUBLE PEAK
			#ifdef DOUBLE_PEAK
			//save all the samples
			if( digiSampleIndex >600 && digiSampleIndex<1000)
				{
				samplesX[digiChannel].push_back(digiSampleIndex);
				samplesY[digiChannel].push_back(digiSampleValue);
				}
			if( digiSampleIndex >4 && digiSampleIndex<50)
				{
				pedestalX[digiChannel].push_back(digiSampleIndex);
				pedestalY[digiChannel].push_back(digiSampleValue);
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
	for (unsigned iCh=0;iCh<nChannels ;++iCh)
		{
		pair<float,float> R=regression(samplesX[iCh],samplesY[iCh]);
		m1[iCh]=R.second; q1[iCh]=R.first;
		pair<float,float> P=regression(pedestalX[iCh],pedestalY[iCh]);
		m0[iCh]=P.second; q0[iCh]=P.first;
		l->FillHisto(Form("th1d_m1_ch%d_HV%d",iCh,int(l->CeF3HV)),m1[iCh] );
		l->FillHisto(Form("th1d_q1_ch%d_HV%d",iCh,int(l->CeF3HV)),q1[iCh] );

		l->FillHisto(Form("th1d_m0_ch%d_HV%d",iCh,int(l->CeF3HV)),m0[iCh] );
		l->FillHisto(Form("th1d_q0_ch%d_HV%d",iCh,int(l->CeF3HV)),q0[iCh] );
		}
		
	for (unsigned int iSample=0;iSample< l->nDigiSamples;iSample++)
	{
 		UInt_t digiGroup   = l->digiGroup[iSample];
 		UInt_t digiChannel = l->digiChannel[iSample] + 8*digiGroup;
 		UInt_t digiSampleIndex = l->digiSampleIndex[iSample];
 		Float_t digiSampleValue = l->digiSampleValue[iSample];
		int HV = int(l->CeF3HV);
		if( digiChannel <nChannels) 
			{
			if ( fabs(m0[digiChannel])> 1./400 )
			{
			l->FillProfile( Form("tprofile_pedestal_ch%d_HV%d_PLUS",digiChannel, HV ),digiSampleIndex +1 , digiSampleValue );
			l->FillProfile( Form("tprofile_pedestal_ch%d_HV%d_PLUS",digiChannel, 0 ),digiSampleIndex +1 , digiSampleValue ); //fill 0 with everything
			}
			else 
			{
			l->FillProfile( Form("tprofile_pedestal_ch%d_HV%d_MINUS",digiChannel, HV ),digiSampleIndex +1 , digiSampleValue );
			l->FillProfile( Form("tprofile_pedestal_ch%d_HV%d_MINUS",digiChannel, 0 ),digiSampleIndex +1 , digiSampleValue ); //fill 0 with everything
			}
			}
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
	   for( unsigned int iCh=0;iCh<nChannels;++iCh)
	      {
		cout<<"[PedestalAnalysis]::[Init] Ch="<<iCh<<"/"<<nChannels<<" HV="<<iHV<<"/"<<HV.size() <<endl;
		l->BookHisto(Form("tprofile_pedestal_ch%d_HV%d",iCh,HV[iHV]),"Pedestal",1024,0,1024,"TProfile");
		// DOUBLE PEAK
		#ifdef DOUBLE_PEAK
		l->BookHisto(Form("tprofile_pedestal_ch%d_HV%d_PLUS",iCh,HV[iHV]),"Pedestal",1024,0,1024,"TProfile"); // there is a trend
		l->BookHisto(Form("tprofile_pedestal_ch%d_HV%d_MINUS",iCh,HV[iHV]),"Pedestal",1024,0,1024,"TProfile"); // there is no trend
		l->BookHisto(Form("th1d_m1_ch%d_HV%d",iCh,HV[iHV]),"M TAIL",20000,-.01,.01,"TH1D"); // there is no trend
		l->BookHisto(Form("th1d_q1_ch%d_HV%d",iCh,HV[iHV]),"Q TAIL",20000,3600-100,3600+100,"TH1D"); // there is no trend
		l->BookHisto(Form("th1d_m0_ch%d_HV%d",iCh,HV[iHV]),"M PED",20000,-1.,1.,"TH1D"); // there is no trend
		l->BookHisto(Form("th1d_q0_ch%d_HV%d",iCh,HV[iHV]),"Q PED",20000,3600-100,3600+100,"TH1D"); // there is no trend
		#endif
		//END DOUBLE PEAK
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
