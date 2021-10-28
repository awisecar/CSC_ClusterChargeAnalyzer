/*
 * basicHistos.h
 *
 *  Created on: 12 Nov 2017
 *      Author: kkuzn
 */

#ifndef INTERFACE_BASICHISTOS_H_
#define INTERFACE_BASICHISTOS_H_
#include <vector>
#include <utility>

#include "TDirectory.h"
#include "TH1F.h"

#include "GifCSC/basics/interface/basicHistosNaming.h"
//!  Abstract class for a set of historgrams with booking and writing into a separate root directory.
/*!
    An inherited class should define individual booking (to be called in the constructor) and filling methods.
	Proper order for using the inherited histogramming class: <br>
	1) open output file and create directory <br>
	2) create a histogramming object <br>
	3) fill histograms with filling methods <br>
	4) call write method (defined in the abstract class) <br>
	5) close root file <br>
	6) delete the histogramming object <br>
*/
class basicHistos{
public:
		//! initializes outdir and debuglevel from parameters, inherited classes should add book() call!
	   /*!
	      \param foutdir directory in a root file.
	      \param debug int for a debug level: 0 - none, 1 - main, 2 - detailed
	      \sa outdir
	      \sa debuglevel
	    */
        basicHistos(TDirectory * foutdir, const int debug=0);
        //! destroys the historgram and description vector (histograms should be already detached from root file via calling write())
        /*!
         \sa histosWithDescritions
         */
        virtual ~basicHistos();
protected:
		//! each element contains a pointer to a histogram and histogram description object.
        std::vector<std::pair<TH1F* , basicHistosNaming*>> histosWithDescritions;
        //! pointer to a directory where histograms to be written.
        TDirectory* outdir;
        //! debug level : 0 - none, 1 - main, 2 - detailed
        const int debuglevel;
private:
        //! purely virtual booking method, to be called by inherited classes in constructor
        virtual bool book()=0;
public:
        //! writes all histograms from histosWithDescritions in outdir and detaches them from root file (required by destruction)
        /*!
         \sa histosWithDescritions, outdir
         */
        bool write();
};

#endif /* INTERFACE_BASICHISTOS_H_ */
