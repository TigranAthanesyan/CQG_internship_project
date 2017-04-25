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
	ValidWordTypeMaker();
	void Reset();
	WordType GetNextType();

private:
	void ResetIndexVector();

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
