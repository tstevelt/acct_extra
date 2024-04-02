/*----------------------------------------------------------------------------
	Program : getargs.c
	Author  : Tom Stevelt
	Date    : 04/02/2024
	Synopsis: Get command line args

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/

#include	"cross_post_chk.h"

static void Usage ( char *Message )
{
	if ( Message != NULL )
	{
		printf ( "%s\n", Message );
	}

	printf ( "USAGE: cross_post_chk -d database -c configfile [-y YYYY] [-r trx trx] [-v]\n" );
	exit ( 1 );

}

void getargs ( int argc, char *argv[] )
{
	int		xa;

	ParmCount = 0;
	DbHostName   = NULL;
	ConfigFile   = NULL;
	UserName     = NULL;
	Password     = NULL;
	DataBaseName = NULL;
	StartTrxnum  = 0;
	EndTrxnum    = 0;
	LimitYear    = NULL;

	for ( xa = 1; xa < argc; xa++ )
	{
		if ( xa + 1 < argc && nsStrcmp ( argv[xa], "-d" ) == 0 )
		{
			xa++;
			DataBaseName = argv[xa];
			ParmCount++;
		}
		else if ( xa + 1 < argc && nsStrcmp ( argv[xa], "-c" ) == 0 )
		{
			xa++;
			ConfigFile = argv[xa];
		}
		else if ( xa + 1 < argc && nsStrcmp ( argv[xa], "-y" ) == 0 )
		{
			xa++;
			LimitYear = argv[xa];
		}
		else if ( xa + 2 < argc && nsStrcmp ( argv[xa], "-r" ) == 0 )
		{
			xa++;
			StartTrxnum  = atol(argv[xa]);
			xa++;
			EndTrxnum    = atol(argv[xa]);
		}
		else if ( nsStrcmp ( argv[xa], "-v" ) == 0 )
		{
			Debug = 1;
		}
#ifdef STUFF
		else if ( xa + 1 < argc && nsStrcmp ( argv[xa], "-h" ) == 0 )
		{
			xa++;
			DbHostName   = argv[xa];
			ParmCount++;
		}
		else if ( xa + 1 < argc && nsStrcmp ( argv[xa], "-u" ) == 0 )
		{
			xa++;
			UserName     = argv[xa];
			ParmCount++;
		}
		else if ( xa + 1 < argc && nsStrcmp ( argv[xa], "-p" ) == 0 )
		{
			xa++;
			Password     = argv[xa];
			ParmCount++;
		}
#endif
		else
		{
			Usage ( "UNKNOWN ARGUMENT" );
		}
	}
	if ( DataBaseName == NULL )
	{
		Usage ( "DATA BASE NAME IS REQUIRED" );
	}
	if ( ConfigFile == NULL )
	{
		Usage ( "CONFIG FILE NAME IS REQUIRED" );
	}
}
