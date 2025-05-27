/*---------------------------------------------------------------------------
    Accounting System Extras

    Copyright (C)  2000 - 2024 Tom Stevelt

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
---------------------------------------------------------------------------*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<ctype.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netinet/tcp.h>
#include	<arpa/inet.h>
#include	<netdb.h>
#include	<signal.h>
#include	<time.h>
#include	<errno.h>

#ifdef	APPLE
#include   <mysql.h>
#include   <errmsg.h>
#else
#include   <mysql/mysql.h>
#include   <mysql/errmsg.h>
#endif

#include	"shslib.h"
#include	"dbylib.h"
#include	"acctlib.h"

#define		SYSTEM
#define		ACCOUNT
#ifdef STUFF
#define		IMPORT
#endif
#define		TRXH
#define		TRXD
#include	"fileacct.h"

TYPE	int		ParmCount;
TYPE	char	*DbHostName;
TYPE	char	*ConfigFile;
TYPE	char	*UserName;
TYPE	char	*Password;
TYPE	char	*DataBaseName;
TYPE	char	*LimitYear;
TYPE	long	StartTrxnum;
TYPE	long	EndTrxnum;
TYPE	int		Debug;

/*----------------------------------------------------------
:.,$d
:r! mkdoc -s *.c
----------------------------------------------------------*/
int main ( int argc, char *argv[] );
void getargs ( int argc, char *argv[] );
