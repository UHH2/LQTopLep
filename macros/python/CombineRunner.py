import os
import sys
import subprocess
from multiprocessing import Process

from functions import *



class CombineRunner:

    def __init__(self, path_datacards, year):
        self.path_datacards = path_datacards
        self.year = year
        print 'created an instance of "CombineRunner". Let\'s go!'


    def CreateDatacards(self, masspoints, categories, backgrounds, systematics, rootfilename):
        for mass in masspoints:
            for cat in categories:
                create_datacard(self.year, mass, cat, backgrounds, systematics, self.path_datacards, 'input/' + rootfilename)

    def CombineChannels(self, masspoints, categories):
        combine_dir = os.getenv('CMSSW_BASE') + '/src/HiggsAnalysis/CombinedLimit'
        if not os.path.exists(combine_dir):
            raise RuntimeError('Combine not set-up where expected: %s.' % (combine dir))
        processes = []
        for mass in masspoints:
            datacards = []
            finalname = self.path_datacards + '/COMB_cat'
            for cat in categories:
                datacards.append(self.path_datacards + '/' + variables_per_category[cat] + '_cat' + cat + '_M' + str(mass) + '.txt')
                finalname += cat
            finalname += '_M' + str(mass) + '.txt'
            command = [combine_dir + '/scripts/combineCards.py']
            command += datacards
            f = open(finalname, 'w')
            processes.append(subprocess.Popen(command, stdout=f))

        for p in processes:
            p.wait()

    def ExecuteCombineCombination(self, masspoints, categories):
        cwd = os.getcwd()
        if not os.path.exists(self.path_datacards + '/output'):
            raise RuntimeError('Combine output directory not where expected: %s.' % (self.path_datacards + '/output'))
        os.chdir(self.path_datacards + '/output');
        combine_dir = os.getenv('CMSSW_BASE') + '/src/HiggsAnalysis/CombinedLimit'
        processes = []
        for mass in masspoints:
            combcard = self.path_datacards + '/COMB_cat'
            for cat in categories:
                combcard += cat
            combcard += '_M' + str(mass) + '.txt'
            command = ['combine', '-n', signaltag, '-m', str(mass), combcard]
            processes.append(subprocess.Popen(command))

        for p in processes:
            p.wait()

        os.chdir(cwd)
