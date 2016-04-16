from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'DoubleEGRun2015C'
config.General.workArea = 'DoubleEGRun2015C'
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'run_data_74X.py'

config.section_("Data")
config.Data.inputDataset = '/DoubleEG/Run2015C_25ns-05Oct2015-v1/AOD'
config.Data.inputDBS = 'global'
config.Data.lumiMask = "https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON.txt"
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 5
#NJOBS = 5  # This is not a configuration parameter, but an auxiliary variable that we use in the next line.
#config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.publication = False
#config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/'
config.Data.outputDatasetTag = 'DoubleEG'
#Use your own username instead of the "lhx". Keep branch tag in the directory name, e.g., PHYS14_720_Dec23_2014.
config.Data.outLFNDirBase = '/store/user/msun/2015C/'

config.Data.ignoreLocality = False

config.section_("Site")
config.Site.storageSite = 'T3_US_FNALLPC'
