./kernel/time/timekeeping.c:293:void do_gettimeofday(struct timeval *tv)
./kernel/time/timekeeping.c:302:EXPORT_SYMBOL(do_gettimeofday);


【./kernel/time/timekeeping.c】
/**
 * do_gettimeofday - Returns the time of day in a timeval
 * @tv:         pointer to the timeval to be set
 *
 * NOTE: Users should be converted to using getnstimeofday()
 */
void do_gettimeofday(struct timeval *tv)
{
        struct timespec now;

        getnstimeofday(&now);
        tv->tv_sec = now.tv_sec;
        tv->tv_usec = now.tv_nsec/1000;
}

EXPORT_SYMBOL(do_gettimeofday);



/**
 * getnstimeofday - Returns the time of day in a timespec
 * @ts:         pointer to the timespec to be set
 *
 * Returns the time of day in a timespec.
 */
void getnstimeofday(struct timespec *ts)
{
        unsigned long seq;
        s64 nsecs;

        WARN_ON(timekeeping_suspended);

        do {
                seq = read_seqbegin(&xtime_lock);

                *ts = xtime;
                nsecs = timekeeping_get_ns();

                /* If arch requires, add in gettimeoffset() */
                nsecs += arch_gettimeoffset();

        } while (read_seqretry(&xtime_lock, seq));

        timespec_add_ns(ts, nsecs);
}

EXPORT_SYMBOL(getnstimeofday);

/* Timekeeper helper functions. */
static inline s64 timekeeping_get_ns(void)
{
        cycle_t cycle_now, cycle_delta;
        struct clocksource *clock;

        /* read clocksource: */
        clock = timekeeper.clock;
        cycle_now = clock->read(clock);

        /* calculate the delta since the last update_wall_time: */
        cycle_delta = (cycle_now - clock->cycle_last) & clock->mask;

        /* return delta convert to nanoseconds using ntp adjusted mult. */
        return clocksource_cyc2ns(cycle_delta, timekeeper.mult,
                                  timekeeper.shift);
}


【./include/linux/clocksource.h】
/**
 * clocksource_cyc2ns - converts clocksource cycles to nanoseconds
 *
 * Converts cycles to nanoseconds, using the given mult and shift.
 *
 * XXX - This could use some mult_lxl_ll() asm optimization
 */
static inline s64 clocksource_cyc2ns(cycle_t cycles, u32 mult, u32 shift)
{
        return ((u64) cycles * mult) >> shift;
}



【./arch/mips/ralink/time.c】
/*
 *  === Ralink systick clock source device implementation ===
 */
static cycle_t ra_systick_read(struct clocksource *cs)
{
        return (*((volatile u32 *)(RALINK_COUNT)));
}

static struct clocksource ra_systick_clocksource = {
        .name           = "Ralink Systick timer",
        .mask           = 0xffff,
        .read           = ra_systick_read,
        .flags          = CLOCK_SOURCE_IS_CONTINUOUS,
};