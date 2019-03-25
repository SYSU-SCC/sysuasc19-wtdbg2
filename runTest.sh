#amplxe-cl -collect hotspots -data-limit=0 -result-dir rhreadPart 
./wtdbg2 -x rs -g 4.6m -i data/selfSampleData.tar.gz -t16 -fo sample/te > log.txt
#amplxe-cl -collect hotspots -data-limit=0 -result-dir rhconsensusPart 
#./wtpoa-cns -t 16 -i sample/te.ctg.lay.gz -fo sample/te.ctg.fa
