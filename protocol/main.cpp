#include "request.h"
#include "encrypt_decrypt.h"
int main()
{
	Request request;
	request.Description();
	request.DataDescription();
	std::cin >> request;

	std::vector<TypedWord> phrases = request.GetPhrasesVector();
	std::set<std::string> dataSet = request.GetDataSet();

	std::string encrypted = Encrypt(phrases, dataSet);
	std::cout << std::endl << encrypted << std::endl;

	std::vector<TypedWord> decrypted = Decrypt(encrypted, dataSet);
	for (int i = 0; i < decrypted.size(); ++i)
		std::cout << decrypted[i].word << " ";

	std::cout << std::endl;

	/*
	while (!request.Close())
	{
		std::cout << std::endl << request << std::endl << std::endl;
		if (request.IsCorrect())
			std::cout << "Request is correct" << std::endl << std::endl;
		else
			std::cout << "Error: " << request.ErrorText() << std::endl << std::endl;
		std::cin >> request;
	}*/
}


/*

#include "request.h"
#include "protocol_tester.h"

int main()
{
	Tester::RequestGenerator requestGenerator;
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
