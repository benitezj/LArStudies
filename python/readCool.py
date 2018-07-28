#!/usr/bin/env python2.5

import sqlite3, os, sys
from optparse import OptionParser
from PyCool import cool

import time
from calendar import timegm
from datetime import datetime
import math

dbSvc = cool.DatabaseSvcFactory.databaseService()
dcsDB = dbSvc.openDatabase("oracle://ATLAS_COOLPROD;schema=ATLAS_COOLOFL_DCS;dbname=CONDBR2")

def timeForCool(T):
    return int(time.mktime(time.strptime(T,"%Y-%m-%d:%H:%M:%S")))*1000000000L

def getHVlineHistory(hvline,T1,T2):

    ##connect to folder in COOL
    folder = dcsDB.getFolder("/LAR/DCS/HV/BARREl/I16")

    ##define time window to read
    t1 = timeForCool(T1)
    t2 = timeForCool(T2)

    ##open file for output
    filename = './HVLine_'+str(hvline)+'_'+str(int(t1/1e9))+'_'+str(int(t1/1e9))+'.dat' 
    f = file(filename,'w')

    ##read from COOL and print values
    itr = folder.browseObjects(t1, t2, cool.ChannelSelection(hvline))
    while itr.goToNext():
        obj=itr.currentRef()
        pl=obj.payload()
        timeFormatted = time.strftime("%Y-%m-%d:%H:%M:%S",localtime(obj.since()/1e9))
        timeSeconds=int(time.mktime(time.strptime(timeFormatted,"%Y-%m-%d:%H:%M:%S")))

        f.write(str(timeSeconds) + ' ' + str(pl["R_VMEAS"]) + ' ' + str(pl["R_IMEAS"]) + ' ' + str(pl["R_STAT"]) + '\n')
        
    itr.close()
    f.close()
    return


############## Main program

getHVlineHistory(241007,"2015-08-22:08:20:00","2015-08-22:08:40:00")

print 'Done!'
