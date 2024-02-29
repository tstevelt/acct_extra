#!/bin/sh

if [ "$2" = '' ]
then
	echo "USAGE: DeleteTranFromFile.sh  db  file"
	exit 1
fi

DB=$1
FILE=$2

rm -f script

for TRXNUM in `cat $FILE`
do
	echo "delete from trxh where trxnum = $TRXNUM;" >> script
	echo "delete from trxd where trxnum = $TRXNUM;" >> script

done

mysql -D $DB < script

rm script

acct_chk -d $DB





