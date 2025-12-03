from CRABClient.UserUtilities import config
config = config()
# See parameter defintions here: https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3ConfigurationFile#CRAB_configuration_parameters

# To submit to crab:
# crab submit -c crabConfig_data.py
# To check job status:
# crab status -d <config.General.workArea>/<config.General.requestName># To resubmit jobs:
# crab resubmit -d <config.General.workArea>/<config.General.requestName>

# Local job directory will be created in:
# <config.General.workArea>/<config.General.requestName>
config.General.workArea = 'crab_pileup'
config.General.requestName = 'pileup'
config.General.transferOutputs = True
config.General.transferLogs = False

# CMS cfg file goes here:
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = './step2_HLT_pileup_with_fileList_cfg.py'
config.JobType.maxMemoryMB = 8000
config.JobType.numCores=8
#config.JobType.maxJobRuntimeMin = 500
# Define input and units per job here:
config.Data.userInputFiles = open('list_gen_files.txt').readlines()
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1 # units: as defined by config.Data.splitting
config.Data.totalUnits =  -1 # -1: all inputs. total jobs submitted = totalUnits / unitsPerJob. cap of 10k jobs per submission
config.Data.publication = False

# Output files will be stored in config.Site.storageSite at directory:
config.Site.storageSite = 'T3_US_FNALLPC'
config.Site.whitelist = ['T3_US_FNALLPC','T2_CH_CERN']
config.Data.outLFNDirBase = '/store/user/ccrovell' # add your username as subdirectory
config.Data.outputPrimaryDataset = 'digi_raw_HToEleEle_m0p0To1p1_pT40To160_ctau0p0_eta0To1p4_pythia8_noPU' 
config.Data.outputDatasetTag = config.General.requestName
