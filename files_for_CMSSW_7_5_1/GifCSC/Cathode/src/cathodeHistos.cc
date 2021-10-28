#include <utility>
#include <iostream>
#include "TDirectory.h"
#include "TH1F.h"
#include "GifCSC/Cathode/interface/cathodeHistos.h"
#include "GifCSC/basics/interface/basicHistosNaming.h"

cathodeHistos::cathodeHistos(TDirectory * dout, const int debug):basicHistos(dout, debug){
	if(debuglevel>1){
		std::cout << " cathodeHistos::cathodeHistos   : directory pointer " << &outdir << "; directory name: ";
		std::cout << outdir->GetPath() << std::endl;
	};
	book();
	if(debuglevel)std::cout << " cathodeHistos::cathodeHistos	   : => completed " << std::endl;
}

bool cathodeHistos::book(){
	outdir->cd(); // important
	TH1F * h;
	TString htitle, hdescr, hfunc;

	// 0 - fill ADC count per time bin for the strip digi with keystrip matched to a comparator digi
	htitle="ADCCounts"; hfunc="ADCCountsVsTimeBin"; hdescr="ADC Counts per TB for Strip Digi with keystrip matched to Comparator Digi";
	h = new TH1F(htitle.Data(), htitle.Data(), 8, -0.5, 7.5);
	histosWithDescritions.push_back(std::make_pair( h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data()) ));

	// 1 - pedestal values --> 0.5f*(ADCCounts[0]+ADCCounts[1])
	htitle="ADCPedestal"; hfunc="ADCPedestal"; hdescr="ADC Pedestal Values";
	h = new TH1F(htitle.Data(), htitle.Data(), 80, 400, 800);
	histosWithDescritions.push_back(std::make_pair( h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data()) ));

	// 2 - Sum of pedestal-subtracted amplitudes for 3 time bins
	// Where pedestal is 0.5f*(ADCCounts[0]+ADCCounts[1])
	htitle="Amplitude3TB"; hfunc="Amplitude3TB"; hdescr="Amplitude Sum, 3 TB's";
	h = new TH1F(htitle.Data(), htitle.Data(), 100, 0, 5000);
	histosWithDescritions.push_back(std::make_pair( h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data()) ));

	// 3 - Sum of pedestal-subtracted amplitudes for 5 time bins
	// Where pedestal is 0.5f*(ADCCounts[0]+ADCCounts[1])
	htitle="Amplitude5TB"; hfunc="Amplitude5TB"; hdescr="Amplitude Sum, 5 TB's";
	h = new TH1F(htitle.Data(), htitle.Data(), 100, 0, 5000);
	histosWithDescritions.push_back(std::make_pair( h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data()) ));

	// 4 - 3x3 Cluster Charge (3 TBs, 3 strips)
	htitle="CC3x3"; hfunc="CC3x3"; hdescr="Cluster Charge 3x3 ADC Sum";
	h = new TH1F(htitle.Data(), htitle.Data(), 100, 0, 5000);
	histosWithDescritions.push_back(std::make_pair( h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data()) ));

	// 5 - 5x3 Cluster Charge (5 TBs, 3 strips)
	htitle="CC5x3"; hfunc="CC5x3"; hdescr="Cluster Charge 5x3 ADC Sum";
	h = new TH1F(htitle.Data(), htitle.Data(), 100, 0, 5000);
	histosWithDescritions.push_back(std::make_pair( h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data()) ));	

	// 6 - 3x3 Cluster Charge (3 TBs, 3 strips) -  No pedestal subtraction from signal
	htitle="CC3x3NoPedSub"; hfunc="CC3x3NoPedSub"; hdescr="Cluster Charge 3x3 ADC Sum, No Pedestal Sub.";
	h = new TH1F(htitle.Data(), htitle.Data(), 100, 0, 5000);
	histosWithDescritions.push_back(std::make_pair( h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data()) ));

	// 7 - 5x3 Cluster Charge (5 TBs, 3 strips) -  No pedestal subtraction from signal
	htitle="CC5x3NoPedSub"; hfunc="CC5x3NoPedSub"; hdescr="Cluster Charge 5x3 ADC Sum, No Pedestal Sub.";
	h = new TH1F(htitle.Data(), htitle.Data(), 100, 0, 5000);
	histosWithDescritions.push_back(std::make_pair( h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data()) ));	

	// 8
	htitle="ADCCountsLower"; hfunc="ADCCountsLowerVsTimeBin"; hdescr="ADC Counts per TB for Strip Digi with keystrip-1";
	h = new TH1F(htitle.Data(), htitle.Data(), 8, -0.5, 7.5);
	histosWithDescritions.push_back(std::make_pair( h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data()) ));

	// 9
	htitle="ADCCountsHigher"; hfunc="ADCCountsHigherVsTimeBin"; hdescr="ADC Counts per TB for Strip Digi with keystrip+1";
	h = new TH1F(htitle.Data(), htitle.Data(), 8, -0.5, 7.5);
	histosWithDescritions.push_back(std::make_pair( h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data()) ));

	// 10
	// Look at charge on keystrip and surrounding strips for a fixed time bin
	// Exclude the key strip from being 3 strips away from either bound
	htitle="ADCCountByStripFrozenTB"; hfunc="ADCCountsVsStripNumber"; hdescr="ADC Counts by Strip Number Where Matched Keystrip is Central Zero Value";
	h = new TH1F(htitle.Data(), htitle.Data(), 7, -3.5, 3.5);
	histosWithDescritions.push_back(std::make_pair( h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data()) ));

	if(debuglevel){
		std::cout << " cathodeHistos::book         : booking ends with following content in the directory :" << std::endl;
		outdir->Print(); // does not work in CMSSW??!
		for (std::vector<std::pair<TH1F*, basicHistosNaming*>>::iterator it=histosWithDescritions.begin(); it!=histosWithDescritions.end(); ++it)
			(*it).second->Print();
	};
	return true;
};

