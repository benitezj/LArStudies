from RecExConfig.RecFlags  import rec
from AthenaCommon.BeamFlags import jobproperties
from AthenaCommon.AthenaCommonFlags import athenaCommonFlags
from AthenaCommon.GlobalFlags import globalflags

#intput = ['/data/benitezj/LAr/NoiseStudies/data15_comm.00264034.physics_MinBias.merge.RAW._lb0819._SFO-ALL._0001.1',
#          '/data/benitezj/LAr/NoiseStudies/data15_comm.00264034.physics_MinBias.merge.RAW._lb0314._SFO-1._0001.1'
#]
 

tuple_name = 'caloD3PD_from_raw.root'

#athenaCommonFlags.EvtMax.set_Value_and_Lock(100)
athenaCommonFlags.BSRDOInput.set_Value_and_Lock(intput)
globalflags.InputFormat.set_Value_and_Lock('bytestream')

rec.doESD.set_Value_and_Lock(True)
rec.readRDO.set_Value_and_Lock(True)
rec.doWriteESD.set_Value_and_Lock(True)
rec.AutoConfiguration=['everything']
rec.doHist.set_Value_and_Lock(False)
rec.doCBNT.set_Value_and_Lock(False)
rec.doWriteTAGCOM.set_Value_and_Lock(False)
rec.doWriteTAG.set_Value_and_Lock(False)
rec.doWriteAOD.set_Value_and_Lock(False)
rec.doAOD.set_Value_and_Lock(False)
rec.doMonitoring.set_Value_and_Lock(False)
rec.readAOD.set_Value_and_Lock(False)


# RecExCommon
include ("RecExCommon/RecExCommon_topOptions.py")

# D3PDMaker calo block
from CaloD3PDMaker.CaloD3PD import CaloD3PD
alg = CaloD3PD(file = tuple_name, tuplename = 'caloD3PD')

# put OF iteration results on SG 
#ToolSvc.LArRawChannelBuilderToolOFCIter.StoreTiming=True
