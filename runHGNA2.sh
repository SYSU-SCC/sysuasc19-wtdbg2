#amplxe-cl -collect hotspots -data-limit=0 -result-dir rhreadPart 
./wtdbg2 -x ccs -g3g -t32 -i /nfs_public/wtdbg/data/Human_NA24385_HG002.15kb.Q20.tar.gz -fo hg002/hg > hgna.log
#amplxe-cl -collect hotspots -data-limit=0 -result-dir rhconsensusPart 
./wtpoa-cns -t 32 -i hg002/hg.ctg.lay.gz -fo hg002/hg.ctg.fa
