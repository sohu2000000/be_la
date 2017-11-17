export ACC_LIB_PATH="-L$(CURDIR)/cgnat-acc/ -lcgnatacc"
all:build-cgnat-acc sample-vnf-checkout build-vnf

sample-vnf-checkout:
	(cd samplevnf ; git checkout . ; git checkout v1.0.1 ; patch -p1 < ../samplevnf-cgnat-patch/build-cgnat-diff.patch ) #samplevnf-build-patch.diff)

build-vnf:
	(cd samplevnf ; sudo ACC_LIB_PATH=$(ACC_LIB_PATH) ./tools/vnf_build.sh --silient ;)
	
build-cgnat-acc:
	$(MAKE) -C cgnat-acc clean
	$(MAKE) -C cgnat-acc all
