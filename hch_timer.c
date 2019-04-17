#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#ifdef _OPENMP
#include <omp.h>
#endif
int my_rank, comm_sz;


//MPF* 主核 profile 嵌套第*层

//使用hch_timer

#define MPF1_ALL                0
#define MPF1_RADIATION          1
#define MPF1_XTPV               2
#define MPF1_VAPSSM             3
#define MPF1_cam_run1           4
#define MPF1_HALO               5


//hch intel timer
#define MAX_HCH_TIMER_LEN 288
#define HCH_TIMER_CNT 10

// #if defined (__i386__)
// #define LT_CCPSS 2900000000
// static __inline__ unsigned long long GetCycleCount(void)
// {
//         unsigned long long int x;
//         __asm__ volatile("rdtsc":"=A"(x));
//         return x;
// }
// #elif defined (__x86_64__)
// #define HCH_CC_TYPE unsigned long long
// #define LT_CCPSS 2900000000
// static __inline__ unsigned long long GetCycleCount(void)
// {
//         unsigned hi,lo;
//         __asm__ volatile("rdtsc":"=a"(lo),"=d"(hi));
//         return ((unsigned long long)lo)|(((unsigned long long)hi)<<32);
// }
// #elif (defined SW2) || (defined SW5)
// #define HCH_CC_TYPE unsigned long
// #define LT_CCPSS 1450000000
// static __inline__ unsigned long GetCycleCount(void)
// {
//     unsigned long time;
//     asm("rtc %0": "=r" (time) : );
//     return time;
// }
// #endif

#define HCH_CC_TYPE unsigned long long
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

HCH_CC_TYPE hch_call_counter[MAX_HCH_TIMER_LEN];
HCH_CC_TYPE hch_cc[MAX_HCH_TIMER_LEN];
HCH_CC_TYPE hch_tmp_cc[MAX_HCH_TIMER_LEN];
char hch_timer_profile_name[40];
char hch_counter_profile_name[40];

void hch_timer_init_()
{
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    int i;
    for(i = 0; i < MAX_HCH_TIMER_LEN; i++){
        hch_cc[i] = 0;
        hch_call_counter[i] = 0;
    }
    // 尝试使用PID环境变量给文件命名
    char * pid_point = getenv("LID");
    if (pid_point != NULL){
        strcpy(hch_timer_profile_name, "hch_timer_profile.");
        strcat(hch_timer_profile_name, pid_point);
        strcat(hch_timer_profile_name, ".csv");
        strcpy(hch_counter_profile_name, "hch_counter_profile.");
        strcat(hch_counter_profile_name, pid_point);
        strcat(hch_counter_profile_name, ".csv");
        // printf("will be write hch_timer_profile in %s \n", hch_timer_profile_name);
    }
    else{
        strcpy(hch_timer_profile_name, "hch_timer_profile");
        strcat(hch_timer_profile_name, ".csv");
        strcpy(hch_counter_profile_name, "hch_counter_profile");
        strcat(hch_counter_profile_name, ".csv");
        printf("WYF : ERROR : can not get LID environment variable! \n");
    }
}

