import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing ('analysis')
options.parseArguments()

fIn = cms.untracked.vstring(options.inputFiles)
fOut = cms.untracked.string(options.outputFile)

process = cms.Process("reader")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.source = cms.Source("DaqSource",
    readerPluginName = cms.untracked.string('CSCFileReader'),
    readerPset = cms.untracked.PSet(
        firstEvent  = cms.untracked.int32(0),
        tfDDUnumber = cms.untracked.int32(0),
        FED841 = cms.untracked.vstring('RUI01'),
        RUI01  = fIn
        #RUI01  = cms.untracked.vstring('3450V_csc_00000001_EmuRUI01_STEP_27s_000_190509_105705_UTC.raw')
  )
)

process.FEVT = cms.OutputModule("PoolOutputModule",
    fileName = fOut,
    #fileName = cms.untracked.string("unpackerOutput.root"),
    outputCommands = cms.untracked.vstring("keep *")
)

process.outpath = cms.EndPath(process.FEVT)