//ADC counts as a function of time bins
bool cathodeHistos::fillADCCounts(int timebin, int ADCcount){
	histosWithDescritions.at(0).first->Fill(timebin, ADCcount);
	return true;
};

bool cathodeHistos::fillADCCountsLower(int timebin, int ADCcount){
	histosWithDescritions.at(8).first->Fill(timebin, ADCcount);
	return true;
};

bool cathodeHistos::fillADCCountsHigher(int timebin, int ADCcount){
	histosWithDescritions.at(9).first->Fill(timebin, ADCcount);
	return true;
};

//Pedestal values
bool cathodeHistos::fillPedestal(double pedestal){
	histosWithDescritions.at(1).first->Fill(pedestal);
	return true;
};

//Summed amplitude values
bool cathodeHistos::fillAmplitudeSum(double amplitudeSum, double amplitudeSum2){
	histosWithDescritions.at(2).first->Fill(amplitudeSum);
	histosWithDescritions.at(3).first->Fill(amplitudeSum2);
	return true;
};

//Cluster charge sums (TBs by strips)
bool cathodeHistos::fillClusterCharge(double clustercharge, double clustercharge2){
	histosWithDescritions.at(4).first->Fill(clustercharge);
	histosWithDescritions.at(5).first->Fill(clustercharge2);
	return true;
};

//Cluster charge sums (TBs by strips) - No Pedestal Subtraction
bool cathodeHistos::fillClusterChargeNoPedSub(double clustercharge, double clustercharge2){
	histosWithDescritions.at(6).first->Fill(clustercharge);
	histosWithDescritions.at(7).first->Fill(clustercharge2);
	return true;
};

//Plotting amplitude for time bin 5 for group of strips around keystrip
bool cathodeHistos::fillChargeByStrip(double relativestripnumber, double ADCcounts){
	histosWithDescritions.at(10).first->Fill(relativestripnumber, ADCcounts);
	return true;
};

//Divide histograms by the number of events used to fill
bool cathodeHistos::scaleHistos(double nStripCompDigiMatch){
	histosWithDescritions.at(0).first->Scale( 1./nStripCompDigiMatch );
	histosWithDescritions.at(8).first->Scale( 1./nStripCompDigiMatch );
	histosWithDescritions.at(9).first->Scale( 1./nStripCompDigiMatch );
	return true;
};

//Divide histograms by the number of events used to fill
bool cathodeHistos::scaleHistosReducedFiducialVolume(double nStripCompDigiMatchReducedVolume){
	histosWithDescritions.at(10).first->Scale( 1./nStripCompDigiMatchReducedVolume );
	return true;
};
