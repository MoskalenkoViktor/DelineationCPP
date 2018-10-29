#pragma once

#include "../../params/params.h"
#include "../../ecg_lead/ecg_lead.h"
#include "../../modulus_maxima/routines.h"
#include "delineation.h"
#include "zcs.h"


void define_p_offset_index(const ECGLead& ecg_lead, WaveDelineation& delineation, ZeroCrossing& zcs, size_t right_peak_zc_id, int end_index) {
    size_t searching_offset_right_border_index;


    int wdc_scale_id = get_p_wdc_scale_id(ecg_lead);
    auto wdc = ecg_lead.wdc[wdc_scale_id];

    if (right_peak_zc_id < zcs.size() - 1)
        auto searching_offset_right_border_index = std::min(end_index, zcs[right_peak_zc_id + 1].index);
    else
        searching_offset_right_border_index = end_index;

    ModulusMaxima current_mm = find_right_mm(zcs[right_peak_zc_id].index, wdc);
    auto offset_mm_candidate_coeff = abs(current_mm.value) * float(OFFSET_MM_COEFF);
    auto offset_mm_candidate_coeff_overflow = abs(current_mm.value) * float(OFFSET_MM_COEFF_OVERFLOW);

    std::vector<ModulusMaxima> mm_list;
    while (current_mm.index < searching_offset_right_border_index) {
        mm_list.push_back(current_mm);
        current_mm = find_right_mm(current_mm.index + 1, wdc);
    }

    if (mm_list.empty()) {
        delineation.offset_index = searching_offset_right_border_index;
        return;
    }

    size_t correct_offset_mm_id = 0;

    if (delineation.specification != WaveSpecification::BIPHASIC) {
        for (size_t offset_mm_id = 1; offset_mm_id < mm_list.size(); offset_mm_id++)
            if (mm_list[offset_mm_id].correctness) {
                if (offset_mm_candidate_coeff < abs(mm_list[offset_mm_id].value) < offset_mm_candidate_coeff_overflow)
                    if (mm_list[correct_offset_mm_id].value * mm_list[offset_mm_id].value > 0)
                        correct_offset_mm_id = offset_mm_id;
                    else
                        break;
                else {
                    delineation.offset_index = mm_list[offset_mm_id].index;
                    return;
                }
            }
    }

    size_t offset_start_searching_index = mm_list[correct_offset_mm_id].index;
    auto threshold = mm_list[correct_offset_mm_id].value * float(ALPHA_ONSET_OFFSET_THR);

    size_t offset_index_candidate_1 = find_right_thc_index(wdc, offset_start_searching_index, end_index, threshold);
    size_t offset_index_candidate_2 = find_right_mm(offset_start_searching_index + 1, wdc).index;
    size_t offset_index = std::min(offset_index_candidate_1, offset_index_candidate_2);

    delineation.offset_index = offset_index;
}