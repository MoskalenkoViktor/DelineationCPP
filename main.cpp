//#include "zero_crossing.h"
#include "ecg_lead/ecg_lead.h"
#include <ctime>
#include <iostream>
#include <iostream>
#include <fstream>


int main()
{
    std::ifstream in("wdc.txt");
    std::vector<std::vector<double>> wdc;
    double value;
    for (size_t i = 0; i < 5; ++i)
    {
        wdc.emplace_back();
        for (size_t j = 0; j < 5120; ++j)
        {
            in >> value;
            wdc.back().push_back(value);
        }
    }

    /*for (size_t d = 0; d < 3; ++d)
    {
        for (size_t i = 0; i < 5; ++i)
        {
            std::vector<double>& w = wdc[i];
            w.insert(w.end(), w.begin(), w.end());
        }
    }*/
    

    clock_t begin = clock();
    ECGLead lead("i", {}, 512.0);
    for (size_t i = 0; i < 1; ++i)
    {
        lead.wdc = wdc;
        lead.calc_mms();
        lead.calc_zcs();
        lead.qrs_del();
    }
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Time: " << elapsed_secs << " sec" << std::endl;
    


    /*std::vector<double> wdc;
    for (int i = 0; i < 10000000; ++i)
    {
        wdc.push_back((static_cast<double>(rand()) / RAND_MAX) - 0.5);
    }
    clock_t begin = clock();
    std::vector<ZeroCrossing> zeros = get_zcs_with_global_mms(wdc, 0, wdc.size());
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Time: " << elapsed_secs << " sec" << std::endl;
    std::cout << zeros.size() << std::endl;*/

    return 0;
}