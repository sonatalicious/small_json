#include"json.h"
#include"parser.h"
#include <sstream>

using namespace sonatalicious::json;
using namespace std;

//�����Ȱ����ͳ�ʼ���� _type,Ȼ���ʼ�� value
Json::Json() : _type(kJsonNull)
{

}

Json::Json(bool value_) : _type(kJsonBool)
{
	_value._bool = value_;
}

Json::Json(int value_) : _type(kJsonInt)
{
	_value._int = value_;
}

Json::Json(double value_) : _type(kJsonDouble)
{
	_value._double = value_;
}

Json::Json(const char* kValue_) : _type(kJsonString)
{
	_value._string = new string(kValue_);
}

Json::Json(const string& kValue_) : _type(kJsonString)
{
	_value._string = new string(kValue_);
}

Json::Json(Type type_) : _type(type_) //����������
{
	switch (_type)
	{
	case kJsonNull:
		break;
	case kJsonBool:
		_value._bool = false;
		break;
	case kJsonInt:
		_value._int = 0;
		break;
	case kJsonDouble:
		_value._double = 0.0;
		break;
	case kJsonString:
		_value._string = new string("");
		break;
	case kJsonArray:
		_value._array = new vector<Json>();
		break;
	case kJsonObject:
		_value._object = new map<string,Json>();
		break;
	default:
		break;
	}
}

Json::Json(const Json& kRhs_) :_type(kJsonObject)
{
	copy(kRhs_);
}

Json::operator bool()
{
	// ������Ƕ��󣬾�ת��Ϊ����
	if (kJsonBool != _type)
	{
		throw new logic_error("type error, not bool value");
	}
	return _value._bool;
}

Json::operator int()
{
	if (kJsonInt != _type)
	{
		throw new logic_error("type error, not int value");
	}
	return _value._int;
}

Json::operator double()
{
	if (kJsonDouble != _type)
	{
		throw new logic_error("type error, not double value");
	}
	return _value._double;
}

Json::operator string()
{
	if (kJsonString != _type)
	{
		throw new logic_error("type error, not string value");
	}
	return *(_value._string); // ��Ϊ��ŵ���ָ�룬����Ҫ������
}

Json& Json::operator [](int idx_) {
	if (kJsonArray != _type) //��������������ͣ�����ת��Ϊ��������
	{
		clear();
		_type = kJsonArray;
		_value._array = new vector<Json>();
	}
	if (idx_ < 0)
	{
		throw new logic_error("array[] index < 0");
	}
	int size = (_value._array)->size();
	if (idx_ >= size) //idx���������С������
	{
		for (int i = size; i <= idx_; ++i)
		{
			_value._array->push_back(Json());
		}
	}
	return _value._array->at(idx_);
}

//�ṹ������
Json& Json::operator [](const char* kKey_)
{
	string name(kKey_);
	return (*this)[name];
}

Json& Json::operator [](const string& kKey_)
{
	if (kJsonObject != _type)
	{
		clear();
		_type = kJsonObject;
		_value._object = new map<string,Json>();
	}
	//�ж� key �Ƿ��� map ����
	return (*(_value._object))[kKey_];
}

// = ����������
Json& Json::operator = (const Json& kRhs_)
{
	clear();
	copy(kRhs_);
	return *this;
}

bool Json::operator == (const Json& kRhs_)
{
	if (kRhs_._type != _type)
	{
		return false;
	}
	switch (_type)
	{
	case kJsonNull:
		return true;
	case kJsonBool:
		return _value._bool == kRhs_._value._bool;
	case kJsonInt:
		return _value._int == kRhs_._value._int;
	case kJsonDouble:
		return _value._double == kRhs_._value._double;
	case kJsonArray:
		return _value._array == kRhs_._value._array;
	case kJsonString:
		return *_value._string == *kRhs_._value._string;
	case kJsonObject:
		return _value._object == kRhs_._value._object;
	default:
		break;
	}
	return false;
}

bool Json::operator != (const Json& kRhs_)
{
	return !((*this) == kRhs_);
}

void Json::append(const Json& kRhs_)
{
	if (kJsonArray != _type)
	{
		clear();
		_type = kJsonArray;
		_value._array = new vector<Json>();
	}
	(_value._array)->push_back(kRhs_);
}

