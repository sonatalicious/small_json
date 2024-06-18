#pragma once

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;
namespace sonatalicious {
namespace json {

class Json
{
public:
	enum Type
	{
		kJsonNull = 0,
		kJsonBool,
		kJsonInt,
		kJsonDouble,
		kJsonString,
		kJsonArray,
		kJsonObject
	};

	Json();
	Json(bool value_);
	Json(int value_);
	Json(double value_);
	Json(const char* kValue_);
	Json(const string& kValue_);
	Json(Type type_);
	Json(const Json& kRhs_);

	operator bool();
	operator int();
	operator double();
	operator string();
	
	//��������
	Json& operator [] (int index_);
	void append(const Json& kRhs_);

	//�ṹ������
	Json& operator [] (const char* kKey_);
	Json& operator [] (const string& kKey_);

	//��Ҫ���һ����ֵ�����������
	Json& operator = (const Json& kRhs_);
	
	void copy(const Json& kRhs_);

	void clear(); //��new�������ڴ���գ���ֹй¶

	string str() const; //�鿴arr�д洢��ֵ

	//��������ĵ�����
	typedef std::vector<Json>::iterator iterator;
	iterator begin()
	{
		return _value._array->begin();
	}
	iterator end()
	{
		return _value._array->end();
	}

	bool operator == (const Json& kRhs_);
	bool operator != (const Json& kRhs_);

	bool isNull() const { return _type == kJsonNull; }
	bool isBool() const { return _type == kJsonBool; }
	bool isInt() const { return _type == kJsonInt; }
	bool isDouble() const { return _type == kJsonDouble; }
	bool isString() const { return _type == kJsonString; }
	bool isArray() const { return _type == kJsonArray; }
	bool isObject() const { return _type == kJsonObject; }

	//��ʾת��
	bool asBool() const;
	int asInt() const;
	double asDouble() const;
	string asSting() const;

	//�ж��������Ƿ���ĳ����������������û��ĳ��key
	bool has(int index_);
	bool has(const char* kKey_);
	bool has(const string& kStr_);

	//�Ƴ�Ԫ��
	void remove(int index_);
	void remove(const char* kKey_);
	void remove(const string& kStr_);
	
	void parse(const string& kStr_);

private:
	// union�������ڴ��ǹ��õģ����ڴ��С�����г�Ա����Ǹ�
	union Value
	{
		bool _bool;
		int _int;
		double _double;
		string* _string;
		vector<Json>* _array;
		map<string, Json>* _object;
	};

	Type _type; //JSON���������ͺ�ֵ
	Value _value;
};

}
}