void hch_timer_finalize_()
{
    HCH_CC_TYPE sum_times[HCH_TIMER_CNT];
    int i, j, k;
    for (i = 0; i < HCH_TIMER_CNT; i++){
        sum_times[i] = 0;
    }
    // 全局求和
    // NOTE: MPI_UNSIGNED_LONG_LONG 可能不支持？？
    MPI_Reduce(&hch_cc, &sum_times, HCH_TIMER_CNT, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    if(my_rank == 0)
    {
        printf("hch_timer_finalize_\n");
        FILE * fp;
        fp = fopen (hch_timer_profile_name, "w");
        fprintf(fp, "my_rank, all");
        fprintf(fp, "\n");
        fprintf(fp, "average,");
        for(j = 0; j < HCH_TIMER_CNT; j++)
        {
            // printf("%.4f,", hch_cc[j] * 1.0 / LT_CCPSS);
            fprintf(fp, "%.4f,", sum_times[j] * 1.0/comm_sz);
        }
        fprintf(fp, "\n");
        fflush(fp);
        fclose(fp);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    for(i = 0; i < comm_sz; i++)
    {
        if(my_rank == i)
        {
            FILE * fp;

            fp = fopen (hch_timer_profile_name, "a");

            // printf("%d,", my_rank);
            fprintf(fp, "%d,", my_rank);

            #ifndef HCH_TIMER_CNT
            for(j = 0; j < MAX_HCH_TIMER_LEN; j++)
            {
                if(hch_cc[j] == 0)
                    break;
                // printf("%.4f,", hch_cc[j] * 1.0 / LT_CCPSS);
                fprintf(fp, "%.4f,", hch_cc[j] * 1.0 );
            }
            #else
            for(j = 0; j < HCH_TIMER_CNT; j++)
            {
                // printf("%.4f,", hch_cc[j] * 1.0 / LT_CCPSS);
                fprintf(fp, "%.4f,", hch_cc[j] * 1.0 );
            }
            #endif

            fprintf(fp, "\n");
            // printf("\n");

            fflush(fp);
            fclose(fp);

            fflush(stdout);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    HCH_CC_TYPE sum_counters[HCH_TIMER_CNT];
    for (i = 0; i < HCH_TIMER_CNT; i++){
        sum_counters[i] = 0;
    }
    MPI_Reduce(&hch_call_counter, &sum_counters, HCH_TIMER_CNT, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    if(my_rank == 0)
    {
        printf("hch_counter_finalize_\n");
        FILE * fp;
        fp = fopen (hch_counter_profile_name, "w");
        fprintf(fp, "my_rank, all, outer mdbg loop");
        fprintf(fp, "\n");
        fprintf(fp, "average,");
        for(j = 0; j < HCH_TIMER_CNT; j++)
        {
            // printf("%.4f,", hch_cc[j] * 1.0 / LT_CCPSS);
            fprintf(fp, "%.4f,", sum_counters[j] * 1.0/comm_sz);
        }
        fprintf(fp, "\n");
        fflush(fp);
        fclose(fp);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    for(i = 0; i < comm_sz; i++)
    {
        if(my_rank == i)
        {
            FILE * fp;

            fp = fopen (hch_counter_profile_name, "a");

            // printf("%d,", my_rank);
            fprintf(fp, "%d,", my_rank);

            #ifndef HCH_TIMER_CNT
            for(j = 0; j < MAX_HCH_TIMER_LEN; j++)
            {
                if(hch_cc[j] == 0)
                    break;
                // printf("%.4f,", hch_cc[j] * 1.0 / LT_CCPSS);
                fprintf(fp, "%.4f,", hch_call_counter[j] * 1.0 );
            }
            #else
            for(j = 0; j < HCH_TIMER_CNT; j++)
            {
                // printf("%.4f,", hch_cc[j] * 1.0 / LT_CCPSS);
                fprintf(fp, "%.4f,", hch_call_counter[j] * 1.0 );
            }
            #endif

            fprintf(fp, "\n");
            // printf("\n");

            fflush(fp);
            fclose(fp);

            fflush(stdout);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

void hch_timer_start_(int* num)
{
    //printf("my_rank = %d, num = %d\n", my_rank, *num);
#ifdef _OPENMP
    if(omp_get_thread_num()==0)
#endif
        hch_tmp_cc[*num] = (HCH_CC_TYPE)getmsTime();
        hch_call_counter[*num] += (HCH_CC_TYPE)1;
}


void hch_timer_stop_(int* num)
{
    //printf("my_rank = %d, num = %d\n", my_rank, *num);
#ifdef _OPENMP
    if(omp_get_thread_num()==0)
#endif
        hch_cc[*num] += (HCH_CC_TYPE)getmsTime() - hch_tmp_cc[*num];
}

void hch_timer_start(int num)
{
    //printf("my_rank = %d, num = %d\n", my_rank, *num);
#ifdef _OPENMP
    if(omp_get_thread_num()==0)
#endif
        hch_tmp_cc[num] = (HCH_CC_TYPE)getmsTime();
        hch_call_counter[num] += (HCH_CC_TYPE)1;
}

void hch_timer_stop(int num)
{
    //printf("my_rank = %d, num = %d\n", my_rank, *num);
#ifdef _OPENMP
    if(omp_get_thread_num()==0)
#endif
        hch_cc[num] += (HCH_CC_TYPE)getmsTime() - hch_tmp_cc[num];
}
