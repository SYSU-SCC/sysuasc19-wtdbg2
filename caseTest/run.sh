#!/bin/bash
module load intelcompiler/2018u4
set -x
today=`date +%Y%m%d.%H%M%S`
export LID=$today
WTDBG_PATH=/GPUFS/nsccgz_yfdu_16/wyf/wtdbg/source/wtdbg2
CASE_INDEX=3
NUM_THREADS=28
#DATA_PATH=/nfs_public/wtdbg/data
DATA_PATH=/GPUFS/nsccgz_yfdu_16/wyf/wtdbg/data
#MPI_RUN="mpirun -n 2 -machinefile machinefile "

export I_MPI_FABRICS=shm:ofa


CASENAMES=( \
        0selfSampleData \
        1melanogaster_A4_subreads \
        2melanogaster_ISO1 \
        3Human_NA19240 \
        4Human_HG00733 \
        5Human_NA24385 \
)

CASENAME=${CASENAMES[$CASE_INDEX]}
export PATH=$WTDBG_PATH:$PATH
if [ $CASE_INDEX == 0 ]
then
        $MPI_RUN wtdbg2 -x rs -g 4.6m -t $NUM_THREADS -i $DATA_PATH/selfSampleData.tar.gz -fo $CASENAME 2>&1 | tee  $CASENAME.log.${today}
elif [ $CASE_INDEX == 1 ]
then
        $MPI_RUN wtdbg2  -x rs -g 144m  -t $NUM_THREADS -i $DATA_PATH/melanogaster_A4_subreads.fastq.gz -fo $CASENAME 2>&1 | tee $CASENAME.log.${today}
elif [ $CASE_INDEX == 2 ]
then
        $MPI_RUN wtdbg2  -x ont -g 144m -t $NUM_THREADS -i $DATA_PATH/melanogaster_ISO1.fastq.gz  -fo $CASENAME 2>&1 | tee $CASENAME.log.${today}
elif [ $CASE_INDEX == 3 ]
then
        $MPI_RUN wtdbg2  -x ont -g 3g -t $NUM_THREADS -i $DATA_PATH/Human_NA19240.fastq.gz  -fo $CASENAME 2>&1 | tee $CASENAME.log.${today}
elif [ $CASE_INDEX == 4 ]
then
        $MPI_RUN wtdbg2 -x sq -g 3g -t $NUM_THREADS -i $DATA_PATH/Human_HG00733_SRR7615963_subreads.fastq.gz -fo $CASENAME 2>&1 | tee $CASENAME.log.${today}
elif [ $CASE_INDEX == 5 ]
then
        $MPI_RUN wtdbg2  -x ccs -g 3g  -t $NUM_THREADS  -i $DATA_PATH/Human_NA24385_HG002.15kb.Q20.tar.gz -fo $CASENAME 2>&1 | tee $CASENAME.log.${today}
fi

#wtpoa-cns -t $NUM_THREADS -i $CASENAME.ctg.lay.gz -fo ${CASENAME}_output.ctg.fa 2>&1 | tee -a $CASENAME.log.${today}
#gzip -d 0selfSampleData.ctg.lay.gz
