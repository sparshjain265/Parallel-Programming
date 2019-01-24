#include<bits/stdc++.h>
using namespace std;

void foo (int n, int m)
{
	auto id = this_thread::get_id();
	string s = "Function Foo" + to_string(m) + "\n";

	for(int i = 0; i < n; i++)
	{
		cout<<id<<endl;
	}
}

int main()
{
	thread t1(foo, 30, 1);
	thread t2(foo, 30, 2);
	t1.join();
	t2.join();

	return 0;
}