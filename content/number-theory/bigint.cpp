#if _LOCAL
#include <__msvc_int128.hpp>
typedef _Signed128 big_signed;
typedef _Unsigned128 big_unsigned;
#else
typedef __int128_t big_signed;
typedef __uint128_t big_unsigned;
#endif
