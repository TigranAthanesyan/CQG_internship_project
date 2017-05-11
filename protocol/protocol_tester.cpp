/**
 * @file     protocol_tester.cpp
 * @authors  Tigran Athanesyan, Anna Vardzelyan
 * @version  1.0
 */

#include "protocol_tester.h"
#include <random>

namespace Tester
{

	/// Simple function that returns random number from the range
	int RandomNumber(const int min, const int max)
	{
		std::random_device rd;
		std::mt19937 randomNumber(rd());
		std::uniform_int_distribution<int> range(min, max);
		return range(randomNumber);
	}

	/// Function for initializing the data vector
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

	ValidWordTypeMaker::ValidWordTypeMaker()
		: m_bitSet(std::bitset<TYPE_SIZE>(15)) /// 15 - 000000001111
	{
		resetIndexVector();
	}

	void ValidWordTypeMaker::Reset()
	{
		m_bitSet = std::bitset<TYPE_SIZE>(15);
		m_isConditionPart = m_canBeMoreLess = false;
		m_allowComma = true;
		resetIndexVector();
	}

	WordType ValidWordTypeMaker::GetNextType()
	{
		if (!m_indexVector.size())
			return end;

		int nextIndex = RandomNumber(0, m_indexVector.size() - 1);
		WordType nextType = static_cast<WordType>(m_indexVector[nextIndex]);

		/// Before returning the type we need to change some settings about which type can be next
		switch (nextType)
		{
		case all:   /// True types: all, quantityOf, close, data
			m_bitSet[all] = m_bitSet[quantityOf] = m_bitSet[close] = m_bitSet[data] = false;
			m_bitSet[that] = m_bitSet[end] = true;
			break;
		case quantityOf:
			if (m_isConditionPart)   /// True types: quantityOf, data
			{
				m_bitSet[quantityOf] = false;
				m_canBeMoreLess = true;
			}
			else   /// True types: all, quantityOf, close, data
			{
				m_bitSet[all] = m_bitSet[quantityOf] = m_bitSet[close] = m_allowComma = false;
			}
			break;
		case close:  /// True types: all, quantityOf, close, data
			m_bitSet[all] = m_bitSet[quantityOf] = m_bitSet[close] = m_bitSet[data] = false;
			m_bitSet[end] = true;
			break;
		case data:
			if (m_isConditionPart)   /// True types: data, quantityOf
			{
				m_bitSet[data] = m_bitSet[quantityOf] = false;
				if (m_canBeMoreLess)
				{
					m_bitSet[is_isNot] = m_bitSet[isMoreThan_isLessThan] = true;
					m_canBeMoreLess = false;
				}
				else
					m_bitSet[is_isNot] = m_bitSet[isDefined_isUndefined] = true;
			}
			else   /// True types: all, quantityOf, close, data
			{
				m_bitSet[all] = m_bitSet[quantityOf] = m_bitSet[close] = m_bitSet[data] = false;
				m_bitSet[that] = m_bitSet[end] = true;
				m_bitSet[comma] = m_allowComma;
			}
			break;
		case that:  /// True types: that, comma, end
			m_bitSet[that] = m_bitSet[comma] = m_bitSet[end] = false;
			m_bitSet[data] = m_bitSet[quantityOf] = m_isConditionPart = true;
			break;
		case comma:    /// True types: comma, that, end
			m_bitSet[comma] = m_bitSet[that] = m_bitSet[end] = false;
			m_bitSet[data] = true;
			break;
		case is_isNot:  /// True types: is_isNot, isDefined_isUndefined, isMoreThan_isLessThan
			m_bitSet[is_isNot] = m_bitSet[isDefined_isUndefined] = m_bitSet[isMoreThan_isLessThan] = false;
			m_bitSet[value] = true;
			break;
		case isDefined_isUndefined:   /// True types: is_isNot, isDefined_isUndefined
			m_bitSet[is_isNot] = m_bitSet[isDefined_isUndefined] = false;
			m_bitSet[and_or] = m_bitSet[end] = true;
			break;
		case isMoreThan_isLessThan:   /// True types: is_isNot, isMoreThan_isLessThan
			m_bitSet[is_isNot] = m_bitSet[isMoreThan_isLessThan] = false;
			m_bitSet[value] = true;
			break;
		case and_or:   /// True types: and_or, end
			m_bitSet[and_or] = m_bitSet[end] = false;
			m_bitSet[data] = m_bitSet[quantityOf] = true;
			break;
		case value:   /// True types: value
			m_bitSet[value] = false;
			m_bitSet[and_or] = m_bitSet[end] = true;
			break;
		case end:  /// True types: that, comma, end, and_or
			m_bitSet[that] = m_bitSet[comma] = m_bitSet[end] = m_bitSet[and_or] = false;
			break;
		}
		resetIndexVector();
		return nextType;
	}

