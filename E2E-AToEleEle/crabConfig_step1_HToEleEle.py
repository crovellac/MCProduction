from CRABClient.UserUtilities import config
config = config()
# See parameter defintions here: https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3ConfigurationFile#CRAB_configuration_parameters

# To submit to crab:
# crab submit -c crabConfig_data.py
# To check job status:
# crab status -d <config.General.workArea>/<config.General.requestName>
# To resubmit jobs:
# crab resubmit -d <config.General.workArea>/<config.General.requestName>

# Local job directory will be created in:
# <config.General.workArea>/<config.General.requestName>
config.General.workArea = 'crab_MC'
config.General.requestName = 'gen'
config.General.transferOutputs = True
config.General.transferLogs = False

# CMS cfg file goes here:
config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = 'step1_GEN-SIM_AToEleEle_cfg.py' # cms cfg file for generating events
config.JobType.maxMemoryMB = 2500

# Define units per job here:
config.Data.splitting = 'EventBased'
#config.Data.unitsPerJob = 15000
#config.Data.unitsPerJob = 30     # units: as defined by config.Data.splitting
#config.Data.totalUnits  = 300000   # test production
#config.Data.totalUnits  = 30000
#config.Data.totalUnits = 15000000
config.Data.totalUnits = 1500000
config.Data.unitsPerJob = 1500

config.Data.publication = False

# Output files will be stored in config.Site.storageSite at directory:
# <config.Data.outLFNDirBase>/<config.Data.outputPrimaryDataset>/<config.Data.outputDatasetTag>/
config.Site.storageSite = 'T3_US_FNALLPC'
config.Data.outLFNDirBase = '/store/user/ccrovell' # add your username as subdirectory
config.Data.outputPrimaryDataset = 'gen_HToEleEle_m0p0To1p1_pT40To160_ctau0p0_eta0To2p4_pythia8_noPU'
config.Data.outputDatasetTag = config.General.requestName
