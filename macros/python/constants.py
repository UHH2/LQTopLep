
from collections import OrderedDict

processes_per_systematic = {
'lumi': 'all',
'rate_ttbar': 'TTbar',
'rate_dy': 'DYJets',
'rate_ttv': 'TTV',
'rate_diboson': 'Diboson',
'rate_singletop': 'SingleTop',
'rate_wjets': 'WJets',
#'rate_qcd': 'QCD',
'rate_qcdmu': 'QCDMu',
'rate_qcdele': 'QCDEle',
'pu': 'all',
'pdf': 'all',
'muid': 'all',
'muiso': 'all',
'mutrigger': 'all',
'eleid': 'all',
'elereco': 'all',
'eletrigger': 'all',
'btag_bc': 'all',
'btag_udsg': 'all',
'scale_TTbar': 'TTbar',
'scale_DYJets': 'DYJets',
'scale_TTV': 'TTV',
'scale_Diboson': 'Diboson',
'scale_SingleTop': 'SingleTop'
}

pdf_per_systematic = {
'lumi': 'lnN',
'pu': 'shape',
'pdf': 'shape',
'muid': 'shape',
'muiso': 'shape',
'mutrigger': 'shape',
'eleid': 'shape',
'elereco': 'shape',
'eletrigger': 'shape',
'btag_bc': 'shape',
'btag_udsg': 'shape',
'rate_ttbar': 'lnN',
'rate_dy': 'lnN',
'rate_ttv': 'lnN',
'rate_diboson': 'lnN',
'rate_singletop': 'lnN',
'rate_wjets': 'lnN',
#'rate_qcd': 'lnN',
'rate_qcdmu': 'lnN',
'rate_qcdele': 'lnN',
'scale_TTbar': 'shape',
'scale_DYJets': 'shape',
'scale_TTV': 'shape',
'scale_Diboson': 'shape',
'scale_SingleTop': 'shape'
}

value_per_systematic = {
'lumi': 1.025,
'pu': 1,
'pdf': 1,
'muid': 1,
'muiso': 1,
'mutrigger': 1,
'eleid': 1,
'elereco': 1,
'eletrigger': 1,
'btag_bc': 1,
'btag_udsg': 1,
'rate_ttbar': 1.056,
'rate_dy': 1.1,
'rate_ttv': 1.25,
'rate_diboson': 1.2,
'rate_singletop': 1.1,
'rate_wjets': 1.25,
#'rate_qcd': 1.5,
'rate_qcdmu': 1.5,
'rate_qcdele': 1.5,
'scale_TTbar': 1,
'scale_DYJets': 1,
'scale_TTV': 1,
'scale_Diboson': 1,
'scale_SingleTop': 1
}

#backgrounds_per_category = {
#'A': ['TTbar', 'DYJets', 'TTV', 'Diboson'], #SingleTop
#'B': ['TTbar', 'DYJets', 'SingleTop', 'TTV', 'Diboson', 'WJets', 'QCD']
#}





#backgrounds_per_channel ?
backgrounds_per_channel_and_category = {
tuple(['A', 'srmu']): ['TTbar', 'DYJets', 'SingleTop', 'TTV', 'Diboson'],
tuple(['A', 'dycrmu']): ['TTbar', 'DYJets', 'SingleTop', 'TTV', 'Diboson'],
tuple(['A', 'srele']): ['TTbar', 'DYJets', 'TTV', 'Diboson'],
tuple(['A', 'dycrele']): ['TTbar', 'DYJets', 'TTV', 'Diboson'],
tuple(['B', 'srmu']): ['TTbar', 'DYJets', 'SingleTop', 'TTV', 'Diboson', 'WJets', 'QCDMu'],
tuple(['B', 'dycrmu']): ['TTbar', 'DYJets', 'SingleTop', 'TTV', 'Diboson', 'WJets', 'QCDMu'],
tuple(['B', 'srele']): ['TTbar', 'DYJets', 'SingleTop', 'TTV', 'Diboson', 'WJets', 'QCDEle'],
tuple(['B', 'dycrele']): ['TTbar', 'DYJets', 'SingleTop', 'TTV', 'Diboson', 'WJets', 'QCDEle'],
tuple(['B', 'ttbar']): ['TTbar', 'DYJets', 'SingleTop', 'TTV', 'Diboson', 'WJets', 'QCDMu'] #QCDEle?
}

# ttbar immer mit LQtoTMu signal?
# evtl einfach beide signal samples in jedem Channel verwenden?
signal_per_channel = {
'srmu': 'LQtoTMu',
'dycrmu': 'LQtoTMu',
'ttbar': 'LQtoTMu', # 'LQtoTE'
'srele': 'LQtoTE',
'dycrele': 'LQtoTE'

}

#signaltag = 'LQtoT'


variables_per_category = {
'A': 'MLQ',
'B': 'ST'
}

categories_per_channel = {
'srmu': ['A', 'B'],
'dycrmu': ['A', 'B'],
'ttbar': ['B'],
'srele': ['A', 'B'],
'dycrele': ['A', 'B']

}

yeartags = {
2016: '2016v3',
2017: '2017v2',
2018: '2018'
}
