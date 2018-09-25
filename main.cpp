#include "zero_crossing.h"
#include <ctime>
#include <iostream>


int main()
{
    std::vector<double> wdc;
    for (int i = 0; i < 10000000; ++i)
    {
        wdc.push_back((static_cast<double>(rand()) / RAND_MAX) - 0.5);
    }
    clock_t begin = clock();
    std::vector<ZeroCrossing> zeros = get_zcs_with_global_mms(wdc, 0, wdc.size());
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Time: " << elapsed_secs << " sec" << std::endl;
    std::cout << zeros.size() << std::endl;

    return 0;
}