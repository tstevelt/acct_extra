PROG = acct_chk
LIB = acct_chk.a
DEF = /usr/local/include
PRG = /usr/local/bin/$(PROG)

#MYSQL_DIR = /usr/lib64
#MYSQL_FLAGS = -lz -lcrypt -lm -lssl -lcrypto

#XLIB = /usr/local/lib64/acctlib.a /usr/local/lib64/dbylib.a  /usr/local/lib64/weblib.a /usr/local/lib64/shslib.a $(MYSQL_DIR)/libmysqlclient.so $(MYSQL_FLAGS)

XLIB = /usr/local/lib64/acctlib.a /usr/local/lib64/weblib.a /usr/local/lib64/dbylib.a /usr/local/lib64/shslib.a  \
		/usr/local/lib/libmysqlclient.a \
		/usr/local/lib/libcrypto.a \
		/usr/local/lib/libssl.a \
		/usr/local/Cellar/zlib/1.3.1/lib/libz.a


#		/usr/local/lib/libxlsxwriter.a \
#		/usr/local/lib64/rptlib.a \
# libta_lib.a


FILES = \
	$(LIB)(acct_chk.o)	\
	$(LIB)(getargs.o)

.SILENT:

$(PRG): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG)"
	gcc -o $(PRG) $(LIB) $(XLIB)

$(LIB): $(FILES)

$(FILES): acct_chk.h $(DEF)/acctlib.h $(DEF)/fileacct.h $(DEF)/dbylib.h $(DEF)/shslib.h

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)

