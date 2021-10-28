#include "GifCSC/Cathode/interface/cathodeHitsTrigs.h"
#include "GifCSC/Cathode/interface/cathodePrintOuts.h"
#include <iostream>

cathodeHitsTrigs::cathodeHitsTrigs(short unsigned     chamber,
//		http://cmsdoxygen.web.cern.ch/cmsdoxygen/CMSSW_7_4_1/doc/html/dc/d98/CSCStripDigi_8h_source.html
		edm::EDGetTokenT<CSCCLCTDigiCollection>       clcttoken,
		edm::EDGetTokenT<CSCComparatorDigiCollection> comptoken,
		edm::EDGetTokenT<CSCStripDigiCollection>      striptoken,
		TDirectory *                                  outputroot,
		short unsigned debuglevel): 
		chamberType(chamber), 
		commonCurrentEvtN(-1), 
		evtsProccededCLCTs(0), 
		nStripCompDigiMatch(0.),
		nStripCompDigiMatchReducedVolume(0.),
		debug(debuglevel)
{
	//CLCT
	clct_token  = clcttoken;
	//comparators
	comp_token  = comptoken;
	//strips
	strip_token = striptoken;

	// histos
	// std::cout << " >>>>>>>> HELLO HISTOS <<<<<<<< " << std::endl;
	cathode_histos = NULL; 
	histogramming = false;
	if(outputroot!=NULL){
		if(debug) std::cout << "Creating Cathode histogram subdirectory in directory " << outputroot->GetPath() << std::endl;
		TDirectory *outdir = outputroot->mkdir("hCathode");
		if(outdir->cd()){
			// std::cout << "Creating cathodeHistos object!" << std::endl;
			cathode_histos   = new cathodeHistos(outdir, debug);
			histogramming = true;
			// std::cout << "histogramming = " << histogramming << std::endl;
		}
		else{
			std::cout << "cathodeHitsTrigs can not create directory for basic CLCT historgrams in file " << outputroot->GetPath() << std::endl;
		}
	} 
	else{
		std::cout << "cathodeHitsTrigs will not save basic historgrams since no directory was prepared!\n";
	};

}

cathodeHitsTrigs::~cathodeHitsTrigs(){
	cathodeHitsTrigs::end();
	delete cathode_histos; // histos should be already written
}

bool cathodeHitsTrigs::end(){
	bool ok = true;
	std::cout << "\nevtsProccededCLCTs = " << evtsProccededCLCTs << std::endl;
	if(histogramming){
		// Scale histograms by 1/(number of entries)
		std::cout << "nStripCompDigiMatch = " << nStripCompDigiMatch << std::endl;
		cathode_histos->scaleHistos(nStripCompDigiMatch);
		std::cout << "nStripCompDigiMatchReducedVolume = " << nStripCompDigiMatchReducedVolume << "\n" << std::endl;
		cathode_histos->scaleHistosReducedFiducialVolume(nStripCompDigiMatchReducedVolume);
		// Write out histograms to ROOT file
		ok = (ok && cathode_histos->write());
	};
	if(debug) std::cout << "cathodeHitsTrigs::end(): writing histogram status: " << ok << std::endl;
	return ok;
}

