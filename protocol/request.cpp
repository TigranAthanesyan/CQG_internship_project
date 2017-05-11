/**
* @file    request.cpp
* @author  Tigran Athanesyan
* @version 1.0
*/

#include "request.h"

/// Function for initializing the data set
std::set<std::string> MakeDataSet()
{
	std::set<std::string> dataSet;
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
	return std::move(dataSet);
}

void Request::Description(std::ostream& output) const
{
	output << "    Request types.." << std::endl << std::endl
		<< "-------------------------------------------------------------------------" << std::endl
		<< " all" << std::endl
		<< " all that \"condition\" (and/or \"condition\" ...)"
		<< " \"data type\"" << std::endl
		<< " \"data type 1\" , \"data type 2\" ..." << std::endl
		<< " \"data type\" that \"condition\" (and/or \"condition\" ...)" << std::endl
		<< " \"data type 1\" , \"data type 2\" ... that \"condition\" (and/or \"condition\"...)" << std::endl
		<< " quantity of \"data type\"" << std::endl
		<< " quantity of \"data type\" that \"condition\" (and/or \"condition\"...)" << std::endl
		<< " close" << std::endl
		<< "-------------------------------------------------------------------------" << std::endl
		<< std::endl << "    Condition types.." << std::endl << std::endl
		<< "-------------------------------------------------------------------------" << std::endl
		<< " \"data type\" is \"value\"" << std::endl
		<< " \"data type\" is not \"value\"" << std::endl
		<< " \"data type\" is defined" << std::endl
		<< " \"data type\" is undefined" << std::endl
		<< " quantity of \"data type\" is more than \"value\"" << std::endl
		<< " quantity of \"data type\" is less than \"value\"" << std::endl
		<< "-------------------------------------------------------------------------" << std::endl << std::endl;
}

void Request::DataDescription(std::ostream& output) const
{
	output << "    Data types.." << std::endl << std::endl
		<< "-------------------------------------------------------------------------" << std::endl;
	unsigned count = 1;
	for (auto i = m_dataSet.begin(); i != m_dataSet.end(); ++i, ++count)
	{
		output << '\t' << *i;
		if (i->size() < 8)
			output << '\t';
		output << (count % 2 ? '\t' : '\n');
	}
	output << "-------------------------------------------------------------------------" << std::endl << std::endl;
}

void Request::SetText(const std::string& text)
{
	std::vector<std::string> tempArray;
	splitToWords(text, tempArray);
	m_phrases.clear();
	getPhrases(tempArray);
	m_errorText = "";
}

