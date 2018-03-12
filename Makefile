#make
#make install
#make init
#make web
#make clean

CC = arm-linux-gnueabihf-gcc

config = $(PWD)/config
init = $(PWD)/init
web = $(PWD)/web
webroot = $(web)/webroot
webserver = $(web)/websvr-goahead-2.18/LINUX
opt=~/opt

initEXE = lterouter
export initEXE
webEXE = router-web
export webEXE

readdr = root@192.168.1.136:/opt

all:
	$(MAKE) -C $(init)
	cd $(webserver) && $(MAKE)

config:
	scp -r $(config)/* $(readdr)/config

init:
	$(MAKE) -C $(init)
	scp -r $(init)/*.sh $(init)/$(initEXE) $(readdr)/init/
	#cd $(init) && $(MAKE)
	
web:
	cd $(webserver) && $(MAKE) 
	cp $(webserver)/$(webEXE) $(webroot)/bin/
	scp -r $(webroot)/* $(readdr)/web/

clean:
	cd $(webserver) && $(MAKE) clean 
	cd $(init) && $(MAKE) clean 

install:
	-mkdir $(opt) $(opt)/config $(opt)/init $(opt)/web
	cp $(config)/* $(opt)/config/
	cp $(init)/*.sh $(init)/$(initEXE) $(opt)/init/
	cp $(webserver)/$(webEXE) $(webroot)/bin/
	cp -r $(webroot)/* $(opt)/web/ 
	scp -r $(opt)/* $(readdr)

.PHONY:all clean config init web install 
