
int xGCD(int a, int b, int& x, int& y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }

    int x1, y1, gcd = xGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

// Solve ax+by=gcd(a,b). Returns {x,y}
p2 xGCD(int a, int b)
{
    bool swapped = false;
    if (a < b)
    {
        swap(a, b);
        swapped = true;
    }
    int p, q;
    xGCD(a, b, p, q);
    if (swapped) swap(p, q);
    return { p,q };
}