#!/usr/bin/perl
# -w
use File::Basename;
$eos="/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select";

$samplesfile=shift;#must be a full path in AFS
$option=shift;

##Determine path where log files will be written
$samplesfile = `readlink -e $samplesfile`;
chomp($samplesfile);
$SUBMITDIR = dirname($samplesfile);

##determine the input and output paths
open(my $FILEHANDLE, '<:encoding(UTF-8)', $samplesfile)
or die "Could not open file '$samplesfile' $!";
$INPUTDIR = <$FILEHANDLE>;
$INPUTDIR =~ s/^\s+|\s+$//g;
$OUTPUTDIR = <$FILEHANDLE>;
$OUTPUTDIR =~ s/^\s+|\s+$//g;

##determine if reading from EOS
$instorage="";
if( `echo $INPUTDIR | grep eos` ne ""){
$instorage="eos";
}

##determine if writing to EOS
$outstorage="";
if( `echo $OUTPUTDIR | grep eos` ne ""){
    $outstorage="eos";

    ##make sure writing to my eos
    if( `echo $OUTPUTDIR | grep benitezj` eq ""){
	die "ERROR: output path not mine.";
    }
}


print "SUBMITDIR = $SUBMITDIR\n";
print "INPUTDIR = $INPUTDIR\n";
print "OUTPUTDIR = $OUTPUTDIR\n";
print "input storage = $instorage\n";
print "output storage = $outstorage\n";

#################################################
##read the samples list and print
#################################################
@samples=`cat $samplesfile | grep merge.RAW | grep -v "#" | grep -v '/' `;
$counter = 0;
foreach $samp (@samples){
	chomp($samp);
	$samp =~ s/^\s+|\s+$//g; ##remove beg/end spaces
	print "$option $samp\n";
	$samples[$counter] = $samp;
	$counter++;
}

####################################################
####Clean out the output directory
####################################################
if($option eq "clean"){
    print "Removing all files inside output directory:\n";
    print "-------------------------\n";
    foreach $samp (@samples){
	if($outstorage eq "eos"){
	    $command="$eos rm -r $OUTPUTDIR/$samp";
	}else {
	    $command="rm -rf $OUTPUTDIR/$samp";
	}
	print "$command \n";
	system($command);
    }


    ####clean out the submission directory
    print "Removing all files in submit directory:\n";
    print "-------------------------\n";
    foreach $samp (@samples){
	$command="rm -rf ${SUBMITDIR}/${samp}";
	print "$command \n";
	system($command);
    }
}



####################################################
####Clean out the output directory
####################################################
if($option eq "size"){
    print "Determine number of events in the run:\n";
    print "-------------------------\n";
    foreach $samp (@samples){
	$NEvts  = `ami show dataset info $samp | grep totalEvents`;
	chomp($NEvts);
	$NFiles = `ami show dataset info $samp | grep nFiles`;
	chomp($NFiles);
	print "$samp: $NFiles , $NEvts \n";
    }

}




####################################################
#######FUNCTION FOR MAKING SHELL EXECUTION SCRIPTS
####################################################
sub makeBatchExecutable {
    $OUTPUTDIR = $_[0];
    $SUBMITDIR = $_[1];
    $sample = $_[2];
    $idx = $_[3];
    $cpin = $_[4];
    $cpout = $_[5];
    $filelist = $_[6];
    $algo = $_[7];


    $outfile="${SUBMITDIR}/${sample}/tuple_${idx}.sh";
    `rm -f $outfile`;
    `touch $outfile`;

    `echo "mkdir -p ./tmp/${sample} " >> $outfile`;
    `echo "${cpin} ${filelist} ./tmp/${sample}/ " >> $outfile`;
    `echo "/bin/ls ./tmp/${sample}/ " >> $outfile`;
    `echo "cd \\\$LS_SUBCWD " >> $outfile`;
    `echo "source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.sh " >> $outfile`;
    `echo "source \\\$AtlasSetup/scripts/asetup.sh " >> $outfile`;
    `echo "printenv " >> $outfile`;
    `echo "cd -" >> $outfile`;
    
    if($algo eq "RAW"){
	###This takes the raw data into an flat ntuple
	`echo "RAWTransform_trf.py --inputBS ./tmp/${sample}/* --outputNTUP outputNTUP.root \>\> tuple.log 2>\&1" >> $outfile`;
	`echo "root -b \\\$LS_SUBCWD/LArStudies/run/RootLogon.C \\\$LS_SUBCWD/LArStudies/run/makeTuple.C\\\\\(\\\\\\"./outputNTUP.root\\\\\\"\\\\\) \>\> tuple.log 2>\&1 " >> $outfile`;
    }
    if($algo eq "COH"){
	##This takes ntuple and produces histograms
	`echo "root -b \\\$LS_SUBCWD/LArStudies/run/RootLogon.C \\\$LS_SUBCWD/LArStudies/run/makeTupleCoherent.C\\\\\(\\\\\\"./tmp/${sample}\\\\\\",\\\\\\".\\\\\\"\\\\\) \>\> tuple.log 2>\&1 " >> $outfile`;
    }

   if($algo eq "RAWCOH"){
	###This takes the raw data directly to histograms
	`echo "RAWTransform_trf.py --inputBS ./tmp/${sample}/* --outputNTUP outputNTUP.root \>\> tuple.log 2>\&1" >> $outfile`;
	`echo "root -b \\\$LS_SUBCWD/LArStudies/run/RootLogon.C \\\$LS_SUBCWD/LArStudies/run/makeTuple.C\\\\\(\\\\\\"./outputNTUP.root\\\\\\"\\\\\) \>\> tuple.log 2>\&1 " >> $outfile`;
	`echo "rm -rf ./tmp/${sample}/*" >> $outfile`;
	`echo "mv tuple.root ./tmp/${sample}/" >> $outfile`;
	`echo "root -b \\\$LS_SUBCWD/LArStudies/run/RootLogon.C \\\$LS_SUBCWD/LArStudies/run/makeTupleCoherent.C\\\\\(\\\\\\"./tmp/${sample}\\\\\\",\\\\\\".\\\\\\"\\\\\) \>\> tuple.log 2>\&1 " >> $outfile`;
    }
    
    `echo "${cpout} tuple.root ${OUTPUTDIR}/${sample}/tuple_${idx}.root " >> $outfile`;
    `echo "mv tuple.log ${SUBMITDIR}/${sample}/tuple_${idx}.athena.log" >> $outfile`;
}

