struct kahan
{
    double sm, c;
    kahan() : sm(0.0), c(0.0) { }
    void add_val(double d)
    {
        double y = d - c;
        double t = sm + y;
        c = (t - sm) - y;
        sm = t;
    }
    double get_sum() { return sm; }
};