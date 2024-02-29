#!/bin/sh

if [ "$1" = '' ]
then
	echo "USAGE: InvoiceData.sh invoice"
	exit 1
fi

echo "select * from arinvh where id = $1;" | mysql -D shs

echo "select * from arinvl where invoice = $1;" | mysql -D shs

