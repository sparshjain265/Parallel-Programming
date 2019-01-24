#include<bits/stdc++.h>
using namespace std;

const int NUM_THREADS = 4;

// H(N) is defined to be the harmonic partial sum to N terms.
// calculateHarmonic(): function given to threads to compute H(N).
// unsigned long start: starting point for the loop; different for each thread.
// unsigned long N: amount of terms to compute; different for each thread; divided evenly by # of threads.
// double *sum: pointer to a double which holds a thread's individual sum for H(N).
void calculateHarmonic(long long start, long long N, double *sum)
{
    for(; start <= N; start++)
    {
        *sum += 1.0/start;
    }
}

void executeWithThreads(long long N)
{
    thread t[NUM_THREADS];  // using four threads
    double sum[NUM_THREADS] = {0.0};    // array of each sum calculated by each thread
    double finalSum = 0.0;  
    clock_t start = clock();

    // Assign threads their task.
    for(int i = 0; i < NUM_THREADS; i++)
    {
        // thread([name of function], [starting point], [N], [address of individual element in sum array])
        t[i] = thread(calculateHarmonic, (((N/4)*i)+1), ((N/4)*(i+1)), &(sum[i]));
    }

    // Join threads to terminate and sum up all individual sums into finalSum.
    for(int i = 0; i < NUM_THREADS; i++)
    {
        t[i].join();
        finalSum += sum[i];
    }

    clock_t end = clock();
    printf("Harmonic sum of %lld is %1.18lf\n", N, finalSum);
    printf("Calculation took %1.18lf seconds.\n", (((double)(end-start)/CLOCKS_PER_SEC))/NUM_THREADS);
}

void executeNoThreads(long long N)
{
    double sum = 0.0;
    clock_t start = clock();
    for(int i = 1; i <= N; i++)
    {
        sum += 1.0/i;
    }
    clock_t end = clock();
    printf("Harmonic sum of %lld is %1.18lf\n", N, sum);
    printf("Calculation took %1.18lf seconds.\n", (((double)(end-start)/CLOCKS_PER_SEC))/NUM_THREADS);
}

int main()
{
    long long N;

    cout << "Enter the amount of terms to calculate the Harmonic sum to: ";
    cin >> N;
    assert(N > 0 && "N must be a nonzero positive integer.");
    cout << endl;

    if(N > NUM_THREADS && N % 4 == 0)
    {
        cout << "Calculating with threads..." << endl;
        executeWithThreads(N);
    }
    else if(N > NUM_THREADS && N % 4 != 0)
    {
        cout << "Calculating without threads..." << endl;
        executeNoThreads(N);
    }
    else
    {
        cout << "Calculating without threads due to small N..." << endl;
        executeNoThreads(N);
    }

    return 0;
}