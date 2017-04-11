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
	std::string number = r.generatePhoneNumber();
	std::string g_number = r.generateGlobalPhoneNumber();
	std::string mail = r.generateMail();
	std::cout << number << std::endl << g_number << std::endl << mail << std::endl;
}
