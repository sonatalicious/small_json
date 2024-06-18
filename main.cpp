#include<iostream>
#include<fstream>
#include<sstream>

#include "json.h"

using namespace std;
using namespace sonatalicious::json;

int main() {

	cout << "===================��������ת��Ϊ Json ����======================" << endl;

	Json test1;
	Json test2 = true;
	Json test3 = 123;
	Json test4 = 1.23;
	Json test5 = "hello,world!";
	 
	bool a = test2;
	int b = test3;
	double c = test4;
	const string& d = test5;
	cout << a << ' ' << b << ' ' << c << ' ' << d << endl;
	cout << "=====================������ Json ����ת��=======================" << endl;
	Json arr;
	arr[0] = true;
	arr[1] = 123;
	arr.append(1.23);
	arr.append("hello,world!");

	cout << arr.str() << endl;

	bool b1 = arr[0];
	int i1 = arr[1];
	double f1 = arr[2];
	const string& kS1 = arr[3];

	cout << "=====================�ṹ���� Json ����ת��=======================" << endl;
	Json obj;
	obj["bool"] = true;
	obj["int"] = 123;
	obj["double"] = 1.23;
	obj["str"] = "hello,world!";
	obj["zarr"] = arr;
	obj["test12321"];
	cout << obj.str() << endl;

	cout << "=====================������������=======================" << endl;
	Json arr1;
	arr1[0] = true;
	arr1[1] = 123;
	arr1[2] = "hello,world!";
	arr1[3] = 1.23;
	arr1[5] = obj;
	for (auto it = arr1.begin(); it != arr1.end(); it++)
	{
		cout << it->str() << endl;
	}

	cout << "=====================�ж���������Ԫ���Ƿ����=======================" << endl;
	cout << arr1.has(1) << endl;
	cout << arr1.has(6) << endl;

	cout << "=====================ɾ��ĳ��Ԫ���ٷ��ؽ��=======================" << endl;
	arr1.remove(0);
	cout << arr1.str() << endl;

	cout << obj.has("hahah") << endl;
	cout << obj.has("str") << endl;

	obj.remove("str");
	cout << obj.str() << endl;

	cout << "=====================����=======================" << endl;
	//const string & kStr = "[\"a\", 1, 2, 3, true, false, null, 123]";
	//const string & kStr = "{\"a\": 1, \"b\": 2,\"c\": 3}";
	const string& kStr = "{\"code\": 0, \"message\" :\"0\" , \"ttl\" :1, \"data\" :{\"refresh\" :false, \"timestamp\" : 1684815765863}}";

	ifstream fIn("./test.json");
	stringstream ss;
	ss << fIn.rdbuf();

	//const string kStr = ss.str();
	//cout << kStr << endl;
	Json v;
	v.parse(kStr); // ���ַ�������Ϊ Json ���ݲ�����v��
	cout << v.str() << endl;

	return 0;
}