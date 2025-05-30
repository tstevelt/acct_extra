/*----------------------------------------------------------------------------
	Program : DumpSession.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Print fields in session record to stdout

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

