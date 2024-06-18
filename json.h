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
	
	//数组重载
	Json& operator [] (int index_);
	void append(const Json& kRhs_);

	//结构体重载
	Json& operator [] (const char* kKey_);
	Json& operator [] (const string& kKey_);

	//需要添加一个赋值的重载运算符
	Json& operator = (const Json& kRhs_);
	
	void copy(const Json& kRhs_);

	void clear(); //对new出来的内存回收，防止泄露

	string str() const; //查看arr中存储的值

	//定义数组的迭代器
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

	//显示转换
	bool asBool() const;
	int asInt() const;
	double asDouble() const;
	string asSting() const;

	//判断数组中是否有某个索引，对象中有没有某个key
	bool has(int index_);
	bool has(const char* kKey_);
	bool has(const string& kStr_);

	//移除元素
	void remove(int index_);
	void remove(const char* kKey_);
	void remove(const string& kStr_);
	
	void parse(const string& kStr_);

private:
	// union联合体内存是公用的，其内存大小是其中成员最大那个
	union Value
	{
		bool _bool;
		int _int;
		double _double;
		string* _string;
		vector<Json>* _array;
		map<string, Json>* _object;
	};

	Type _type; //JSON包含的类型和值
	Value _value;
};

}
}
