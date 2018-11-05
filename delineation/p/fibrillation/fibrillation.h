#pragma once

#include <cmath>
#include <iostream>
#include "../../../params/params.h"
#include "../../../ecg_lead/ecg_lead.h"

void fib_analysis_imbalance(const ECGLead& ecg_lead) {

    std::vector<double> signal = ecg_lead.filter;

    std::vector<WaveDelineation> p_dels = ecg_lead.p_dels;
    std::vector<Morphology> p_morphs = ecg_lead.p_morphs;
    std::cout << "P = " << ecg_lead.p_dels.size() << std::endl;
//    for(size_t del_id = p_dels.size()-1; del_id>=0; --del_id) {
//
//// Check for length imbalance
//        bool is_del_len = false;
//        auto left_len = float(p_dels[del_id].onset_index - p_dels[del_id].peak_index);
//        auto right_len = float(p_dels[del_id].peak_index - p_dels[del_id].offset_index);
//        auto len_part = float(FIB_LEN_PART);
//
//        if (left_len / right_len > len_part || left_len / right_len < 1.0 / len_part)
//            is_del_len = true;
//
//// Check for amplitude imbalance
//        bool is_del_ampl = false;
//        auto left_ampl = std::fabs(signal[p_dels[del_id].onset_index] - signal[p_dels[del_id].peak_index]);
//        auto right_ampl = std::fabs(signal[p_dels[del_id].peak_index] - signal[p_dels[del_id].offset_index]);
//        auto ampl_part = float(FIB_AMPL_PART);
//
//        if (left_ampl / right_ampl > ampl_part || left_ampl / right_ampl < 1.0 / ampl_part)
//            is_del_ampl = true;
//
//        if (is_del_len || is_del_ampl) {
//            p_dels.erase(p_dels.begin() + del_id);
//            p_morphs.erase(p_morphs.begin() + del_id);
//        }
//    }
}

void fib_analysis_shortage(ECGLead& ecg_lead) {

    auto target = ecg_lead.qrs_dels.size() - 1;
    size_t num_p_dels = ecg_lead.p_dels.size();

    auto part = float(FIB_DELS_PART);
    auto min_num_dels = int(target * part);

    if (num_p_dels < min_num_dels) {
        ecg_lead.p_dels.clear();
        ecg_lead.p_morphs.clear();
    }
}