#include "./timer.h"

#define TIMER_INTERVAL 1

struct NzmTimer {
    NzmTimer *next;
    int diff_sec;
    int diff_usec;
    long long id;

    void *data;

    void (*func)(void *data);
};

NzmTimer *timer_list = NULL;
pthread_mutex_t lock;
long long current_id = 0;

int register_timer(int sec, int usec, void (*func)(void *data), void *data){
    pthread_mutex_lock(&lock);

    NzmTimer *timer = new NzmTimer;
    timer->data = data;
    timer->func = func;
    timer->diff_sec = sec;
    timer->diff_usec = usec;
    timer->id = current_id++;

    if(!timer){
        return -1;
    }
    
    if(!timer_list){
        timer_list = timer;
        timer_list->next = NULL;
    }else{
        NzmTimer *next_chunk = timer_list->next;
        timer_list->next = timer;
        timer->next = next_chunk;
    }

    pthread_mutex_unlock(&lock);

    return timer->id;
}

int clear_timer(long long id){
    pthread_mutex_lock(&lock);

    NzmTimer *tmp;
    if(timer_list->id == id){
        tmp = timer_list;
        timer_list = timer_list->next;
        delete tmp;
    }
    for(NzmTimer *timer = timer_list; timer != NULL; timer = timer->next){
        if(timer->next != NULL){
            if(timer->next->id == id){
                tmp = timer->next;
                timer->next = tmp->next;
                delete tmp;
                break;
            }
        }else{
            break;
        }
    }

    pthread_mutex_unlock(&lock);
    return 0;
}

void timer_checkout(){
    pthread_mutex_lock(&lock);
    NzmTimer *tmp;

    timer_list->diff_usec += TIMER_INTERVAL;

    do{
        if(timer_list->diff_usec >= timer_list->diff_sec){
            tmp = timer_list;
            timer_list = timer_list->next;
            tmp->func(tmp->data);
            delete tmp;
        }else{
            break;
        }
    }while(timer_list != NULL);


    for(NzmTimer *timer = timer_list; timer != NULL ; timer = timer->next){
        if(timer->next != NULL){
            timer->next->diff_usec += TIMER_INTERVAL;
            if(timer->next->diff_usec > timer->next->diff_sec){
                tmp = timer->next;
                timer->next = tmp->next;
                
                delete tmp;
            }
        }else{
            break;
        }
    }

    pthread_mutex_unlock(&lock);
}