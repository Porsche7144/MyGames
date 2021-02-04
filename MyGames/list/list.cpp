#include <iostream>
#include <list>

using namespace std;

class Tdata
{
public:
	int iValue;
	int fValue;
	int* pArray;

	// 초기화 에러로 인한 빈생성자, 소멸자
	Tdata()
	{

	}
	~Tdata()
	{

	}
};

int main()
{
	list<Tdata*> list1;

	for (int i = 0; i < 10; i++)
	{
		Tdata* a = new Tdata;
		a->iValue = i;
		a->fValue = (float)i;
		a->pArray = new int[3];
		a->pArray[0] = 100;
		a->pArray[1] = 200;
		a->pArray[2] = 300;
		list1.push_back(a);
	}

	for (list<Tdata*>::iterator iter = list1.begin(); iter != list1.end();)
	{
		// list시 삭제 처리조건에 iter++ or ++iter 하면 오류.

		Tdata* data = *iter;
		cout << data->iValue << " " << data->fValue << " " << data->pArray[0] << " ";

		list1.erase(iter++);
		//iter = list1.erase(iter);
	}
	cout << endl;

	for (list<Tdata*>::iterator iter = list1.begin(); iter != list1.end(); iter++)
	{
		Tdata* data = *iter;
		delete[] data->pArray;
		delete data;
		list1.erase(iter);
	}
	list1.clear();


	list<Tdata> list2;
	for (int i = 0; i < 10; i++)
	{
		// Tdata a 초기화 에러. 
		Tdata a;
		a.iValue = i;
		a.fValue = (float)i;
		a.pArray = new int[3];
		a.pArray[0] = 100;
		a.pArray[1] = 200;
		a.pArray[2] = 300;
		list2.push_back(a);
	}

	list<Tdata> list3(list2.begin(), list2.end());
	for (list<Tdata>::iterator iter = list3.begin(); iter != list3.end(); iter++)
	{
		Tdata data = *iter;
		data.pArray[0] = 9000;
		cout << data.iValue << data.fValue << " ";
	}
	cout << endl;

	list2.clear();
	list3.clear();

	return 0;
}