#include <string>
#include <iostream>
#include <set>
#include <vector>
#include <iterator>
#include <algorithm>
#include <ctime>
#include <bitset>

#ifndef PROTOCOLTESTER_H
#define PROTOCOLTESTER_H

#define TYPE_SIZE 12

enum WordType
{
	all,
	quantityOf,
	close,
	data,
	that,
	comma,
	is_isNot,
	isDefined_isUndefined,
	isMoreThan_isLessThan,
	and_or,
	value,
	end
};

enum ValueType
{
	name,
	mr_mrs,
	phone,
	number,
	e_mail,
	undefined
};

class ValidWordTypeMaker
{
public:
	ValidWordTypeMaker() : m_bitSet(std::bitset<TYPE_SIZE>(15))   //  15 - 000000001111     all, quantityOf, close, data is true
	{
		srand(static_cast<int>(time(NULL)));
		ResetIndexVector();
	}
	void Reset()
	{
		m_bitSet = std::bitset<TYPE_SIZE>(15);
		m_isConditionPart = false;
		m_canBeMoreLess = false;
		ResetIndexVector();
	}
	WordType GetNextType()
	{
		if (!m_indexVector.size())
			return end;

		int nextIndex = rand() % m_indexVector.size();
		WordType nextType = static_cast<WordType>(m_indexVector[nextIndex]);

		switch (nextType)
		{
		case all:   // true types: all, quantityOf, close, data
			m_bitSet[all] = m_bitSet[quantityOf] = m_bitSet[close] = m_bitSet[data] = false;
			m_bitSet[that] = m_bitSet[end] = true;
			break;
		case quantityOf:
			if (m_isConditionPart)   //  true types: quantityOf, data
			{
				m_bitSet[quantityOf] = false;
				m_canBeMoreLess = true;
			}
			else   //   true types: all, quantityOf, close, data
			{
				m_bitSet[all] = m_bitSet[quantityOf] = m_bitSet[close] = m_allowComma = false;
			}
			break;
		case close:  //  true types: all, quantityOf, close, data
			m_bitSet[all] = m_bitSet[quantityOf] = m_bitSet[close] = m_bitSet[data] = false;
			m_bitSet[end] = true;
			break;
		case data:
			if (m_isConditionPart)   //  true types: data, quantityOf
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
			else     //  true types: all, quantityOf, close, data
			{
				m_bitSet[all] = m_bitSet[quantityOf] = m_bitSet[close] = m_bitSet[data] = false;
				m_bitSet[that] = m_bitSet[end] = true;
				m_bitSet[comma] = m_allowComma;
			}
			break;
		case that:  //  true types: that, comma, end
			m_bitSet[that] = m_bitSet[comma] = m_bitSet[end] = false;
			m_bitSet[data] = m_bitSet[quantityOf] = m_isConditionPart = true;
			break;
		case comma:    //  true types: comma, that, end
			m_bitSet[comma] = m_bitSet[that] = m_bitSet[end] = false;
			m_bitSet[data] = true;
			break;
		case is_isNot:  //  true types: is_isNot, isDefined_isUndefined, isMoreThan_isLessThan
			m_bitSet[is_isNot] = m_bitSet[isDefined_isUndefined] = m_bitSet[isMoreThan_isLessThan] = false;
			m_bitSet[value] = true;
			break;
		case isDefined_isUndefined:   //  true types: is_isNot, isDefined_isUndefined
			m_bitSet[is_isNot] = m_bitSet[isDefined_isUndefined] = false;
			m_bitSet[and_or] = m_bitSet[end] = true;
			break;
		case isMoreThan_isLessThan:   // true types: is_isNot, isMoreThan_isLessThan
			m_bitSet[is_isNot] = m_bitSet[isMoreThan_isLessThan] = false;
			m_bitSet[value] = true;
			break;
		case and_or:   //  true types: and_or, end
			m_bitSet[and_or] = m_bitSet[end] = false;
			m_bitSet[data] = m_bitSet[quantityOf] = true;
			break;
		case value:   //   true types: value
			m_bitSet[value] = false;
			m_bitSet[and_or] = m_bitSet[end] = true;
			break;
		case end:  //  true types: that, comma, end, and_or
			m_bitSet[that] = m_bitSet[comma] = m_bitSet[end] = m_bitSet[and_or] = false;
			break;
		}
		ResetIndexVector();
		return nextType;
	}

private:
	void ResetIndexVector()
	{
		m_indexVector.clear();
		for (int i = 0; i < TYPE_SIZE; ++i)
		{
			if (m_bitSet[i])
				m_indexVector.push_back(i);
		}
	}

	std::bitset<TYPE_SIZE> m_bitSet;
	std::vector<int> m_indexVector;
	bool m_isConditionPart = false;
	bool m_canBeMoreLess = false;
	bool m_allowComma = true;
};

class RequestGenerator
{
public:
	RequestGenerator() { srand(static_cast<int>(time(NULL))); }
	std::string GenerateRequest();

	static std::vector<std::string> m_dataVector;

private:
	std::string joinWords(const std::vector<std::string>&) const;
	std::string generateData() const;
	std::string generatePhoneNumber(unsigned = 8) const;
	std::string generateNumber(int, int) const;
	std::string generateMail() const;
	std::string generateWord(size_t, bool = false) const;

	ValidWordTypeMaker m_typeMaker;
};
#endif // PROTOCOLTESTER_H
