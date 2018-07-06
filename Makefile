#make
#make install
#make init
#make web
#make clean

CC = arm-linux-gnueabihf-gcc

config = $(PWD)/config
init = $(PWD)/init
dtu = $(PWD)/dtu
security = $(PWD)/security
web = $(PWD)/web
webroot = $(web)/webroot
webserver = $(web)/websvr-goahead-2.18/LINUX
opt = ~/cpe_FileSystem/opt
staticPage = ~/ubuntushare/staticPage

initEXE = lterouter
export initEXE
dtuEXE = suyi_dtu
export dtuEXE
webEXE = router-web
export webEXE

#readdr = root@90.1.2.1:/opt
readdr = root@192.168.1.1:/opt

all:
	$(MAKE) -C $(init)
	$(MAKE) -C $(dtu)
	cd $(webserver) && $(MAKE)

config:
	cp $(config)/* $(opt)/config/
	scp -r $(config)/* $(readdr)/config

init:
	$(MAKE) -C $(init)
	cp $(init)/*.sh $(init)/$(initEXE) $(opt)/init/
	scp -r $(init)/*.sh $(init)/$(initEXE) $(readdr)/init/
	#cd $(init) && $(MAKE)
dtu:
	$(MAKE) -C $(dtu)
	cp $(dtu)/$(dtuEXE) $(opt)/dtu/
	scp -r $(dtu)/$(dtuEXE) $(readdr)/dtu/
	#cd $(init) && $(MAKE)

security:
	cp $(security)/* $(opt)/security/
	scp -r $(security)/* $(readdr)/security/
	#cd $(init) && $(MAKE)
	
web:
	cd $(webserver) && $(MAKE) 
	cp $(webserver)/$(webEXE) $(webroot)/bin/
	cp -r $(webroot)/* $(opt)/web/
	scp -r $(webroot)/* $(readdr)/web/

clean:
	cd $(webserver) && $(MAKE) clean 
	cd $(init) && $(MAKE) clean 
	cd $(dtu) && $(MAKE) clean 

opt:
	rm $(opt)/* -rf
	-mkdir $(opt) $(opt)/config $(opt)/init $(opt)/security $(opt)/dtu $(opt)/web $(opt)/log $(opt)/upgrade
	cp $(config)/* $(opt)/config/
	cp $(init)/*.sh $(init)/$(initEXE) $(opt)/init/
	cp $(dtu)/$(dtuEXE) $(opt)/dtu/
	cp $(security)/* $(opt)/security/
	cp $(webserver)/$(webEXE) $(webroot)/bin/
	cp -r $(webroot)/* $(opt)/web/
	cp -r $(staticPage) $(opt)/web/
	rm -rf $(opt)/web/staticPage/.git
#system.tar 恢复出厂设置使用 解压时必须进入对应的目录
	cd $(opt);tar -cf upgrade/config.tar ./config

#网页升级包，升级后删除
package:
	rm -rf ~/ubuntushare/package/*
	cd $(opt);tar -cf ~/ubuntushare/package/cpe.tar ./*
	cd ~/ubuntushare/package;md5sum cpe.tar > cpe.md5;tar -cf system.tar ./*

install:
	scp -r $(opt)/* $(readdr)

.PHONY:all clean config init dtu web install security
