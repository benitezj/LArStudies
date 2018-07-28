# echo "setup LArStudies LArStudies-03-02-21 in /afs/cern.ch/user/b/benitezj/scratch0/LArStudies/20.1.4.2_LArCAF/LArCalorimeter"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/atlas.cern.ch/repo/sw/software/x86_64-slc6-gcc48-opt/20.1.4/CMT/v1r25p20140131; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtLArStudiestempfile=`${CMTROOT}/${CMTBIN}/cmt.exe -quiet build temporary_name`
if test ! $? = 0 ; then cmtLArStudiestempfile=/tmp/cmt.$$; fi
${CMTROOT}/${CMTBIN}/cmt.exe setup -sh -pack=LArStudies -version=LArStudies-03-02-21 -path=/afs/cern.ch/user/b/benitezj/scratch0/LArStudies/20.1.4.2_LArCAF/LArCalorimeter  -no_cleanup $* >${cmtLArStudiestempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/${CMTBIN}/cmt.exe setup -sh -pack=LArStudies -version=LArStudies-03-02-21 -path=/afs/cern.ch/user/b/benitezj/scratch0/LArStudies/20.1.4.2_LArCAF/LArCalorimeter  -no_cleanup $* >${cmtLArStudiestempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtLArStudiestempfile}
  unset cmtLArStudiestempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtLArStudiestempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtLArStudiestempfile}
unset cmtLArStudiestempfile
return $cmtsetupstatus

