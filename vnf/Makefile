all:sample-vnf-checkout build-vnf

sample-vnf-checkout:
	(cd samplevnf ; git checkout . ; git checkout v1.0.1 ; patch -p1 < ../samplevnf-cgnat-patch/build-cgnat-diff.patch ) #samplevnf-build-patch.diff)

build-vnf:
	(cd samplevnf ; sudo ./tools/vnf_build.sh --silient ;)
