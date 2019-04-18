#include <math.h>
// #include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include "hch_timer.h"

// int my_rank, comm_sz;


// //MPF* 主核 profile 嵌套第*层

// //使用hch_timer

// #define MPF1_ALL                0
// #define MPF1_RADIATION          1
// #define MPF1_XTPV               2
// #define MPF1_VAPSSM             3
// #define MPF1_cam_run1           4
// #define MPF1_HALO               5
// #define TIMER_ALL                           0
// #define TIMER_proc_alignments_core          1
// #define TIMER_index_kbm                     2
// #define TIMER_mdbg                          3
// #define TIMER_write_alignment               4


// //hch intel timer
#if defined (__i386__)
#define HCH_CC_TYPE unsigned long long
#define CCPS 2900000000
static __inline__ unsigned long long GetCycleCount(void)
{
        unsigned long long int x;
        __asm__ volatile("rdtsc":"=A"(x));
        return x;
}
#elif defined (__x86_64__)
#define HCH_CC_TYPE unsigned long long
#define CCPS 2900000000
static __inline__ unsigned long long GetCycleCount(void)
{
        unsigned hi,lo;
        __asm__ volatile("rdtsc":"=a"(lo),"=d"(hi));
        return ((unsigned long long)lo)|(((unsigned long long)hi)<<32);
}
#elif (defined SW2) || (defined SW5)
#define HCH_CC_TYPE unsigned long
#define CCPS 1450000000
static __inline__ unsigned long GetCycleCount(void)
{
    unsigned long time;
    asm("rtc %0": "=r" (time) : );
    return time;
}
#endif

// #define HCH_CC_TYPE unsigned long
static __inline__ HCH_CC_TYPE getmsTime(void)
{
    // struct timeval tv;
    // gettimeofday(&tv,NULL);
    // // printf("second:%ld\n",tv.tv_sec);  //秒
    // // printf("millisecond:%ld\n",tv.tv_sec*1000 + tv.tv_usec/1000);  //毫秒
    // // printf("microsecond:%ld\n",tv.tv_sec*1000000 + tv.tv_usec);  //微秒
    // unsigned long time = tv.tv_sec*1000 + tv.tv_usec ;
    // unsigned long time = tv.tv_sec*1000000 + tv.tv_usec;
    // asm("rtc %0": "=r" (time) : );
    
    return GetCycleCount();
}

static __inline__ HCH_CC_TYPE getusTime(void)
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    // printf("second:%ld\n",tv.tv_sec);  //秒
    // printf("millisecond:%ld\n",tv.tv_sec*1000 + tv.tv_usec/1000);  //毫秒
    // printf("microsecond:%ld\n",tv.tv_sec*1000000 + tv.tv_usec);  //微秒
    // unsigned long time = tv.tv_sec*1000 + tv.tv_usec ;
    HCH_CC_TYPE time = tv.tv_sec*1000 + tv.tv_usec;
    // asm("rtc %0": "=r" (time) : );
    return time;
}

#define thread_num 36 

HCH_CC_TYPE timer_cc[thread_num][timer_num];
HCH_CC_TYPE timer_temp_cc[thread_num][timer_num];


void lt_timer_init(){
    int i=0;
    int j=0;
    for(i = 0; i < thread_num; i++){
        for(j = 0; j < timer_num; j++)
        {
            timer_cc[i][j]=0;
            timer_temp_cc[i][j]=0;
        }
    }
}

void lt_timer_start(int num, int tid){
    timer_temp_cc[tid][num]=GetCycleCount();
}

int lt_timer_stop(int num,int tid){
    timer_cc[tid][num] += GetCycleCount() - timer_temp_cc[tid][num];
    // if(num == 14 && tid ==0){
    //     printf("%f\n", (GetCycleCount() - timer_temp_cc[tid][num]) * 1000.0/CCPS);
    // }
}

void lt_count(int item, int tid, int num){
    timer_cc[tid][item] +=num;
}

void lt_timer_finalize(){
    int i=0,j=0;
    FILE * fp;
    fp = fopen ("hch_timer_profile.csv", "w");
    // fprintf(fp, "all, proc_alignments_core, index_kbm, mdbg, write_alignment, changeRDFlag, chainning_hit, editGraph, result_time, loop_mdbg, clearkbm, query_index_kbm, map_kbm, sorthit_mat, push_kmer_match_kbm, map_kbm_pre,map_index_sort");
    fprintf(fp, "all, iter_ctgcns, mcns_inner_loop, end_tripog");
    fprintf(fp, "\n");
// #define TIMER_ALL                           0
// #define TIMER_proc_alignments_core          1
// #define TIMER_index_kbm                     2
// #define TIMER_mdbg                          3
// #define TIMER_write_alignment               4

    // printf("%d,", my_rank);
    // fprintf(fp, "%d,", 0);
    for(i = 0; i < thread_num; i++){
        for(j = 0; j < timer_num; j++)
        {
            // printf("%.4f,", hch_cc[j] * 1.0 / LT_CCPSS);
            fprintf(fp, "%.4f,", timer_cc[i][j] * 1.0 / CCPS );
        }    
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n");
    // printf("\n");

    fflush(fp);
    fclose(fp);

    fflush(stdout);
}