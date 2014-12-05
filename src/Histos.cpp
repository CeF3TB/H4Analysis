#include "interface/Histos.hpp"
int Histos::BookHisto(string name, string title, int nBins, double xmin, double xmax,string type)
{
	map<string,TH1*>::iterator it;
	it=histoContainer.find(name);
	// if already exist, return
	if ( it!=histoContainer.end() ) return 1;
	// else create it
	if (type== "TH1D") histoContainer[name]=new TH1D(name.c_str(),title.c_str(),nBins,xmin,xmax);
	else if (type == "TProfile") histoContainer[name]=new TProfile(name.c_str(),title.c_str(),nBins,xmin,xmax); 
	else if (type== "TH1F") histoContainer[name]=new TH1F(name.c_str(),title.c_str(),nBins,xmin,xmax);
	else if (type== "TH2D") histoContainer[name]=new TH2D(name.c_str(),title.c_str(),nBins,xmin,xmax,nBins,xmin,xmax); // squared
	else if (type== "TH2F") histoContainer[name]=new TH2F(name.c_str(),title.c_str(),nBins,xmin,xmax,nBins,xmin,xmax);
	return 0;

}

int Histos::BookHisto(string name, string title, int nBins, double xmin, double xmax,double nBinsY,double ymin, double ymax,string type)
{
	map<string,TH1*>::iterator it;
	it=histoContainer.find(name);
	// if already exist, return
	if ( it!=histoContainer.end() ) return 1;
	// else create it
	if (type== "TH2D") histoContainer[name]=new TH2D(name.c_str(),title.c_str(),nBins,xmin,xmax,nBinsY,ymin,ymax);
	if (type== "TH2F") histoContainer[name]=new TH2F(name.c_str(),title.c_str(),nBins,xmin,xmax,nBinsY,ymin,ymax);
	return 0;

}

void Histos::FillHisto(string name, double value, double weight)
	{
	histoContainer[name]->Fill(value,weight);
	}
void Histos::FillProfile(string name, double x,double y, double weight)
	{
	//cout<<"[Histos]::[FillProfile]"<<name<<endl; //DEBUG
	TH1 * h= histoContainer[name]; // avoid double search
	if ( dynamic_cast<TProfile*> (h) == NULL ) 
		{
		Print();
		cout <<"[Histos]::[FillProfile] Cannot cast Histo "<<name<<" to TProfile"<<endl;
		assert(0);
		}
	dynamic_cast<TProfile*>(h)->Fill(x,y,weight);
	}
void Histos::FillTH2(string name, double x,double y, double weight)
	{
	//cout<<"[Histos]::[FillProfile]"<<name<<endl; //DEBUG
	TH1 * h= histoContainer[name]; // avoid double search
	if ( dynamic_cast<TH2*> (h) == NULL ) 
		{
		cout <<"[Histos]::[FillProfile] Cannot cast Histo "<<name<<" to TH2"<<endl;
		assert(0);
		}
	dynamic_cast<TH2*>(h)->Fill(x,y,weight);
	}
void Histos::Write()
	{
	fOut->cd();
	map<string,TH1*>::iterator it;
	for( it=histoContainer.begin(); it!=histoContainer.end() ;it++)
		{
		it->second->Write();
		}
	return;
	}

void Histos::Print()
{
	map<string,TH1*>::iterator it;
	cout<<"------------HISTOS PRINT --------------"<<endl;
	for (it=histoContainer.begin();it!=histoContainer.end();it++)
		{
		cout<<"H="<<it->first<<" == "<< it->second->GetName();
		if ( it->second->InheritsFrom("TH1D") ) cout<<"TH1D ";
		if ( it->second->InheritsFrom("TH2D") ) cout<<"TH2D ";
		if ( it->second->InheritsFrom("TH1F") ) cout<<"TH1F ";
		if ( it->second->InheritsFrom("TH2F") ) cout<<"TH2F ";
		if ( it->second->InheritsFrom("TProfile") ) cout<<"TProfile ";
		
		cout <<endl;
		}
	cout<<"---------------------------------------"<<endl;
}
