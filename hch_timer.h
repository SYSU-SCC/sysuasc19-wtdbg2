#define timer_num 18

// #define lt_timer_start(num) lt_timer_start(num, 0)
// #define lt_timer_stop(num) lt_timer_stop(num, 0)
#define TIMER_ALL                           0
#define TIMER_proc_alignments_core          1
#define TIMER_index_kbm                     2
#define TIMER_mdbg                          3
#define TIMER_write_alignment               4

void lt_timer_init();
void lt_timer_start(int num, int tid);
int lt_timer_stop(int num, int tid);
void lt_timer_finalize();