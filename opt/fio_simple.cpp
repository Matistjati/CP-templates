#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
struct FastIO {
    char *p;
    FastIO() {
        struct stat st;
        fstat(0, &st);
        p = (char*)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    }
    // Only handles unsigned!!!
    template<typename T>
    FastIO& operator>> (T& x) {
        while (*p < '0') p++;
        x = *p++ - '0';
        while (*p >= '0') x = x * 10 + *p++ - '0';
        return *this;
    }
} fio;
#define cin fio
