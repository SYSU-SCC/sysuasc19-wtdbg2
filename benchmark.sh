export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
module load nvme/impi/2019.3
module load nvme/gcc/8.3.0
make clean && make