#! /bin/bash
tmp_dir=`pwd`/export-dir/

#prepare workspace
rm -rf $tmp_dir
mkdir $tmp_dir

#export
git checkout-index -a -f --prefix=$tmp_dir

#create tar
tar -zcvf export.vnf-only.tar.gz $tmp_dir

#clean up
rm -rf $tmp_dir
