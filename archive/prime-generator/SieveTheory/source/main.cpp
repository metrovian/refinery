#include "Sieve.h"
#include "EratosthenesSieve.h"
#include "EulerSieve.h"
#include "AtkinSieve.h"
#include "PritchardSieve.h"
#include "SundaramSieve.h"
#include <windows.h>

void get_time(void (*func)())
{
	LARGE_INTEGER freq;
	LARGE_INTEGER start;
	LARGE_INTEGER end;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);

	func();

	QueryPerformanceCounter(&end);

	printf("Elapsed Time: %lf seconds\n\n", (double)(end.QuadPart - start.QuadPart) / freq.QuadPart);
}

uint64_t max = 1000000;

void test1() { EratosthenesSieve s; s.run(std::to_string(max), "x64/test1.txt"); }
void test2() { EulerSieve s; s.run(std::to_string(max), "x64/test2.txt"); }
void test3() { AtkinSieve s; s.run(std::to_string(max), "x64/test3.txt"); }
void test4() { PritchardSieve s; s.run(std::to_string(max), "x64/test4.txt"); }
void test5() { SundaramSieve s; s.run(std::to_string(max), "x64/test5.txt"); }

int main()
{
	get_time(test1);
	get_time(test2);
	get_time(test3);
	get_time(test4);
	get_time(test5);

	return 0;
}