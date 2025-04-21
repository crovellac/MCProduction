from CRABClient.UserUtilities import config
config = config()

Mass = '6'

inputDataset_ ={
'3p7':'/HToAATo4Tau_hadronic_tauDecay_M3p7_Run3_2023/lpcml-3p7_DIGI-Premix-26240d1e6039ee29161351aa2c33106e/USER'
,'4':'/HToAATo4Tau_hadronic_tauDecay_M4_Run3_2023/lpcml-4_DIGI-Premix-26240d1e6039ee29161351aa2c33106e/USER'
,'5':'/HToAATo4Tau_hadronic_tauDecay_M5_Run3_2023/lpcml-5_DIGI-Premix-26240d1e6039ee29161351aa2c33106e/USER'
,'6':'/HToAATo4Tau_hadronic_tauDecay_M6_Run3_2023/lpcml-6_DIGI-Premix-26240d1e6039ee29161351aa2c33106e/USER'
}.get(Mass, None)


inputProcess_ = {
'3p7': "/HToAATo4Tau_hadronic_tauDecay_M3p7_Run3_2023/phys_diffraction-3p7_DIGI-Premix_hadronic-c017b2c35ae16f5766f4c67c30206b8e/USER"
,'14':"/HToAATo4Tau_hadronic_tauDecay_M14_Run3_2023/phys_diffraction-14_DIGI-Premix_hadronic-c017b2c35ae16f5766f4c67c30206b8e/USER"
}.get(Mass, None)


#config.section_('General')
config.General.requestName = '%s_AODSIM_newBigProd'%Mass
config.General.workArea = 'crab_bigProduction'
config.General.transferOutputs = True
config.General.transferLogs = True

config.Data.inputDataset =inputDataset_
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'step3_AODSIM_cfg.py'
config.JobType.maxMemoryMB = 8000
config.JobType.numCores = 4 

#config.Data.inputDBS = 'global'
config.Data.inputDBS = 'phys03'
config.JobType.allowUndistributedCMSSW = True

config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1 

config.Data.ignoreLocality = True
config.Site.whitelist = [
    'T2_US_Caltech', 'T2_US_MIT', 'T2_US_Nebraska', 'T2_US_Purdue', 'T2_US_UCSD', 'T2_US_Wisconsin','T2_CH_CERN',
    'T2_AT_Vienna', 'T2_BE_IIHE', 'T2_BE_UCL', 'T2_BR_SPRACE', 'T2_BR_UERJ'
]
config.Data.outLFNDirBase = '/store/group/lpcml/rchudasa/MCGenerationRun3'
#config.Site.storageSite = 'T2_CH_CERN'
config.Site.storageSite = 'T3_US_FNALLPC'
config.Data.publication = True 
config.Data.outputDatasetTag = config.General.requestName
