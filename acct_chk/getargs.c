/*----------------------------------------------------------------------------
	Program : getargs.c
	Author  : Tom Stevelt
	Date    : 2000-2024
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

#include	"acct_chk.h"

static void Usage ( char *Message )
{
	if ( Message != NULL )
	{
		printf ( "%s\n", Message );
	}

	printf ( "USAGE: acct_chk -d database [-h host] [-u user] [-p password]\n" );
	exit ( 1 );

}

void getargs ( int argc, char *argv[] )
{
	int		xa;

	ParmCount = 0;
	DbHostName   = NULL;
	UserName     = NULL;
	Password     = NULL;
	DataBaseName = NULL;

	for ( xa = 1; xa < argc; xa++ )
	{
		if ( xa + 1 < argc && nsStrcmp ( argv[xa], "-d" ) == 0 )
		{
			xa++;
			DataBaseName = argv[xa];
			ParmCount++;
		}
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
		else
		{
			Usage ( "UNKNOWN ARGUMENT" );
		}
	}
	if ( DataBaseName == NULL )
	{
		Usage ( "DATA BASE NAME IS REQUIRED" );
	}
}
