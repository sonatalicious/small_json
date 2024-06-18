#include <stdexcept>
#include <iostream>
#include "parser.h"

using namespace sonatalicious::json;

Parser::Parser() :_str(""), _idx(0) {};
void Parser::load(const string& kStr_)
{
	_str = kStr_;
	_idx = 0;
}

//递归，将空白字符，回车，空行，换行等忽略掉
void Parser::skipWriteSpace()
{
	while (_str[_idx] == ' ' || _str[_idx] == '\n' || _str[_idx] == '\r' || _str[_idx] == '\t')
	{
		_idx++;
	}
}

char Parser::getNextToken()
{
	skipWriteSpace();
	char ch = _str[_idx++]; //获取当前字符串索引，然后++，最后返回
	return ch;
}

Json Parser::parse() //核心
{
	char ch = getNextToken();
	switch (ch)
	{
	case 'n':
		--_idx;
		return parseNull();
	case 't': case 'f':
		--_idx;
		return parseBool();
	case '-':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		--_idx;
		return parseNumber();
	case '"':
		return Json(parseString());
	case '[':
		return parseArray();
	case '{':
		return parseObject();
	default:
		break;
	}
	throw std::logic_error("unexpected char");
}

Json Parser::parseNull()
{
	if (0 == _str.compare(_idx, 4, "null"))
	{
		_idx += 4;
		return Json();
	}
	throw std::logic_error("parse null error!");
}

Json Parser::parseBool()
{
	if (0 == _str.compare(_idx, 4, "true"))
	{
		_idx += 4;
		return Json(true);
	}
	else if (0 == _str.compare(_idx, 5, "false"))
	{
		_idx += 5;
		return Json(false);
	}
	throw std::logic_error("parse bool error");
}

Json Parser::parseNumber()
{
	int pos = _idx; // 先记录原始位置
	if ('-' == _str[_idx])
		++_idx;
	if (_str[_idx] < '0' || _str[_idx]> '9')
		throw std::logic_error("parse number error!");

	while (_str[_idx] >= '0' && _str[_idx] <= '9')
	{
		_idx++;
	}
	//判断是否有小数点
	if ('.' != _str[_idx])
	{
		int i = std::atoi(_str.c_str() + pos);
		return Json(i);
	}
	//是浮点数型
	++_idx;
	if (_str[_idx] < '0' || _str[_idx]> '9') {
		throw std::logic_error("parse number error!");
	}
	while (_str[_idx] >= '0' || _str[_idx] <= '9')
	{
		_idx++;
	}
	double f = std::atof(_str.c_str() + pos);
	return Json(f);
}

string Parser::parseString()
{
	string out;
	while (true)
	{
		char ch = _str[_idx++];
		if ('"' == ch) //说明字符串结束
		{
			break;
		}
		if ('\\' == ch)
		{
			ch = _str[_idx++];
			switch (ch)
			{
			case '\n':
				out += '\n';
				break;
			case '\r':
				out += '\r';
				break;
			case '\t':
				out += '\t';
				break;
			case '\b':
				out += '\b';
				break;
			case '\f':
				out += '\f';
				break;
			case '"':
				out += "\\\"";
				break;
			case '\\':
				out += "\\\\";
				break;
			case 'u':
				out += '\\u';
				for (int i = 0; i < 4; i++)
				{
					out += _str[_idx++];
				}
				break;

			default:
				break;
			}
		}
		else
		{
			out += ch;
		}

	}
	return out;
}
Json Parser::parseArray()
{
	// 声明对象类型
	Json arr(Json::kJsonArray); //创建临时 Json 对象，其类型为 Json 数组
	char ch = getNextToken();
	if (']' == ch)
	{
		return arr;
	}
	_idx--;
	while (true)
	{
		arr.append(parse()); //递归调用，解析它的元素，可能是数组，bool，结构体等
		ch = getNextToken();
		if(']' == ch)
		{
			break;
		}
		if (',' != ch)
		{
			throw new std::logic_error("parse array error!");
		}
		_idx++;
	}
	return arr;
}
Json Parser::parseObject()
{
	Json obj(Json::kJsonObject);
	char ch = getNextToken();
	if (ch == '}')
		return obj;
	_idx--;
	while (true)
	{
		ch = getNextToken();
		if (ch != '"') //对象为键值对，key为字符串，以 " 开头
		{
			throw new std::logic_error("parse object error!");
		}
		string key = parseString();
		ch = getNextToken();
		if (':' != ch)
		{
			throw new std::logic_error("parse object error!");
		}
		obj[key] = parse();
		ch = getNextToken();
		if ('}' == ch)
		{
			break;
		}
		if (',' != ch)
		{
			throw new std::logic_error("parse object error!");

		}
		_idx++;
	}
	//std::cout << obj.str();
	return obj;
}
