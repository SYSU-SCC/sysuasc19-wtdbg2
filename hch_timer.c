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


// //hch intel timer
// #define MAX_HCH_TIMER_LEN 288
// #define HCH_TIMER_CNT 20

// // #if defined (__i386__)
// // #define LT_CCPSS 2900000000
// // static __inline__ unsigned long long GetCycleCount(void)
// // {
// //         unsigned long long int x;
// //         __asm__ volatile("rdtsc":"=A"(x));
// //         return x;
// // }
// // #elif defined (__x86_64__)
// // #define HCH_CC_TYPE unsigned long long
// // #define LT_CCPSS 2900000000
// // static __inline__ unsigned long long GetCycleCount(void)
// // {
// //         unsigned hi,lo;
// //         __asm__ volatile("rdtsc":"=a"(lo),"=d"(hi));
// //         return ((unsigned long long)lo)|(((unsigned long long)hi)<<32);
// // }
// // #elif (defined SW2) || (defined SW5)
// // #define HCH_CC_TYPE unsigned long
// // #define LT_CCPSS 1450000000
// // static __inline__ unsigned long GetCycleCount(void)
// // {
// //     unsigned long time;
// //     asm("rtc %0": "=r" (time) : );
// //     return time;
// // }
// // #endif

#define HCH_CC_TYPE unsigned long
static __inline__ unsigned long getmsTime(void)
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    // printf("second:%ld\n",tv.tv_sec);  //秒
    // printf("millisecond:%ld\n",tv.tv_sec*1000 + tv.tv_usec/1000);  //毫秒
    // printf("microsecond:%ld\n",tv.tv_sec*1000000 + tv.tv_usec);  //微秒
    unsigned long time = tv.tv_sec*1000 + tv.tv_usec/1000;
    // asm("rtc %0": "=r" (time) : );
    return time;
}

// HCH_CC_TYPE hch_cc[MAX_HCH_TIMER_LEN];
// HCH_CC_TYPE hch_tmp_cc[MAX_HCH_TIMER_LEN];

// void hch_timer_init_()
// {
//     MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

//     int i;
//     for(i = 0; i < MAX_HCH_TIMER_LEN; i++)
//         hch_cc[i] = 0;
// }

// //hch intel timer

// void hch_timer_manual(long* arr, int cnt, const char* fname, const char* header)
// {
//     if(my_rank == 0)
//     {
//         FILE * fp;
//         fp = fopen (fname, "w");
//         fprintf(fp, header);
//         fprintf(fp, "\n");
//         fflush(fp);
//         fclose(fp);
//     }
//     MPI_Barrier(MPI_COMM_WORLD);
//     int i, j, k;
//     for(i = 0; i < comm_sz; i++)
//     {
//         if(my_rank == i)
//         {
//             FILE * fp;

//             fp = fopen (fname, "a");

//             fprintf(fp, "%d,", my_rank);

//             for(j = 0; j < cnt; j++)
//             {
//                 fprintf(fp, "%.4f,", arr[j] * 1.0);
//             }

//             fprintf(fp, "\n");

//             fflush(fp);
//             fclose(fp);

//             fflush(stdout);
//         }
//         MPI_Barrier(MPI_COMM_WORLD);
//     }
// }

// void hch_timer_finalize_()
// {
//     if(my_rank == 0)
//     {
//         printf("hch_timer_finalize_\n");
//         FILE * fp;
//         fp = fopen ("hch_timer_profile.csv", "w");
//         fprintf(fp, "my_rank, all, thread_pgz_func, merge_row_rdaln_pog, realign_msa_pog_core");
//         fprintf(fp, "\n");
//         fflush(fp);
//         fclose(fp);
//     }
//     MPI_Barrier(MPI_COMM_WORLD);
//     int i, j, k;
//     for(i = 0; i < comm_sz; i++)
//     {
//         if(my_rank == i)
//         {
//             FILE * fp;

//             fp = fopen ("hch_timer_profile.csv", "a");

//             // printf("%d,", my_rank);
//             fprintf(fp, "%d,", my_rank);

//             #ifndef HCH_TIMER_CNT
//             for(j = 0; j < MAX_HCH_TIMER_LEN; j++)
//             {
//                 if(hch_cc[j] == 0)
//                     break;
//                 // printf("%.4f,", hch_cc[j] * 1.0 / LT_CCPSS);
//                 fprintf(fp, "%.4f,", hch_cc[j] * 1.0 );
//             }
//             #else
//             for(j = 0; j < HCH_TIMER_CNT; j++)
//             {
//                 // printf("%.4f,", hch_cc[j] * 1.0 / LT_CCPSS);
//                 fprintf(fp, "%.4f,", hch_cc[j] * 1.0 );
//             }
//             #endif

//             fprintf(fp, "\n");
//             // printf("\n");

//             fflush(fp);
//             fclose(fp);

//             fflush(stdout);
//         }
//         MPI_Barrier(MPI_COMM_WORLD);
//     }
// }

// void hch_timer_start(int num)
// {
//     // if(omp_get_thread_num()==0)
//     //printf("my_rank = %d, num = %d\n", my_rank, *num);
//         hch_tmp_cc[num] = (HCH_CC_TYPE)getmsTime();
// }

// void hch_timer_stop(int num)
// {
//     //printf("my_rank = %d, num = %d\n", my_rank, *num);
//     // if(omp_get_thread_num()==0)
//         hch_cc[num] += (HCH_CC_TYPE)getmsTime() - hch_tmp_cc[num];
// }

// void hch_timer_start_(int* num)
// {
//     //printf("my_rank = %d, num = %d\n", my_rank, *num);
//     // if(omp_get_thread_num()==0)
//         hch_tmp_cc[*num] = (HCH_CC_TYPE)getmsTime();
// }

// void hch_timer_stop_(int* num)
// {
//     //printf("my_rank = %d, num = %d\n", my_rank, *num);
//     // if(omp_get_thread_num()==0)
//         hch_cc[*num] += (HCH_CC_TYPE)getmsTime() - hch_tmp_cc[*num];
// }


HCH_CC_TYPE timer_cc[timer_num];
HCH_CC_TYPE timer_temp_cc[timer_num];


void lt_timer_init(){
    int i=0;
    for(i = 0; i < timer_num; i++)
    {
        timer_cc[i]=0;
        timer_temp_cc[i]=0;
    }
}

void lt_timer_start(int num){
    timer_temp_cc[num]=getmsTime();
}

void lt_timer_stop(int num){
    timer_cc[num] = getmsTime() - timer_temp_cc[num];
}

void lt_timer_finalize(){
    int j=0;
    printf("hch_timer_finalize_\n");
    FILE * fp;
    fp = fopen ("hch_timer_profile.csv", "w");
    fprintf(fp, "all, thread_pgz_func, thread_mdbg_func, thread_midx_func");
    fprintf(fp, "\n");
    
    // printf("%d,", my_rank);
    // fprintf(fp, "%d,", 0);
    for(j = 0; j < timer_num; j++)
    {
        // printf("%.4f,", hch_cc[j] * 1.0 / LT_CCPSS);
        fprintf(fp, "%.4f,", timer_cc[j] * 1.0 / 1000 );
    }

    fprintf(fp, "\n");
    // printf("\n");

    fflush(fp);
    fclose(fp);

    fflush(stdout);
}