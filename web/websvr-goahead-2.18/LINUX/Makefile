#
# Makefile for the GoAhead web server reference source base
#  for the Linux OS
#
# Copyright (c) GoAhead Software, Inc. 1995-2000
#
# $Id: Makefile,v 1.2 2002/11/01 16:20:19 mmitchell Exp $
#

CC=arm-linux-gnueabihf-gcc
AR=arm-linux-gnueabihf-ar

all:	compile

ARCH	= libwebs.a
#NAME	= a6g2c-webs
NAME	= $(webEXE)

# User Management switch
UMSW	= -DUSER_MANAGEMENT_SUPPORT

# Digest Access switch
DASW	= -DDIGEST_ACCESS_SUPPORT

#
#	SSL switches
#
# You must uncomment this line to enable SSL
# SSLPATCHFILE = ../websSSL.o
#
# Then uncomment these three lines if you are using the RSAREF library
# and set them to the correct path.
#
# SSLINC = /RSASSL/library/include
# SSLLIB = /RSASSL/library/lib/libsslc.a
# SSLSW	= -DWEBS_SSL_SUPPORT -I$(SSLINC)
#
# Or alternatively,
#
# Uncomment these three lines if you are using the OpenSSL library.
# Change the value of SSLINC to the path where your OpenSSL header
# files reside, and change the value of SSLLIB to the path to your
# OpenSSL libssl.a and libcrypto.a.
#
# SSLINC = /usr/include/openssl
# SSLLIB = /usr/lib/libssl.a /usr/lib/libcrypto.a
# SSLSW	= -DWEBS_SSL_SUPPORT -I$(SSLINC) -DOPENSSL


# If-Modified-Support switches (requires math library, libm.a)
# IFMODSW = -DWEBS_IF_MODIFIED_SUPPORT
# IFMODLIB = /usr/lib/libm.a

# Dependencies
DEPEND_FILES	= ../asp.o ../balloc.o ../base64.o ../cgi.o ../default.o  \
				  ../ejlex.o ../ejparse.o ../form.o \
				  ../h.o ../handler.o ../mime.o ../misc.o ../page.o  \
				  ../ringq.o ../rom.o \
				  ../sock.o ../sockGen.o $(SSLPATCHFILE) \
				  ../security.o ../sym.o ../uemf.o ../url.o ../value.o \
				  ../md5c.o ../um.o ../umui.o ../websda.o ../emfdb.o \
				  ../webrom.o ../webs.o ../websuemf.o main.o a6g2cimpl.o set.o status.o setip.o \
				 
CFLAGS	= -DWEBS -DUEMF -DOS="LINUX" -DLINUX $(UMSW) $(DASW) $(SSLSW) $(IFMODSW) -I$(KERNELDIR)/include
OTHERS	= -DB_STATS -DB_FILL -DDEBUG
DEBUG	= -g -Wall
IFLAGS 	= -I..
LDFLAGS	= $(SSLLIB) $(IFMODLIB)
#suyidir = /mnt/hgfs/ubuntuShare/lterouter/web/websvr-goahead-2.18/LINUX/suyi
#suyi_web = $(suyidir)/suyi_web.o
SUYI = libsuyi.a
SUYI_FILES = suyi/suyi_common.o suyi/RunningStatus.o suyi/settingLTE.o \
			 suyi/settingLAN.o suyi/Account.o suyi/index.o suyi/Version.o\
			 suyi/Diagnose.o suyi/left.o suyi/login.o suyi/settingWH.o\
			 suyi/SystemLog.o suyi/head.o suyi/module.o suyi/TR069.o
compile: $(NAME)
#
#	Build archive of objects
#
#$(suyi_web):

$(SUYI): $(SUYI_FILES)
	$(AR) $(ARFLAGS) $(SUYI) $?

$(ARCH): $(DEPEND_FILES)
	$(AR) $(ARFLAGS) $(ARCH) $?

