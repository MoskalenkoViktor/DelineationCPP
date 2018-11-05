//#include "zero_crossing.h"
#include "ecg_lead/ecg_lead.h"
#include <ctime>
#include <iostream>
#include <iostream>
#include <fstream>


int main()
{
    std::ifstream in("input_2321.txt");

    size_t size_filter;
    size_t size_wdc;
    in >> size_filter;
    in >> size_wdc;

    std::vector<double> filter;
    std::vector<std::vector<double>> wdc;

    double value;
    for (size_t i = 0; i < size_filter; ++i)
    {
        in >> value;
        filter.push_back(value);
    }

    for (size_t i = 0; i < 5; ++i)
    {
        wdc.emplace_back();
        for (size_t j = 0; j < size_wdc; ++j)
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
    

    ECGLead lead("lead_i", {}, 500.0);
    lead.wdc = wdc;
    lead.filter_ = filter;

    clock_t begin = clock();
    for (size_t i = 0; i < 12; ++i)
    {
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