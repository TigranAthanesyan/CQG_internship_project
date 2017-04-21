#include "protocolTester.h"

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

	/*std::vector<std::string> MakeKeywordVectorFirstPart()
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
	}*/
}

std::string RequestGenerator::GenerateRequest()
{
	std::string request;
	WordType nextType = m_typeMaker.GetNextType();
	ValueType nextValueType = undefined;

	while (nextType != end)
	{
		switch (nextType)
		{
		case all:
			request += "all ";
			break;
		case quantityOf:
			request += "quantity of ";
			nextValueType = number;
			break;
		case close:
			request += "close ";
			break;
		case data:
		{
			std::string data = generateData();
			request += data + ' ';
			if (nextValueType != number)
			{
				if (data == "last name" || data == "first name" || data == "city" || data == "state" || data == "country")
					nextValueType = name;
				else if (data == "mr. / mrs.")
					nextValueType = mr_mrs;
				else if (data == "phone")
					nextValueType = phone;
				else if (data == "home e-mail" || data == "work e-mail")
					nextValueType = e_mail;
			}
			break;
		}
		case that:
			request += "that ";
			nextValueType = undefined;
			break;
		case comma:
			request += ", ";
			break;
		case is_isNot:
			request += rand() % 2 ? "is " : "is not ";
			break;
		case isDefined_isUndefined:
			request += rand() % 2 ? "is defined " : "is undefined ";
			nextValueType = undefined;
			break;
		case isMoreThan_isLessThan:
			request += rand() % 2 ? "is more than " : "is less than ";
			break;
		case and_or:
			request += rand() % 2 ? "and " : "or ";
			break;
		case value:
			switch (nextValueType)
			{
			case name:
				request += generateWord(rand() % 10 + 2, true) + ' ';
				break;
			case mr_mrs:
				request += rand() % 2 ? "mr " : "mrs ";
				break;
			case phone:
				request += generatePhoneNumber() + ' ';
				break;
			case number:
				request += generateNumber(1, 20) + ' ';
				break;
			case e_mail:
				request += generateMail() + ' ';
				break;
			case undefined:
				request += generateWord(rand() % 10 + 2) + ' ';
				break;
			}
			nextValueType = undefined;
			break;
		}
		nextType = m_typeMaker.GetNextType();
	}
	m_typeMaker.Reset();
	return request;
}

std::string RequestGenerator::joinWords(const std::vector<std::string>& wordsVector) const
{
	std::string request;
	for (unsigned int i = 0; i < wordsVector.size(); ++i)
		request += (wordsVector[i] + ' ');

	return request;
}

std::string RequestGenerator::generateData() const
{
	int dataIndex = rand() % m_dataVector.size();
	return m_dataVector[dataIndex];
}

std::string RequestGenerator::generatePhoneNumber(unsigned numberQuantity) const
{
	std::string phoneNumber = "+";
	for (int i = 0; i < 3; ++i)
		phoneNumber += '0' + rand() % 10;
	phoneNumber += ' ';
	for (int i = 0; i < numberQuantity; ++i)
		phoneNumber += '0' + rand() % 10;
	return phoneNumber;
}

std::string RequestGenerator::generateNumber(int min, int max) const
{
	int number = rand() % (max - min) + min;
	return std::to_string(number);
}

std::string RequestGenerator::generateMail() const
{
	std::string mail;
	const int beforeAtSymbolMaxSize = 20;
	const int beforeDotSymbolMaxSize = 10;
	const int afterDotSymbolMaxSize = 3;

	mail += generateWord(rand() % beforeAtSymbolMaxSize + 1) + '@';
	mail += generateWord(rand() % beforeDotSymbolMaxSize + 1) + '.';
	mail += generateWord(rand() % afterDotSymbolMaxSize + 1);

	return mail;
}

std::string RequestGenerator::generateWord(size_t size, bool startWithCapital) const
{
	std::string word;
	for (int i = 0; i < size; ++i)
		word += rand() % 26 + 'a';
	if (startWithCapital)
		word[0] = toupper(word[0]);
	return word;
}

std::vector<std::string> RequestGenerator::m_dataVector = ::MakeDataVector();