bool Request::IsCorrect()
{
	bool dataIsValid = true, quantityOfIsValid = true, allowComma = true;
	bool thatIsValid = false, commaIsValid = false, valueIsValid = false;
	bool conditionIsValid = false, andOrIsValid = false, isConditionPart = false;
	bool isQuantityOf = false;

	for (auto i = m_phrases.begin(); i != m_phrases.end(); ++i)
	{
		switch (i->type)
		{
		case all:
			if (i != m_phrases.begin()) /// can be only at first of expression
			{
				m_errorText = "\"all\" can be only at first of expression";
				return false;
			}
			dataIsValid = quantityOfIsValid = allowComma = false;
			thatIsValid = true; /// after can be only "that"
			break;

		case quantityOf:
			if (!quantityOfIsValid)
			{
				m_errorText = "\"quantity of\" was used not correctly";
				return false;
			}
			if (i + 1 == m_phrases.end()) /// can not be the last word
			{
				m_errorText = "after \"quantity of\" is expected any data name";
				return false;
			}
			quantityOfIsValid = allowComma = false; /// can not be the request like quantity of many data types
			isQuantityOf = true;
			break;

		case data:
			if (!dataIsValid)
			{
				m_errorText = "\"" + i->word + "\" was used not correctly";
				return false;
			}
			dataIsValid = false;
			if (!isConditionPart) ///  before condition part
				thatIsValid = commaIsValid = true; ///  after can be "that" or comma
			else ///  condition part 
			{
				if (i + 1 == m_phrases.end()) /// can not be the last word
				{
					m_errorText = "after \"" + i->word + "\" is expected a continuation of condition";
					return false;
				}
				conditionIsValid = true; /// after can be only condition
			}
			break;

		case that:
			if (!thatIsValid)
			{
				m_errorText = "\"that\" was used not correctly";
				return false;
			}
			if (i + 1 == m_phrases.end()) /// can not be the last word
			{
				m_errorText = "after \"that\" is expected a condition";
				return false;
			}
			thatIsValid = commaIsValid = isQuantityOf = false;
			isConditionPart = dataIsValid = quantityOfIsValid = true; ///  after can be data or "quantity of"
			break;

		case comma:
			if (!allowComma)
			{
				m_errorText = "when requested \"quantity of\" use of comma is not allowed";
				return false;
			}
			if (!commaIsValid)
			{
				m_errorText =  "comma was used not correctly";
				return false;
			}
			if (i + 1 == m_phrases.end()) /// can not be the last word
			{
				m_errorText = "after comma is expected a data name";
				return false;
			}
			thatIsValid = commaIsValid = false;
			dataIsValid = true; ///  after can be only data
			break;

		case isDefined_isUndefined:
			if (!conditionIsValid)
			{
				m_errorText = "\"" + i->word + "\" was used not correctly";
				return false;
			}
			if (isQuantityOf)
			{
				m_errorText = "when requested \"quantity of\" use of \"" + i->word + "\" is not allowed";
				return false;
			}
			conditionIsValid = isQuantityOf = false;
			andOrIsValid = true; ///  after can be only and/or
			break;

		case is_isNot:
			if (!conditionIsValid)
			{
				m_errorText = "\"" + i->word + "\" was used not correctly";
				return false;
			}
			if (i + 1 == m_phrases.end()) /// can not be the last word
			{
				m_errorText = "after \"" + i->word + "\" is expected a value";
				return false;
			}
			conditionIsValid = isQuantityOf = false;
			valueIsValid = true; ///  after can be only value
			break;

		case isMoreThan_isLessThan:
			if (!conditionIsValid)
			{
				m_errorText = "\"" + i->word + "\" was used not correctly";
				return false;
			}
			if (!isQuantityOf)
			{
				m_errorText = "when is not requested \"quantity of\" use of \"" + i->word + "\" is not allowed";
				return false;
			}
			if (i + 1 == m_phrases.end()) /// can not be the last word
			{
				m_errorText = "after \"" + i->word + "\" is expected a value";
				return false;
			}
			conditionIsValid = isQuantityOf = false;
			valueIsValid = true; /// after can be only value
			break;

		case and_or:
			if (!andOrIsValid)
			{
				m_errorText = "\"" + i->word + "\" was used not correctly";
				return false;
			}
			if (i + 1 == m_phrases.end()) /// can not be the last word
			{
				m_errorText = "after \"" + i->word + "\" is expected a condition";
				return false;
			}
			andOrIsValid = false;
			dataIsValid = quantityOfIsValid = true; /// after can be data or quantity of
			break;

		case close:
			if (m_phrases.size() != 1) ///  can be only alone
			{
				m_errorText = "use of \"close\" is allowed only alone";
				return false;
			}
			break;

		case value:
			if (!valueIsValid)
			{
				m_errorText = "\"" + i->word + "\" was used not correctly";
				return false;
			}
			valueIsValid = isQuantityOf = false;
			andOrIsValid = true; ///  after can be only and/or

			if (((i - 2)->word == "work e-mail" || (i - 2)->word == "home e-mail") && (i - 1)->type == is_isNot && !isEmail(i->word))
			{
				m_errorText = "\"" + i->word + "\" is not correct e-mail";
				return false;
			}
			if ((i - 2)->word == "phone" && (i - 1)->type == is_isNot && !isPhoneNumber(i->word))
			{
				m_errorText = "\"" + i->word + "\" is not correct phone number";
				return false;
			}
			if ((i - 1)->type == isMoreThan_isLessThan && !isNumber(i->word))
			{
				m_errorText = "\"" + i->word + "\" is not correct number";
				return false;
			}
			if ((i - 3)->type == quantityOf && (i - 1)->type == is_isNot && !isNumber(i->word))
			{
				m_errorText = "\"" + i->word + "\" is not correct number";
				return false;
			}
			break;
		}
	}
	return true;
}

bool Request::Close() const
{
	return m_phrases.size() == 1 && m_phrases.back().type == close;
}

std::string Request::ErrorText() const
{
	return m_errorText;
}

bool Request::isData(const std::string& word) const
{
	return std::find(m_dataSet.begin(), m_dataSet.end(), word) != m_dataSet.end();
}

