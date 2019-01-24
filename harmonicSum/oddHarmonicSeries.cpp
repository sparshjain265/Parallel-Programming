#include<bits/stdc++.h>
using namespace std;

// Calculate odd Harmonic sum of ith to (i+n)th term
void oddHarmonic1(long long i, long long n, double *sum)
{
	*sum = 0;
	// ((i%2)*2 - 1) is 1 for odd terms and -1 for even terms
	// 1 - 1/3 
	for(; i <= n; i++)
		*sum += ((i%2)*2 - 1.0) / (2*i - 1.0);

}

double oddHarmonic2(long long i, long long n)
{
	double sum = 0.0;
	for(; i <= n; i++)
		sum += ((i%2)*2 - 1.0) / (2*i - 1.0);
	return sum;
}

void useAsync(long long N, int p)
{
	cout<<"Using async"<<endl;

	auto start = chrono::high_resolution_clock::now();

	future<double> futures[p];

	long long f = N/p + p - N%p;

	for(int i = 0; i < p; i++)
		futures[i] = async(oddHarmonic2, f*i + 1, ((f*(i+1)) < N ? (f*(i+1)) : N));
	
	double sum = 0.0;

	for(auto& f : futures)
		sum += f.get();
	
	auto stop = chrono::high_resolution_clock::now();

	cout<<"Ans: "<<sum<<endl;
	
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

	cout<<"Time Taken: "<<duration.count()<<endl;
}

void useThreads(long long N, int p)
{
	cout<<"Using Threads"<<endl;

	auto start = chrono::high_resolution_clock::now();

	thread t[p];	//create threads
	double sum[p];

	long long f = N/p + p - N%p;

	//launch threads
	for(int i = 0; i < p; i++)
		t[i] = thread(oddHarmonic1, f*i + 1, ((f*(i+1)) < N ? (f*(i+1)) : N), &sum[i]);
	
	double finalSum = 0;
	for(int i = 0; i < p; i++)
	{
		t[i].join();
		finalSum += sum[i];
	}

	auto stop = chrono::high_resolution_clock::now();

	cout<<"Ans: "<<finalSum<<endl;
	
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

	cout<<"Time Taken: "<<duration.count()<<endl;

}

void useNoThreads(long long N)
{
	cout<<"Computing serially"<<endl;
	auto start = chrono::high_resolution_clock::now();

	double sum = oddHarmonic2(1, N);

	auto stop = chrono::high_resolution_clock::now();

	cout<<"Ans: "<<sum<<endl;
	
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

	cout<<"Time Taken: "<<duration.count()<<endl;
}	

int main()
{
	
	long long N;
	int p;
	cout<<"Enter the number of terms you want to calculate: ";
	cin>>N;
	cout<<endl;
	cout<<"Enter the number of threads you want to use: ";
	cin>>p;
	cout<<endl;

	useNoThreads(N);
	useThreads(N, p);
	useAsync(N, p);

	return 0;
}