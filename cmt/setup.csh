# echo "setup LArStudies LArStudies-03-02-21 in /afs/cern.ch/user/b/benitezj/scratch0/LArStudies/20.1.4.2_LArCAF/LArCalorimeter"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/atlas.cern.ch/repo/sw/software/x86_64-slc6-gcc48-opt/20.1.4/CMT/v1r25p20140131
endif
source ${CMTROOT}/mgr/setup.csh
set cmtLArStudiestempfile=`${CMTROOT}/${CMTBIN}/cmt.exe -quiet build temporary_name`
if $status != 0 then
  set cmtLArStudiestempfile=/tmp/cmt.$$
endif
${CMTROOT}/${CMTBIN}/cmt.exe setup -csh -pack=LArStudies -version=LArStudies-03-02-21 -path=/afs/cern.ch/user/b/benitezj/scratch0/LArStudies/20.1.4.2_LArCAF/LArCalorimeter  -no_cleanup $* >${cmtLArStudiestempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/${CMTBIN}/cmt.exe setup -csh -pack=LArStudies -version=LArStudies-03-02-21 -path=/afs/cern.ch/user/b/benitezj/scratch0/LArStudies/20.1.4.2_LArCAF/LArCalorimeter  -no_cleanup $* >${cmtLArStudiestempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtLArStudiestempfile}
  unset cmtLArStudiestempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtLArStudiestempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtLArStudiestempfile}
unset cmtLArStudiestempfile
exit $cmtsetupstatus

