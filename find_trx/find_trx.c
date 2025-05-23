/*----------------------------------------------------------------------------
	Program : find_trx.c
	Author  : Tom Stevelt
	Date    : 04/02/2024
	Synopsis: Check investments accounts for posts outside their account group
	Return  : 

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/

#define		MAIN
#include	"find_trx.h"

/*---------------------------------------------------------------------------
+-----------+-------------+------+-----+------------+-------+
| Field     | Type        | Null | Key | Default    | Extra |
+-----------+-------------+------+-----+------------+-------+
| acctnum   | int(11)     | NO   | PRI | 0          |       |
| accttype  | char(1)     | NO   |     |            |       |
| acctname  | char(30)    | NO   |     |            |       |
| extranum  | char(20)    | YES  |     | NULL       |       |
| extraline | char(20)    | YES  |     | NULL       |       |
| openbal   | int(11)     | NO   |     | 0          |       |
| currbal   | int(11)     | NO   |     | 0          |       |
| recondate | date        | YES  |     | 0000-00-00 |       |
| reconbal  | int(11)     | NO   |     | 0          |       |
| daterange | smallint(6) | NO   |     | 3          |       |
| inserted  | int(11)     | YES  |     | NULL       |       |
| updated   | int(11)     | YES  |     | NULL       |       |
+-----------+-------------+------+-----+------------+-------+
+---------+-------------+------+-----+------------+-------+
| Field   | Type        | Null | Key | Default    | Extra |
+---------+-------------+------+-----+------------+-------+
| trxnum  | int(11)     | NO   | PRI | 0          |       |
| status  | smallint(6) | NO   |     | 1          |       |
| refnum  | char(8)     | YES  |     | NULL       |       |
| trxdate | date        | NO   |     | 0000-00-00 |       |
+---------+-------------+------+-----+------------+-------+
+---------+-------------+------+-----+---------+-------+
| Field   | Type        | Null | Key | Default | Extra |
+---------+-------------+------+-----+---------+-------+
| trxnum  | int(11)     | NO   | PRI | 0       |       |
| seqnum  | smallint(6) | NO   | PRI | 1       |       |
| acctnum | int(11)     | NO   | MUL | 0       |       |
| payee   | char(40)    | YES  |     | NULL    |       |
| amount  | int(11)     | NO   |     | 0       |       |
+---------+-------------+------+-----+---------+-------+
---------------------------------------------------------------------------*/

int	UseErrorSys = 1;

int main ( int argc, char *argv[] )
{
	DBY_QUERY	*QueryOne;
	DBY_QUERY	*QueryTwo;
#ifdef STUFF
	char		Fragment[256];
	FILE		*fp;
	char		buffer[1024];
#define		MAXTOKS		7
	char		*tokens[MAXTOKS];
	int			tokcnt;
#endif
	int			Checked = 0;
	int			Errors = 0;
	int			Fixed = 0;

	getargs ( argc, argv );

	/*--------------------------------------------------------------
		init database
	--------------------------------------------------------------*/
#ifdef STUFF
	if ( ParmCount == 4 )
	{
		dbyInit ( &MySql, DbHostName, UserName, Password, DataBaseName, 3306, LOGFILENAME );
	}
	else
#endif
	{
		StartDb ( DataBaseName );
	}

	if ( LoadOneAccount ( AccountNumber ) != 1 )
	{
		printf ( "  Invalid account number %ld\n", AccountNumber );
		Errors++;
	}

	sprintf ( StatementOne, "select distinct trxnum from trxd where acctnum = %ld order by trxnum", AccountNumber );

	QueryOne = dbySelect ( "find_trx", &MySql, StatementOne, LOGFILENAME );
	while (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) != NULL )
	{
		xtrxh.xtrxnum = safe_atol (  QueryOne->EachRow[0] );
		if ( Debug )
		{
			printf ( "  %ld\n",  xtrxh.xtrxnum );
		}
		Checked++;

		sprintf ( StatementTwo, "select trxh.trxnum, refnum, trxdate, seqnum, trxd.acctnum, payee, amount, acctname from trxh, trxd, account \
 where trxh.trxnum = trxd.trxnum and trxd.acctnum = account.acctnum and trxh.trxnum = %ld order by seqnum", xtrxh.xtrxnum );

		QueryTwo = dbySelect ( "find_trx", &MySql, StatementTwo, LOGFILENAME );
		while (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) != NULL )
		{
			Fixed++;

			printf ( "%s %s %s %s %s %-12.12s %10.2f %s\n",
				QueryTwo->EachRow[0],
				QueryTwo->EachRow[1],
				QueryTwo->EachRow[2],
				QueryTwo->EachRow[3],
				QueryTwo->EachRow[4],
				QueryTwo->EachRow[5],
				atof(QueryTwo->EachRow[6]) / 100.0,
				QueryTwo->EachRow[7] );
		}

		printf ( "\n" );

	}


