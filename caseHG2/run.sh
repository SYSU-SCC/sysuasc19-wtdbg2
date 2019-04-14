#
mpirun -np 4 -genv I_MPI_FABRICS shm:ofa -machinefile ../machinefile ./../wtdbg2 -x ccs -g3g -t32 -i /nfs_public/wtdbg/data/Human_NA24385_HG002.15kb.Q20.tar.gz -fo hg > hgna.log 2 >&1
# > log.txt 2>&1
# mpirun -np 2 -genv I_MPI_FABRICS shm:ofa -machinefile ../machinefile ./../wtdbg2 -x rs -g 4.6m -i /GPUFS/nsccgz_yfdu_16/liangt/wtdbgdata/selfSampleData.tar.gz -t16 -fo te
#./wtpoa-cns -t 16 -i sample/te.ctg.lay.gz -fo sample/te.ctg.fa
 