####################################################
######Make the execution scripts
####################################################
if($option eq "create"){
    #Number of files to merge
    $nmerge=shift;
    print "Number of files to merge per job: $nmerge \n";
    $algo=shift;
    print "Algo: $algo \n";
    if($algo eq ""){
	print "Ivalid algorithm\n";
	return;
    }
 
    #Determine how to read the input
    if($instorage eq "eos"){
	$cpin="$eos cp";
    }else {
	$cpin="/bin/cp";
    }

    #determine how to write the output
    if($outstorage eq "eos"){
	$cpout="$eos cp";
    }else{
	$cpout="/bin/cp";
    }
    
    $totalcounter=0;
    foreach $samp (@samples){
	`mkdir $SUBMITDIR/$samp`;

	#create the output directory
	if($outstorage eq "eos"){
	    $command="$eos mkdir $OUTPUTDIR/$samp";
	    system($command);
	}else{
	    if($SUBMITDIR ne $OUTPUTDIR){
		$command="mkdir $OUTPUTDIR/$samp";
		system($command);
	    }
	}

	#get list of files in storage
	if($instorage eq "eos"){
	    if($algo eq "RAW" || $algo eq "RAWCOH" ){
		#Files are in this paths:
		#/eos/atlas/atlastier0/rucio/data15_comm/physics_MinBias/00263659/data15_comm.00263659.physics_MinBias.merge.RAW
		@sampletoken=split(/\./,$samp);
		$PathToInput="$INPUTDIR/${sampletoken[0]}/${sampletoken[2]}/${sampletoken[1]}/$samp";
		@dirlist=`$eos ls $PathToInput | grep SFO`;
	    }else {
		$PathToInput="$INPUTDIR/$samp";
		@dirlist=`$eos ls $INPUTDIR/$samp/ | grep tuple | grep root`;
	    }
	}else {
	    $PathToInput="$INPUTDIR/$samp";
	    @dirlist=`/bin/ls $INPUTDIR/$samp | grep SFO`;
	}

	##loop over the input files and merge
	$filelist="";
	$mergecounter=0;
	$idx=0;	
	for $f (@dirlist){
	    chomp($f);
	    $filelist = "${filelist} $PathToInput/${f}";
	    $mergecounter++;

	    if( $mergecounter == $nmerge ){
 		makeBatchExecutable($OUTPUTDIR,$SUBMITDIR,$samp,$idx,$cpin,$cpout,$filelist,$algo);
		$filelist="";
		$mergecounter=0;
		$idx++;
		$totalcounter++;
	    }
	}
	if($mergecounter>0){
	        makeBatchExecutable($OUTPUTDIR,$SUBMITDIR,$samp,$idx,$cpin,$cpout,$filelist,$algo);	
	        $idx++;
		$totalcounter++;
	}

	print "$idx : ${samp}\n";
    }
    print "$totalcounter total jobs created.\n";
}

