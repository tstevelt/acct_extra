#!/bin/sh

for DATABASE in acct demo shs
do
	DBFILE=$DATABASE.budb
	mysqldump -h 127.0.0.1 $DATABASE > $DBFILE
done

ls -lt | head



