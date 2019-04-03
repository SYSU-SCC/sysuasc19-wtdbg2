#define timer_num 9

// #define lt_timer_start(num) lt_timer_start(num, 0)
// #define lt_timer_stop(num) lt_timer_stop(num, 0)

void lt_timer_init();
void lt_timer_start(int num, int tid);
int lt_timer_stop(int num, int tid);
void lt_timer_finalize();