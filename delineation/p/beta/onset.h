//
// Created by Blintsov Sergey on 22/10/2018.
//

#pragma once;

#include "../../../params/params.h"
#include "../../../ecg_lead/ecg_lead.h"
#include "../../../modulus_maxima/routines.h"
#include "../delineation.h"
#include "../zcs.h"

void define_p_onset_index(const ECGLead& ecg_lead, WaveDelineation& delineation, std::vector<ZeroCrossing>& zcs, size_t left_peak_zc_id, size_t begin_index) {

    size_t searching_onset_left_border_index;

    int wdc_scale_id = get_p_wdc_scale_id(ecg_lead);
    std::vector<double> wdc = ecg_lead.wdc[wdc_scale_id];

    if (left_peak_zc_id > 0)
        searching_onset_left_border_index = std::max(begin_index, zcs[left_peak_zc_id - 1].index);
    else
        searching_onset_left_border_index = begin_index;

    ModulusMaxima current_mm = find_left_mm(zcs[left_peak_zc_id].index, wdc);
    double onset_mm_candidate_coeff = abs(current_mm.value) * float(ALPHA_ONSET_MM);

    std::vector<ModulusMaxima> mm_list;
    while (current_mm.index > searching_onset_left_border_index) {
        mm_list.push_back(current_mm);
        current_mm = find_left_mm(current_mm.index - 1, wdc);
    }

    if (&mm_list == nullptr) {
        delineation.onset_index = searching_onset_left_border_index;
        return;
    }

    size_t correct_onset_mm_id = 0;

    for (size_t onset_mm_id = 1; onset_mm_id<mm_list.size(); onset_mm_id++) {
        if (mm_list[onset_mm_id].correctness)
            if (abs(mm_list[onset_mm_id].value) > onset_mm_candidate_coeff)
                correct_onset_mm_id = onset_mm_id;
    }

    size_t onset_start_searching_index = mm_list[correct_onset_mm_id].index;
    double threshold = mm_list[correct_onset_mm_id].value * float(ALPHA_ONSET_OFFSET_THR);

    size_t onset_index_candidate_1 = find_left_thc_index(wdc, onset_start_searching_index, begin_index, threshold);
    size_t onset_index_candidate_2 = find_left_mm(onset_start_searching_index - 1, wdc).index;
    size_t onset_index = std::max(onset_index_candidate_1, onset_index_candidate_2);

    delineation.onset_index = onset_index;
}
