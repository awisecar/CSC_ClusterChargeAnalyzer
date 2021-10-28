/*
 * basicHistosNaming.h
 *
 *  Created on: 12 Nov 2017
 *      Author: kkuzn
 */
#include "GifCSC/basics/interface/basicHistosNaming.h"
#include <iostream>

basicHistosNaming::basicHistosNaming(const char* name, const char* title,
		const char* functionname, const char* description) {
	hname        = std::string(name);
	htitle       = std::string(title);
	hfunctionname= std::string(functionname);
	hdescription = std::string(description);
}

void basicHistosNaming::Print() {
	std::cout << "    histo naming: " << hname << "; " << htitle << "; " << hfunctionname << "; " << hdescription << std::endl;
}
