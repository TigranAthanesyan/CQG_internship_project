#include "protocol.h"
#include "protocolTester.h"

int main()
{
  /*Request request;
	request.Description();
	request.DataDescription();
	std::cin >> request;
	while (!request.Close())
	{
	std::cout << std::endl << request << std::endl << std::endl;
	std::cout << "Request " << (request.IsCorrect() ? "is " : "is not ") << "correct" << std::endl << std::endl;
	std::cin >> request;
	}*/

	RequestGenerator r;
	for (int i = 0; i < 20; ++i)
		std::cout << r.GenerateRequest() << std::endl;
	/*
	for (int i = 0; i < 256; ++i)
		std::cout << i << "     " << char(i) << std::endl;*/
}
