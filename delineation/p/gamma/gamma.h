#pragma once

#include "../routines.h"

Morphology get_p_morph(const ECGLead& ecg_lead, size_t del_id, const WaveDelineation& delineation) {
    auto main_scale_id = get_p_wdc_scale_id(ecg_lead);
    auto aux_scale_id = int(BETA_SCALE);
    PMorphologyData p_morph_data_main = PMorphologyData(ecg_lead, delineation, main_scale_id);
    PMorphologyData p_morph_data_aux = PMorphologyData(ecg_lead, delineation, aux_scale_id);

    // TODO Write hasattr
    int num_zcs_main = 0;
//    if (hasattr(p_morph_data_main, "zcs"))
//        num_zcs_main = p_morph_data_main.zcs.size();
//    else
//        num_zcs_main = 0;

    Degree degree = Degree::UNKNOWN;

// In the case of adequate data
    if (p_morph_data_aux.correct == 1) {
        std::vector<ZeroCrossing> zcs = p_morph_data_aux.zcs;
        size_t p_zc_id = p_morph_data_aux.peak_zc_id;

// Check: how many big zcs in delineation
        auto zc_ampl_th = zcs[p_zc_id].g_ampl * float(BETA_PEAK_ZC_AMPL);
        std::vector<size_t> big_zcs_ids;
        for (size_t zc_id = 0; zc_id <= zcs.size(); ++zc_id)
            if (zcs[zc_id].g_ampl > zc_ampl_th)
                big_zcs_ids.push_back(zc_id);

        auto num_big_zcs = big_zcs_ids.size();

// Checking degree
        if (num_zcs_main > 2)
            degree = Degree::UNKNOWN;
        else
            if (num_big_zcs == num_zcs_main)
                degree = Degree::SATISFYINGLY;
            else
                if (num_big_zcs == num_zcs_main + 1)
                    degree = Degree::DOUBTFULLY;
                else
                    degree = Degree::UNKNOWN;

//Getting points
        auto points = points_processing(ecg_lead, delineation, p_morph_data_aux);
    }
    else
// Getting points
        if (p_morph_data_main.correct)
            auto points = points_processing(ecg_lead, delineation, p_morph_data_main);
        else
            auto points = points_processing_trivial(ecg_lead, delineation);

    std::vector<int> branch_id;
    branch_id.push_back(0);
    branch_id.push_back(0);

    auto morphology = Morphology(del_id, points, degree, branch_id);
    return morphology;
}