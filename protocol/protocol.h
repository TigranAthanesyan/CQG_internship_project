#include <string>
#include <iostream>
#include <set>
#include <vector>
#include <iterator>
#include <algorithm>

#pragma once

enum WordType 
{
	all,
	quantityOf,
	close,
	that,
	comma,
	is_isNot,
	isDefined_isUndefined,
	isMoreThan_isLessThan,
	and_or,
	data,
	value
};

struct TypedWord
{
	TypedWord(std::string w, WordType wt) : word(w), type(wt) {}
	std::string word;
	WordType type;
};

class Request
{
public:
	void Description(std::ostream& = std::cout) const;       //  prints the rules of request protocol
	void DataDescription(std::ostream& = std::cout) const;   //  prints the types of data
	void SetText(const std::string&);                        //  takes the text and turns it into a query
	bool IsCorrect();                                        //  verifies the correctness of the text
	bool Close() const;                                      //  checks the instruction for the end of the program
	std::string ErrorText() const;                           //  gets error text

	friend std::istream& operator >> (std::istream& input, Request& request)  //  receives text from the file stream
	{
		std::string text;
		std::getline(input, text);
		request.SetText(text);
		return input;
	}
	friend std::ostream& operator <<(std::ostream& output, const Request& request) // outputs the content to the file stream
	{
		size_t size = request.maximumSize();
		for (auto i = request.m_phrases.begin(); i != request.m_phrases.end(); ++i)
		{
			output << i->word;
			for (size_t count = i->word.size(); count < size; ++count)
				output << '-';
			output << "------ " << (i->type == data ? "data" :
				(i->type == value ? "value" : "keyword")) << std::endl;
		}
		return output;
	}

private:
	bool isData(const std::string&) const;
	bool isNumber(const std::string&) const;
	bool isEmail(const std::string&) const;
	bool isPhoneNumber(const std::string&) const;
	void splitToWords(const std::string&, std::vector<std::string>&) const;
	void getPhrases(const std::vector<std::string>&);
	size_t maximumSize() const;

	static std::set<std::string> m_dataSet;                   //  container for storing data types

	std::vector<TypedWord> m_phrases;
	std::string m_errorText;
};