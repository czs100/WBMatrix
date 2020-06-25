#include "matriz_inversa.h"
#include "stdint.h"
#ifdef __GNUC__
#include <x86intrin.h>
#endif
#ifdef _MSC_VER
#include <intrin.h>
#endif
#pragma intrinsic(__rdtsc)

//Repeat test times and calculate on average for accuracy
#define TEST 100000

//CPU cycles set start;
uint64_t start_rdtsc()
{
	return __rdtsc();
}

//CPU cycles set end;
uint64_t end_rdtsc()
{
    return __rdtsc();
}

double a8[8][8]={
{0,1,0,1,0,0,0,1},
{1,1,0,0,1,0,1,1},
{0,1,1,1,1,0,1,0},
{1,1,0,1,0,1,0,0},
{1,0,0,0,1,0,0,1},
{0,1,0,0,1,0,1,1},
{1,1,1,0,1,0,1,1},
{1,1,0,0,0,0,0,1}
};
double a16[16][16]={
{0,1,0,1,0,1,0,0,0,0,0,0,0,1,1,1},
{1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1},
{0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0},
{1,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0},
{1,1,0,1,0,1,0,1,0,0,0,0,1,1,1,0},
{1,1,1,0,1,0,1,0,1,0,1,0,1,0,0,0},
{1,1,1,0,0,0,1,1,0,0,0,1,0,1,0,0},
{0,0,0,0,0,0,1,1,1,1,0,1,0,0,0,0},
{0,1,0,0,1,0,1,0,1,0,1,0,0,0,0,1},
{0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1},
{0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1},
{0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0},
{0,0,1,0,1,1,0,0,0,0,0,1,0,1,0,1},
{0,0,0,0,1,0,1,1,1,1,0,1,0,0,1,0},
{0,0,1,0,0,1,0,1,1,1,0,0,1,1,0,1},
{1,0,0,1,1,1,0,1,1,1,0,0,0,0,1,1}
};
double a32[32][32]={
{0,0,1,0,0,1,0,0,1,0,1,1,0,0,0,1,0,0,1,1,1,1,1,0,1,0,0,0,1,0,1,0},
{0,0,1,0,0,0,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
{0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,0,1,0,0,1,0,1,0,1,1,1,1,0,1,1,1,1},
{0,0,0,0,1,1,1,0,0,1,0,1,0,1,1,0,1,0,1,1,1,1,1,0,0,0,1,0,1,0,1,1},
{1,1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,1,0,1,1,1,0,1,1,0,1},
{0,0,0,1,1,0,0,1,0,1,0,1,0,0,1,1,0,0,0,0,1,0,0,0,1,1,0,1,0,0,0,1},
{1,0,0,0,1,0,1,1,1,1,1,0,1,1,0,1,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,1},
{1,0,0,1,1,0,0,0,1,0,0,0,1,1,1,0,1,1,1,1,1,0,1,0,0,1,1,0,1,0,1,0},
{1,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,1,0,0,1,1,0,0,0,1,0,1,0},
{1,0,1,1,0,0,0,0,0,0,1,0,0,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,1,0,1,0},
{1,0,0,1,0,1,1,1,0,1,1,0,1,1,1,1,1,1,1,0,0,1,0,1,1,0,0,0,0,0,1,0},
{1,1,0,1,1,1,0,0,0,0,0,0,0,1,1,0,1,1,1,1,0,1,0,0,1,1,0,1,1,1,1,0},
{1,1,0,1,1,1,1,0,1,0,1,1,0,1,0,1,0,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1},
{1,1,1,1,0,0,1,0,1,0,0,1,0,0,1,0,1,0,1,1,0,1,1,1,0,0,0,0,1,1,1,1},
{1,1,1,1,0,0,1,0,0,0,1,1,0,1,1,0,0,0,1,0,0,1,1,0,1,0,1,1,1,0,1,0},
{0,0,1,1,1,1,1,1,1,0,1,1,0,0,0,0,1,0,1,1,1,1,0,1,1,0,0,1,1,0,1,1},
{1,1,0,1,0,0,0,0,0,1,0,1,0,0,1,1,1,0,0,0,1,1,0,1,0,1,0,0,1,1,0,1},
{0,1,0,1,0,0,1,0,0,0,0,0,0,1,1,1,1,0,1,0,1,0,0,1,0,0,1,0,1,1,0,0},
{0,1,1,1,0,1,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,1,0,1,0,1,0,0,0,1,0,0},
{1,0,1,0,0,1,1,0,1,0,0,1,0,1,1,1,1,0,1,1,1,1,1,1,0,1,0,1,1,0,0,0},
{0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,1,0,1,1,0},
{0,0,0,0,1,0,1,0,0,1,0,1,0,1,0,1,0,0,1,1,0,0,1,0,1,0,1,1,1,0,0,1},
{0,1,1,0,1,0,0,1,0,0,0,0,1,0,1,0,1,0,0,1,1,0,1,0,1,1,1,1,0,1,0,1},
{1,0,1,1,0,1,1,1,1,1,0,1,1,1,0,1,0,0,1,0,1,1,1,0,1,0,1,1,1,0,0,0},
{0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,1,1,0,1,0,0,1,1,0,1,1,0,1,1,0,1,1},
{0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,1,1,1,0,1},
{1,0,1,0,0,0,1,0,1,0,1,1,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0},
{0,0,1,1,0,0,0,0,0,0,1,0,1,1,0,1,0,0,1,1,1,0,0,0,1,0,1,0,0,0,0,1},
{0,0,0,1,0,0,1,1,0,0,1,0,1,0,1,0,0,0,1,1,0,1,0,1,1,1,1,0,1,0,1,1},
{1,1,0,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,0,1,0,1,0,1,0,1,1,0,0,1,0,1},
{0,1,1,1,0,1,0,1,0,0,0,1,1,0,0,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1},
{1,1,1,0,0,1,1,0,0,1,0,0,0,0,0,0,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0}
};
double a64[64][64]={
{0,0,0,0,1,0,1,1,1,0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,1,1,0,1,1,0,1,0,1,1,0,0,0,0,1,1,1,0,1,1,1,0,1,1,0,1,0,0,1,0,0,0,1,1,1,0,1,0,1,0},
{0,0,1,1,0,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,0,0,1,0,0,1,0,1,1,1,1,0,1,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,1,1,0,0,0,1,1,1,0},
{0,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,1,0,0,0,1,0,1,1,1,1,0,0,0,1,0,1,0,1,1,1,1,0,1,0,0,0,0,0,1,1,0,1,1,1,1,0,0,0,1,0,0,0,1,1,0},
{1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,1,1,0,1,1,0,0,0,1,0,1,1,0,1,0,0,0,0,1,0,1,0,1,0,0,1},
{1,1,0,0,1,1,1,0,1,1,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,1,1,0,1,1,1,1,0,1,0,1,0,0,0,1,1,1,1,0},
{0,0,0,0,1,0,1,1,1,1,0,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,0,0,1,1,1,1,0,0,0,0,1,0,1,0,1,1,1,0,0,0,1,1,1,0,1,0,1,1,1,1,0,0,1,0,0},
{0,0,0,1,1,1,0,1,0,1,0,0,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,1,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1,1,0,0,1,1,0,0,0},
{0,1,1,1,1,0,0,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1},
{0,0,0,0,1,0,1,1,0,1,1,1,0,1,1,0,1,0,0,1,0,0,1,1,1,1,1,0,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,0,0,1,1,0,0,1,1,1,1,0,1,0,0,1,1,1,0,1,0},
{1,0,1,0,0,1,1,0,1,0,1,1,1,0,1,0,1,0,0,0,0,1,0,1,0,1,0,1,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,1,1,0,1,0,1,1,0,1,0,0,0,1},
{1,1,0,0,0,1,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,1,0,1,0,1,0,0,1,1,1,1,1,0,0,0,0,1,0,1,1,0,1,1,1,1,1,1},
{0,0,1,1,1,0,0,0,1,0,0,0,0,1,1,0,1,0,0,1,1,1,0,1,0,1,0,1,1,0,1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,0,0,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,1,0},
{0,0,0,0,0,1,0,0,1,1,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,1,1,0,0,0,1,1,1,0,0,1,1,0,0,0,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,1,1,0,0,0,0,1},
{0,0,0,0,1,0,1,1,0,1,1,1,1,0,0,0,1,1,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,1,1,0,0,1,1,0,0,1,0,1,1,0,0,1,0,1,1,0,0},
{0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,1,0,0,1},
{0,0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,1,0,1,0,1,1,0,1,0,0,0,1,1,1,1,1,0,0,1,0,1,0,0,0,0,1,1,1,0,1,1,1,0,0,0,1},
{1,1,0,0,0,0,0,1,1,0,0,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,1,1,1,1,0,1,1,1,1,1,1,0,1,0,1,0,0,0,0,0,1,0,0,1,0,0,0,1,1,0,1,1,1,0,1,0},
{0,1,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,0,1,1,0,0,0,1,0,1,1,0,0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,0,1,0,1,1,1,0,1,1,1,1,1,1,1,0},
{1,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,0,0,1,0,0,0,1,0,1,0,1,1,0,1,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,1,1,0,1,0,0,0,1,0,0,1,1,1,0,1,0,1,1,0},
{0,0,1,0,0,0,0,1,0,1,0,0,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,0,0,1,0,1,1,0,1,1,0,0,1,0,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1,1,0,0,1,0,0,1,1,1},
{0,0,1,1,1,1,1,1,0,0,0,1,1,0,1,0,1,1,1,1,0,0,1,1,0,0,1,0,1,0,1,1,0,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,1,0,1,1,0,0,1,1,1,1,0,0,0,1,0},
{1,1,0,1,0,1,1,0,0,1,0,1,0,1,1,0,0,1,0,1,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,0,0,1,0,0,1,1,1,1,0,1,1,0,0,1,1},
{0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,1,1,1,0,1,0,1,1,1,0,0,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,0,1,0,1,1,1,1,1,1,0,0,0,1,0,0,1,1,1,0,1,0,1,0},
{0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,0,1,1,1,1,0,0,0,0,1,0,1,1,0,1,1,1,0,0,0,0,0,0,0,1,1,0,1,1,1,1,0,1,0,1,1,1,0,1,1,0,1,0},
{1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,1,0,1,1,0,1,0,0,1,0,1,0,1,0,0,1,1,1,0,0,1,1,0,0,1,0,1,0,0,1,1},
{0,0,1,0,1,1,0,1,0,0,1,0,1,0,0,1,0,0,1,1,1,0,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0},
{1,1,0,0,1,1,1,1,1,0,0,1,0,1,1,0,1,1,1,1,0,0,1,1,0,0,1,0,0,0,1,1,0,0,0,0,0,1,1,0,1,0,1,1,1,1,1,0,0,1,0,1,1,1,1,1,1,1,1,0,1,0,1,1},
{1,0,1,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,0,0,0,0,1,0,1,1,0,1,1,1,0,0,1,1,0,0,1,1,0,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1},
{0,0,0,0,0,1,1,0,1,0,0,0,1,1,1,1,1,0,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,1,0,0,1,0,1,0,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,0,1},
{0,0,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,1,1,1,1,0,1,1,1,1,0,1,0,0,0,1,1,1,0,1,1,0,1,0,1,1,1,0,1,1,1,0,1,0,0,1,1,1},
{0,0,0,0,0,0,0,1,0,1,1,1,1,0,1,1,0,0,0,1,0,1,0,1,0,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,1,0,1,0,1,1,0,1,0,1,0,1,0,0,1,0,0,1,0,0,0,1,0,1},
{0,0,1,1,1,0,1,1,0,0,1,0,0,1,1,1,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,1,0,1,1,1,0,0,1,0,0,0,1,0,1,0,0,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,1,1,1,1,1,0,1,0,0,0,0,0,1,0,0,1,0,1,1,0,0,0,1,1,0,0,1,1,1,1,0,1,0,0,1,0,1,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,0,0,1,1,1,1,0,0,1},
{1,0,0,1,1,0,1,1,0,1,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,1,1,0,1,1,1,1,0,0,0,1,0,1,0,1,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,1,0,0,1,0,1,0,1,1,0,1,0,0,1,1,0,1,0,0,0,1,1,1,1,1,0,1,0,1,1,0,0,0,1,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,1,0,0,1,0,0},
{1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,0,1,0,0,1,0,0,0,0,1,1,1,0,1,0,0,0,1,1,0,0,1,1,0,1,0,0,1,1,1,1,0,1,1,0,0,1,0,1,1,1,1,1},
{0,1,0,1,0,0,1,0,0,1,1,0,1,0,1,0,0,0,1,1,0,1,1,0,1,0,1,1,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,1,0,1,0,1,1,0,1,1,0,1,1,0},
{0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,0,0,1,1,0,0,0,1,0,0,0,1,0,1,0,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,0,1,1,1,0,1,1,0,0,0,0,1,1,0,1,0,0,0,0},
{1,0,0,0,1,0,1,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,1,0,1,0,1,1,0,0,1,0,1,0,1,0,1,1,1,1,0,1,1,0,1,1,0,1,1,0,0,0,1,0,1,1,0,1,1,0,0,1,0},
{0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,1,1,1,0,0,1,0,0,0,1,1,1,0,0,1,1,0,0,0,1,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,0},
{1,1,1,1,0,0,0,0,0,0,1,0,1,0,1,1,0,1,0,1,1,0,0,0,0,1,1,0,0,1,1,1,1,0,0,0,0,1,0,1,0,0,1,0,0,0,0,0,1,1,0,0,1,1,1,1,0,0,0,0,1,1,1,0},
{0,1,1,0,1,1,0,1,1,0,1,0,0,0,1,1,1,0,1,1,1,0,0,0,1,1,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,1,0,0,0,0,1,1,0,1,1,1,1,1,0,0,1,0,0,1,0,1},
{1,1,0,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1,0,1,1,0,1,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,1,0,1,0,0,1,0,1,0,1,1,1,0,1,1},
{0,1,1,1,0,0,0,1,0,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,1,1,1,0,0,0,0,0,0,1,0,0,1,1,0,1,1,0,1,0,0},
{0,0,0,1,0,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,0,0,0,1,0,1,1,0,0,1,0,0,1,0,0,0,1,0,1,1,0,0,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,1,0,0,1,1,0,1},
{0,1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,1,0,1,0,1,1,1,1,1,1,0,0,1,0,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,1,1,0,1,0,0,1,0,0,1,0,0,1,1,1,0},
{0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,0,0,1,1,0,0,0,1,1,1,0,0,0,1,0,1,0,1,1,0,1,0,0,0,1,1,1,1,0,0,1,0,1},
{0,0,1,1,0,0,0,0,1,1,0,0,1,0,1,1,1,1,1,0,1,0,0,0,0,0,0,1,1,0,1,0,0,0,0,0,0,1,0,0,0,0,1,1,1,0,1,0,1,0,0,0,1,1,1,1,1,0,1,1,1,0,1,0},
{0,1,1,0,0,0,0,1,1,1,0,1,0,0,1,1,0,0,1,1,1,0,0,0,0,0,0,1,0,1,1,1,0,0,0,1,0,1,0,1,1,1,0,0,0,0,1,0,0,1,0,0,0,0,1,1,0,1,1,0,1,1,0,0},
{0,1,0,1,0,0,1,0,0,0,0,0,0,0,1,0,0,1,0,1,1,0,1,0,0,1,1,1,0,1,0,1,0,0,0,1,0,1,1,0,0,1,0,1,0,0,1,1,0,1,1,1,1,0,1,0,1,0,1,1,1,0,0,0},
{0,1,1,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1,1,0,0,0,1,1,0,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,0,0,1,0,0,1,0,0,0,0,1,1,0},
{1,0,1,0,1,1,0,0,1,1,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,1,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1,0,1,1,0,1,0,1,0,0,1,0},
{1,0,1,0,0,0,1,1,1,1,0,0,1,0,1,1,0,1,0,0,0,0,0,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,0,1,0,1,1,0,0,1,1,0,0,0,0,1,0,0,1,1,1,1,1,1,0,1,0,0},
{1,1,1,0,0,0,1,0,0,1,0,0,0,0,1,1,1,1,0,0,1,0,1,0,1,0,1,1,0,1,0,0,0,0,0,0,0,1,0,1,1,1,1,0,1,0,1,1,1,0,0,1,1,1,1,1,1,0,1,0,0,0,1,0},
{0,0,1,1,1,1,0,1,1,1,0,1,1,1,1,0,0,1,0,0,0,1,0,1,0,1,1,0,0,1,0,0,1,0,0,0,0,0,1,0,1,1,1,0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0,1,1,0,0,1},
{1,1,0,0,0,1,0,0,1,1,0,1,1,1,1,1,1,0,1,1,0,1,0,0,1,1,1,0,0,1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,0,0,1,1,0,0,0,1,1,0,1,1,0,0,1,1,1,1,1,1},
{1,1,1,0,0,0,1,1,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,0,1,1,0,1,1,0,0,1,0,0,1,1,0,0,0,0,1,1,0,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,0,1,0,1,1},
{1,0,0,1,1,0,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,1,0,1,1,0,0,1,1,1,0,0,1,0,0,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,1,1,1,1,0,0,0,1,1},
{1,0,0,0,1,0,0,1,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,1,1,0,1,1,1,1,0,0,1,1,1,1,0,1,1,1,1,0,1,1,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1},
{0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,0,1,1,1,0,0,1,0,1,0,1,0,1,0,1,1,0,1,0,1},
{0,0,1,0,1,0,1,0,1,1,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,0,0,1,1,1,0,1,0,0,0,0,1,0,0,1,0,1,0,0},
{1,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,1,0,0,1,0,0,0,0,1,0,1,0,1,0,0,0,1,1,0,1,0,1,1,1,0,0,1,0,0,0,0,1,1,1,0,1,0,1,0,0,1,1,0,1,0,1,1},
{1,1,1,0,1,0,0,0,0,0,1,0,1,1,0,1,0,0,0,0,0,0,1,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,0,1,1,0,1,0,0,1,0,1,1,1,0,0,0,1,0,0,1,0,1,1,0,1,1,0},
{1,0,1,1,1,1,0,1,1,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,1,1,0,1,0,0,1,1,0,1,0,0,1,1,0,1,1,1,1,0,0,0,1,0}
};

int main()
{
	uint64_t begin;
    uint64_t end;
    uint64_t ans = 0;
    int i;

    printf("Inverse Matrix Method performance test:\n");
    mmatrix *A8;
    A8 = memmatr(8,8);
    for (int i=0;i<8;i++)
	{
        for (int j=0;j<8;j++)
        {
            A8->M[i][j]=a8[i][j];
        }
    }
    begin = start_rdtsc();
    for (i = 0; i < TEST; i++)
    {
        invermmatrix(A8);
    }
    end = end_rdtsc();
    ans = (end - begin);
    printf("compute 8*8 inverse matrix cost %llu CPU cycles\n", (ans) / TEST);
/*
    mmatrix *A16;
    A16 = memmatr(16,16);
    for (int i=0;i<16;i++)
	{
        for (int j=0;j<16;j++)
        {
            A16->M[i][j]=a16[i][j];
        }
    }
    begin = start_rdtsc();
    for (i = 0; i < TEST; i++)
    {
        invermmatrix(A16);
    }
    end = end_rdtsc();
    ans = (end - begin);
    printf("compute 16*16 inverse matrix cost %llu CPU cycles\n", (ans) / TEST);

    mmatrix *A32;
    A32 = memmatr(32,32);
    for (int i=0;i<32;i++)
	{
        for (int j=0;j<32;j++)
        {
            A32->M[i][j]=a32[i][j];
        }
    }
    begin = start_rdtsc();
    for (i = 0; i < TEST; i++)
    {
        invermmatrix(A32);
    }
    end = end_rdtsc();
    ans = (end - begin);
    printf("compute 32*32 inverse matrix cost %llu CPU cycles\n", (ans) / TEST);
*/    
	return 0;
}