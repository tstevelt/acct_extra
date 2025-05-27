/*----------------------------------------------------------------------------
	Program : getargs.c
	Author  : Tom Stevelt
	Date    : 04/02/2024
	Synopsis: Get command line args

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/
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
