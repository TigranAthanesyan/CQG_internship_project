/**
 * @file    request.h
 * @author  Tigran Athanesyan
 * @version 1.0
 */

#include <string>
#include <iostream>
#include <set>
#include <vector>
#include <iterator>
#include <algorithm>

#pragma once

/**
 * To identify words or word's groups
 * whose behavior needs to consider
 * to check the correctness of the received request
 */
enum WordType 
{
	all,                     /// Keyword "all"
	quantityOf,              /// Keyword "quantity of"
	close,                   /// Keyword "close"
	that,                    /// Keyword "that"
	comma,                   /// Keyword ","
	is_isNot,                /// Keywords "is" and "is not"
	isDefined_isUndefined,   /// Keywords "is defined" and "is undefined"
	isMoreThan_isLessThan,   /// Keywords "is more than" and "is less than"
	and_or,                  /// Keywords "and" and "or"
	data,                    /// All data names
	value                    /// All values of datas
};

/**
 * A simpule structure
 * that contains a word or phrase
 * with the type of phrase
 */
struct TypedWord
{
	TypedWord(std::string w, WordType wt) : word(w), type(wt) {}
	std::string word;
	WordType type;
};

/**
 * Class Request that takes any text
 * split that into phrases
 * turns it into a query
 * and checks the correctness of text
 */

class Request
{
public:
	void Description(std::ostream& = std::cout) const;       ///  Prints the rules of request protocol
	void DataDescription(std::ostream& = std::cout) const;   ///  Prints the types of data
	void SetText(const std::string&);                        ///  Takes the text and turns it into a query
	bool IsCorrect();                                        ///  Verifies the correctness of the text
	bool Close() const;                                      ///  Checks the instruction for the end of the program
	std::string ErrorText() const;                           ///  Gets error text
	
	std::vector<TypedWord> GetPhrasesVector() { return m_phrases; }
	std::set<std::string> GetDataSet() { return m_dataSet; }

	friend std::istream& operator >> (std::istream& input, Request& request)  ///  Receives text from the file stream
	{
		std::string text;
		std::getline(input, text);
		request.SetText(text);
		return input;
	}
	friend std::ostream& operator <<(std::ostream& output, const Request& request) /// Outputs the content to the file stream
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

	/**
	 * Functions isData, isNumber, isEmail, isPhoneNumber
	 * to check if the input text is
	 * data name, correct number, correct e-mail address, correct phone number
	 */
	bool isData(const std::string&) const;
	bool isNumber(const std::string&) const;
	bool isEmail(const std::string&) const;
	bool isPhoneNumber(const std::string&) const;

	/**
	 * Functions splitToWords and getPhrases
	 *
	 * First one takes the whole request text,
	 * splits it into seperate words or symbol ","
	 * and saves that all in vector
	 *
	 * Second one takes the vector from previous function
	 * turns the words to defined phrases with there types
	 * and saves that all phrases in vector
	 */
	void splitToWords(const std::string&, std::vector<std::string>&) const;
	void getPhrases(const std::vector<std::string>&);

	/**
	 * A simple function maximumSize
	 * that checks the sizes of each phrase in vector
	 * and return the maximum one
	 */
	size_t maximumSize() const;

	static std::set<std::string> m_dataSet;  ///  Container for storing all possible data types

	std::vector<TypedWord> m_phrases;        ///  Container for storing the request with phrases
	std::string m_errorText;                 ///  String for printing error messages if there are
};