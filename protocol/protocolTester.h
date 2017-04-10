#include <string>
#include <iostream>
#include <set>
#include <vector>
#include <iterator>
#include <algorithm>
#include <ctime>

#ifndef PROTOCOLTESTER_H
#define PROTOCOLTESTER_H

class RequestGenerator
{
public:
	std::string GenerateRequest() const;


	static std::vector<std::string> m_data;
	static std::vector<std::string> m_firstPartKeywordsVector;
	static std::vector<std::string> m_conditionPartKeywordsVector;

//private:
	std::string joinWords(const std::vector<std::string>& keywordsVector) const;
	std::string generatePhoneNumber() const;
	std::string generateMail() const;

};

#endif // PROTOCOLTESTER_H