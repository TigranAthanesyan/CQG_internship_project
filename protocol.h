#include <string>
#include <iostream>
#include <set>
#include <iterator>
#include <algorithm>
#ifndef PROTOCOL_H
#define PROTOCOL_H

void MakeDataSet(std::set<std::string>& dataSet)
{
	dataSet.insert("last name");
	dataSet.insert("first name");
	dataSet.insert("mr. / mrs.");
	dataSet.insert("acct. name");
	dataSet.insert("phone");
	dataSet.insert("home e-mail");
	dataSet.insert("type");
	dataSet.insert("industry segment");
	dataSet.insert("work e-mail");
	dataSet.insert("address");
	dataSet.insert("city");
	dataSet.insert("state");
	dataSet.insert("postal code");
	dataSet.insert("country");
	dataSet.insert("last contacted date");
	dataSet.insert("referral type");
	dataSet.insert("referral detail");
	dataSet.insert("sales rep.");
	dataSet.insert("sales rep. login");
	dataSet.insert("company office");
	dataSet.insert("trs admin");
	dataSet.insert("auth to trade");
	dataSet.insert("job title");
	dataSet.insert("contact id");
}

void MakeKeywordsSet(std::set<std::string>& keywordsSet)
{
	keywordsSet.insert("get");
	keywordsSet.insert("count");
	keywordsSet.insert("from");
	keywordsSet.insert("if");
}

bool IsCorrect(std::string& text, const std::set<std::string>& set)
{
	std::transform(text.begin(), text.end(), text.begin(), ::tolower);
	return std::find(set.begin(), set.end(), text) != set.end();
}

#endif // PROTOCOL_H