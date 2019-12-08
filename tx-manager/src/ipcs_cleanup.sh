#!  /bin/bash

echo "Removing Shared Memory, Semaphores and Messages... "

ME=`whoami`
echo "User :" $ME

IPCS_S=`ipcs -s | egrep "0x[0-9a-f]+ [0-9]+" | grep $ME | cut -f2 -d" "`
IPCS_M=`ipcs -m | egrep "0x[0-9a-f]+ [0-9]+" | grep $ME | cut -f2 -d" "`
IPCS_Q=`ipcs -q | egrep "0x[0-9a-f]+ [0-9]+" | grep $ME | cut -f2 -d" "`

if [ $IPCS_S ]
then
  echo "Removing SEMAPHORE :" $IPCS_S
else  
  echo "No SEMAPHORES to remove."
fi

if [ $IPCS_M ]
then
  echo "Removing SHARED MEMORY :" $IPCS_M
else  
  echo "No SHARED MEMORY to remove."
fi

if [ $IPCS_Q ]
then
  echo "Removing MESSAGE :" $IPCS_Q
else  
  echo "No MESSAGE(s) to remove."
fi

for id in $IPCS_S; 
do
  ipcrm -s $id;
done

for id in $IPCS_M;
do
  ipcrm -m $id;
done

for id in $IPCS_Q;
do
  ipcrm -q $id;
done

echo "...Done... "
