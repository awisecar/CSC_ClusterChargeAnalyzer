import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing ('analysis')
options.parseArguments()

process = cms.Process("GIFanlzrTest")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
# process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        # 'file:/afs/cern.ch/work/a/awisecar/ClusterChargeAnalyzer/CMSSW_6_2_12/src/IORawData/unpackerOutput.root'
        # 'file:/afs/cern.ch/work/a/awisecar/ClusterChargeAnalyzer/CMSSW_6_2_12/src/IORawData/ccHFO-09May/unpackerOutput_3600V.root'
        # 'file:/afs/cern.ch/work/a/awisecar/ClusterChargeAnalyzer/CMSSW_6_2_12/src/IORawData/ccCF4-21Feb/unpackerOutput_3600V.root'
        options.inputFiles
    )
)

process.MessageLogger = cms.Service("MessageLogger",
       destinations   = cms.untracked.vstring('myDebugOutputFile.txt'),
       debugModules = cms.untracked.vstring('*'),
       message = cms.untracked.PSet(threshold = cms.untracked.vstring('DEBUG'))
)
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

process.load("Configuration/Geometry/GeometryIdeal2015Reco_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load("Configuration/StandardSequences/RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("RecoMuon.MuonSeedGenerator.standAloneMuonSeeds_cff")

process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v0'

process.options = cms.untracked.PSet(
        SkipEvent = cms.untracked.vstring('LogicError','ProductNotFound')
)

#=====================================================================================================
#http://cmslxr.fnal.gov/source/EventFilter/CSCRawToDigi/plugins/CSCDCCUnpacker.cc
#http://cmsdoxygen.web.cern.ch/cmsdoxygen/CMSSW_7_5_1/doc/html/db/dd7/cscUnpacker__cfi_8py_source.html
#muonCSCDigis = EventFilter.CSCRawToDigi.muonCSCDCCUnpacker_cfi.muonCSCDCCUnpacker.clone()
process.muonCSCDigis.SuppressZeroLCT   =cms.untracked.bool(False)
# EventDump works only partly in CMSSW_7_5_1
process.muonCSCDigis.FormatedEventDump = cms.untracked.bool(False)
# Unpack general status digis?
process.muonCSCDigis.UnpackStatusDigis = cms.bool(True) 
# Unpack FormatStatus digi?
process.muonCSCDigis.UseFormatStatus = cms.bool(True)
# Turn on lots of output
#process.muonCSCDigis.Debug = cms.untracked.bool(False)
#process.muonCSCDigis.PrintEventNumber = cms.untracked.bool(False)
# Visualization of raw data in corrupted events
#process.muonCSCDigis.VisualFEDInspect = cms.untracked.bool(False)
#process.muonCSCDigis.VisualFEDShort   = cms.untracked.bool(False)
#=====================================================================================================

 
process.GIFanlzrTest = cms.EDAnalyzer('GIFanlzr',
    wireDigiTag  = cms.InputTag("muonCSCDigis", "MuonCSCWireDigi"),
    alctDigiTag  = cms.InputTag('muonCSCDigis', 'MuonCSCALCTDigi'),
    clctDigiTag  = cms.InputTag('muonCSCDigis', 'MuonCSCCLCTDigi'),
    lctDigiTag   = cms.InputTag('muonCSCDigis', 'MuonCSCCorrelatedLCTDigi'),
    stripDigiTag = cms.InputTag("muonCSCDigis",  'MuonCSCStripDigi'),
    comparatorDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCComparatorDigi"),
    tmbStatusDigiTag = cms.InputTag("muonCSCDigis","MuonCSCTMBStatusDigi"),
    #cscRecHitTag = cms.InputTag("csc2DRecHits",""),
    #cscSegTag    = cms.InputTag("cscSegments"),
    
    # technical info - statis digis, requires process.muonCSCDigis.UnpackStatusDigis = cms.bool(True) 
    chamberTag     = cms.untracked.int32(3),   # 0 - both; 1 - ME1/1; 2 - ME2/1; # 3 - minCSC(904); 
    anodeTag       = cms.untracked.int32(0),   # 0 - none; 1 - proceed; 2 - anodes only (test11)
    cathodeTag     = cms.untracked.int32(1),   # 0 - none; 1 - proceed; 2 - cathodes only (test11c) 
    lctTag         = cms.untracked.int32(0),   # 0 - none; 1 - proceed;  
    tmbTag         = cms.untracked.int32(1), # 0 - none; 1 - proceed;
    debugTag       = cms.untracked.int32(2),  # 0 - no debug, 1 analyzer, 2 all (event printouts)     
    outputFileName = cms.untracked.string(options.outputFile)                          
)

#process.p = cms.Path(process.muonCSCDigis)
process.p = cms.Path(process.muonCSCDigis*process.GIFanlzrTest)
