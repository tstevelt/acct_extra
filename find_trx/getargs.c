/*----------------------------------------------------------------------------
	Program : getargs.c
	Author  : Tom Stevelt
	Date    : 04/02/2024
	Synopsis: Get command line args

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/

#include	"find_trx.h"

static void Usage ( char *Message )
{
	if ( Message != NULL )
	{
		printf ( "%s\n", Message );
	}

	printf ( "USAGE: find_trx -d database -a acctnum [-v]\n" );
	exit ( 1 );

}

void getargs ( int argc, char *argv[] )
{
	int		xa;

	
	DataBaseName = NULL;
	AccountNumber = 0;

	for ( xa = 1; xa < argc; xa++ )
	{
		if ( xa + 1 < argc && nsStrcmp ( argv[xa], "-d" ) == 0 )
		{
			xa++;
			DataBaseName = argv[xa];
		}
		else if ( xa + 1 < argc && nsStrcmp ( argv[xa], "-a" ) == 0 )
		{
			xa++;
			AccountNumber = atol(argv[xa]);
		}
		else if ( nsStrcmp ( argv[xa], "-v" ) == 0 )
		{
			Debug = 1;
		}
		else
		{
			Usage ( "UNKNOWN ARGUMENT" );
		}
	}
	if ( DataBaseName == NULL )
	{
		Usage ( "DATA BASE NAME IS REQUIRED" );
	}
	if ( AccountNumber == 0 )
	{
		Usage ( "ACCOUNT NUMBERIS REQUIRED" );
	}
}
