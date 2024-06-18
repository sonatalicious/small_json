#pragma once

#include <string>
#include "json.h"

using std::string;

namespace sonatalicious {
namespace json {

class Parser {
public:
	Parser();
	void load(const string& kStr_);//��Ҫ�������ַ���
	Json parse(); //����

private:
	void skipWriteSpace(); //���ո񡢻س������С����С��Ʊ���Ⱥ��Ե�
	char getNextToken(); //�����ַ��Ǵ�ǰ���������Ҫ��ȡ��һ���ַ��ķ���
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
