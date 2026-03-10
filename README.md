# MCProduction

Repository for generating Monte Carlo samples for E2E analyses. For analysis, see [MLAnalyzer_run3](https://github.com/crovellac/MLAnalyzer_run3).

The step-by-step instructions for setting up this repository are given below.

## Setting up CMSSW
```
export SCRAM_ARCH=el8_amd64_gcc11
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsrel CMSSW_13_0_13
cd CMSSW_13_0_13/src
eval `scram runtime -sh`
cmsenv
```

## Add GeneratorInterface
```
git cms-init
git cms-addpkg GeneratorInterface/Pythia8Interface
git cms-addpkg GeneratorInterface/GenFilters
```

## Add this repository
```
git clone https://github.com/crovellac/MCProduction.git
cd MCProduction
git checkout run3
cd ..
```

## Move filters and plugins
Note that we have to add some custom plugins and filters from this repo into `GeneratorInterface`.
```
cp -r MCProduction/GeneratorInterface .
scram b -j16
```

## Test generating a sample
```
cd MCProduction/E2E-AToEleEle
cmsRun step1_GEN-SIM_AToEleEle_cfg.py
```
