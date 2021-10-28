/*
 * basicHistosNaming.h
 *
 *  Created on: 12 Nov 2017
 *      Author: kkuzn
 */

#ifndef INTERFACE_BASICHISTOSNAMING_H_
#define INTERFACE_BASICHISTOSNAMING_H_

#include <string>

class basicHistosNaming{
public:
	basicHistosNaming(const char * name, const char * title, const char * functionname, const char * description);
	~basicHistosNaming(){};
	void Print();
private:
	std::string hname;
	std::string htitle;
	std::string hfunctionname;
	std::string hdescription;
};

#endif /* INTERFACE_BASICHISTOSNAMING_H_ */
