#include "protocol.h"
#include "protocolTester.h"

int main()
{
	RequestGenerator requestGenerator;
	Request request;
	std::string requestText;

	while (!request.Close())
	{
		requestText = requestGenerator.GenerateRequest();
		request.SetText(requestText);
		std::cout << requestText << std::endl << std::endl << request << std::endl;
		std::cout << "Request " << (request.IsCorrect() ? "is " : "is not ") << "correct" << std::endl << std::endl;
		std::cin.get();
	}

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
}


/*Request request;
request.Description();
request.DataDescription();
std::cin >> request;
while (!request.Close())
{
std::cout << std::endl << request << std::endl << std::endl;
std::cout << "Request " << (request.IsCorrect() ? "is " : "is not ") << "correct" << std::endl << std::endl;
std::cin >> request;
referral detail , country , acct. name that city is undefined and home e-mail is value and quantity of first name is less than value
}*/