#include <vector>
#include <map>
#include "TH1F.h"
using namespace std;

class STAT{
// I think I passed to the class instead of the namespace for dictionary problems with pyroot
public:
static float mean(vector<float> &a );
static float mean(vector<float> &a ,vector<float>&e_a);
static float median(vector<float> &a);
static float rms(vector<float> &a);
static float corrPearson(vector<float> &a, vector<float> &b);
static float corrSpearman(vector<float> &a ,vector<float> &b);
static pair<float,float> regression(vector<float>&a,vector<float>&b);
//error 0 = <0,0> 1=<1,1> 2=<0,1>
static pair<float,float> regression(vector<float>&a,vector<float>&b, vector<float>&e_b,vector<float> &e2);

static void drawFitError(TH1*h,pair<float,float> &R,vector<float> &e2,float sigma=1);

static float ConfidenceInterval(std::vector<float> &v,std::pair<float,float>&r,float Q);

// ---- TEMPLATE FIT ----

static float evalVector(std::vector<float> &x, int i);

static float linearInterpolation(float x, pair<float,float> &p1, pair<float,float> &p2);

// chi2= (y1[i+dx]-y2[i])^2 /rms 
// status: 0 = ok
static float chi2(std::vector<float> &y1,std::vector<float>&y2,float rms=1,float ampl=1,float dx=0);

static void simpleTemplateFit(vector<float> &y, vector<float> &ref, float &chi2, float&amplitude, float &dT,int &status, float rms=1) ;

};
