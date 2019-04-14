#
mpirun -np 1 -genv I_MPI_FABRICS shm:ofa -machinefile ../machinefile ./../wtdbg2 -x rs -g 4.6m -i /nfs_public/asc19/liangt/inputdata/selfSampleData.tar.gz -t16 -fo te 
# > log.txt 2>&1
# mpirun -np 2 -genv I_MPI_FABRICS shm:ofa -machinefile ../machinefile ./../wtdbg2 -x rs -g 4.6m -i /GPUFS/nsccgz_yfdu_16/liangt/wtdbgdata/selfSampleData.tar.gz -t16 -fo te
#./wtpoa-cns -t 16 -i sample/te.ctg.lay.gz -fo sample/te.ctg.fa
