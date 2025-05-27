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



