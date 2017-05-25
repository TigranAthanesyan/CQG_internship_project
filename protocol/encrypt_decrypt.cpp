#include "encrypt_decrypt.h"
#include <sstream>
enum State
{
	none,
	positive,
	negative
};

std::string Encrypt(const std::vector<TypedWord>& phrases, const std::set<std::string>& dataSet)
{
	std::string encryptedRequest;
	for (int i = 0; i < phrases.size(); ++i)
	{
		switch (phrases[i].type)
		{
		case all:
			encryptedRequest += std::to_string(all) + ',' + std::to_string(none);
			break;
		case quantityOf:
			encryptedRequest += std::to_string(quantityOf) + ',' + std::to_string(none);
			break;
		case close:
			encryptedRequest += std::to_string(close) + ',' + std::to_string(none);
			break;
		case that:
			encryptedRequest += std::to_string(that) + ',' + std::to_string(none);
			break;
		case comma:
			encryptedRequest += std::to_string(comma) + ',' + std::to_string(none);
			break;
		case is_isNot:
			encryptedRequest += std::to_string(is_isNot) + ',' + ((phrases[i].word == "is") ? std::to_string(positive) : std::to_string(negative));
			break;
		case isDefined_isUndefined:
			encryptedRequest += std::to_string(isDefined_isUndefined) + ',' + ((phrases[i].word == "is defined") ? std::to_string(positive) : std::to_string(negative));
			break;
		case isMoreThan_isLessThan:
			encryptedRequest += std::to_string(isMoreThan_isLessThan) + ',' + ((phrases[i].word == "is more than") ? std::to_string(positive) : std::to_string(negative));
			break;
		case and_or:
			encryptedRequest += std::to_string(and_or) + ',' + ((phrases[i].word == "and") ? std::to_string(positive) : std::to_string(negative));
			break;
		case data:
			encryptedRequest += std::to_string(data) + ',';
			int indicator;
			indicator = 0;
			for (auto it = dataSet.begin(); it != dataSet.end(); ++it, ++indicator)
			{
				if (phrases[i].word == *it)
				{
					encryptedRequest += std::to_string(indicator);
					break;
				}
			}
			break;
		case value:
			encryptedRequest += std::to_string(value) + ',' + phrases[i].word;
			break;
		}

		if (i < phrases.size() - 1)
			encryptedRequest += ',';
	}

	return std::move(encryptedRequest);
}

std::vector<TypedWord> Decrypt(const std::string& encryptedRequest, const std::set<std::string>& dataSet)
{
	std::vector<TypedWord> decryptedRequest;
	std::istringstream iss(encryptedRequest);
	std::string valueWord;
	int type, state;
	char dummy;
	while (iss >> type)
	{

		iss >> dummy;
		if (type != value)
			iss >> state;
		else
			 getline(iss, valueWord, dummy);

		switch (type)
		{
		case all:
			decryptedRequest.push_back(TypedWord("all", all));
			break;
		case quantityOf:
			decryptedRequest.push_back(TypedWord("quantity of", quantityOf));
			break;
		case close:
			decryptedRequest.push_back(TypedWord("close", close));
			break;
		case that:
			decryptedRequest.push_back(TypedWord("that", that));
			break;
		case comma:
			decryptedRequest.push_back(TypedWord(",", comma));
			break;
		case is_isNot:
			decryptedRequest.push_back(TypedWord((state == positive ? "is" : "is not"), is_isNot));
			break;
		case isDefined_isUndefined:
			decryptedRequest.push_back(TypedWord((state == positive ? "is defined" : "is undefined"), isDefined_isUndefined));
			break;
		case isMoreThan_isLessThan:
			decryptedRequest.push_back(TypedWord((state == positive ? "is more than" : "is less than"), and_or));
			break;
		case and_or:
			decryptedRequest.push_back(TypedWord((state == positive ? "and" : "or"), and_or));
			break;
		case data:
			int indicator;
			indicator = 0;
			for (auto it = dataSet.begin(); it != dataSet.end(); ++it, ++indicator)
			{
				if (indicator == state)
				{
					decryptedRequest.push_back(TypedWord(*it, data));
					break;
				}
			}
			break;
		case value:
			decryptedRequest.push_back(TypedWord(valueWord, value));
			break;
		}

		if (!(iss >> dummy))
			break;
	}

	return std::move(decryptedRequest);
}