template<int max_val, typename T>
struct ValArray // optimized vector[val] = {occurences}
{
    vector<T> content;
    vector<int> len, base;
    ValArray(vector<T>& arr) : content(sz(arr)), len(max_val), base(max_val)
    {
        for (auto x : arr) len[x]++;
        base = len;
        len.assign(sz(len), 0);
        base.insert(begin(base), 0);
        repp(i, 1, sz(base)) base[i] += base[i - 1];
        rep(i, sz(arr)) content[base[arr[i]] + len[arr[i]]++] = i;
    }

    span<T> operator[](T val)
    {
        return span<T>(content.begin() + base[val], content.begin() + base[val] + len[val]);
    };
};

