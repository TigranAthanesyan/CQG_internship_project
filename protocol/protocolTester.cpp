#include "protocolTester.h"

//m_dataSet
//m_keywordVector

namespace
{
	std::vector<std::string> MakeDataVector()
	{
		std::vector<std::string> dataVector;
		dataVector.push_back("last name");
		dataVector.push_back("first name");
		dataVector.push_back("mr. / mrs.");
		dataVector.push_back("acct. name");
		dataVector.push_back("phone");
		dataVector.push_back("home e-mail");
		dataVector.push_back("type");
		dataVector.push_back("industry segment");
		dataVector.push_back("work e-mail");
		dataVector.push_back("address");
		dataVector.push_back("city");
		dataVector.push_back("state");
		dataVector.push_back("postal code");
		dataVector.push_back("country");
		dataVector.push_back("last contacted date");
		dataVector.push_back("referral type");
		dataVector.push_back("referral detail");
		dataVector.push_back("sales rep.");
		dataVector.push_back("sales rep. login");
		dataVector.push_back("company office");
		dataVector.push_back("trs admin");
		dataVector.push_back("auth to trade");
		dataVector.push_back("job title");
		dataVector.push_back("contact id");
		return dataVector;
	}

	std::vector<std::string> MakeKeywordVectorFirstPart()
	{
		//  pushing keywords
		std::vector<std::string> keywordVector;
		keywordVector.push_back("all");
		keywordVector.push_back("that");
		keywordVector.push_back("quantity of");
		keywordVector.push_back(",");
		keywordVector.push_back("close");
		return keywordVector;
	}

	std::vector<std::string> MakeKeywordVectorConditionPart()
	{
		//  pushing condition keywords
		std::vector<std::string> conditionKeywordVector;
		conditionKeywordVector.push_back("is");
		conditionKeywordVector.push_back("or");
		conditionKeywordVector.push_back("and");
		conditionKeywordVector.push_back("is not");
		conditionKeywordVector.push_back("is more than");
		conditionKeywordVector.push_back("is less than");
		conditionKeywordVector.push_back("is defined");
		conditionKeywordVector.push_back("is undefined");

		// can be in condition part too
		conditionKeywordVector.push_back("quantity of");
		conditionKeywordVector.push_back("that");
		conditionKeywordVector.push_back(",");
		return conditionKeywordVector;
	}
}

////////--TODO--////////
std::string RequestGenerator::GenerateRequest() const
{
	std::srand(unsigned(std::time(0)));
	std::vector<std::string> keywordsVector;

	bool allIsValid = true, isConditionPart = false, conditionIsValid = false;
	bool dataIsValid = false, quantityOfIsValid = true, allowComma = true;
	bool thatIsValid = false, commaIsValid = false, valueIsValid = false;
	bool andOrIsValid = false, moreLessIsValid = false, closeIsValid = true;
	bool mustBeAData = false;

	// first part

	const int firstPartSize = m_data.size() - 1;
	for (int i = 0; i < firstPartSize - 1; ++i)
	{
		int chooseVectorOfWords = std::rand() % 2;

		if (chooseVectorOfWords == 1 || (mustBeAData && !isConditionPart)) // we are in m_data
		{
			keywordsVector.push_back(m_data[std::rand() % m_data.size()]);

			// choose keyword "that" or ','
		}
	}



	return joinWords(keywordsVector);
}

std::string RequestGenerator::joinWords(const std::vector<std::string>& keywordsVector) const
{
	std::string request;
	for (unsigned int i = 0; i < keywordsVector.size(); ++i)
		request += (keywordsVector[i] + ' ');

	return request;
}

std::string RequestGenerator::generatePhoneNumber() const
{
	std::string phoneNumber;
	const int maxPhoneNumberSize = 20;
	srand(static_cast<int>(time(NULL)));

	// first symbol can be only '+' or digit
	int firstSymbol = std::rand() % 2;
	//phoneNumber += (firstSymbol * '+' + (1 - firstSymbol)*(rand() % 10 + '0'));
	//  += 1 * '+'
	//  += 1 * 
	phoneNumber += (firstSymbol ? rand() % 10 + '0' : '+');
	//next symbol can be only ' ' or digit
	bool spaceIsValid = true;
	int thisNumberSize = std::rand() % maxPhoneNumberSize + 1;
	for (unsigned int i = 0; i < thisNumberSize; ++i)
	{
		int nextSymbol = std::rand() % 2;
		if (nextSymbol == 0 && spaceIsValid)
		{
			phoneNumber += ' ';
			spaceIsValid = false; // space isn't valid after space
		}
		else
		{
			phoneNumber += (std::rand() % 10 + '0');
			spaceIsValid = true;
		}
	}

	return phoneNumber;
}

std::string RequestGenerator::generateGlobalPhoneNumber() const
{
	std::string phoneNumber = "+";
	srand(static_cast<int>(time(NULL)));
	for (int i = 0; i < 3; ++i)
		phoneNumber += '0' + rand() % 10;
	phoneNumber += ' ';
	const unsigned numberMinQuantity = 8;
	unsigned numbersQuantity = rand() % numberMinQuantity + numberMinQuantity;
	while (numbersQuantity --> 0)
		phoneNumber += '0' + rand() % 10;
	return phoneNumber;
}

std::string RequestGenerator::generateMail() const
{
	std::string mail;
	const int beforeAtSymbolMaxSize = 20;
	const int beforeDotSymbolMaxSize = 10;
	const int afterDotSymbolMaxSize = 5;

	srand(unsigned(std::time(0)));

	std::string beforeAtSymbols;
	for (int i = 0; i < 26; ++i)
		beforeAtSymbols += 'a' + i;

	for (int i = 0; i < 10; ++i)
		beforeAtSymbols += '0' + i;

	beforeAtSymbols += "-_.";
	int dummysSize = 3;

	// before at can be digits, alpha and dummy symbols
	bool dummyIsValid = false;
	int thisMailBeforeAtSymbolSize = rand() % beforeAtSymbolMaxSize + 1;
	for (int i = 0; i < thisMailBeforeAtSymbolSize; ++i)
	{
		int nextSymbolIndex = rand() % beforeAtSymbols.size();

		if ((nextSymbolIndex > beforeAtSymbols.size() - dummysSize - 1) && dummyIsValid)
		{
			mail += beforeAtSymbols[nextSymbolIndex];
			dummyIsValid = false; // dummy isn't valid after dummy
		}
		else
		{
			mail += beforeAtSymbols[rand() % 26];
			dummyIsValid = true;
		}
	}

	mail += '@';

	// after at can be only alpha symbols
	int thisMailBeforeDotSymbolSize = rand() % beforeDotSymbolMaxSize + 1;
	for (int i = 0; i < thisMailBeforeDotSymbolSize; ++i)
		mail += ('a' + rand() % 26);

	mail += '.';

	// after dot can be only alpha symbols
	int thisMailAfterDotSymbolSize = rand() % afterDotSymbolMaxSize + 1;
	for (int i = 0; i < thisMailAfterDotSymbolSize; ++i)
		mail += ('a' + rand() % 26);

	return mail;
}

std::vector<std::string> RequestGenerator::m_data = ::MakeDataVector();
std::vector<std::string> RequestGenerator::m_firstPartKeywordsVector = ::MakeKeywordVectorFirstPart();
std::vector<std::string> RequestGenerator::m_conditionPartKeywordsVector = ::MakeKeywordVectorConditionPart();