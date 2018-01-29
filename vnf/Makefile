include  $(BE_ROOT)/mk/config.mk
export ACC_LIB_PATH="-L$(CURDIR)/cgnat-acc/ -lcgnatacc"
all:submodule-update build-cgnat-acc build-vnf

submodule-update:
	if [ ! -d samplevnf/tools ];then  git submodule init ; git submodule update; (cd samplevnf ; git checkout . ; git branch -D webase; git branch webase opnfv-5.1.0 ; git am --abort ; git am --ignore-space-change --ignore-whitespace ../opnfv-5.1.0-patch/*.patch ) ; fi;

build-vnf:
	(cd samplevnf ; sudo ACC_LIB_PATH=$(ACC_LIB_PATH) ACC_INCLUDE_PATH="$(include_path)" ./Setup.sh ;)
	
build-cgnat-acc:
	$(MAKE) -C cgnat-acc clean
	$(MAKE) -C cgnat-acc all

clean:
	$(MAKE) -C cgnat-acc clean
	
