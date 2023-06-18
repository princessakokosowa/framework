#ifndef INCLUDE_TIME_H
#define INCLUDE_TIME_H

core_function
f64 Time_stamp(void) {
    // `counter_start` and `counter_end` are both, eh, counters, which are dependent on
    // the clock frequency.
    //
    // To get the time in seconds, you need to know the start and end stamp counter and
    // divide it by the clock frequency.
    //
    // @TODO
    // We can have the time returned in e.g. milliseconds or nanoseconds, `frequency`
    // should be multiplied accordingly, but I haven't decided yet how to do this. Should
    // it be a separate procedure, or e.g. an if-else block in this procedure and
    // possibly pass the mode as an argument? I don't know, mate, but it's not really
    // relevant to me at the moment.
    //     ~ princessakokosowa, 8th of March 2023
    isize static counter_start;
    isize static frequency;
    if (counter_start == 0) QueryPerformanceCounter(cast(LARGE_INTEGER *, &counter_start));
    if (frequency     == 0) QueryPerformanceFrequency(cast(LARGE_INTEGER *, &frequency));

    isize counter_end;
    QueryPerformanceCounter(cast(LARGE_INTEGER*, &counter_end));

    isize counter_split = counter_end - counter_start;
    f64   split         = cast(f64, counter_split) / cast(f64, frequency);

    return split;
}

#endif INCLUDE_TIME_H