#ifdef STUFF
	/*----------------------------------------------------------
		for each group ...
	----------------------------------------------------------*/
	for ( int xg = 0; xg < Count; xg++ )
	{
		printf ( "%s\n", Array[xg].GroupName );

		/*----------------------------------------------------------
			first validate group account numbers
		----------------------------------------------------------*/
		Errors = 0;
		if ( LoadOneAccount ( Array[xg].SecurityAcct ) != 1 )
		{
			printf ( "  Invalid account number %ld\n",  Array[xg].SecurityAcct );
			Errors++;
		}
		if ( LoadOneAccount ( Array[xg].IncomeAcct ) != 1 )
		{
			printf ( "  Invalid account number %ld\n",  Array[xg].IncomeAcct );
			Errors++;
		}
		if ( Array[xg].AccountCount == 5 )
		{
			if ( LoadOneAccount ( Array[xg].DistAcct ) != 1 )
			{
				printf ( "  Invalid account number %ld\n",  Array[xg].DistAcct );
				Errors++;
			}
			if ( LoadOneAccount ( Array[xg].ContraAcct ) != 1 )
			{
				printf ( "  Invalid account number %ld\n",  Array[xg].ContraAcct );
				Errors++;
			}
		}
		if ( Errors  )
		{
			printf ( "  SKIPPING THIS GROUP %s\n", Array[xg].GroupName );
			continue;
		}
		
		if ( Array[xg].AccountCount == 3 )
		{
			sprintf ( StatementOne, 
				"select distinct trxd.trxnum from trxd, trxh where trxd.trxnum = trxh.trxnum and trxd.acctnum in ( %ld, %ld, %ld )",
					Array[xg].AssetAcct, Array[xg].SecurityAcct, Array[xg].IncomeAcct );
		}
		else
		{
			sprintf ( StatementOne, 
				"select distinct trxd.trxnum from trxd, trxh where trxd.trxnum = trxh.trxnum and trxd.acctnum in ( %ld, %ld, %ld, %ld, %ld )",
					Array[xg].AssetAcct, Array[xg].SecurityAcct, Array[xg].IncomeAcct, Array[xg].DistAcct, Array[xg].ContraAcct );
		}

		if ( LimitYear != NULL )
		{
			sprintf ( Fragment, " and trxh.trxdate like '%%%s%%' ", LimitYear );
			strcat ( StatementOne, Fragment );
		}

		if ( StartTrxnum > 0 )
		{
			sprintf ( Fragment, " and trxd.trxnum >= %ld and trxd.trxnum <= %ld", StartTrxnum, EndTrxnum );
			strcat ( StatementOne, Fragment );
		}

		strcat ( StatementOne, " order by trxh.trxdate" );

		if ( Debug )
		{
			printf ( "\n%s\n", StatementOne );
		}
		QueryOne = dbySelect ( "find_trx", &MySql, StatementOne, LOGFILENAME );
		while (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) != NULL )
		{
			xtrxh.xtrxnum = safe_atol (  QueryOne->EachRow[0] );
			if ( Debug )
			{
				printf ( "  %ld\n",  xtrxh.xtrxnum );
			}

			sprintf ( StatementTwo, "select acctnum, seqnum, amount, payee from trxd where trxd.trxnum = %ld", xtrxh.xtrxnum );
			if ( Debug )
			{
				printf ( "%s\n", StatementTwo );
			}
			QueryTwo = dbySelect ( "find_trx", &MySql, StatementTwo, LOGFILENAME );
			while (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) != NULL )
			{
				xtrxd.xacctnum = safe_atol (  QueryTwo->EachRow[0] );
				xtrxd.xseqnum  = safe_atol (  QueryTwo->EachRow[1] );
				xtrxd.xamount  = safe_atol (  QueryTwo->EachRow[2] );
				snprintf ( xtrxd.xpayee, sizeof(xtrxd.xpayee), "%s", QueryTwo->EachRow[3] );
				if ( Debug )
				{
					printf ( "    %ld %d\n",  xtrxd.xacctnum, xtrxd.xseqnum );
				}

				int		PrintIt = 0;

				if ( Array[xg].AccountCount == 3 )
				{
					if (( xtrxd.xacctnum !=  Array[xg].AssetAcct    ) &&
						( xtrxd.xacctnum !=  Array[xg].SecurityAcct ) &&
						( xtrxd.xacctnum !=  Array[xg].IncomeAcct   ))
					{
						PrintIt = 1;
					}
				}
				else
				{
					if (( xtrxd.xacctnum !=  Array[xg].AssetAcct    ) &&
						( xtrxd.xacctnum !=  Array[xg].SecurityAcct ) &&
						( xtrxd.xacctnum !=  Array[xg].IncomeAcct   ) &&
						( xtrxd.xacctnum !=  Array[xg].DistAcct     ) &&
						( xtrxd.xacctnum !=  Array[xg].ContraAcct   ))
					{
						PrintIt = 1;
					}
				}
				if ( PrintIt )
				{
					LoadOneTrxh ( xtrxh.xtrxnum );
					LoadOneAccount ( xtrxd.xacctnum );
					printf ( "  TRX %5ld %-8.8s %2d/%02d/%04d %9.2f %-20.20s, CROSS ACCT %5ld %s\n",
						xtrxh.xtrxnum,
						xtrxh.xrefnum,
						xtrxh.xtrxdate.month,
						xtrxh.xtrxdate.day,
						xtrxh.xtrxdate.year4,
						(double) xtrxd.xamount / 100.0,
						xtrxd.xpayee,
						xtrxd.xacctnum,
						xaccount.xacctname );
				}
			}
		}
	}
#endif

	dbyClose ( &MySql );

	/*--------------------------------------------------------------
		print results and quit.  if any errors, tell shell.
	--------------------------------------------------------------*/
	printf ( "checked %d, fixed %d, errors %d\n", Checked, Fixed, Errors );

	if ( Errors > 0 )
	{
		return ( 2 );
	}

	if ( Fixed > 0 )
	{
		return ( 1 );
	}

	return ( 0 );
}
