#include "protocol.h"

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
	return dataSet;
}

std::vector<std::string> MakeKeywordVector()
{
	//  pushing keywords
	std::vector<std::string> keywordVector;
	keywordVector.push_back("all");
	keywordVector.push_back("that");
	keywordVector.push_back("and");
	keywordVector.push_back("or");
	keywordVector.push_back("quantity of");
	keywordVector.push_back("is");
	keywordVector.push_back("is not");
	keywordVector.push_back("is more than");
	keywordVector.push_back("is less than");
	keywordVector.push_back("is defined");
	keywordVector.push_back("is undefined");
	keywordVector.push_back(",");
	keywordVector.push_back("close");
	//  pushing parts of keywords
	keywordVector.push_back("quantity");
	keywordVector.push_back("of");
	keywordVector.push_back("not");
	keywordVector.push_back("more");
	keywordVector.push_back("less");
	keywordVector.push_back("than");
	keywordVector.push_back("defined");
	keywordVector.push_back("undefined");
	return keywordVector;
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

void Request::Clear()
{
	m_phrases.clear();
}

void Request::SetText(const std::string& text)
{
	std::vector<std::string> tempArray;
	splitToWords(text, tempArray);
	m_phrases.clear();
	getPhrases(tempArray);
}

bool Request::IsCorrect() const
{
	bool dataIsValid = true, quantityOfIsValid = true, allowComma = true;
	bool thatIsValid = false, commaIsValid = false, valueIsValid = false;
	bool conditionIsValid = false, andOrIsValid = false, isConditionPart = false;
	bool isQuantityOf = false;
	std::string word;
	for (auto i = m_phrases.begin(); i != m_phrases.end(); ++i)
	{
		if (*i == "all")
		{
			if (i != m_phrases.begin())   // can be only at first of expression
				return false;
			dataIsValid = quantityOfIsValid = allowComma = false;
			thatIsValid = true;           // after can be only "that"
		}
		else if (*i == "quantity of")
		{
			if (!quantityOfIsValid || i + 1 == m_phrases.end())  // can not be the last word
				return false;
			quantityOfIsValid = allowComma = false;     // can not be the request like quantity of many data types
			isQuantityOf = true;
			//if (isConditionPart)
			//moreLessIsValid = true;
		}
		else if (isData(*i))
		{
			if (!dataIsValid)
				return false;
			dataIsValid = false;
			if (!isConditionPart)                    //  before condition part
				thatIsValid = commaIsValid = true;
			else                                     //  condition part 
			{
				if (i + 1 == m_phrases.end())        // can not be the last word
					return false;
				conditionIsValid = true;             // after can be only condition
			}
		}
		else if (*i == "that")
		{
			if (!thatIsValid || i + 1 == m_phrases.end())    //  can not be the last word
				return false;
			thatIsValid = commaIsValid = isQuantityOf = false;
			isConditionPart = dataIsValid = quantityOfIsValid = true;            //  after can be only data
		}
		else if (*i == ",")
		{
			if (!allowComma || !commaIsValid || i + 1 == m_phrases.end())
				return false;
			thatIsValid = commaIsValid = false;
			dataIsValid = true;                              //  after can be only data
		}
		else if (*i == "is defined" || *i == "is undefined")
		{
			if (!conditionIsValid || isQuantityOf)
				return false;
			conditionIsValid = isQuantityOf = false;
			andOrIsValid = true;                              //  after can be only and/or
		}
		else if (*i == "is" || *i == "is not")
		{
			if (!conditionIsValid || i + 1 == m_phrases.end())  //  can not be the last word
				return false;
			conditionIsValid = isQuantityOf = false;
			valueIsValid = true;                              //  after acn be only value
		}
		else if (*i == "is more than" || *i == "is less than")
		{
			if (!conditionIsValid || !isQuantityOf || i + 1 == m_phrases.end())
				return false;
			conditionIsValid = isQuantityOf = false;
			valueIsValid = true;
		}
		else if (*i == "and" || *i == "or")
		{
			if (!andOrIsValid || i + 1 == m_phrases.end())    //  can not be the last word
				return false;
			andOrIsValid = false;
			dataIsValid = quantityOfIsValid = true;                               //  after can be only data
		}
		else if (*i == "close")
		{
			if (m_phrases.size() != 1)                        //  can be only alone
				return false;
		}
		else // is value
		{
			if (!valueIsValid)
				return false;
			valueIsValid = isQuantityOf = false;
			andOrIsValid = true;                              //  after can be only and/or

			if ((*(i - 2) == "work e-mail" || *(i - 2) == "home e-mail") && !isEmail(*i))
				return false;
			if (*(i - 2) == "phone" && !isPhoneNumber(*i))
				return false;

			if ((*(i - 1) == "is more than" || *(i - 1) == "is less than") && !isNumber(*i))
				return false;
		}
	}
	return true;
}

bool Request::Close() const
{
	return m_phrases.size() == 1 && m_phrases.back() == "close";
}

bool Request::isData(const std::string& word) const
{
	std::string temp = word;
	std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
	return std::find(m_dataSet.begin(), m_dataSet.end(), temp) != m_dataSet.end();
}

bool Request::isKeyword(const std::string & word, bool partsOfPhrase) const
{
	std::string temp = word;
	std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
	if (partsOfPhrase)
		return std::find(m_keywordVector.begin(), m_keywordVector.end(), temp) != m_keywordVector.end();
	auto end = std::find(m_keywordVector.begin(), m_keywordVector.end(), "quantity");
	return std::find(m_keywordVector.begin(), end, temp) != end;
}

bool Request::isSpecialKeyword(const std::string& s) const
{
	return s == "all" || s == "is defined" || s == "is undefined" || s == "that" || s == "and" || s == "or";
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
		if (word[i] != ' ' && spaceIsValid)
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

void Request::getPhrases(std::vector<std::string>& tempArray)
{
	bool isNotFirstWord = false, isNotFirstKeyword = false;
	for (auto i = tempArray.begin(); i != tempArray.end(); ++i)
	{
		if (isKeyword(*i, true))  //  including the parts of keywords
		{
			std::transform(i->begin(), i->end(), i->begin(), ::tolower);
			if (isNotFirstKeyword)  //  is the continuation of the previous keyword
			{
				m_phrases.back() += ' ' + *i;
			}
			else // is first keyword
			{
				m_phrases.push_back(*i);
				isNotFirstKeyword = true;
				isNotFirstWord = false;
			}
			if (isSpecialKeyword(m_phrases.back()))
				isNotFirstKeyword = false;  //  after "is defined" / "is undefined" can be "or" / "and"
		}
		else // is not keyword
		{
			if (isNotFirstWord)  //  is the continuation of the previous word (data type or value)
				m_phrases.back() += ' ' + *i;
			else // is first word 
			{
				m_phrases.push_back(*i);
				isNotFirstWord = true;
				isNotFirstKeyword = false;
			}
			if (isData(m_phrases.back()))
				std::transform(m_phrases.back().begin(), m_phrases.back().end(), m_phrases.back().begin(), ::tolower);
		}
	}
}

size_t Request::maximumSize() const  //  returns the size of the longest word in the vector
{
	size_t maxSize = 0;
	for (auto i = m_phrases.begin(); i != m_phrases.end(); ++i)
	{
		if (i->size() > maxSize)
			maxSize = i->size();
	}
	return maxSize;
}

std::set<std::string> Request::m_dataSet = MakeDataSet();
std::vector<std::string> Request::m_keywordVector = MakeKeywordVector();