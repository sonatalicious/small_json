#pragma once

#include <string>
#include "json.h"

using std::string;

namespace sonatalicious {
namespace json {

class Parser {
public:
	Parser();
	void load(const string& kStr_);//需要解析的字符串
	Json parse(); //核心

private:
	void skipWriteSpace(); //将空格、回车、空行、换行、制表符等忽略掉
	char getNextToken(); //解析字符是从前往后，因此需要获取下一个字符的方法
	Json parseNull();
	Json parseBool();
	Json parseNumber();
	string parseString();
	Json parseArray();
	Json parseObject();
private:
	string _str;
	int _idx;
};

}
}
