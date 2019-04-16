#
# mpirun -np 2 -genv I_MPI_FABRICS shm:ofa -machinefile ../machinefile ./../wtdbg2 -x rs -g144m -t32 -i /nfs_public/wtdbg/data/melanogaster_A4_subreads.fastq.gz -fo a4
mpirun -np 2 -genv I_MPI_FABRICS shm:ofa -machinefile ../machinefile ./../wtdbg2 -x rs -g144m -t32 -i /GPUFS/nsccgz_yfdu_16/liangt/wtdbgdata/melanogaster_A4_subreads.fastq.gz -fo a4  2>&1 | tee log.txt
#./wtpoa-cns -t 16 -i sample/te.ctg.lay.gz -fo sample/te.ctg.fa