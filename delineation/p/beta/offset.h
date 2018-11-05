//
// Created by Blintsov Sergey on 22/10/2018.
//

#pragma once;

#include <list>

#include "../../../params/params.h"
#include "../../../ecg_lead/ecg_lead.h"
#include "../routines.h"
#include "../../../modulus_maxima/modulus_maxima.h"
#include "../delineation.h"
#include "../zcs.h"


void define_p_offset_index(const ECGLead& ecg_lead, WaveDelineation& delineation, std::vector<ZeroCrossing> zcs, size_t right_peak_zc_id, size_t end_index) {

    size_t searching_offset_right_border_index;

    int wdc_scale_id = get_p_wdc_scale_id(ecg_lead);
    std::vector<double> wdc = ecg_lead.wdc[wdc_scale_id];
    std::vector<ModulusMaxima> all_mms = ecg_lead.mms[wdc_scale_id];
    ZeroCrossing right_peak_zc = zcs[right_peak_zc_id];

    if (right_peak_zc_id < zcs.size() - 1)
        searching_offset_right_border_index = std::min(end_index, zcs[right_peak_zc_id + 1].index);
    else
        searching_offset_right_border_index = end_index;

    ModulusMaxima current_mm = all_mms[right_peak_zc.r_mms[0].id];
    double offset_mm_candidate_coeff = abs(current_mm.value) * float(ALPHA_OFFSET_MM);
    double offset_mm_candidate_coeff_overflow = abs(current_mm.value) * float(ALPHA_OFFSET_MM_OVERFLOW);

    std::vector<ModulusMaxima> mm_list;
    //std::list<ModulusMaxima> mm_list;
    while (current_mm.index < searching_offset_right_border_index) {
        mm_list.push_back(current_mm);
        current_mm = all_mms[current_mm.id + 1];
    }

    if (&mm_list == nullptr) {
        delineation.offset_index = searching_offset_right_border_index;
        return;
    }

    size_t correct_offset_mm_id = 0;

    if (delineation.specification != WaveSpecification::BIPHASIC) {

        for(size_t offset_mm_id = 1; offset_mm_id<mm_list.size(); offset_mm_id++) {
            if (mm_list[offset_mm_id].correctness) {
                if (offset_mm_candidate_coeff < abs(mm_list[offset_mm_id].value) < offset_mm_candidate_coeff_overflow) {
                    if (mm_list[correct_offset_mm_id].value * mm_list[offset_mm_id].value > 0)
                        correct_offset_mm_id = offset_mm_id;
                }
                else
                    break;
            }
            else {
                delineation.offset_index = mm_list[offset_mm_id].index;
                return;
            }
        }
    }

    size_t offset_start_searching_index = mm_list[correct_offset_mm_id].index;
    double threshold = mm_list[correct_offset_mm_id].value * float(ALPHA_ONSET_OFFSET_THR);

    size_t offset_index_candidate_1 = find_right_thc_index(wdc, offset_start_searching_index, end_index, threshold);
    size_t offset_index_candidate_2 = all_mms[mm_list[correct_offset_mm_id].id + 1].index;
    size_t offset_index = std::min(offset_index_candidate_1, offset_index_candidate_2);

    delineation.offset_index = offset_index;
}