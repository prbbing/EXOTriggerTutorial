import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import math
import os
import sys
process = cms.Process ('TriggerEfficiencyAnalyzer')
process.load ('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.source = cms.Source( "PoolSource",
    fileNames = cms.untracked.vstring(#"root://cms-xrd-global.cern.ch//store/mc/RunIIFall15DR76/WprimeToMuNu_M-5000_TuneCUETP8M1_13TeV-pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A297B0C3-F58D-E511-B25C-02163E016C02.root",
    "root://cms-xrd-global.cern.ch//store/mc/RunIIFall15DR76/WprimeToMuNu_M-5000_TuneCUETP8M1_13TeV-pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A297B0C3-F58D-E511-B25C-02163E016C02.root"),
    secondaryFileNames = cms.untracked.vstring("file:hlt_DIGI_L1_DIGI2RAW_HLT.root"),
)

process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

process.options = cms.untracked.PSet(
)

process.TFileService = cms.Service ('TFileService',
    fileName = cms.string ('efficiency.root')
)
process.maxEvents = cms.untracked.PSet (
    input = cms.untracked.int32 (1000)
)
process.TriggerEfficiencyAnalyzer = cms.EDAnalyzer ('TriggerEfficiencyAnalyzer', 
     triggerBitsLabel = cms.InputTag("TriggerResults", "", "HLT2"),
     triggerSummaryLabel = cms.InputTag("hltTriggerSummaryAOD","","HLT2"),
     recoMuonLabel = cms.InputTag ('muons'),
     filterTag = cms.InputTag ("hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09","","HLT2"),
     histograms = cms.VPSet(
             cms.PSet (
                     variable = cms.string("Pt"),
                     nBins = cms.int32(200),
                     lBin = cms.double(0),
                     hBin = cms.double(5000)
                      ),
             cms.PSet (
                     variable = cms.string("Eta"),
                     nBins = cms.int32(25),
                     lBin = cms.double(-2.5),
                     hBin = cms.double(2.5)
                      ),
             cms.PSet (
                     variable = cms.string("Phi"),
                     nBins = cms.int32(32),
                     lBin = cms.double(-3.2),
                     hBin = cms.double(3.2)
                      ),
      ),
)



process.myPath = cms.Path (process.TriggerEfficiencyAnalyzer)

