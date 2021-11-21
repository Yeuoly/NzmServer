#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>

int register_timer(int sec, int usec, void (*func)(void *data), void *data);