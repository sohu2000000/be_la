#! /bin/bash
export_name="export-dir"
tmp_dir=`pwd`/$export_name/

#prepare workspace
rm -rf $tmp_dir
mkdir $tmp_dir

#export
git checkout-index -a -f --prefix=$tmp_dir

#create tar
tar -zcvf export.vnf-only.tar.gz $export_name

#clean up
rm -rf $tmp_dir
