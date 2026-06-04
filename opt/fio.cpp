#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
alignas(64) const char digits[201] =
    "0001020304050607080910111213141516171819"
    "2021222324252627282930313233343536373839"
    "4041424344454647484950515253545556575859"
    "6061626364656667686970717273747576777879"
    "8081828384858687888990919293949596979899";
struct FastIO {
    char *p, ob[1 << 20], *op = ob;;
    FastIO() {
        struct stat st;
        fstat(0, &st);
        p = (char*)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    }
    inline unsigned read_uint() {
        while (*p < '0') p++;
        unsigned x = *p++ - '0';
        while (*p >= '0') {
            x = x * 10 + *p++ - '0';
        }
        return x;
    }
    inline void write_uint(unsigned x) {
        if (op - ob >= (1 << 20) - 12) {
            write(1, ob, op - ob);
            op = ob;
        }
        char buf[12];
        int i = 12;
        buf[--i] = '\n';
        while (x >= 100) {
            unsigned const idx = (x % 100) * 2;
            x /= 100;
            buf[--i] = digits[idx + 1];
            buf[--i] = digits[idx];
        }
        if (x >= 10) {
            unsigned const idx = x * 2;
            buf[--i] = digits[idx + 1];
            buf[--i] = digits[idx];
        } else {
            buf[--i] = (char)(x + '0');
        }
        int len = 12 - i;
        memcpy(op, &buf[i], len);
        op += len;
    }
    void flush() {
        write(1, ob, op - ob);
    }
};
