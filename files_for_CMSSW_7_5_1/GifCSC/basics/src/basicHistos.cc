/*
 * basicHistos.cc
 *
 *  Created on: 12 Nov 2017
 *      Author: kkuzn
 */
#include "GifCSC/basics/interface/basicHistos.h"
#include <iostream>

basicHistos::basicHistos(TDirectory* foutdir, int debug):outdir(foutdir), debuglevel(debug){
	outdir->cd(); // will crash here if foutdir is NULL
	if(debuglevel){
		std::cout << "basicHistos::basicHistos 1: directory pointer " << &outdir << "; directory name: ";
		std::cout << outdir->GetPath() << std::endl;
	};
}

basicHistos::~basicHistos() {
	for (std::vector<std::pair<TH1F* , basicHistosNaming*>>::iterator it=histosWithDescritions.begin(); it!=histosWithDescritions.end(); ++it){
		TH1F * h = it->first;
		delete h;
		delete (*it).second;
	};
	if(debuglevel)std::cout << "~basicHistos::basicHistos : => completed\n";
}


bool basicHistos::write() {
	bool ok = outdir->cd();
	if(ok){
		/* TDirectory::Print() does not work in CMSSW
		if(debuglevel){
			if(debuglevel)std::cout << "basicHistos::write        : => following histograms will be saved\n";
			outdir->Print();
		};*/
		for (std::vector<std::pair<TH1F*, basicHistosNaming*>>::iterator it=histosWithDescritions.begin(); it!=histosWithDescritions.end(); ++it){
			TH1F * h = (*it).first;
			h->Write();
			h->SetDirectory(0); // allows to run destructor after closing file
		};
	}
	return ok;
}
