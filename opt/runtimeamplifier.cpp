struct RuntimeAmplifier { // Kattis rounds to two decimals: increase effective precision
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
