// Minified fast output writer from https://github.com/Matistjati/CP-templates
#include <unistd.h>
alignas(64) static char kD2[200],kD4[10000][4];
struct FastOutput{static const int BUF=1<<15;char b[BUF+64],*p=b;
FastOutput(){char*q=kD2;for(int a=0;a<10;a++)for(int c=0;c<10;c++){*q++=char('0'+a);*q++=char('0'+c);}q=kD4[0];for(int a=0;a<100;a++)for(int c=0;c<100;c++){*q++=kD2[2*a];*q++=kD2[2*a+1];*q++=kD2[2*c];*q++=kD2[2*c+1];}}
static uint32_t d4(uint32_t i){uint32_t v;memcpy(&v,kD4[i],4);return v;}
static uint64_t blk8(uint32_t x){return (uint64_t)d4(x/10000)|(uint64_t)d4(x%10000)<<32;}
static char*emit8(char*p,uint32_t x){uint64_t v=blk8(x);unsigned s=(unsigned)__builtin_ctzll((v^0x3030303030303030ULL)|1ULL<<56)&56;uint64_t w=v>>s;memcpy(p,&w,8);return p+8-(s>>3);}
static char*emit32(char*p,uint32_t x){if(x<100000000u)return emit8(p,x);uint32_t h=x/100000000u;uint64_t l=blk8(x-h*100000000u);if(h>=10){memcpy(p,kD2+h*2,2);memcpy(p+2,&l,8);return p+10;}*p=char('0'+h);memcpy(p+1,&l,8);return p+9;}
static char*emit64(char*p,uint64_t x){if(x<100000000ull)return emit8(p,(uint32_t)x);if(x<10000000000000000ull){uint64_t l=blk8((uint32_t)(x%100000000ull));p=emit8(p,(uint32_t)(x/100000000ull));memcpy(p,&l,8);return p+8;}uint64_t r=x%10000000000000000ull,m=blk8((uint32_t)(r/100000000ull)),l=blk8((uint32_t)(r%100000000ull));p=emit8(p,(uint32_t)(x/10000000000000000ull));memcpy(p,&m,8);memcpy(p+8,&l,8);return p+16;}
template<class T>static char*ev(char*p,T x){using U=std::make_unsigned_t<T>;U u=(U)x;if constexpr(std::is_signed_v<T>){U m=(U)(x>>(sizeof(T)*8-1));u=(U)((u^m)-m);*p='-';p+=m&1;}if constexpr(sizeof(U)<=4)return emit32(p,(uint32_t)u);else return emit64(p,(uint64_t)u);}
static void raw(const char*q,size_t n){while(n){ssize_t k=::write(1,q,n);if(k<=0)return;q+=k;n-=(size_t)k;}}
void flush(){raw(b,(size_t)(p-b));p=b;}
~FastOutput(){flush();}
void need(size_t n){if(p+n>b+BUF)flush();}
template<class T,class=std::enable_if_t<std::is_integral_v<T>>>FastOutput&operator<<(T x){need(24);p=ev(p,x);return *this;}
FastOutput&operator<<(bool v){need(1);*p++=char('0'+v);return *this;}
FastOutput&operator<<(char c){need(1);*p++=c;return *this;}
FastOutput&put(const char*s,size_t n){if(n>(size_t)BUF){flush();raw(s,n);return *this;}need(n);memcpy(p,s,n);p+=n;return *this;}
FastOutput&operator<<(const char*s){return put(s,strlen(s));}
FastOutput&operator<<(std::string_view s){return put(s.data(),s.size());}
template<class T>void print(const std::vector<T>&v,char sep=' ',char end='\n'){size_t i=0,n=v.size();while(i<n){size_t f=(size_t)(b+BUF-p)/24;if(!f){flush();continue;}size_t j=i+(f<n-i?f:n-i);char*q=p;for(;i<j;i++){q=ev(q,v[i]);*q++=sep;}p=q;}if(n){if(end)p[-1]=end;else p--;}else if(end){need(1);*p++=end;}}
}printer;
#define cout printer
