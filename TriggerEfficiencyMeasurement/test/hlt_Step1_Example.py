# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: hlt -s DIGI,L1,DIGI2RAW,HLT:hltdev:/users/biliu/Tutorial2016/Example/V2 --processName HLT2 --mc --datatier GEN-SIM-DIGI-RAW-HLTDEBUG --eventcontent FEVTDEBUGHLT --era=Run2_2016 --geometry=Extended2016,Extended2016Reco --conditions=80X_mcRun2_asymptotic_ForTSGStudies_from2016EcalTune_30fb_v1 -n 100 --no_exec --python_filename hlt_Step1_Example.py
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('HLT2',eras.Run2_2016)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# Input source
process.source = cms.Source("PoolSource",
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring("root://cms-xrd-global.cern.ch//store/mc/RunIIFall15DR76/WprimeToMuNu_M-5000_TuneCUETP8M1_13TeV-pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A297B0C3-F58D-E511-B25C-02163E016C02.root"),
    secondaryFileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch//store/mc/RunIISummer15GS/WprimeToMuNu_M-5000_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v1/30000/0E84FDE7-0050-E511-A77F-02163E00B694.root'),
    inputCommands = cms.untracked.vstring('keep *', 
        'drop *_genParticles_*_*', 
        'drop *_genParticlesForJets_*_*', 
        'drop *_kt4GenJets_*_*', 
        'drop *_kt6GenJets_*_*', 
        'drop *_iterativeCone5GenJets_*_*', 
        'drop *_ak4GenJets_*_*', 
        'drop *_ak7GenJets_*_*', 
        'drop *_ak8GenJets_*_*', 
        'drop *_ak4GenJetsNoNu_*_*', 
        'drop *_ak8GenJetsNoNu_*_*', 
        'drop *_genCandidatesForMET_*_*', 
        'drop *_genParticlesForMETAllVisible_*_*', 
        'drop *_genMetCalo_*_*', 
        'drop *_genMetCaloAndNonPrompt_*_*', 
        'drop *_genMetTrue_*_*', 
        'drop *_genMetIC5GenJs_*_*'),
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('hlt nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW-HLTDEBUG'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(10485760),
    fileName = cms.untracked.string('hlt_DIGI_L1_DIGI2RAW_HLT.root'),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
import HLTrigger.Configuration.Utilities
process.loadHltConfiguration("hltdev:/users/biliu/Tutorial2016/Example/V2",type='GRun')
from HLTrigger.Configuration.CustomConfigs import ProcessName
process = ProcessName(process)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_ForTSGStudies_from2016EcalTune_30fb_v1', '')

process.TFileService = cms.Service ('TFileService',
    fileName = cms.string ('efficiency.root')
)
process.TriggerEfficiencyAnalyzer = cms.EDAnalyzer ('TriggerEfficiencyAnalyzer', 
     triggerBitsLabel = cms.InputTag("TriggerResults", "", "HLT2"),
     triggerSummaryLabel = cms.InputTag("hltTriggerSummaryAOD","","HLT2"),
     recoMuonLabel = cms.InputTag ('muons'),
     filterTag = cms.InputTag ("hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09","","HLT2"),
     histograms = cms.VPSet(
             cms.PSet (
                     variable = cms.string("Pt"),
                     nBins = cms.int32(50),
                     lBin = cms.double(0),
                     hBin = cms.double(1000)
                      ),
             cms.PSet (
                     variable = cms.string("Eta"),
                     nBins = cms.int32(10),
                     lBin = cms.double(-2.5),
                     hBin = cms.double(2.5)
                      ),
             cms.PSet (
                     variable = cms.string("Phi"),
                     nBins = cms.int32(8),
                     lBin = cms.double(-3.2),
                     hBin = cms.double(3.2)
                      ),
      ),
)

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.effiicency_step = cms.EndPath(process.TriggerEfficiencyAnalyzer)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.endjob_step,process.effiicency_step])

# customisation of the process.

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforFullSim 

#call to customisation function customizeHLTforFullSim imported from HLTrigger.Configuration.customizeHLTforMC
process = customizeHLTforFullSim(process)

# End of customisation functions