bool cathodeHitsTrigs::clctInEvent(const edm::Event& iEvent){
	std::cout << "\n ~~~ cathodeHitsTrigs::clctInEvent ~~~ " << std::endl;
	//http://cmsdoxygen.web.cern.ch/cmsdoxygen/CMSSW_7_5_1/doc/html/d5/d6e/CSCCLCTDigiCollection_8h_source.html
	//typedef MuonDigiCollection<CSCDetId, CSCCLCTDigi> CSCCLCTDigiCollection;
	//http://cmsdoxygen.web.cern.ch/cmsdoxygen/CMSSW_7_5_1/doc/html/da/d26/classCSCCLCTDigi.html
	bool ok = false;
	short unsigned int N = 0;

	edm::Handle<CSCCLCTDigiCollection> clct;
	iEvent.getByToken(clct_token, clct);
	clcts.clear();
	// iterate through CSCCLCTDigiCollection ---
	
	for (CSCCLCTDigiCollection::DigiRangeIterator clctit=clct->begin(); clctit!=clct->end(); ++clctit) {

		// Look at CSCDetId first ---
		CSCDetId id = (CSCDetId)(*clctit).first;
		if(id.chamber()!=chamberType) continue;
		if(debug) std::cout   <<  "CLCT from: chamber " << id.chamber()
							  << ", station " << id.station()
							  << ", ring " << id.ring()
							  << ", layer " << id.layer() << std::endl;

		// Now look at the CSCCLCTDigi ---
		std::vector<CSCCLCTDigi>::const_iterator digiItr = (*clctit).second.first;
		std::vector<CSCCLCTDigi>::const_iterator last    = (*clctit).second.second;
		if(digiItr!=last) evtsProccededCLCTs++;
		// iterate through vector of CSCCLCTDigi's (?)
		while (digiItr != last) {

			// print out info for CSCLCTDigi object
			if (debug) CLCT_print(*digiItr);

			if ( (*digiItr).isValid() ){
				CLCTstruct clct; 
				clct.layer    = id.layer();
				clct.keyStrip = (*digiItr).getKeyStrip();
				clct.TB       = (*digiItr).getBX();
				clcts.push_back(clct);
				// if(histogramming)  cathode_histos->fillCLCTHistos(*digiItr);
				N++;
			};
			++digiItr;
		};
	}; // end loop CSCCLCTDigiCollection

	if (N>0) ok = true;
	return ok;
};



bool cathodeHitsTrigs::compInEvent(const edm::Event& iEvent){
	std::cout << "\n ~~~ cathodeHitsTrigs::compInEvent ~~~ " << std::endl;
	//https://github.com/cms-sw/cmssw/blob/CMSSW_7_5_X/DataFormats/CSCDigi/interface/CSCComparatorDigi.h
	//MuonDigiCollection<CSCDetId, CSCComparatorDigi> CSCComparatorDigiCollection;
	bool ok = false;
	short unsigned int N = 0;

	edm::Handle<CSCComparatorDigiCollection> comp;
	iEvent.getByToken(comp_token, comp);
	comps.clear();

	// if(debug)std::cout << "cathodeHitsTrig::compInEvent: ";
	for (CSCComparatorDigiCollection::DigiRangeIterator compit=comp->begin(); compit!=comp->end(); ++compit) {

		// Look at CSCDetId first ---
		CSCDetId id = (CSCDetId)(*compit).first;
		if(id.chamber()!=chamberType) continue;
		// if(debug) std::cout   << "Comparator from: chamber " << id.chamber()
		// 					  << ", station " << id.station()
		// 					  << ", ring " << id.ring()
		// 					  << ", layer " << id.layer() << std::endl;

		// Now look at the CSCComparatorDigi ---
		std::vector<CSCComparatorDigi>::const_iterator digiItr = (*compit).second.first;
		std::vector<CSCComparatorDigi>::const_iterator last    = (*compit).second.second;
		if(debug){
			///comparator here can be either 0 or 1 for left or right halfstrip of given strip
			if(digiItr!=last) std::cout << "\t    Chamber - Layer - Strip - Comp - firstTimeBin" << std::endl;
			while (digiItr != last) {
				std::cout << "Comparator:    " << id.chamber() << "        " << id.layer()<< "       "
					// Get the strip number
					<< digiItr->getStrip()  << "       " 
					// Get Comparator readings
					<< digiItr->getComparator() << "        "
					// Return bin number of first time bin which is ON. Counts from 0.
					<< digiItr->getTimeBin() << std::endl;

				// Return vector of the bin numbers for which time bins are ON.
				const std::vector<int> TBs = digiItr->getTimeBinsOn();

				std::cout << "Time bins that are ON: ";
				for (short unsigned int tb=0; tb<TBs.size(); tb++) std::cout << "  " << TBs[tb];
				std::cout << std::endl;

				COMPstruct comp; comp.layer = id.layer();
				comp.keyStrip = (*digiItr).getStrip();
				comp.TB       = (*digiItr).getTimeBin();
				comps.push_back(comp);

				++digiItr;
			};
		};
	}; // end loop CSCComparatorDigiCollection
	if (N>0) ok = true;
	return ok;
};

