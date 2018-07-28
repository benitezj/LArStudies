if [ "$1" == "events" ]; then
root -b LArCalorimeter/LArStudies/run/RootLogon.C LArCalorimeter/LArStudies/run/printNTUPevents.C\(\"./outputNTUP.root\"\)
else
root -b LArCalorimeter/LArStudies/run/RootLogon.C LArCalorimeter/LArStudies/run/printNTUPcells.C\(\"./outputNTUP.root\"\)
fi
