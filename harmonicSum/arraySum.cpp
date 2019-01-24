#include<bits/stdc++.h>
using namespace std;

void partialSum (int *a, int start, int end, int *pSum)
{
	int sum = 0;
	for(int i = start; i <= end; i++)
		sum += a[i];
	*pSum = sum;
	// cout<<to_string(sum) + "\n";
}

int main()
{
	int a[20];
	for(int i = 0; i < 20; i++)
	{
		a[i] = i;
		// cout<<a[i]<<endl;
	}
	
	thread t[5];
	int pSum[5];
	for(int i = 0; i < 5; i++)
		t[i] = thread(partialSum, a, i * 4, i*4 + 3, &pSum[i]);
	
	for(int i = 0; i < 5; i++)
		t[i].join();
	
	int sum = 0;
	for(int i = 0; i < 5; i++)
		sum += pSum[i];

	cout<<"Total: "<<sum<<endl;
	return 0;
}