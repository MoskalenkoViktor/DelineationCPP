#pragma once
#include <vector>
#include <string>

//#include "../modulus_maxima/modulus_maxima.h"
//#include "../zero_crossing/zero_crossing.h"
//#include "../delineation/wave_delineation.h"
#include "../delineation/qrs/delineation.h"
#include "../filtration/cwt_filtration.h"
#include "../filtration/common_filtration.h"
#include "../delineation/p/delineation.h"
#include "../discrete_wavelet_transform/wdc.h"
#include "../zero_crossing/zero_crossing.h"

// TODO Release exception class InvalidECGLead

class ECGLead {
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
// NEW
    std::vector<std::vector<double>> wdc;
    std::vector<double> filter;
    std::vector<double> characteristics;
    double sampling_rate;

    std::vector<WaveDelineation> cur_qrs_dels_seq;
    std::vector<WaveDelineation> cur_p_dels_seq;
    std::vector<WaveDelineation> cur_t_dels_seq;

    void delineation();
// END NEW
    std::vector<std::vector<ZeroCrossing>> zcs;
    std::vector<std::vector<int>> ids_zcs;

    std::vector<std::vector<ModulusMaxima>> mms;
    std::vector<std::vector<int>> ids_mms;

    std::vector<WaveDelineation> qrs_dels;
    std::vector<Morphology> qrs_morphs;

    std::vector<WaveDelineation> p_dels;
    std::vector<Morphology> p_morphs;

    std::vector<WaveDelineation> t_dels;
    std::vector<Morphology> t_morphs;

private:
    std::vector<double> signal_;
    std::vector<double> origin_;
    std::string name_;
};
