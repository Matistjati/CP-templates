// Minified fast input reader from https://github.com/Matistjati/CP-templates
#pragma GCC target("avx2")
#include <sys/mman.h>
#include <sys/stat.h>
#include <charconv>
#include <immintrin.h>
struct FioShuf{unsigned char t[2][17][16];constexpr FioShuf():t{}{for(int g=0;g<2;g++)for(int l=0;l<=16;l++){int o=16-(l-g);for(int k=0;k<16;k++)t[g][l][k]=l>=g&&k>=o?(unsigned char)(k-o+g):0x80;}}};
static constexpr FioShuf fioSH{};
struct FastInput{const char*base,*p,*end;uint64_t mask;bool ok=true;
static uint64_t wmask(const char*q){__m256i s=_mm256_set1_epi8(' ');uint32_t a=_mm256_movemask_epi8(_mm256_cmpeq_epi8(s,_mm256_max_epu8(s,_mm256_loadu_si256((const __m256i*)q)))),b=_mm256_movemask_epi8(_mm256_cmpeq_epi8(s,_mm256_max_epu8(s,_mm256_loadu_si256((const __m256i*)(q+32)))));return a|(uint64_t)b<<32;}
FastInput(){struct stat st;fstat(0,&st);size_t z=st.st_size,r=(z+4095)&~4095UL;char*q=(char*)mmap(0,r+4096,PROT_READ,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);mmap(q,z,PROT_READ,MAP_PRIVATE|MAP_FIXED|MAP_POPULATE,0,0);base=p=q;end=q+z;mask=wmask(q);}
static uint64_t pack16(__m128i x){x=_mm_and_si128(x,_mm_set1_epi8(0x0F));x=_mm_maddubs_epi16(x,_mm_set1_epi16(0x010A));x=_mm_madd_epi16(x,_mm_set1_epi32(0x00010064));x=_mm_packus_epi32(x,x);x=_mm_madd_epi16(x,_mm_set1_epi32(0x00012710));return (uint64_t)(uint32_t)_mm_cvtsi128_si32(x)*100000000ULL+(uint32_t)_mm_extract_epi32(x,1);}
const char*token(long&n){for(;;){if(p>=end){ok=false;n=0;return p;}while(!mask){base+=64;mask=wmask(base);}const char*d=base+__builtin_ctzll(mask),*s=p;mask&=mask-1;p=d+1;if(d!=s){n=d-s;return s;}}}
template<class T>static T num(const char*s,long n){bool g=false;if constexpr(std::is_signed_v<T>)g=*s=='-';std::make_unsigned_t<T> u;if(__builtin_expect(n>16,0)){long k=n-g-16;uint64_t pre=0;for(long i=0;i<k;i++)pre=pre*10+(unsigned char)s[g+i]-'0';u=pre*10000000000000000ULL+pack16(_mm_loadu_si128((const __m128i*)(s+n-16)));}else u=pack16(_mm_shuffle_epi8(_mm_loadu_si128((const __m128i*)s),_mm_loadu_si128((const __m128i*)fioSH.t[g][n])));return g?T(0)-T(u):T(u);}
explicit operator bool()const{return ok;}
template<class T,class=std::enable_if_t<std::is_integral_v<T>>>FastInput&operator>>(T&x){long n;const char*s=token(n);x=num<T>(s,n);return *this;}
template<class T>void read(T*a,long c){const char*b=base,*q=p;uint64_t m=mask;for(;c;c--){const char*s=q,*d;for(;;){while(!m){b+=64;m=wmask(b);}d=b+__builtin_ctzll(m);m&=m-1;if(d!=s)break;s=d+1;}*a++=num<T>(s,d-s);q=d+1;}base=b;p=q;mask=m;}
template<class T>void read(std::vector<T>&v,long c=-1){if(c<0)c=v.size();if((size_t)c>v.size())v.resize(c);read(v.data(),c);}
template<class T>FastInput&operator>>(std::vector<T>&v){read(v);return *this;}
FastInput&operator>>(std::string&s){long n;const char*t=token(n);s.assign(t,n);return *this;}
FastInput&operator>>(std::string_view&s){long n;const char*t=token(n);s=std::string_view(t,n);return *this;}
FastInput&operator>>(char*s){long n;const char*t=token(n);memcpy(s,t,n);s[n]=0;return *this;}
FastInput&operator>>(char&c){long n;c=*token(n);return *this;}
FastInput&operator>>(double&x){long n;const char*t=token(n);std::from_chars(t,t+n,x);return *this;}
}scanner;
#define cin scanner
