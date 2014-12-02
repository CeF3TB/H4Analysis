#ifndef FILLER_H
#define FILLER_H

#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include <iostream>
#include "interface/Event.hpp"
#include <assert.h>
#include "interface/OutFile.hpp"

using namespace std;

class Filler :public virtual OutFile { 
friend class BaseAnalysis;

protected:
	TTree *fChain;	
	tree out;

	void SetBranchesH4tree();
	void SetBranchesOuttree();
	void SetBranchesCommon();
	bool isActive(string name);
	// ---------
	void SetBranchAddress(string name,unsigned long long* ptr);
	void SetBranchAddress(string name,unsigned int* ptr);
	void SetBranchAddress(string name,int* ptr);
	void SetBranchAddress(string name,float* ptr);
	void SetBranchAddress(string name,double* ptr);

	void SetBranchArray(string name,unsigned long long* ptr,string n);
	void SetBranchArray(string name,unsigned int* ptr,string n);
	void SetBranchArray(string name,int* ptr,string n);
	void SetBranchArray(string name,float* ptr,string n);
	void SetBranchArray(string name,double* ptr,string n);

	void SetBranchAddress(string name,vector<float>** 	ptr);
	void SetBranchAddress(string name,vector<double>** 	ptr);
	void SetBranchAddress(string name,vector<unsigned int>** ptr);
	void SetBranchAddress(string name,vector<int>** 	ptr);
	void SetBranchAddress(string name,vector<bool>** 	ptr);
	void SetBranchAddress(string name,vector<unsigned long long>** ptr);
public:
	string outTreeName;
	map<string,int> activeBranches;	
	Filler(){};
	~Filler(){};
	inline void AddActiveBranch(string name) { activeBranches[name]=1;};
	void Init();
	void Write();
	void SetBranches(); // DO NOT PUT THIS VIRTUAL
	void SetBranchAddress();
	//template<class T>
	//void SetBranchAddress(string name,T ptr);

	void Fill( );//{ fChain->Fill() ;};
	void SetAliases();

};

// --- #ifndef FILLER_CPP
// --- template<class T>
// --- void SetBranchAddress(string name,T ptr);
// --- 	{
// ---   	cout<<"[Filler]::[ERROR]: Unable to handle Branch "<<name<<endl;
// ---  	assert(0);
// --- 	};
// --- #endif

#endif