bool cathodeHitsTrigs::stripInEvent(const edm::Event& iEvent, bool givenCOMPs){
	std::cout << "\n ~~~ cathodeHitsTrigs::stripInEvent ~~~ " << std::endl;
	//https://github.com/cms-sw/cmssw/blob/CMSSW_7_5_X/DataFormats/CSCDigi/interface/CSCStripDigi.h
	//MuonDigiCollection<CSCDetId, CSCStripDigi> CSCStripDigiCollection;
	//float pedestal() const {return 0.5f*(ADCCounts[0]+ADCCounts[1]);}
	//float amplitude() const {return ADCCounts[4]-pedestal();}
	bool ok = false;
	edm::Handle<CSCStripDigiCollection> strip;
	iEvent.getByToken(strip_token, strip);

	if(debug){
		// std::cout << "cathodeHitsTrig::stripInEvent: "<<std::endl;
		std::cout << "\tChamber - Layer - Strip - Pedestal - Amplitude" << std::endl;
	}

    // Iterate through CSCStripDigiCollection ---
	int digiItrIndex = 0;
	int stripCompMatchIndex = -1;

	// Need to make sure keystrip number is between 4 and 13 to fill charge by strip plot
	// Basically I'm excluding the bounds of the chamber to make things easier
	int matchedCompKeyStrip = -1;
	bool stripNotOnEdge = 0;

	// We iterate over all of the strips in one layer
	for (CSCStripDigiCollection::DigiRangeIterator strit=strip->begin(); strit!=strip->end(); ++strit) {

		// Grab CSCDetId first 
		CSCDetId id = (CSCDetId)(*strit).first;
		if(id.chamber()!=chamberType) continue;

		// Now grab CSCStripDigi's
		std::vector<CSCStripDigi>::const_iterator digiItr = (*strit).second.first;
		std::vector<CSCStripDigi>::const_iterator last    = (*strit).second.second;
		while (digiItr != last) {

			// This printout is for every CSCStripDigi
			// if (debug and digiItr!=last) {
			// 	std::cout << digiItrIndex << "\t  " << id.chamber() <<"        "<< id.layer()<< "       "
			// 	// Get the strip number
			// 	<< digiItr->getStrip()  << "       " 
			// 	// float pedestal() const {return 0.5f*(ADCCounts[0]+ADCCounts[1]);}
			// 	<< digiItr->pedestal() << "       "
			// 	// float amplitude() const {return ADCCounts[4]-pedestal();}
			// 	<< digiItr->amplitude() << std::endl;
			// }
			

			// Loop through all comparators retrieved above in cathodeHitsTrigs::compInEvent()
			short unsigned int compindx=0;
			if(givenCOMPs){
				// Find the compindx where the comparator has the same layer as the strip digi
				while( (compindx<comps.size()) and comps[compindx].layer!=id.layer()) compindx++;
				// If none of the comparators has the same layer as the strip digi, go to next strip digi
				if(compindx==comps.size()){
					++digiItrIndex;
					++digiItr;
					continue;
				}
			}
			
			// If strip number of strip digi equals that of the comparator digi, print out ADC counts
			// So, basically looking for a ComparatorDigi/StripDigi match on the same layer and strip
			if(digiItr->getStrip() == comps[compindx].keyStrip){
				stripCompMatchIndex = digiItrIndex;
				matchedCompKeyStrip = comps[compindx].keyStrip;
				// Checking if the keyStrip is at least 3 strips away from edge of layer
				if (matchedCompKeyStrip > 3 && matchedCompKeyStrip < 14) stripNotOnEdge = 1;

				const std::vector<int>                 ADCs = digiItr->getADCCounts();
				const std::vector<short unsigned int>  OVFs = digiItr->getADCOverflow();

				std::cout << digiItrIndex << "\t  " << id.chamber() <<"        "<< id.layer()<< "       "
				// Get the strip number
				<< digiItr->getStrip()  << "       " 
				// float pedestal() const {return 0.5f*(ADCCounts[0]+ADCCounts[1]);}
				<< digiItr->pedestal() << "       "
				// float amplitude() const {return ADCCounts[4]-pedestal();}
				<< digiItr->amplitude() << std::endl;

				std::cout << " ^^^^^ MATCH ^^^^^ \t>>> ADCs per time bin: ";
				for (short unsigned int tb=0; tb < ADCs.size(); tb++){
					std::cout << "   " << ADCs[tb] << "(" << OVFs[tb]<< ")";
				}

				cathode_histos->fillPedestal( 0.5*(ADCs[0]+ADCs[1]) );

				double amplitudeSum3TB(0.), amplitudeSum5TB(0.);
				double amplitude3(0.), amplitude4(0.), amplitude5(0.), amplitude6(0.), amplitude7(0.);
				amplitude3 = ADCs[3]-(0.5*(ADCs[0]+ADCs[1]));
				amplitude4 = ADCs[4]-(0.5*(ADCs[0]+ADCs[1]));
				amplitude5 = ADCs[5]-(0.5*(ADCs[0]+ADCs[1]));
				amplitude6 = ADCs[6]-(0.5*(ADCs[0]+ADCs[1]));
				amplitude7 = ADCs[7]-(0.5*(ADCs[0]+ADCs[1]));
				amplitudeSum3TB += (amplitude4 + amplitude5 + amplitude6);
				amplitudeSum5TB += (amplitude3 + amplitude4 + amplitude5 + amplitude6 + amplitude7);
				cathode_histos->fillAmplitudeSum(amplitudeSum3TB, amplitudeSum5TB);

				nStripCompDigiMatch += 1;
				if (stripNotOnEdge) nStripCompDigiMatchReducedVolume += 1;
				std::cout << std::endl;

				// std::cout << "amplitudeSum3TB = " << amplitudeSum3TB << std::endl;
				// std::cout << "amplitudeSum5TB = " << amplitudeSum5TB << std::endl;
			};

			++digiItrIndex;
			++digiItr;
		};


	}; // end loop CSCStripDigiCollection

	// Now find the two adjacent (on either side of the matched keystrip) 
	// strips' cluster charge
	if (stripCompMatchIndex >= 0){

		std::cout << "\nThere was a strip-comparator digi match" << std::endl;
		std::cout << "stripCompMatchIndex = " << stripCompMatchIndex << std::endl;
		std::cout << "Keystrip number = " << matchedCompKeyStrip << std::endl;

		if(debug){
			// std::cout << "cathodeHitsTrig::stripInEvent: "<<std::endl;
			std::cout << "\n\tChamber - Layer - Strip - Pedestal - Amplitude" << std::endl;
		}

		double clustercharge3x3(0.);
		double clustercharge5x3(0.);
		double clustercharge3x3NoPedSub(0.);
		double clustercharge5x3NoPedSub(0.);

		digiItrIndex = 0;
		for (CSCStripDigiCollection::DigiRangeIterator strit=strip->begin(); strit!=strip->end(); ++strit) {

			// Grab CSCDetId first 
			CSCDetId id = (CSCDetId)(*strit).first;
			if(id.chamber()!=chamberType) continue;
			// Now grab CSCStripDigi's
			std::vector<CSCStripDigi>::const_iterator digiItr = (*strit).second.first;
			std::vector<CSCStripDigi>::const_iterator last    = (*strit).second.second;

			while (digiItr != last) {

				if (stripCompMatchIndex-3 == digiItrIndex){
					if (histogramming){
						const std::vector<int>                 ADCs = digiItr->getADCCounts();
						// Filling with ADC counts from time bin #5
						if (stripNotOnEdge) cathode_histos->fillChargeByStrip(-3, ADCs[5]);
					}
				}

				else if (stripCompMatchIndex-2 == digiItrIndex){
					if (histogramming){
						const std::vector<int>                 ADCs = digiItr->getADCCounts();
						// Filling with ADC counts from time bin #5
						if (stripNotOnEdge) cathode_histos->fillChargeByStrip(-2, ADCs[5]);
					}
				}

				else if (stripCompMatchIndex-1 == digiItrIndex){
					std::cout << digiItrIndex << "\t  " << id.chamber() <<"        "<< id.layer()<< "       "
					<< digiItr->getStrip()  << "       " 
					<< digiItr->pedestal() << "        "
					<< digiItr->amplitude();

					const std::vector<int>                 ADCs = digiItr->getADCCounts();
					const std::vector<short unsigned int>  OVFs = digiItr->getADCOverflow();
					std::cout << " \t>>> ADCs per time bin: ";
					for (short unsigned int tb=0; tb < ADCs.size(); tb++){
						std::cout << "   " << ADCs[tb] << "(" << OVFs[tb]<< ")";
						if (histogramming) cathode_histos->fillADCCountsLower(tb, ADCs[tb]);
					}
					std::cout << std::endl;
					if (histogramming){
						// Filling with ADC counts from time bin #5
						if (stripNotOnEdge) cathode_histos->fillChargeByStrip(-1, ADCs[5]);
					}

					double amplitude3(0.), amplitude4(0.), amplitude5(0.), amplitude6(0.), amplitude7(0.);
					amplitude3 = ADCs[3]-(0.5*(ADCs[0]+ADCs[1]));
					amplitude4 = ADCs[4]-(0.5*(ADCs[0]+ADCs[1]));
					amplitude5 = ADCs[5]-(0.5*(ADCs[0]+ADCs[1]));
					amplitude6 = ADCs[6]-(0.5*(ADCs[0]+ADCs[1]));
					amplitude7 = ADCs[7]-(0.5*(ADCs[0]+ADCs[1]));
					clustercharge3x3 += (amplitude4 + amplitude5 + amplitude6);
					clustercharge5x3 += (amplitude3 + amplitude4 + amplitude5 + amplitude6 + amplitude7);
					clustercharge3x3NoPedSub += (ADCs[4]+ADCs[5]+ADCs[6]);
					clustercharge5x3NoPedSub += (ADCs[3]+ADCs[4]+ADCs[5]+ADCs[6]+ADCs[7]);
				}
				else if (stripCompMatchIndex == digiItrIndex){
					std::cout << digiItrIndex << "\t  " << id.chamber() <<"        "<< id.layer()<< "       "
					<< digiItr->getStrip()  << "       " 
					<< digiItr->pedestal() << "        "
					<< digiItr->amplitude();

					const std::vector<int>                 ADCs = digiItr->getADCCounts();
					const std::vector<short unsigned int>  OVFs = digiItr->getADCOverflow();
					std::cout << " \t>>> ADCs per time bin: ";
					for (short unsigned int tb=0; tb < ADCs.size(); tb++){
						std::cout << "   " << ADCs[tb] << "(" << OVFs[tb]<< ")";
						if (histogramming) cathode_histos->fillADCCounts(tb, ADCs[tb]);
					}
					std::cout << std::endl;
					if (histogramming){
						// Filling with ADC counts from time bin #5
						if (stripNotOnEdge) cathode_histos->fillChargeByStrip(0, ADCs[5]);
					}

					double amplitude3(0.), amplitude4(0.), amplitude5(0.), amplitude6(0.), amplitude7(0.);
					amplitude3 = ADCs[3]-(0.5*(ADCs[0]+ADCs[1]));
					amplitude4 = ADCs[4]-(0.5*(ADCs[0]+ADCs[1]));
					amplitude5 = ADCs[5]-(0.5*(ADCs[0]+ADCs[1]));
					amplitude6 = ADCs[6]-(0.5*(ADCs[0]+ADCs[1]));
					amplitude7 = ADCs[7]-(0.5*(ADCs[0]+ADCs[1]));
					clustercharge3x3 += (amplitude4 + amplitude5 + amplitude6);
					clustercharge5x3 += (amplitude3 + amplitude4 + amplitude5 + amplitude6 + amplitude7);
					clustercharge3x3NoPedSub += (ADCs[4]+ADCs[5]+ADCs[6]);
					clustercharge5x3NoPedSub += (ADCs[3]+ADCs[4]+ADCs[5]+ADCs[6]+ADCs[7]);
				}
				else if (stripCompMatchIndex+1 == digiItrIndex){
					std::cout << digiItrIndex << "\t  " << id.chamber() <<"        "<< id.layer()<< "       "
					<< digiItr->getStrip()  << "        " 
					<< digiItr->pedestal() << "       "
					<< digiItr->amplitude();

					const std::vector<int>                 ADCs = digiItr->getADCCounts();
					const std::vector<short unsigned int>  OVFs = digiItr->getADCOverflow();
					std::cout << " \t>>> ADCs per time bin: ";
					for (short unsigned int tb=0; tb < ADCs.size(); tb++){
						std::cout << "   " << ADCs[tb] << "(" << OVFs[tb]<< ")";
						if (histogramming) cathode_histos->fillADCCountsHigher(tb, ADCs[tb]);
					}
					std::cout << std::endl;
					if (histogramming){
						// Filling with ADC counts from time bin #5
						if (stripNotOnEdge) cathode_histos->fillChargeByStrip(1, ADCs[5]);
					}

					double amplitude3(0.), amplitude4(0.), amplitude5(0.), amplitude6(0.), amplitude7(0.);
					amplitude3 = ADCs[3]-(0.5*(ADCs[0]+ADCs[1]));
					amplitude4 = ADCs[4]-(0.5*(ADCs[0]+ADCs[1]));
					amplitude5 = ADCs[5]-(0.5*(ADCs[0]+ADCs[1]));
					amplitude6 = ADCs[6]-(0.5*(ADCs[0]+ADCs[1]));
					amplitude7 = ADCs[7]-(0.5*(ADCs[0]+ADCs[1]));
					clustercharge3x3 += (amplitude4 + amplitude5 + amplitude6);
					clustercharge5x3 += (amplitude3 + amplitude4 + amplitude5 + amplitude6 + amplitude7);
					clustercharge3x3NoPedSub += (ADCs[4] + ADCs[5] + ADCs[6]);
					clustercharge5x3NoPedSub += (ADCs[3] + ADCs[4] + ADCs[5] + ADCs[6] + ADCs[7]);
				}

				else if (stripCompMatchIndex+2 == digiItrIndex){
					if (histogramming){
						const std::vector<int>                 ADCs = digiItr->getADCCounts();
						// Filling with ADC counts from time bin #5
						if (stripNotOnEdge) cathode_histos->fillChargeByStrip(2, ADCs[5]);
					}
				}

				else if (stripCompMatchIndex+3 == digiItrIndex){
					if (histogramming){
						const std::vector<int>                 ADCs = digiItr->getADCCounts();
						// Filling with ADC counts from time bin #5
						if (stripNotOnEdge) cathode_histos->fillChargeByStrip(3, ADCs[5]);
					}
				}

				++digiItrIndex;
				++digiItr;
			}

		} // end loop CSCStripDigiCollection

		// std::cout << "clustercharge3x3 = " << clustercharge3x3 << std::endl;
		// std::cout << "clustercharge5x3 = " << clustercharge5x3 << std::endl;
		// std::cout << "clustercharge3x3NoPedSub = " << clustercharge3x3NoPedSub << std::endl;
		// std::cout << "clustercharge5x3NoPedSub = " << clustercharge5x3NoPedSub << std::endl;

		// fill cluster charge histos
		cathode_histos->fillClusterCharge(clustercharge3x3, clustercharge5x3);
		// cathode_histos->fillClusterChargeNoPedSub(clustercharge3x3NoPedSub, clustercharge5x3NoPedSub); // this plot doesn't make sense
		
	} //end second loop

	std::cout << std::endl;
	return ok;
};