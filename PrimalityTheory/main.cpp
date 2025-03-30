#include "Primality.h"
#include "Fermat.h"
#include "Lucas.h"
#include "MillerRabin.h"
#include "SolovayStrassen.h"

int main()
{
	Primality tests;

	//tests.push_back(Fermat::test(5));
	//tests.push_back(Lucas::test(5));
	//tests.push_back(MillerRabin::test(5));
	tests.push_back(SolovayStrassen::test(5));

	srand(time(NULL));

	for (uint64_t i = 0; i < 100; ++i)
	{
		uint64_t num = rand();

		std::cout << num << (tests.is_prime(num) ? " is prime" : " is composite");
		std::cout << std::endl;
	}

	return 0;
}