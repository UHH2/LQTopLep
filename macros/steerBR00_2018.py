

from python.CombineRunner import *


path_datacards = '/nfs/dust/cms/user/frahmmat/CMSSW_10_2_X_v2/CMSSW_10_2_16/src/UHH2/LQTopLep/data/datacards'
CombineRunner = CombineRunner(path_datacards, 2018)



systematics = ['lumi', 'rate_ttbar', 'rate_dy', 'rate_ttv', 'rate_diboson', 'rate_singletop', 'rate_wjets', 'rate_qcdmu', 'pu', 'pdf', 'muid', 'muiso', 'mutrigger', 'eleid', 'elereco', 'eletrigger', 'btag_bc', 'btag_udsg', 'scale_TTbar', 'scale_DYJets', 'scale_TTV', 'scale_Diboson', 'scale_SingleTop']


backgrounds = ['TTbar', 'DYJets', 'SingleTop', 'TTV', 'Diboson', 'WJets', 'QCDMu'] #QCDMu needed for ttbar


categories = ['A', 'B']
channels = ['srmu', 'ttbar', 'dycrmu'] # 'srele', 'srmu', 'ttbar', 'dycrele', 'dycrmu'

signaltag = 'LQtoTL'
BR = '00'

masspoints = [200, 300, 400, 500, 600, 700, 800, 900, 1000, 1200, 1400, 1700, 2000]


CombineRunner.CreateDatacards(masspoints, categories, channels, backgrounds, systematics, 'combine_histograms_mc_fullsyst_BR_00_2018.root', signaltag)
CombineRunner.CombineChannels(masspoints, categories, channels)
CombineRunner.ExecuteCombineCombination(masspoints, categories, channels, signaltag, BR)

