/*----------------------------------------------------------------------------
	Program : acct_chk.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Check and update account balances.
	Return  : 

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		11/20/2020	Report out-of-balance transactions.
	tms		12/12/2020	Fix import.previous

----------------------------------------------------------------------------*/

#define		MAIN
#include	"acct_chk.h"

typedef struct
{
	char	Type;
	char	*FieldName;
} RECORD;

static	RECORD	Array [] =
{
	{ 'A', "assetnext" },
	{ 'L', "liabnext" },
	{ 'E', "equitynext" },
	{ 'I', "incomenext" },
	{ 'X', "expensenext" },
};

int	UseErrorSys = 1;

int main ( int argc, char *argv[] )
{
	DBY_QUERY	*QueryOne;
	DBY_QUERY	*QueryTwo;
	int			xt;
	long		ThisAffected;
	long		MaxTrx, SumAmount;
	long		Balance;
	int			Verbose = 0;
	int			Checked = 0;
	int			Errors = 0;
	int			Fixed = 0;
	long		CurrentTrx;

	getargs ( argc, argv );

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

	/*--------------------------------------------------------------
		load current system record
	--------------------------------------------------------------*/
	LoadSystem ();

	/*--------------------------------------------------------------
		set next account numbers in system
	--------------------------------------------------------------*/
	for ( xt = 0; xt < 5; xt++ )
	{
		sprintf ( StatementOne, 
			"select %s from account where accttype = '%c'", 
				FIELDS_ACCOUNT, Array[xt].Type );

		QueryOne = dbySelect ( "acct_chk", &MySql, StatementOne, LOGFILENAME );

		while (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) != NULL )
		{
			xaccount.xacctnum = safe_atol ( QueryOne->EachRow[ACCOUNT_acctnum] );
			xaccount.xopenbal = safe_atol ( QueryOne->EachRow[ACCOUNT_openbal] );
			xaccount.xcurrbal = safe_atol ( QueryOne->EachRow[ACCOUNT_currbal] );
			
			Checked++;

			sprintf ( StatementTwo, 
				"select sum(amount) from trxd where acctnum = %ld",
						xaccount.xacctnum );

			QueryTwo = dbySelect ( "acct_chk", &MySql, StatementTwo, LOGFILENAME );

			if (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) != NULL )
			{
				if ( QueryTwo->EachRow[0] != NULL )
				{
					SumAmount = safe_atol ( QueryTwo->EachRow[0] );
				}
				else
				{
					SumAmount = 0L;
				}

				if ( SumAmount + xaccount.xopenbal == xaccount.xcurrbal )
				{
					if ( Verbose )
					{
						printf ( "account %ld current balance %.2f okay\n",
							xaccount.xacctnum, (double) xaccount.xcurrbal / 100.0 );
					}
				}
				else
				{
					sprintf ( StatementTwo, 
						"update account set currbal = openbal + %ld where acctnum = %ld",
								SumAmount,  xaccount.xacctnum );

					ThisAffected = dbyUpdate ( "acct_chk", &MySql, StatementTwo, 0, LOGFILENAME );

					if ( ThisAffected == 0 )
					{
						printf ( "account %ld not updated, should be %.2f\n",
							xaccount.xacctnum, (double) (xaccount.xopenbal + SumAmount) / 100.0 );

						Errors++;
					}
					else
					{
						printf ( "account %ld updated\n", xaccount.xacctnum );
						Fixed++;
					}
				}
			}

			/*----------------------------------------------------------
				find max trxdate and fix import.previous if needed.
			----------------------------------------------------------*/
			if ( LoadOneImport ( xaccount.xacctnum ) ==  1 )
			{
				sprintf ( StatementTwo, 
					"select max(trxdate) from trxh, trxd where trxh.trxnum = trxd.trxnum and seqnum = 1 and acctnum = %ld",
							ximport.xacctnum );

				QueryTwo = dbySelect ( "acct_chk", &MySql, StatementTwo, LOGFILENAME );

				if (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) != NULL )
				{
					if ( QueryTwo->EachRow[0] != NULL )
					{
						StringToDateval ( QueryTwo->EachRow[0], &xtrxh.xtrxdate );
					}

					if ( CompareDateval ( &xtrxh.xtrxdate, &ximport.xprevious ) == 0 )
					{
						if ( Verbose )
						{
							printf ( "import.previous date is okay\n" );
						}
					}
					else
					{
						sprintf ( StatementTwo, 
							"update import set previous = '%04d-%02d-%02d' where acctnum = %ld",
									xtrxh.xtrxdate.year4, xtrxh.xtrxdate.month, xtrxh.xtrxdate.day,  ximport.xacctnum );

						ThisAffected = dbyUpdate ( "acct_chk", &MySql, StatementTwo, 0, LOGFILENAME );

						if ( ThisAffected == 0 )
						{
							printf ( "import %ld not updated, should be %04d-%02d-%02d\n",
								ximport.xacctnum, xtrxh.xtrxdate.year4, xtrxh.xtrxdate.month, xtrxh.xtrxdate.day );

							Errors++;
						}
						else
						{
							printf ( "import %ld updated\n", ximport.xacctnum );
							Fixed++;
						}
					}
				}
			}
		}
	}

	/*--------------------------------------------------------------
		set next transaction number in system
	--------------------------------------------------------------*/
	sprintf ( StatementOne, "select max(trxnum) from trxh" );
	QueryOne = dbySelect ( "acct_chk", &MySql, StatementOne, LOGFILENAME );

	Checked++;

	if (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) == NULL )
	{
		MaxTrx = 0L;
	}
	else
	{
		MaxTrx = safe_atol ( QueryOne->EachRow[0] );
	}
	MaxTrx ++;

	if ( MaxTrx == xsystem.xtrxnext )
	{
		if ( Verbose )
		{
			printf ( "system.trxnext %ld okay\n", MaxTrx );
		}
	}
	else
	{
		sprintf ( StatementOne, "update system set trxnext = %ld", MaxTrx );

		ThisAffected = dbyUpdate ( "acct_chk", &MySql, StatementOne, 0, LOGFILENAME );
		if ( ThisAffected == 0 )
		{
			printf ( "system.trxnext not updated, should be %ld\n", MaxTrx );
			Errors++;
		}
		else
		{
			printf ( "system.trxnext updated, set to %ld\n", MaxTrx );
			Fixed++;
		}
	}

	/*--------------------------------------------------------------
		find out-of-balance transactions
	--------------------------------------------------------------*/
	CurrentTrx = -1L;
	sprintf ( StatementOne, "select %s from trxd order by trxnum, seqnum", FIELDS_TRXD );
	QueryOne = dbySelect ( "acct_chk", &MySql, StatementOne, LOGFILENAME );
	while (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) != NULL )
	{
		xtrxd.xtrxnum = safe_atol (  QueryOne->EachRow[TRXD_trxnum] );
		xtrxd.xseqnum = safe_atoi (  QueryOne->EachRow[TRXD_seqnum] );

		if ( CurrentTrx != xtrxd.xtrxnum )
		{
			CurrentTrx = xtrxd.xtrxnum;
			Checked++;

//			StringToDateval ( QueryOne->EachRow[TRX_trxdate], &xtrx.xtrxdate );
			snprintf ( xtrxd.xpayee, sizeof(xtrxd.xpayee), "%s", QueryOne->EachRow[TRXD_payee] );
			xtrxd.xacctnum =  safe_atol (  QueryOne->EachRow[TRXD_acctnum] );

			if ( xtrxd.xseqnum != 1 )
			{
				LoadOneAccount ( xtrxd.xacctnum );

				printf ( "MISSING PRIMARY TRX trx %7ld %-30.30s, account %ld %s\n",
						xtrxd.xtrxnum,
						xtrxd.xpayee,
						xtrxd.xacctnum,
						xaccount.xacctname );

				Errors++;

			}
			else
			{
				Balance = CalcTrxBalance ( xtrxd.xtrxnum );

				if ( Balance != 0 )
				{
					LoadOneAccount ( xtrxd.xacctnum );

					printf ( "BALANCE %7.2f trx %7ld %-30.30s, account %ld %s\n",
						(double) Balance / 100.0,
						xtrxd.xtrxnum,
						xtrxd.xpayee,
						xtrxd.xacctnum,
						xaccount.xacctname );

					Errors++;

					/*----------------------------------------------------------
						tms 12/03/2020 remove test on > STATUS_BALANCE
					----------------------------------------------------------*/
					sprintf ( StatementTwo, 
						"update trxh set status = %d where trxnum = %ld",
									STATUS_BALANCE, xtrxd.xtrxnum );

					dbyUpdate ( "acct_chk", &MySql, StatementTwo, 0, LOGFILENAME );
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
