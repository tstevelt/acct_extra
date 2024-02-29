#!/bin/sh

if [ "$1" = '' ]
then
	echo "USAGE: BackupAll.sh go" 
	exit 1 
fi

for DATABASE in acct demo shs
do
	DBFILE=$DATABASE.budb
	mysqldump -h 127.0.0.1 $DATABASE > $DBFILE
done

ls -lt | head



