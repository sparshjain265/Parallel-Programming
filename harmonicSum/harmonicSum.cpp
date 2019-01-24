#include<bits/stdc++.h>

double calculate_harmonic(long long start, long long N)
{
    double sum = 0.0;
    for (; start <= N; ++start)
    {
        sum += 1.0 / start;
    }
    return sum;
}

int main()
{
    const std::size_t n_async = 4;
    const std::size_t N = 150;
    std::array<std::future<double>, n_async> futures;
    for (auto i = 0U; i < n_async; ++i) {
        futures[i] = std::async(calculate_harmonic, ((N / 4) * i) + 1,
            ((N / 4) * (i + 1)));
    }

    double final_sum = 0.0;
    for (auto&& future : futures) {
        final_sum += future.get();
    }
    std::cout << "final sum: " << final_sum << "\n";
}