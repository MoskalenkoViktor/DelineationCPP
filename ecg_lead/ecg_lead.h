#pragma once
#include <vector>
#include "../modulus_maxima/modulus_maxima.h"
#include "../zero_crossing/zero_crossing.h"
#include "../delineation/wave_delineation.h"


class ModulusMaxima;
class ZeroCrossing;


class ECGLead
{
public:
    ECGLead(const std::string& lead_name, 
        const std::vector<double>& data, double sample_rate);
    void cwt_filtration();
    void common_filtration();
    void adaptive_filtration();
    void dwt();
    void calc_mms();
    void calc_zcs();
    void qrs_del();
    void t_del();
    void p_del();
    void del_correction();
    void calc_characteristics();
    void init_plot_data();
    void print_del_info();

    std::vector<std::vector<double>> wdc;
    std::vector<double> filter;
    double rate;

    std::vector<std::vector<ZeroCrossing>> zcs;
    std::vector<std::vector<int>> ids_zcs;
    std::vector<std::vector<ModulusMaxima>> mms;

    std::vector<WaveDelineation> qrs_dels;

private:
    std::vector<double> signal_;
    std::vector<double> origin_;
    std::string name_;
};
