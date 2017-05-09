#include "protocol.h"
#include "protocolTester.h"

int main()
{
	Request request;
	request.Description();
	request.DataDescription();
	std::cin >> request;
	while (!request.Close())
	{
		std::cout << std::endl << request << std::endl << std::endl;
		if (request.IsCorrect())
			std::cout << "Request is correct" << std::endl << std::endl;
		else
			std::cout << "Error: " << request.ErrorText() << std::endl << std::endl;
		std::cin >> request;
	}
}

/*
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
}
*/