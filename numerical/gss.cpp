static const long long F[] = {
    1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 
    1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 
    196418, 317811, 514229, 832040, 1346269, 2178309, 3524578, 5702887, 
    9227465, 14930352, 24157817, 39088169, 63245986, 102334155, 
    165580141, 267914296, 433494437, 701408733, 1134903170, 1836311903,
    2971215073LL, 4807526976LL
};

int k = 2;
while (k < 47 && F[k] <= n + 1) k++;

long long offset = -1;
long long m1 = offset + F[k - 2];
long long m2 = offset + F[k - 1];

const long long INF = 4e18;
long long c1 = (m1 <= n) ? cost(m1) : INF;
long long c2 = (m2 <= n) ? cost(m2) : INF;

while (k > 2) {
    if (c1 >= c2) {
        offset = m1;
        m1 = m2;
        c1 = c2;
        k--;
        m2 = offset + F[k - 1];
        c2 = (m2 <= n) ? cost(m2) : INF;
    } else {
        k--;
        m2 = m1;
        c2 = c1;
        m1 = offset + F[k - 2];
        c1 = (m1 <= n) ? cost(m1) : INF;
    }
}

