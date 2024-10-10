int binpow(int a, int b, int m) // a^b mod m
{
    int ret = 1;
    int e = a;
    while (b)
    {
        if (b % 2) ret = (ret * e) % m;
        e = (e * e) % m;
        b >>= 1;
    }

    return ret;
}