bool Request::isNumber(const std::string& word) const
{
	for (int i = 0; i < word.size(); ++i)
	{
		if (!isdigit(word[i]))
			return false;
	}
	return true;
}

bool Request::isEmail(const std::string& word) const
{
	auto atIter = std::find(word.begin(), word.end(), '@');
	if (std::count(word.begin(), word.end(), '@') != 1 || atIter == word.begin())
		return false;

	if (std::count(atIter, word.end(), '.') != 1 || word.back() == '.')
		return false;

	if (*word.begin() >= '1' && *word.begin() <= '9')
		return false;

	for (auto i = word.begin(); i != word.end(); ++i)
	{
		if (((*i) >= 'A' && *i <= 'Z') || *i == ' ')
			return false;
	}
	return true;
}

bool Request::isPhoneNumber(const std::string& word) const
{
	if ((word[0] != '+' && !isdigit(word[0])) || word.size() < 10)
		return false;

	bool spaceIsValid = true;
	for (int i = 1; i < word.size(); ++i)
	{
		if (isdigit(word[i]))
		{
			spaceIsValid = true;
			continue;
		}
		if (word[i] == ' ' && spaceIsValid)
		{
			spaceIsValid = false;
			continue;
		}
		return false;
	}
	return true;
}

void Request::splitToWords(const std::string& text, std::vector<std::string>& tempArray) const
{
	std::string temp = "";
	for (int i = 0; i < text.size(); ++i)
	{
		if (text[i] == ' ' || text[i] == ',')
		{
			if (temp != "")
			{
				tempArray.push_back(temp);
				temp = "";
			}
			if (text[i] == ',')
				tempArray.push_back(",");
			continue;
		}
		temp += text[i];
		if (i == text.size() - 1 && temp != "")
			tempArray.push_back(temp);
	}
}

void Request::getPhrases(const std::vector<std::string>& tempArray)
{
	for (auto i = tempArray.begin(); i != tempArray.end(); ++i)
	{
		if (*i == "all")
			m_phrases.push_back(TypedWord(*i, all));
		else if (*i == "that")
			m_phrases.push_back(TypedWord(*i, that));
		else if (*i == "close")
			m_phrases.push_back(TypedWord(*i, close));
		else if (*i == "and" || *i == "or")
			m_phrases.push_back(TypedWord(*i, and_or));
		else if (*i == ",")
			m_phrases.push_back(TypedWord(*i, comma));
		else if (*i == "is")
			m_phrases.push_back(TypedWord(*i, is_isNot));
		else if (*i == "not" && !m_phrases.empty() && m_phrases.back().word == "is")
			m_phrases.back().word += ' ' + *i;
		else if ((*i == "defined" || *i == "undefined") && !m_phrases.empty() && m_phrases.back().word == "is")
		{
			m_phrases.back().word += ' ' + *i;
			m_phrases.back().type = isDefined_isUndefined;
		}
		else if ((*i == "more" || *i == "less") && !m_phrases.empty() && m_phrases.back().word == "is")
		{
			m_phrases.back().word += ' ' + *i;
			m_phrases.back().type = value;
		}
		else if ((*i == "than") && !m_phrases.empty() && (m_phrases.back().word == "is more" || m_phrases.back().word == "is less"))
		{
			m_phrases.back().word += ' ' + *i;
			m_phrases.back().type = isMoreThan_isLessThan;
		}
		else if (*i == "quantity")
			m_phrases.push_back(TypedWord(*i, value));
		else if (*i == "of" && !m_phrases.empty() && m_phrases.back().word == "quantity")
		{
			m_phrases.back().word += ' ' + *i;
			m_phrases.back().type = quantityOf;
		}
		else if (isData(*i))
			m_phrases.push_back(TypedWord(*i, data));
		else if (!m_phrases.empty() && m_phrases.back().type == value)
		{
			m_phrases.back().word += ' ' + *i;
			if (isData(m_phrases.back().word))
				m_phrases.back().type = data;
		}
		else
			m_phrases.push_back(TypedWord(*i, value));
	}
}

size_t Request::maximumSize() const
{
	size_t maxSize = 0;
	for (auto i = m_phrases.begin(); i != m_phrases.end(); ++i)
	{
		if (i->word.size() > maxSize)
			maxSize = i->word.size();
	}
	return maxSize;
}

std::set<std::string> Request::m_dataSet = MakeDataSet();