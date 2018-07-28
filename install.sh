###Setup for timing studies:
aset
asetup 20.1.4.2,gcc48 --grouparea=/afs/cern.ch/user/l/larcomm/w0/digitMon/LArCAF

###Need this package:
atlasoff/LArCalorimeter/LArCafJobs/trunk

##These are for more info:
atlasoff/LArCalorimeter/LArROD/trunk
atlasoff/LArCalorimeter/LArSamplesMon/trunk
atlasoff/PhysicsAnalysis/DerivationFramework/DerivationFrameworkCore/trunk
atlasoff/PhysicsAnalysis/DerivationFramework/DerivationFrameworkHiggs/trunk
atlasoff/PhysicsAnalysis/DerivationFramework/DerivationFrameworkInterfaces/trunk

##Command from Poppy to transform the RAW data into "LArSamples" format
LArCAF_trf.py --inputBS root://eosatlas//eos/atlas/atlastier0/rucio/data15_cos/physics_CosmicCalo/00263848/data15_cos.00263848.physics_CosmicCalo.merge.RAW/data15_cos.00263848.physics_CosmicCalo.merge.RAW._lb0092._SFO-ALL._0001.1 --outputNTUP test.root


####To compute the coherent noise we need to compute the ratio:
sqrt [ RMS_sum ^ 2  -   Sum( RMS_i ^ 2 )   ]   /  Sum( RMS_i )

* where RMS_i is the RMS value for one particular channel. 
* The  Sum is over the channels in one FEB
* RMS_sum is the RMS value for the quantity : Sum(x_i) , where x_i is the sample[0]-pedestal value for one channel
* For a given channel the RMS is given by  
     RMS_i = sqrt[ Sum_j( x_i_j ^2 ) / N ] 
  - where x_i_j is the sample[0]-pedestal value for one channel (i)  in one event (j), 
  - and N is the number of events
* In terms of processing the data one needs to track the quantities:
  -  R_k = Sum_j( x_i_j ^2 ) : sum over one job 
  -  N_k :  number of events for each job 
  - The two quantities need to be added before computing the ratio.
      RMS_i = sqrt[ Sum_k( R_k ) /  Sum_k( N_k ) ]
 