	void ValidWordTypeMaker::resetIndexVector()
	{
		m_indexVector.clear();
		for (int i = 0; i < TYPE_SIZE; ++i)
		{
			if (m_bitSet[i])
				m_indexVector.push_back(i);
		}
	}

	std::string RequestGenerator::GenerateRequest()
	{
		std::string request;
		WordType nextType = m_typeMaker.GetNextType(); /// Getting the next type
		ValueType nextValueType = undefined;           /// Will be changed after any data

		while (nextType != end)
		{
			switch (nextType)
			{
			case all:
				request += "all ";
				break;
			case quantityOf:
				request += "quantity of ";
				nextValueType = number; /// Only in this case the following value will be number
				break;
			case close:
				request += "close ";
				break;
			case data:
			{
				std::string data = generateData();
				request += data + ' ';
				/// We need to set the value type, if it is not a quantity of something
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
				request += RandomNumber(0, 1) ? "is " : "is not ";
				break;
			case isDefined_isUndefined:
				request += RandomNumber(0, 1) ? "is defined " : "is undefined ";
				nextValueType = undefined;
				break;
			case isMoreThan_isLessThan:
				request += RandomNumber(0, 1) ? "is more than " : "is less than ";
				break;
			case and_or:
				request += RandomNumber(0, 1) ? "and " : "or ";
				break;
			case value:
				switch (nextValueType)
				{
				case name:
					request += generateWord(RandomNumber(2, 16), true) + ' ';
					break;
				case mr_mrs:
					request += RandomNumber(0, 1) ? "mr " : "mrs ";
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
					request += generateWord(RandomNumber(4, 16)) + ' ';
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

	std::string RequestGenerator::generateData() const
	{
		int dataIndex = RandomNumber(0, m_dataVector.size() - 1);
		return m_dataVector[dataIndex];
	}

	std::string RequestGenerator::generatePhoneNumber(const unsigned numberQuantity) const
	{
		int areaCode = RandomNumber(100, 999);
		int phoneNumber = RandomNumber(pow(10, numberQuantity - 1), pow(10, numberQuantity) - 1);
		return "+" + std::to_string(areaCode) + " " + std::to_string(phoneNumber);
	}

	std::string RequestGenerator::generateNumber(const int min, const int max) const
	{
		return std::to_string(RandomNumber(min, max));
	}

	std::string RequestGenerator::generateMail() const
	{
		std::string mail;
		const int beforeAtSymbolMaxSize = 20;
		const int beforeDotSymbolMaxSize = 10;

		mail += generateWord(RandomNumber(3, beforeAtSymbolMaxSize)) + '@';
		mail += generateWord(RandomNumber(3, beforeDotSymbolMaxSize)) + '.';
		mail += generateWord(RandomNumber(2, 3));

		return mail;
	}

	std::string RequestGenerator::generateWord(const size_t size, const bool startWithCapital) const // returns random word
	{
		std::string word;
		for (int i = 0; i < size; ++i)
			word += RandomNumber(0, 25) + 'a';
		if (startWithCapital)
			word[0] = toupper(word[0]);
		return word;
	}

	std::vector<std::string> RequestGenerator::m_dataVector = MakeDataVector();

}