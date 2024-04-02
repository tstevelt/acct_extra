/*----------------------------------------------------------------------------
	Program : cross_post_chk.c
	Author  : Tom Stevelt
	Date    : 04/02/2024
	Synopsis: Check investments accounts for posts outside their account group
	Return  : 

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/

#define		MAIN
#include	"cross_post_chk.h"

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
typedef struct
{
	char	GroupName[30];
	long	AssetAcct;
	long	SecurityAcct;
	long	IncomeAcct;
} RECORD;

#define		MAXGROUPS		20

static	RECORD	Array [MAXGROUPS];
static	int		Count;

int	UseErrorSys = 1;

int main ( int argc, char *argv[] )
{
	DBY_QUERY	*QueryOne;
	DBY_QUERY	*QueryTwo;
	char		Fragment[256];
	FILE		*fp;
	char		buffer[1024];
	char		*tokens[5];
	int			tokcnt;
	int			Checked = 0;
	int			Errors = 0;
	int			Fixed = 0;

	getargs ( argc, argv );

	if (( fp = fopen ( ConfigFile, "r" )) == NULL )
	{
		printf ( "Cannot open %s for reading\n", ConfigFile );
		exit ( 1 );
	}

	while ( fgets ( buffer, sizeof(buffer), fp ) != NULL )
	{
		if ( buffer[0] == '#' )
		{
			continue;
		}

		if (( tokcnt = GetTokensD ( buffer, ",\n\r", tokens, 5 )) < 4 )
		{
			continue;
		}
		
		if ( Count >= MAXGROUPS )
		{
			printf ( "Exceeds MAXGROUPS\n" );
			exit ( 1 );
		}

		snprintf ( Array[Count].GroupName, 30, "%s", tokens[0] );
		Array[Count].AssetAcct    = atol(tokens[1]);
		Array[Count].SecurityAcct = atol(tokens[2]);
		Array[Count].IncomeAcct   = atol(tokens[3]);
		Count++;
	}

	fclose ( fp );

	if ( Count == 0 )
	{
		printf ( "No groups loaded from %s, nothing to do\n", ConfigFile );
		exit ( 1 );
	}

	/*--------------------------------------------------------------
		init database
	--------------------------------------------------------------*/
	if ( ParmCount == 4 )
	{
		dbyInit ( &MySql, DbHostName, UserName, Password, DataBaseName, 3306, LOGFILENAME );
	}
	else
	{
		StartDb ( DataBaseName );
	}

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
		if ( LoadOneAccount ( Array[xg].AssetAcct ) != 1 )
		{
			printf ( "  Invalid account number %ld\n",  Array[xg].AssetAcct );
			Errors++;
		}
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
		if ( Errors  )
		{
			printf ( "  SKIPPING THIS GROUP %s\n", Array[xg].GroupName );
			continue;
		}
		
		sprintf ( StatementOne, 
			"select distinct trxd.trxnum from trxd, trxh where trxd.trxnum = trxh.trxnum and trxd.acctnum in ( %ld, %ld, %ld )",
				Array[xg].AssetAcct, Array[xg].SecurityAcct, Array[xg].IncomeAcct );

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
		QueryOne = dbySelect ( "cross_post_chk", &MySql, StatementOne, LOGFILENAME );
		while (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) != NULL )
		{
			xtrxh.xtrxnum = safe_atol (  QueryOne->EachRow[0] );
			if ( Debug )
			{
				printf ( "  %ld\n",  xtrxh.xtrxnum );
			}

			sprintf ( StatementTwo, "select acctnum, seqnum, amount from trxd where trxd.trxnum = %ld", xtrxh.xtrxnum );
			if ( Debug )
			{
				printf ( "%s\n", StatementTwo );
			}
			QueryTwo = dbySelect ( "cross_post_chk", &MySql, StatementTwo, LOGFILENAME );
			while (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) != NULL )
			{
				xtrxd.xacctnum = safe_atol (  QueryTwo->EachRow[0] );
				xtrxd.xseqnum  = safe_atol (  QueryTwo->EachRow[1] );
				xtrxd.xamount  = safe_atol (  QueryTwo->EachRow[2] );
				if ( Debug )
				{
					printf ( "    %ld %d\n",  xtrxd.xacctnum, xtrxd.xseqnum );
				}

				if (( xtrxd.xacctnum !=  Array[xg].AssetAcct    ) &&
					( xtrxd.xacctnum !=  Array[xg].SecurityAcct ) &&
					( xtrxd.xacctnum !=  Array[xg].IncomeAcct   ))
				{
					LoadOneTrxh ( xtrxh.xtrxnum );
					LoadOneAccount ( xtrxd.xacctnum );
					printf ( "  TRX %5ld %-8.8s %2d/%02d/%04d %9.2f, CROSS ACCT %5ld %s\n",
						xtrxh.xtrxnum,
						xtrxh.xrefnum,
						xtrxh.xtrxdate.month,
						xtrxh.xtrxdate.day,
						xtrxh.xtrxdate.year4,
						(double) xtrxd.xamount / 100.0,
						xtrxd.xacctnum,
						xaccount.xacctname );
				}
			}
		}
	}

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
