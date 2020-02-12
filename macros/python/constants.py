
from collections import OrderedDict

processes_per_systematic = {
'lumi': 'all',
'rate_ttbar': 'TTbar',
'rate_dy': 'DYJets',
'rate_ttv': 'TTV',
'rate_diboson': 'Diboson',
'rate_singletop': 'SingleTop',
'rate_wjets': 'WJets',
'rate_qcd': 'QCDMu',
'pu': 'all',
'pdf': 'all',
'muid': 'all',
'muiso': 'all',
'mutrigger': 'all',
'eleid': 'all',
'elereco': 'all',
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
'btag_bc': 'shape',
'btag_udsg': 'shape',
'rate_ttbar': 'lnN',
'rate_dy': 'lnN',
'rate_ttv': 'lnN',
'rate_diboson': 'lnN',
'rate_singletop': 'lnN',
'rate_wjets': 'lnN',
'rate_qcd': 'lnN',
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
'btag_bc': 1,
'btag_udsg': 1,
'rate_ttbar': 1.056,
'rate_dy': 1.1,
'rate_ttv': 1.25,
'rate_diboson': 1.2,
'rate_singletop': 1.1,
'rate_wjets': 1.25,
'rate_qcd': 1.5,
'scale_TTbar': 1,
'scale_DYJets': 1,
'scale_TTV': 1,
'scale_Diboson': 1,
'scale_SingleTop': 1
}

backgrounds_per_category = {
'A': ['TTbar', 'DYJets', 'SingleTop', 'TTV', 'Diboson'],
'B': ['TTbar', 'DYJets', 'SingleTop', 'TTV', 'Diboson', 'WJets', 'QCDMu']
}

signaltag = 'LQtoTMu'

variables_per_category = {
'A': 'MLQ',
'B': 'ST'
}

categories_per_channel = {
'srmu': ['A', 'B'],
'ttbar': ['B']
}

yeartags = {
2016: '2016v3',
2017: '2017v2',
2018: '2018'
}
