// minima ($n$+k)*log($max_v$)/log(k)
// Find nearest k that is power of two
// If k^2>max_v, then it can be unrolled into two loops, one using only mod and the other only division
const int radix = 1 << 16;
const int radixmod = radix - 1;
void countsort(vi& nums, int power)
{
    vi out(nums.size());
    vi count(radix);
    rep(i, radix) count[i] = 0;

    repe(num, nums) count[(num / power) & radixmod]++;
    repp(i, 1, radix) count[i] += count[i - 1];

    for (int i = nums.size() - 1; i >= 0; i--)
    {
        int num = nums[i];
        out[count[(num / power) & radixmod] - 1] = num;
        count[(num / power) & radixmod]--;
    }

    nums.swap(out);
}

void radixsort(vi& nums)
{
    int m = *max_element(all(nums));
    for (int i = 1; m / i > 0; i *= radix)
    {
        countsort(nums, i);
    }
}