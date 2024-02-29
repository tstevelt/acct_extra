#!/bin/sh


if [ "$2" = '' ]
then
	echo "USAGE: ShowTran.sh db trxnum"
	exit 1
fi

echo "select * from trxh where trxnum = $2;" | mysql -D $1
echo "select trxd.*, account.acctname from trxd, account where trxnum = $2 and trxd.acctnum = account.acctnum;" | mysql -D $1

