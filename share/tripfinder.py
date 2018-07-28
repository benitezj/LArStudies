#!/bin/env python
import sys,os,getopt

#To work around the damn DBRelease. I *hate* DBreleases!!!!
os.environ["CORAL_DBLOOKUP_PATH"]="/afs/cern.ch/atlas/software/builds/AtlasCore/16.6.3/InstallArea/XML/AtlasAuthentication"

from PyCool import cool
from time import *


def printStatus(stat):

    if stat & 0x400:
        result="Channel ON"
    else:
        result="Channel OFF"
    
    if stat & 1:
        result+= " Current Trip"
    if stat & 2:
        result+= " Sum error"
    if stat & 0x200:
        result+= " Input error"
    if stat & 0x800:
        result+=" Ramping"
    if stat & 0x1000:
        result+=" Emergency stop"
    if stat & 0x2000:
        result+=" Kill enable"
    if stat & 0x4000:
        result+= " Current Limit Error"
    if stat & 0x8000:
        result+= " Voltage Limit Error"
    return result


def isAllOk(stat):
    #Ignore kill-enable bit, after that must be "channel on" and nothing else
    return (stat & 0xde81)==0x400 

def isOff(stat):
    return (stat & 0x400)==0


def isRamp(stat):
    return (stat & 0x800)!=0

def isStableZero(stat,v,i):
    return (not isRamp(stat) and v<20 and i<0.002)

def runLumiToTimeStamp(runnumber):
    run=long(runnumber)
    dbSvc = cool.DatabaseSvcFactory.databaseService()
    db=dbSvc.openDatabase("COOLONL_TRIGGER/CONDBR2")
    f=db.getFolder("/TRIGGER/LUMI/LBLB")

    t1=run<<32
    t2=(run<<32) + 0xFFFFFFFFL

    runstart=None
    runstop=None
    lbtimes=[]
    itr=f.browseObjects(cool.ValidityKey(t1),cool.ValidityKey(t2),cool.ChannelSelection(0))
    while itr.goToNext():
        obj=itr.currentRef()
        pl=obj.payload()
        lbstart=pl["StartTime"]
        if runstart is None:
            runstart=lbstart
        runstop=pl["EndTime"]
        lbtimes+=[(obj.since()&0xFFFFFFFF,runstop),]
    itr.close()
    db.closeDatabase()
    #print lbtimes
    return (runstart, runstop, lbtimes)


def eventInfo(obj,stat,v,i,lbtimes=None):
    result="Channel: %i, %s V=%.2f I=%.4f %s" %  (obj.channelId(),asctime(localtime(obj.since()/1e9)),v,i,printStatus(stat))

    lastlb=0
    if lbtimes is not None:
        t=obj.since();
        for lb,lbend in lbtimes:
            if t<lbend:
                result+=" LB:%i" % lb
                break
            lastlb=lb
    
    return result


def findLArHVTrip(db,folderName,t1,t2,lbtimes=None):
    
    allTrips=[]
    if not db.existsFolder(folderName):
        print "ERROR: Folder",folderName,"does not exist"
        return None
    
    f=db.getFolder(folderName)

    #lastVoltage=dict()
    #lastStat=dict()

    offMap=dict()

    tripped=set()
    stablezero=set()


    itr=f.browseObjects(cool.ValidityKey(t1),cool.ValidityKey(t2),
                        cool.ChannelSelection.all())
    while itr.goToNext():
        obj=itr.currentRef()
        chid=obj.channelId();
        pl=obj.payload()
        stat=pl["R_STAT"]
        v=pl["R_VMEAS"]
        i=pl["R_IMEAS"]

        #if chid==322000:
        #    print "Status",eventInfo(obj,stat,v,i,lbtimes)

        if not offMap.has_key(chid):
            offMap[chid]=isOff(stat)

        if offMap[chid]==True: continue

        
        if chid in tripped: #Channel previously found to be tripped
            #print "Status: ",eventInfo(obj,stat,v,i,lbtimes)
            if isAllOk(stat) and v>50:
                print "Recovered: ",eventInfo(obj,stat,v,i,lbtimes)
                tripped.remove(chid)
                if chid in stablezero: stablezero.remove(chid)
            if chid in stablezero:
                if not isStableZero(stat,v,i):
                    print "Not any more stable at zero voltage",eventInfo(obj,stat,v,i,lbtimes)
                    stablezero.remove(chid)
            else: # Not (yet) in stablezero:
                if isStableZero(stat,v,i):
                    print "Stable at zero voltage",eventInfo(obj,stat,v,i,lbtimes) # LB+1!!!
                    stablezero.add(chid)
        else: #Not (yet) found to be
            #if not isOff(stat) and not isAllOk(stat):
            if not isAllOk(stat):
                print "\nTrip: ",eventInfo(obj,stat,v,i,lbtimes)
                tripped.add(chid)
                
        pass
    
    itr.close()                         
    return 



if __name__=="__main__":

    def usage():
        print "Usage:"
        print "%s <runnumber>" % sys.argv[0].split("/")[-1]
        sys.exit(-1)
      
    if len(sys.argv)!=2:
        usage()

    rns=sys.argv[1]
    if not rns.isdigit():
        usage();

    run=long(rns)
    #run=178109

    t1,t2,lbtimes=runLumiToTimeStamp(run)

    #print t1/1000000000,t2/1000000000,lbtimes
    print "Searching for LAr HV Trips during run %i, lasting from %s to %s" % (run,asctime(localtime(t1/1e9)),asctime(localtime(t2/1e9)))
    exit

    dbSvc = cool.DatabaseSvcFactory.databaseService()
    db=dbSvc.openDatabase("COOLOFL_DCS/CONDBR2")

    findLArHVTrip(db,"/LAR/DCS/HV/BARREL/I8",t1,t2,lbtimes)
    findLArHVTrip(db,"/LAR/DCS/HV/BARREl/I16",t1,t2,lbtimes)

    db.closeDatabase()
