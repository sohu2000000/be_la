include  $(BE_ROOT)/mk/config.mk
export ACC_LIB_PATH="-L$(CURDIR)/cgnat-acc/ -lcgnatacc"
all:submodule-update build-cgnat-acc sample-vnf-checkout build-vnf

submodule-update:
	if [ ! -d samplevnf/tools ];then  git submodule init ; git submodule update ; fi;
sample-vnf-checkout:
	(cd samplevnf ; git checkout . ; git checkout v1.0.1 ; patch -p1 < ../samplevnf-cgnat-patch/build-cgnat-diff.patch ) #samplevnf-build-patch.diff)

build-vnf:
	(cd samplevnf ; sudo ACC_LIB_PATH=$(ACC_LIB_PATH) ACC_INCLUDE_PATH="$(include_path)" ./tools/vnf_build.sh --silient ;)
	
build-cgnat-acc:
	$(MAKE) -C cgnat-acc clean
	$(MAKE) -C cgnat-acc all

clean:
	$(MAKE) -C cgnat-acc clean
	
