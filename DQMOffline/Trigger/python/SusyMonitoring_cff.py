import FWCore.ParameterSet.Config as cms

from DQMOffline.Trigger.RazorMonitor_cff import *
from DQMOffline.Trigger.VBFSUSYMonitor_cff import *
from DQMOffline.Trigger.LepHTMonitor_cff import *
from DQMOffline.Trigger.susyHLTEleCaloJets_cff import *
from DQMOffline.Trigger.TopMonitor_cfi import hltTOPmonitoring

susyMuEGMonitoring = hltTOPmonitoring.clone()
susyMuEGMonitoring.FolderName = cms.string('HLT/SUSY/MuonEG/')
susyMuEGMonitoring.nmuons = cms.uint32(1)
susyMuEGMonitoring.nphotons = cms.uint32(1)
susyMuEGMonitoring.nelectrons = cms.uint32(0)
susyMuEGMonitoring.njets = cms.uint32(0)
susyMuEGMonitoring.enablePhotonPlot =  cms.bool(True)
susyMuEGMonitoring.muoSelection = cms.string('pt>26 & abs(eta)<2.1 & isPFMuon & isGlobalMuon & isTrackerMuon & numberOfMatches>1  & innerTrack.hitPattern.trackerLayersWithMeasurement>5 & innerTrack.hitPattern.numberOfValidPixelHits>0  & globalTrack.hitPattern.numberOfValidMuonHits>0 & globalTrack.normalizedChi2<10 & (pfIsolationR04.sumChargedHadronPt + max(pfIsolationR04.sumNeutralHadronEt + pfIsolationR04.sumPhotonEt - (pfIsolationR04.sumPUPt)/2.,0.) )/pt<0.15') 
susyMuEGMonitoring.phoSelection = cms.string('(pt > 30 && abs(eta)<1.4442 && hadTowOverEm<0.0597 && full5x5_sigmaIetaIeta()<0.01031 && chargedHadronIso<1.295 && neutralHadronIso < 5.931+0.0163*pt+0.000014*pt*pt && photonIso < 6.641+0.0034*pt) || (pt > 30 && abs(eta)>1.4442 && hadTowOverEm<0.0481 && full5x5_sigmaIetaIeta()<0.03013 && chargedHadronIso<1.011 && neutralHadronIso < 1.715+0.0163*pt+0.000014*pt*pt && photonIso < 3.863+0.0034*pt)')
susyMuEGMonitoring.numGenericTriggerEventPSet.hltPaths = cms.vstring('HLT_Mu17_Photon30_IsoCaloId*')
susyMuEGMonitoring.denGenericTriggerEventPSet.hltPaths = cms.vstring('')

susyMonitorHLT = cms.Sequence(
    susyHLTRazorMonitoring
  + susyHLTVBFMonitoring
  + LepHTMonitor
  + susyHLTEleCaloJets
  + susyMuEGMonitoring 
)
