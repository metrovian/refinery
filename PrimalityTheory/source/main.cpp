#include "PrimalityTest.h"
#include "Fermat.h"
#include "Lucas.h"
#include "MillerRabin.h"
#include "SolovayStrassen.h"

int main()
{
	PrimalityTest test;

	test.push_back("Fermat", 5);
	test.push_back("Lucas", 5);
	test.push_back("Miller-Rabin", 5);
	test.push_back("Solovay-Strassen", 5);

	srand(time(NULL));

	for (uint64_t i = 0; i < 100; ++i)
	{
		uint64_t num = rand();

		std::cout << num << (test.is_prime(num) ? " is prime" : " is composite");
		std::cout << std::endl;
	}

	return 0;
}