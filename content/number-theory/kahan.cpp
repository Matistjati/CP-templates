// Compiler might optimize out the algorithm, consider disabling ofast
typedef double real;
struct Real
{
    real x, c;
    Real() : x(0), c(0) {}
    Real(real v) : x(v), c(0) {}
    Real operator+(real d)
    {
        double y = d - c;
        double t = x + y;
        c = (t - x) - y;
        x = t;
    }
    double get() { return x; }
};

