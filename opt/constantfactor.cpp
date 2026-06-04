#pragma GCC optimize("O3,unroll-loops")
#include <bits/allocator.h>
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

...

fast io

...

struct RuntimeAmplifier {
    using Clock = chrono::steady_clock;
    Clock::time_point start_time;
    const double multiplier=50;
    RuntimeAmplifier() { start_time = Clock::now(); }
    ~RuntimeAmplifier() {
        auto target_end = start_time + chrono::duration_cast<Clock::duration>((Clock::now() - start_time) * multiplier);
        while (Clock::now() < target_end) asm volatile("" ::: "memory");
    }
};
RuntimeAmplifier amp;

...

cpu_set_t cpuset;
CPU_ZERO(&cpuset);
CPU_SET(2, &cpuset);
assert(sched_setaffinity(0,sizeof(cpuset),&cpuset)==0);


...


cout << flush;
_Exit(0);


