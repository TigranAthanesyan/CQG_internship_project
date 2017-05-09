#include <string>
#include <iostream>
#include <set>
#include <vector>
#include <iterator>
#include <algorithm>
#include <bitset>

#pragma once

namespace Tester
{

#define TYPE_SIZE 12

	enum WordType  //  possible types of words
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

	enum ValueType  //  possible types of value
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
		void resetIndexVector();

		std::bitset<TYPE_SIZE> m_bitSet; // contains bool values to show valid word types
		std::vector<int> m_indexVector;  // contains the numbers of bitset which are true
		bool m_isConditionPart = false;  // turns to true after keyword "that"
		bool m_canBeMoreLess = false;    // turns to true after keyword "quantity of"
		bool m_allowComma = true;        // turns to false after keyword "quantity of"
	};

	class RequestGenerator
	{
	public:
		RequestGenerator() {};
		std::string GenerateRequest();

		static std::vector<std::string> m_dataVector;

	private:
		std::string generateData() const;
		std::string generatePhoneNumber(const unsigned = 8) const;
		std::string generateNumber(const int, const int) const;
		std::string generateMail() const;
		std::string generateWord(const size_t, bool = false) const;

		ValidWordTypeMaker m_typeMaker;
	};

}
