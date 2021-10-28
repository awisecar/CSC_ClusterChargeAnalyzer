#ifndef cathodeHistos_h
#define cathodeHistos_h

#include "GifCSC/basics/interface/basicHistos.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTDigi.h"

class cathodeHistos:public basicHistos{
public:
	cathodeHistos(TDirectory * dout, const int debug=0);
	bool fillADCCounts(int timebin, int ADCcount);
	bool fillADCCountsLower(int timebin, int ADCcount);
	bool fillADCCountsHigher(int timebin, int ADCcount);
	bool fillPedestal(double pedestal);
	bool fillAmplitudeSum(double amplitudeSum, double amplitudeSum2);
	bool fillClusterCharge(double clustercharge, double clustercharge2);
	bool fillClusterChargeNoPedSub(double clustercharge, double clustercharge2);
	bool fillChargeByStrip(double relativestripnumber, double ADCcounts);
	bool scaleHistos(double nStripCompDigiMatch);
	bool scaleHistosReducedFiducialVolume(double nStripCompDigiMatchReducedVolume);

private:
	bool book(); // the only what has to be rewritten from basicHistos
};
#endif
