#include <string>
#include <iostream>
#include <set>
#include <iterator>
#include <algorithm>
#include "protocol.h"

int main()
{
	std::set<std::string> dataSet;
	std::set<std::string> keywordsSet;
	MakeDataSet(dataSet);
	MakeKeywordsSet(keywordsSet);

	std::string data, keyword;
	do
	{
		std::cout << "Press \"close\" for end.." << std::endl;
		std::cin >> keyword;
		std::cin.get();
		std::getline(std::cin, data);
		std::cout << (IsCorrect(data, dataSet) && IsCorrect(keyword, keywordsSet) ? "Good\n" : "Bad\n");
		std::cout << "keyword: \"" << keyword << "\"    data: \"" << data << "\"\n";
	} while (keyword != "close");
}