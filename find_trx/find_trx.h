
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

TYPE	long	AccountNumber;
TYPE	char	*DataBaseName;
TYPE	int		Debug;

/*----------------------------------------------------------
:.,$d
:r! mkdoc -s *.c
----------------------------------------------------------*/
int main ( int argc, char *argv[] );
void getargs ( int argc, char *argv[] );
