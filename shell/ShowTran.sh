#!/bin/sh
#     Accounting System Extras
# 
#     Copyright (C)  2000 - 2024 Tom Stevelt
# 
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU Affero General Public License as
#     published by the Free Software Foundation, either version 3 of the
#     License, or (at your option) any later version.
# 
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU Affero General Public License for more details.
# 
#     You should have received a copy of the GNU Affero General Public License
#     along with this program.  If not, see <https://www.gnu.org/licenses/>.


if [ "$2" = '' ]
then
	echo "USAGE: ShowTran.sh db trxnum"
	exit 1
fi

echo "select * from trxh where trxnum = $2;" | mysql -D $1
echo "select trxd.*, account.acctname from trxd, account where trxnum = $2 and trxd.acctnum = account.acctnum;" | mysql -D $1

