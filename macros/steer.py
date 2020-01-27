
from python.CombineRunner import *


path_datacards = '/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/LQTopLep/data/datacards'
CombineRunner = CombineRunner(path_datacards, 2016)

systematics = ['lumi', 'rate_ttbar', 'rate_dy', 'rate_ttv', 'rate_diboson', 'rate_singletop', 'rate_wjets', 'rate_qcd', 'pu', 'pdf', 'muid', 'muiso', 'mutrigger', 'eleid', 'elereco', 'btag_bc', 'btag_udsg', 'scale_TTbar', 'scale_DYJets', 'scale_TTV', 'scale_Diboson', 'scale_SingleTop']

backgrounds = ['TTbar', 'DYJets', 'SingleTop', 'TTV', 'Diboson', 'WJets', 'QCDMu']

categories = ['A', 'B']

masspoints = [200, 300, 400, 500, 600, 700, 800, 900, 1000, 1200, 1400, 1700, 2000]


CombineRunner.CreateDatacards(masspoints, categories, backgrounds, systematics, 'combine_histograms_mc_fullsyst_much.root')
CombineRunner.CombineChannels(masspoints, categories)
CombineRunner.ExecuteCombineCombination(masspoints, categories)