####################################################
####function for batch submition
####################################################
sub submit {
    $path = $_[0];
    $idx = $_[1];
    $qu = $_[2];

    if($qu ne "1nh" && $qu ne "8nh" && $qu ne "1nd" && $qu ne "2nd"){ 
	$qu="8nh";
    }
    
    system("rm -f ${path}/tuple_${idx}.log");
    $command="bsub -C 0 -R \"pool>10000\" -q ${qu} -J ${idx} -o ${path}/tuple_${idx}.log < ${path}/tuple_${idx}.sh";	
    print "$command\n";
    system("$command");
}

####################################################
#submit all jobs
####################################################
if($option eq "sub" ){
    ##provide queue
    $qu=shift;

    foreach $samp (@samples){
	$idx=0;	
	for $f (`/bin/ls $SUBMITDIR/$samp | grep tuple_ | grep .sh`){
	    chomp($f);
	    submit("$SUBMITDIR/$samp",$idx,$qu);
	    $idx++;
	}
	print "\n Submitted $idx jobs for ${samp}\n";
    }
}

####################################################
###Check the log files and root files
####################################################
if($option eq "check"){
    $qu=shift;##if queue is provided then resubmit
    
    $sampleindex=0;
    foreach $samp (@samples){
	print "${SUBMITDIR}/${samp}:\n";
	$idx=0;	
	$failcounter=0;
	$cpulimitcounter=0;
	$nologcounter=0;
	$norootcounter=0;
	for $f (`/bin/ls $SUBMITDIR/$samp | grep tuple_ | grep .sh | grep -v "~" `){
	    chomp($f);
	    $logfile="${SUBMITDIR}/${samp}/tuple_${idx}.log";
	    $rootfile="${OUTPUTDIR}/${samp}/tuple_${idx}.root";

	    $failed=0;

	    #check a log file was produced
	    if(!(-e "${logfile}")){ 
		print "No log: ${logfile}\n"; 
		$failed = 1; 
		$nologcounter++;
	    }else {
		$success = `cat ${logfile} | grep "Successfully completed."`;
		if($success eq "" ){
		    print "Not successfully completed \n ${job}\n";
		    $failed = 1;
		    $cpulimitcounter += `cat ${logfile} | grep TERM_CPULIMIT` ne "" ; 
		}
	    }

	
	    #check the root file exists 
	    if($outstorage eq "eos"){
		if( `$eos ls $rootfile | grep .root` eq "" ){
		    $failed = 1;
		    $norootcounter++; 
		}
	    }else {
		if(!(-e "${rootfile}")){
		    print "No root file \n ${logfile}\n";
		    $failed = 1;
		    $norootcounter++; 
		}
	    }
	    
	    ###Resubmit
	    if($failed == 1){
		$failcounter++;	
		if($qu ne ""){
		    submit("${SUBMITDIR}/${samp}",$idx,$qu);
		    print "Job $idx resubmitted.\n";
		}
	    }
	    
	    $idx++;
	}

	$samplesummary[$sampleindex]="Failed $failcounter / $idx : TERM_CPULIMIT($cpulimitcounter) NOLOG($nologcounter) NOROOT($norootcounter)";
	$sampleindex++;
    }

    ######Print a summary of the counters:
    $sampleindex=0;
    foreach $samp (@samples){
	print "${samp}\n";
	print "$samplesummary[$sampleindex]\n";
	$sampleindex++;
    }

}

####################################################
####  merge the ntuple/histograms
####################################################
if($option eq "merge"){
    if($outstorage eq "eos"){
	die "Error. Files are in eos. cannot do hadd";
    }
    
    foreach $sample (@samples){
	`rm -f $OUTPUTDIR/$sample/merged.root`;
	
	####hadd seems to fail on large number of histograms
	#$command="hadd  $OUTPUTDIR/$sample/merged.root  $OUTPUTDIR/$sample/tuple_*.root";
	#print "$command\n";
	#system($command);

	
	###create the execution script
	$outfile="${OUTPUTDIR}/${sample}/merged.sh";
	print "$outfile\n";
	`rm -f $outfile`;
	`touch $outfile`;
	`echo "cd \\\$LS_SUBCWD " >> $outfile`;
	`echo "source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.sh " >> $outfile`;
	`echo "source \\\$AtlasSetup/scripts/asetup.sh " >> $outfile`;
	`echo "printenv " >> $outfile`;
	`echo "mergeHist.exe ${OUTPUTDIR}/${sample}" >> $outfile`;
	
	####submit to batch
	`bsub -C 0 -q 1nd -o ${OUTPUTDIR}/${sample}/merged.log < ${outfile}`;

    }    

}

####################################################
####remove root files to save space, but only after merging them
####################################################
if($option eq "delete"){
    if($outstorage eq "eos"){
	die "Error. Files are in eos. cannot delete";
    }
    $enable=shift;

    foreach $sample (@samples){
	$command="rm -f $OUTPUTDIR/$sample/tuple_*.root";
	if($enable==1){
	    print "$command\n";
	    system($command);
	}
    }    

}