// copy ���������ǳ��������
void Json::copy(const Json& kRhs_)
{
	_type = kRhs_._type;
	switch (_type)
	{
	case kJsonNull:
		break;
	case kJsonBool:
		_value._bool = kRhs_._value._bool;
		break;
	case kJsonInt:
		_value._int = kRhs_._value._int;
		break;
	case kJsonDouble:
		_value._double = kRhs_._value._double;
		break;
	case kJsonString:
		_value._string = new string(*kRhs_._value._string); // �ĳ����
		break;
	case kJsonArray:
		_value._array = new vector<Json>(*kRhs_._value._array); // �ĳ����
		break;
	case kJsonObject:
		_value._object = kRhs_._value._object;
		break;
	}
}

// clear ��ֹ�ڴ�й©
void Json::clear()
{
	switch (_type)
	{
	case kJsonNull:
		break;
	case kJsonBool:
		_value._bool = false;
		break;
	case kJsonInt:
		_value._int =0;
		break;
	case kJsonDouble:
		_value._double = 0.0;
		break;
	case kJsonString:
		delete _value._string;
		break;
	case kJsonArray:
	{
		// �����Ԫ�أ��� delete �ڴ�
		for (auto it = _value._array->begin(); it != _value._array->end(); it++)
		{
			it->clear(); // �ݹ�ѭ������
		}
		delete _value._array;
	}
		break;
	case kJsonObject:
	{
		// �����Ԫ�أ��� delete �ڴ�
		for (auto it = _value._object->begin(); it != _value._object->end(); it++)
		{
			(it->second).clear();
		}
		delete _value._object;
	}
	break;
	default:
		break;
	}
	_type = kJsonNull;
}

// �� Json �е����ݴ�ӡ����
string Json::str() const
{
	stringstream ss;
	switch (_type)
	{
	case kJsonNull:
		ss << "null";
		break;
	case kJsonBool:
		if (_value._bool)
		{
			ss << "true";
		}
		else {
			ss << "false";
		}
		break;
	case kJsonInt:
		ss << _value._int;
		break;
	case kJsonDouble:
		ss << _value._double;
		break;
	case kJsonString:
		ss << '\"' << *(_value._string) << '\"';
		break;
	case kJsonArray:
	{
		ss << '[';
		for (auto it = _value._array->begin(); it != _value._array->end(); it++)
		{
			if (it != _value._array->begin())
			{
				ss << ',';
			}
			ss << it->str(); //�ݹ����
		}
		ss << ']';
		break;
	}
	case kJsonObject:
	{
		ss << '{';
		for (auto it = (_value._object)->begin(); it != (_value._object)->end(); it++)
		{
			if (it != (_value._object)->begin())
			{
				ss << ',';
			}
			ss << '\"' << it->first << '\"' << ':' << it->second.str(); //key ���ַ��������ݹ����
		}
		ss << '}';
		break;
	}
	default:
		break;
	}
	return ss.str();
}

bool Json::asBool() const
{
	if (kJsonBool != _type)
	{
		throw new logic_error("type error, not bool value");
	}
	return _value._bool;
}

int Json::asInt() const
{
	if (kJsonInt != _type)
	{
		throw new logic_error("type error, not int value");
	}
	return _value._int;
}

double Json::asDouble() const
{
	if (kJsonDouble != _type)
	{
		throw new logic_error("type error, not double value");
	}
	return _value._double;
}

string Json::asSting() const
{
	if (kJsonString != _type)
	{
		throw new logic_error("type error, not string value");
	}
	return *(_value._string);
}

bool Json::has(int idx_)
{
	if (kJsonArray != _type)
	{
		return false;
	}
	int size = (_value._array)->size();
	return (idx_ >= 0 && idx_ < size);
}

bool Json::has(const char* kKey_)
{
	string name(kKey_);
	return has(name); // ֱ�ӵ�������ĺ���
}

bool Json::has(const string& kKey_)
{
	if (kJsonObject != _type)
	{
		return false;
	}
	return ((_value._object)->find(kKey_) != (_value._object)->end());
}

void Json::remove(int idx_)
{
	if (kJsonArray != _type)
	{
		return;
	}
	int size = _value._array->size();
	if (idx_ < 0 || idx_ >= size)
	{
		return;
	}
	(_value._array)->at(idx_).clear(); // �����������ȷ������������ֱ�ӵ��� erase ������
	(_value._array)->erase((_value._array)->begin() + idx_);
}

void Json::remove(const char* kStr_)
{
	string name(kStr_);
	remove(name);
}

void Json::remove(const string& kStr_)
{
	auto it = _value._object->find(kStr_);
	if ((_value._object->end()) == it)
	{
		return;
	}
	(*_value._object)[kStr_].clear();
	(_value._object)->erase(kStr_);
}

void Json::parse(const string& kStr_)
{
	Parser p;
	p.load(kStr_);
	*this = p.parse();
}

