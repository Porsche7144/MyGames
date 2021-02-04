#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <set>


using namespace std;

//template <typename T> class IsFunction
//{
//public:
//	bool operator()(T& val)
//	{
//		return (val == 'a');
//	}
//};

//class Tdata
//{
//public:
//	int m_iValue;
//};
//bool operator<(const Tdata& ihs, const Tdata& rhs)
//{
//	return ihs.m_iValue < rhs.m_iValue;
//}

class Tdata
{
public:
	int Value;
};

bool operator<(const Tdata& ihs, const Tdata& rhs)
{
	return ihs.Value < rhs.Value;
}

int main()
{

	set<Tdata> list;

	Tdata a1;
	Tdata a2;
	Tdata a3;

	//a1.Value = 3;
	//a2.Value = 5;
	//a3.Value = 8;

	list.insert(a1);
	list.insert(a2);
	list.insert(a3);

	multiset<char> set1;
	set1.insert('G');
	set1.insert('a');
	set1.insert('m');
	set1.insert('e');
	set1.insert('A');
	set1.insert('k');
	set1.insert('a');
	set1.insert('d');
	set1.insert('e');
	set1.insert('m');
	set1.insert('y');

	std::list<char> list1;
	std::list<char> list3;

	list1.push_back('g');
	list1.push_back('a');
	list1.push_back('m');
	list1.push_back('e');

	//list1.~list<char>();
	/*list1.clear();*/

	int iNUm = list1.size();

	// a가 몇개 있는지 찾아라.
	int iCnt = count(list1.begin(), list1.end(), 'a');




	return 0;

}