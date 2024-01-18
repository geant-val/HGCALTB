##**************************************************
## \file parser.py
## \brief: Implementation of HGCALTB parser
##         for geant-val usage
## \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
##          @lopezzot
## \start date: 16 January 2024
##**************************************************

#!/usr/bin/env python3

import os.path
import os
import subprocess

from gts.BaseParser import BaseParser, mergeROOT, mktemp
from gts.utils import getJSON
import ROOT

class Test(BaseParser):
    TEST = "HGCALTB"
    IGNOREKEYS = ["ENERGY"] # we nedd all energies in the analysis
    def parse(self,jobs):
        energies = ['20.','50.','80.','100.','120.','200.','250.','300.']
        physlist = jobs[0]['PHYSLIST']
        g4ver = jobs[0]['VERSION']
        print('start parsing ' + str(len(jobs)) + ' files from Geant4 ' + g4ver + ' using ' + physlist)
        dirName = 'analysis_'+g4ver+'_'+physlist
        os.mkdir(dirName)
        for job in jobs:
            for counter, energy in enumerate(energies):
                if job['ENERGY'] == energies[counter] :
                    os.system('cp ' + job['path'] + '/HGCALTBout_Run0.root ' + dirName + '/HGCALTBout_Run' + str(counter) + '.root')
                    #os.system('cp ' + job['path'] + '/energy.C ' + dirName)
                    os.system('cp ' + 'energy.C ' + dirName)
        cmnd = ['root', '-l', '-b', '-q', 'energy.C("")']
        proc = subprocess.Popen(cmnd, cwd=dirName, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
        output, error = proc.communicate()
        print(Output: output)
        print(Error: error)
        # dummy
        yield getJSON(jobs[0], 'chart',
            mctool_name='GEANT4',
            mctool_model=physlist,
            targetName='HGCALTB',
            beamParticle='pi-',
            beamEnergies=list(),
            observableName='Energy Resolution',
            xAxisName=list(),
            yAxisName=list(),
            xValues=list(),
            yValues=list(),
            yStatErrorsMinus=list(),
            yStatErrorsPlus=list(),
        )
