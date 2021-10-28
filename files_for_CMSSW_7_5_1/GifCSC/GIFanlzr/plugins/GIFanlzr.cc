// -*- C++ -*-
//
// Package:    GifCSC/GIFanlzr
// Class:      GIFanlzr
// 
/**\class GIFanlzr GIFanlzr.cc GifCSC/GIFanlzr/plugins/GIFanlzr.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Katerina Kuznetsova
//         Created:  Mon, 06 Nov 2017 13:21:13 GMT
//
//


// system include files
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TFile.h"

#include "GifCSC/Anode/interface/anodeHitsTrigs.h"
#include "GifCSC/Cathode/interface/cathodeHitsTrigs.h"
#include "GifCSC/LCT/interface/LCT.h"
#include "GifCSC/StatusDigis/interface/StatusDigis.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class GIFanlzr : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit GIFanlzr(const edm::ParameterSet&);
      ~GIFanlzr();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      TFile * outputfile = NULL;

	  // Switches for analyzer
      unsigned int chamberType;  // 0 - both; 1 - ME1/1; 2 - ME2/1; >10 - P5 (not yet)
      unsigned int anodeTag;     // 0 - none; 1 - proceed;
      unsigned int cathodeTag;   // 0 - none; 1 - proceed;
      unsigned int lctTag;       // 0 - none; 1 - proceed;
      unsigned int tmbTag;       // 0 - none; 1 - proceed;
      unsigned int statusTag;    // 0 - none; any of status tags above
      unsigned int debugTag;     // 0 - no debug, 1 analyzer, 2 all
	  std::string outputFileName;
      unsigned int NofChambers;

      std::vector <anodeHitsTrigs*>   chamberwiseAnodeInfo;
      std::vector <cathodeHitsTrigs*> chamberwiseCathodeInfo;
      std::vector <LCT*>              chamberwiseLCTInfo;
      std::vector <StatusDigis*>      chamberwiseStatusInfo;

	  // EDM Tokens
      edm::EDGetTokenT<CSCCLCTDigiCollection>          clct_token;
      edm::EDGetTokenT<CSCComparatorDigiCollection>    comp_token;
      edm::EDGetTokenT<CSCStripDigiCollection>        strip_token;
      edm::EDGetTokenT<CSCALCTDigiCollection>          alct_token;
      edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection> lct_token;
      edm::EDGetTokenT<CSCTMBStatusDigiCollection>     tmb_token;
      ///!!!TBD!!! here should be a struct fo tokens for StatusDigis

      //more status collections here
};

GIFanlzr::GIFanlzr(const edm::ParameterSet& iConfig){
	std::cout << "\n>>>>> Welcome to the GIF Analyzer <<<<<\n" << std::endl;
	usesResource("TFileService");

	// Grab input tags from cfg
	chamberType  = iConfig.getUntrackedParameter<int>("chamberTag");

	anodeTag                  = iConfig.getUntrackedParameter<int>("anodeTag");
	if(anodeTag) alct_token   = consumes<CSCALCTDigiCollection>(iConfig.getParameter<edm::InputTag>("alctDigiTag"));

	cathodeTag      = iConfig.getUntrackedParameter<int>("cathodeTag");
	if(cathodeTag){
		clct_token  = consumes<CSCCLCTDigiCollection>(iConfig.getParameter<edm::InputTag>("clctDigiTag"));
		comp_token  = consumes<CSCComparatorDigiCollection>(iConfig.getParameter<edm::InputTag>("comparatorDigiTag"));
		strip_token = consumes<CSCStripDigiCollection>(iConfig.getParameter<edm::InputTag>("stripDigiTag"));
	};

	lctTag          = iConfig.getUntrackedParameter<int>("lctTag");
	if(lctTag){
		lct_token   = consumes<CSCCorrelatedLCTDigiCollection>(iConfig.getParameter<edm::InputTag>("lctDigiTag"));
	}

	//StatusDigi
	tmbTag                      = iConfig.getUntrackedParameter<int>("tmbTag");
	if( tmbTag)	tmb_token       = consumes<CSCTMBStatusDigiCollection>(iConfig.getParameter<edm::InputTag>("tmbStatusDigiTag"));

	statusTag = (tmbTag);
	//!!!TBD!!! fill the structure of status tokens

    //debug level
	debugTag     = iConfig.getUntrackedParameter<int>("debugTag");
	int inDL = 0; // inner debug level
	if (debugTag>0) inDL=debugTag-1;

	// Printout switch values -----
	std::cout << " --- Value of input switches --- " << std::endl;
	std::cout << "chamberType: " << chamberType << std::endl;
	std::cout << "anodeTag: " << anodeTag << std::endl;
	std::cout << "cathodeTag: " << cathodeTag << std::endl;
	std::cout << "lctTag: " << lctTag << std::endl;
	std::cout << "tmbTag: " << tmbTag << std::endl;
	std::cout << "debugTag: " << debugTag << std::endl;

	short unsigned int chtmin = 1;
	short unsigned int chtmax = 2;
	NofChambers = 2;
	if( chamberType>0 and chamberType<3) {
		chtmin=chtmax=chamberType;
		NofChambers=1;
	}else if(chamberType==3){
		chamberType=3;
		NofChambers=1;
		chtmin=chtmax=chamberType;
	}else if(chamberType>3) {
		std::cout << "Unknown chamber type " << chamberType << "exiting...\n";
		exit(11);
	};

	// !!!TBD!!! (1)name from config
	outputFileName = iConfig.getUntrackedParameter<std::string>("outputFileName", "analysisOut.root");
	char outputFileNameChar[outputFileName.size()+1];
	strcpy(outputFileNameChar, outputFileName.c_str());
	outputfile   = new TFile(outputFileNameChar, "RECREATE"); // our NULL
	TDirectory * outdir = NULL;

	// loop over chambers
	std::cout << "\nNofChambers: " << NofChambers << std::endl;
	std::cout << "chtmin = " << chtmin << std::endl;
	std::cout << "chtmax = " << chtmax << std::endl;
	std::cout << std::endl;
	for(short unsigned int i=chtmin; i<=chtmax; i++){
		if(outputfile){
			TString chname="ME"; chname+=i; chname+="1";
			outputfile->cd(); 
			outdir = outputfile->mkdir(chname.Data());
		};
		if(anodeTag) chamberwiseAnodeInfo.push_back(new anodeHitsTrigs (i, alct_token, inDL, anodeTag>1));
		if(cathodeTag) chamberwiseCathodeInfo.push_back(
			new cathodeHitsTrigs(i, clct_token, comp_token, strip_token, outdir, inDL)
		);
		if(lctTag) chamberwiseLCTInfo.push_back(new LCT(i, lct_token, outdir, inDL));
		// !!!TBD!!! should be a structure of tokens!!! currently only tmb_token
		if(statusTag) chamberwiseStatusInfo.push_back(new StatusDigis(i, tmb_token, outdir, inDL));
	}; // end loop over chambers
} //end of constructor

GIFanlzr::~GIFanlzr(){
	if(anodeTag>0)
		for(std::vector<anodeHitsTrigs*>::iterator aii = chamberwiseAnodeInfo.begin(); aii!=chamberwiseAnodeInfo.end(); ++aii)
			delete *aii;
	if(cathodeTag>0)
		for(std::vector<cathodeHitsTrigs*>::iterator cii = chamberwiseCathodeInfo.begin(); cii!=chamberwiseCathodeInfo.end(); ++cii)
			delete *cii;
	if(lctTag>0)
		for(std::vector<LCT*>::iterator lii = chamberwiseLCTInfo.begin(); lii!=chamberwiseLCTInfo.end(); ++lii)
			delete *lii;
	if(statusTag>0)
		for(std::vector<StatusDigis*>::iterator sii = chamberwiseStatusInfo.begin(); sii!=chamberwiseStatusInfo.end(); ++sii)
			delete *sii;

	if(outputfile)outputfile->Close();
}

void GIFanlzr::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
	std::cout << "\n -------> GIFanlzr::analyze()" << std::endl;
	for(short unsigned int i=0; i < NofChambers; i++){

		std::cout << "Looping over chamber #" << i+1 << " out of " << NofChambers << "\n" << std::endl;

		if(anodeTag) {
			std::cout << " ----> Doing anodes" << std::endl;
			chamberwiseAnodeInfo.at(i)->alctInEvent(iEvent);
		}
		if(cathodeTag) {
			std::cout << " ----> Doing cathodes" << std::endl;
			chamberwiseCathodeInfo.at(i)->clctInEvent(iEvent);
			chamberwiseCathodeInfo.at(i)->compInEvent(iEvent);
			chamberwiseCathodeInfo.at(i)->stripInEvent(iEvent, true);
		};
		if(lctTag){   
			std::cout << " ----> Doing LCTs" << std::endl; 
			chamberwiseLCTInfo.at(i)->lctInEvent(iEvent);
		}
		if(statusTag) {
			if(tmbTag) chamberwiseStatusInfo.at(i)->tmbStatInEvent(iEvent);
		};

	}
	if(debugTag) std::cout << "\n\n";
}

void GIFanlzr::beginJob(){
    std::cout << "\n ---> GIFanlzr::beginJob()" << std::endl;
}

void GIFanlzr::endJob() {
	std::cout << "\n ---> GIFanlzr::endJob()" << std::endl;
	/* zachem ja eto delala?! 8)
	for(short unsigned int i=0; i<NofChambers; i++){
		chamberwiseCathodeInfo.at(i)->end();
		chamberwiseLCTInfo.at(i)->end();
		//chamberwiseAnodeInfo.at(i)->alctInEvent(iEvent);
	}*/
}

void GIFanlzr::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GIFanlzr);