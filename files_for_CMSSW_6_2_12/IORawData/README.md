# IORawData
Unpacker to convert raw file to root file containing fedRawDataCollection.  

Original documentation:  https://twiki.cern.ch/twiki/bin/view/CMS/CSCTestBeamDataTreatment

Instructions to run code on lxplus:

    Install version 6_2_X (I use 6_2_12) of CMSSW. Better to do this on some other machine, as to run analysis on DAQ machine is a bad idea. I use lxplus.
    cmsrel CMSSW_6_2_12
    cd CMSSW_6_2_12/src/ 
    cmsenv 
    git clone ssh://git@gitlab.cern.ch:7999/CSC-GIF/IORawData.git
    scram b 
    cmsRun readFile.py 

Get .raw file with data. If you work on lxplus, than there is no need to log on to emugif1. Use wget command from lxplus like: wget http://emugif1.cern.ch/data/current/csc_00000001_EmuRUI01_STEP_27_001_150831_172544_UTC.raw

The result is a root file with fedRawDataCollection, which you should process using the Gif/Gif package in CMSSW_7_5_1.

`Test Commit by Riju.`    
Test commit by Chris
