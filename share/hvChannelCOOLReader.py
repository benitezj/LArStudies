#!/bin/env python
import sys,os,getopt

#To work around the damn DBRelease. I *hate* DBreleases!!!!
os.environ["CORAL_DBLOOKUP_PATH"]="/afs/cern.ch/atlas/software/builds/AtlasCore/16.6.3/InstallArea/XML/AtlasAuthentication"

from PyCool import cool
from time import *


dbname="COMP200"
#dbname="CONDBR2"

def runLumiToTimeStamp(runnumber):
    run=long(runnumber)
    dbSvc = cool.DatabaseSvcFactory.databaseService()
    db=dbSvc.openDatabase("COOLONL_TRIGGER/"+dbname)
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
    result="Channel: %i, %s V=%.2f I=%.4f" %  (obj.channelId(),asctime(localtime(obj.since()/1e9)),v,i)

    lastlb=0
    if lbtimes is not None:
        t=obj.since();
        for lb,lbend in lbtimes:
            if t<lbend:
                result+=" LB:%i" % lb
                break
            lastlb=lb
    
    return result


def printChannelInfo(db,folderName,chnl,t1,t2,lbtimes=None):
    
    if not db.existsFolder(folderName):
        print "ERROR: Folder",folderName,"does not exist"
        return None
    
    f=db.getFolder(folderName)

    #lastVoltage=dict()
    #lastStat=dict()

    itr=f.browseObjects(cool.ValidityKey(t1),
                        cool.ValidityKey(t2),
                        cool.ChannelSelection.all())
    while itr.goToNext():
        obj=itr.currentRef()
        chid=obj.channelId();
        pl=obj.payload()
        stat=pl["R_STAT"]
        v=pl["R_VMEAS"]
        i=pl["R_IMEAS"]

        if chid==chnl:
            print "Status",eventInfo(obj,stat,v,i,lbtimes)
    
    itr.close()                         
    return 



if __name__=="__main__":

    def usage():
        print "Usage:"
        print "%s <runnumber>" % sys.argv[0].split("/")[-1]
        print "%s <channelID>" % sys.argv[1].split("/")[-1]
        sys.exit(-1)
      
    if len(sys.argv)!=3:
        usage()

    rns=sys.argv[1]
    if not rns.isdigit():
        usage();

    run=long(rns)
    #run=178109

    chnls=sys.argv[2]
    if not chnls.isdigit():
        usage();

    chnl=long(chnls)

    t1,t2,lbtimes=runLumiToTimeStamp(run)
    
    if t1 is None:
        print "invalid t1 variable", exit()
    
    #print t1,t2,lbtimes
    #print "Reading channel current in run %i, lasting from %s to %s" % (run,asctime(localtime(t1/1e9)),asctime(localtime(t2/1e9)))

    dbSvc = cool.DatabaseSvcFactory.databaseService()
    db=dbSvc.openDatabase("COOLOFL_DCS/"+dbname)

    #printChannelInfo(db,"/LAR/DCS/HV/BARREL/I8",t1,t2,lbtimes)
    printChannelInfo(db,"/LAR/DCS/HV/BARREl/I16",chnl,t1,t2,lbtimes)

    db.closeDatabase()
