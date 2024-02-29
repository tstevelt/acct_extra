/*----------------------------------------------------------------------------
	Program : DumpSession.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Print fields in session record to stdout

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/


#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>

#define		MAIN
#include	"shslib.h"
#include	"dbylib.h"
#include	"acctlib.h"

int main ( int argc, char *argv[] )
{
	if ( argc < 2 )
	{
		printf ( "USAGE: DumpSession SessionID\n" );
		exit ( 1 );
	}

	if ( ReadSession ( argv[1], &SessionRecord ) == 0 )
	{
		DumpSession ( &SessionRecord );
	}

	return ( 0 );
}

