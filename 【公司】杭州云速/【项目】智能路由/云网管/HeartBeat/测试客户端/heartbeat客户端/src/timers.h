#ifndef TIMER_H
#define TIMER_H

#include <map>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <sys/time.h>

typedef long long time_value;

struct timer_event {
    time_t times;
    void *arg;
    int eventtype;
};
time_value get_now();


class timer_link {
    static const int64_t mintimer = 1000;

   public:
    timer_link() {}

    virtual ~timer_link() {}

    int add_timer(void *, time_t);

    void *get_timer(time_t tnow);
 
    void remote_timer(void *);

    time_t get_mintimer() const;

    int get_arg_time_size() const { return timer_arg_time.size(); }
    int get_time_arg_size() const { return timer_time_arg.size(); }

    void show() const;

   private:
    std::multimap<void *, time_t> timer_arg_time;

    std::multimap<time_t, void *> timer_time_arg;
};

#endif
