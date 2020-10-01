import os
import sys
import subprocess
import time
import signal
from multiprocessing import Process
from multiprocessing import Pool
from constants import *



def write_lines(path, filename, lines):
    outfile = open(path+filename, 'w')
    for line in lines:
        outfile.write(line)
        outfile.write('\n')
    outfile.close()

def get_lines_datacard_header(category, channel, mass):
    lines = []
    lines.append('# Datacard for variable %s in channel %s, category %s, for masspoint %i \n' % (variables_per_category[category], channel, category, mass))
    lines.append('# HEADER')
    lines.append('imax 1')
    lines.append('jmax %i' % (len(backgrounds_per_channel_and_category[category, channel])))
    lines.append('kmax *')
    return lines

def get_lines_datacard_channels(varcat):
    lines = []
    lines.append('# CHANNELS')
    lines.append('bin %s' % (varcat))
    lines.append('observation -1')
    return lines

def get_lines_datacard_input(rootfilename, year):
    lines = []
    #     shapes * * input/combine_histograms_mc_fullsyst_much.root $CHANNEL__$PROCESS_2016v3 $CHANNEL__$PROCESS_2016v3__$SYSTEMATIC
    lines.append('# INPUT')
    lines.append('shapes * * %s $CHANNEL__$PROCESS_%s $CHANNEL__$PROCESS_%s__$SYSTEMATIC' % (rootfilename, yeartags[year], yeartags[year]))
    return lines

def get_lines_datacard_processes(category, varcat, mass, backgrounds, channel):
    lines = []
    lines.append('# PROCESSES')

    line = 'bin        '
    for i in range(len(backgrounds_per_channel_and_category[category, channel]) + 1):
        line += varcat + '  '
    lines.append(line)


    #line = 'process    ' + signaltag + 'M' + str(mass) + '  '
    
    line = 'process    ' + signal_per_channel[channel] + 'M' + str(mass) + '  '


    for bkg in backgrounds:
        if bkg in backgrounds_per_channel_and_category[category, channel]:
            #if bkg == 'QCD' and "ele" in channel:
            #    line += 'QCDEle  '
            #elif bkg == 'QCD':
            #    line += 'QCDMu  '
            #else:
            line += bkg + '  '
    lines.append(line)

    line = 'process    0  '
    idx = 1
    for bkg in backgrounds:
        if bkg in backgrounds_per_channel_and_category[category, channel]:
            line += str(idx) + '  '
            idx += 1
    lines.append(line)

    line = 'rate       -1  '
    for bkg in backgrounds:
        if bkg in backgrounds_per_channel_and_category[category, channel]:
            line += '-1  '
    lines.append(line)

    return lines




def get_lines_datacard_systematics(category, systematics, backgrounds, channel):
    lines = []
    lines.append('# SYSTEMATICS')
    for syst in systematics:
        if not syst in processes_per_systematic.keys():
            raise RuntimeError('Systematic %s not in constant dictionaries.' % (syst))
        print 'syst: %s' % (syst)
        line = syst + '  ' + pdf_per_systematic[syst] + '  '
        # first for signal:
        if processes_per_systematic[syst] == 'all':
            line += str(value_per_systematic[syst]) + '  '
        else:
            line += '-  '
        for bkg in backgrounds:
            if bkg not in backgrounds_per_channel_and_category[category, channel]: continue
            if processes_per_systematic[syst] == 'all' or processes_per_systematic[syst] == bkg:
                line += str(value_per_systematic[syst]) + '  '
            else:
                line += '-  '
        lines.append(line)

    return lines

def get_lines_datacard_statistics():
    lines = []
    lines.append('# STATISTICS')
    lines.append('* autoMCStats 1 0 1')
    return lines



def create_datacard(year, mass, category, channel, backgrounds, systematics, path_datacards, rootfilename):
    print 'Creating datacard for mass %i and category %s. ' % (mass, category)

    if not os.path.exists(path_datacards):
        raise RuntimeError('Path %s does not exist.' % (path_datacards))
    # else: print 'Datacard directory: %s' % (path_datacards)
    if not os.path.exists(path_datacards + '/' + rootfilename):
        raise RuntimeError('Rootfile %s does not exist.' % (path_datacards + '/' + rootfilename))
    # else: print 'rootfile containing histograms: %s' % (path_datacards + '/' + rootfilename)

    filename_datacard = variables_per_category[category] + '_' + channel + '_cat' + category + '_M' + str(mass) + '.txt'
    # print 'filename: %s ' % (filename_datacard)
    # print 'going to create file: %s' % (path_datacards + '/' + filename_datacard)
    varcat = variables_per_category[category] + '_' + channel + '_cat' + category
    separator = ['-----------------------------\n']



    lines_header = get_lines_datacard_header(category, channel, mass) + separator
    lines_channels = get_lines_datacard_channels(varcat) + separator
    lines_input = get_lines_datacard_input(rootfilename, year)
    lines_processes = get_lines_datacard_processes(category, varcat, mass, backgrounds, channel)
    lines_systematics = get_lines_datacard_systematics(category, systematics, backgrounds, channel)
    lines_statistics = get_lines_datacard_statistics()








    lines = lines_header + lines_channels + lines_input + lines_processes + lines_systematics + lines_statistics
    write_lines(path_datacards + '/', filename_datacard, lines)