#
#	Primary link
#
$(NAME): Makefile $(ARCH) $(SUYI) upldForm.o
	$(CC) -o $(NAME) $(DEBUG) \
		$(CFLAGS) $(IFLAGS) \
		main.o $(SUYI) upldForm.o $(ARCH) $(LDFLAGS)

clean:
	rm -f $(NAME) $(ARCH) $(DEPEND_FILES) $(SUYI) $(SUYI_FILES)

install:
	mkdir -m 755 -p /home/webroot/bin
	mkdir -m 755 -p /home/webroot/cgi-bin
	cp a6g2c-webs
	cp -r ../web/graphics/ /home/webroot/
	cp -r ../web/style/ /home/webroot/
	cp a6g2c.asp /home/webroot/default.asp
	cp resphtml.tm /home/webroot/

#suyi_files
suyi/settingLAN.o: suyi/settingLAN.c
suyi/settingLTE.o: suyi/settingLTE.c
suyi/RunningStatus.o: suyi/RunningStatus.c
suyi/suyi_common.o: suyi/suyi_common.c
suyi/Account.o: suyi/Account.c
suyi/index.o: suyi/index.c
suyi/Version.o: suyi/Version.c
suyi/Diagnose.o: suyi/Diagnose.c
suyi/left.o: suyi/left.c
suyi/login.o: suyi/login.c
suyi/settingWH.o: suyi/settingWH.c
suyi/SystemLog.o: suyi/SystemLog.c
suyi/head.o: suyi/head.c
suyi/module.o: suyi/module.c
suyi/TR069.o: suyi/TR069.c

#	Dependencies
../asp.o:  ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../balloc.o: ../balloc.c ../uemf.h

../base64.o:  ../base64.c ../webs.h ../wsIntrn.h  ../ej.h ../ejIntrn.h ../uemf.h

../cgi.o:  ../webs.h ../wsIntrn.h ../uemf.h

../default.o:  ../default.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../ejlex.o:  ../ejlex.c ../ej.h ../ejIntrn.h ../uemf.h

../ejparse.o:  ../ejparse.c ../ej.h ../ejIntrn.h ../uemf.h

../emfdb.o:  ../emfdb.h ../wsIntrn.h ../uemf.h

../form.o:  ../form.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../h.o:  ../h.c ../uemf.h

../handler.o:  ../handler.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../md5c.o:  ../md5.h ../wsIntrn.h ../uemf.h

../mime.o:  ../mime.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../misc.o:  ../misc.c ../uemf.h

../page.o:  ../page.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../ringq.o:  ../ringq.c ../uemf.h

../rom.o:  ../rom.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../security.o:  ../security.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../sock.o:  ../sock.c ../uemf.h

../sockGen.o:  ../sockGen.c ../uemf.h

../sym.o:  ../sym.c ../uemf.h

../uemf.o:  ../uemf.c ../uemf.h

../um.o:  ../webs.h ../wsIntrn.h ../um.h ../uemf.h

../umui.o:  ../webs.h ../wsIntrn.h ../um.h ../uemf.h

../url.o:  ../url.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../value.o:  ../value.c ../uemf.h

../webrom.o:  ../webrom.c ../webs.h ../wsIntrn.h ../uemf.h

../webs.o:  ../webs.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../websda.o:  ../webs.h ../wsIntrn.h ../websda.h ../uemf.h

../websuemf.o:  ../websuemf.c ../webs.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

../websSSL.o:  ../websSSL.c ../websSSL.h ../wsIntrn.h ../ej.h ../ejIntrn.h ../uemf.h

upldForm.o:  upldForm.c ../webs.h
#setip.o: setip.c
a6g2cimpl.o: a6g2cimpl.c a6g2cimpl.h
#routr-manager.o: router_status.h router_set.h status.c set.c setip.c
#./suyi/suyi_web.o: ./suyi/suyi_common.h  ./suyi/RunningStatus.c

main.o:  main.c ../wsIntrn.h ../webs.h ../ej.h ../ejIntrn.h ../uemf.h

#
#	Transition rules (add -o to put object in right directory)
#
.c.o:
	$(CC) -c -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) $<
##	$(STRIP) -s $(NAME) $(NAME)
