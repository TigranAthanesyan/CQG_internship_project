#include <string>
#include <iostream>
#include <set>
#include <vector>
#include <iterator>
#include <algorithm>
#ifndef PROTOCOL_H
#define PROTOCOL_H

class Request
{
public:
	void Description(std::ostream& = std::cout) const;       //  prints the rules of request protocol
	void DataDescription(std::ostream& = std::cout) const;   //  prints the types of data
	void Clear();                                            //  frees the contents of an array of words
	void SetText(const std::string&);                        //  takes the text and turns it into a query
	bool IsCorrect() const;                                  //  verifies the correctness of the text
	bool Close() const;                                      //  checks the instruction for the end of the program

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
			output << *i;
			for (size_t count = i->size(); count < size; ++count)
				output << '-';
			output << "------ " << (request.isKeyword(*i) ? "keyword" :
				(request.isData(*i) ? "data" : "value")) << std::endl;
		}
		return output;
	}

	static std::set<std::string> m_dataSet;                   //  container for storing data types
	static std::vector<std::string> m_keywordVector;          //  container for storing keywords

private:
	bool isData(const std::string&) const;
	bool isKeyword(const std::string&, bool = false) const;
	bool isSpecialKeyword(const std::string&) const;          //  keyword that after can be another keyword
	bool isNumber(const std::string&) const;
	bool isEmail(const std::string&) const;
	bool isPhoneNumber(const std::string&) const;
	void splitToWords(const std::string&, std::vector<std::string>&) const;
	void getPhrases(std::vector<std::string>&);
	size_t maximumSize() const;

	std::vector<std::string> m_phrases;
};

#endif // PROTOCOL_H