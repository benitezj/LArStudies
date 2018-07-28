
##Make the noise tuple for the October run
cd LArCalorimeter/LArStudies/macros/
rootb makeTupleD3PD.C
cd -

###Plot the noise in the October run
rootb LArCalorimeter/LArStudies/plots/plotNoise.C\(\"NTUPLES/LArSamples/OctoberRun/Tuple/tuple.root\",\"October\"\)

###Make the Coherent noise tuple for the October run
rootb LArCalorimeter/LArStudies/run/RootLogon.C LArCalorimeter/LArStudies/run/makeTupleCoherent.C\(\"NTUPLES/LArSamples/OctoberRun/Tuple\",\"NTUPLES/LArSamples/OctoberRun/TupleCoherent\"\)

##Plot the Coherenet noise in the October run
rootb LArCalorimeter/LArStudies/plots/plotNoise.C\(\"NTUPLES/LArSamples/OctoberRun/TupleCoherent/tuple.root\",\"OctoberCoh\"\)

