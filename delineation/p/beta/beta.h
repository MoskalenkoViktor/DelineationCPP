//
// Created by Blintsov Sergey on 15/10/2018.
//

#pragma once
#include "peak.h"
#include "flexure.h"
#include "biphasic.h"
#include "onset.h"
#include "offset.h"
#include "legacy.h"
#include "../zcs.h"
#include "../../wave_delineation.h"
#include "../../../ecg_lead/ecg_lead.h"
#include "../../../params/params.h"


WaveDelineation get_p_del(const ECGLead& ecg_lead, const size_t qrs_id) {
    WaveDelineation delineation;

    if (ecg_lead.qrs_dels[qrs_id].specification == WaveSpecification::EXTRA) {
        return delineation;
    }

    double rate = ecg_lead.rate;
    auto mm_window = int(float(ALPHA_MM_WINDOW) * rate);

    std::vector<ZeroCrossing> zcs = get_p_zcs(ecg_lead, qrs_id, mm_window);

    if (zcs.empty()) {
        return delineation;
    }

    if (((zcs[-1].s_r_mm.index - zcs[-1].index) > int(float(ALPHA_RIGHT_MM_DIST) * rate)) ||
        (abs(zcs[-1].s_r_mm.value) / abs(zcs[-1].s_l_mm.value) > float(ALPHA_OFFSET_MM_SHARP))) {
        zcs.pop_back();
    }

    if (zcs.empty()) {
        return delineation;
    }

    if (((zcs[0].index - zcs[0].s_l_mm.index) > int(float(ALPHA_LEFT_MM_DIST) * rate)) || (
            abs(zcs[0].s_l_mm.value) / abs(zcs[0].s_r_mm.value) > float(ALPHA_ONSET_MM_SHARP))) {
        zcs.pop_back();
    }

    if (zcs.empty()) {
        return delineation;
    }

    int window = get_window(ecg_lead, qrs_id);
    size_t begin_index = get_p_begin_index(ecg_lead, qrs_id);
    size_t end_index = get_p_end_index(ecg_lead, qrs_id);

    if (window < int(float(ALPHA_PEAK_BEGIN_SHIFT) * rate)) {
        return delineation;
    }

    if (!is_p_peak_zc_candidate_exist(ecg_lead, qrs_id, zcs) ) {
        return delineation;
    }

    auto peak_zc_id = get_p_peak_zc_id(ecg_lead, qrs_id, zcs);

    if (is_small_p(ecg_lead, qrs_id, zcs, peak_zc_id) ) {
        return delineation;
    }

    ZeroCrossing peak_zc = zcs[peak_zc_id];
    size_t peak_index = peak_zc.index;
    delineation.peak_index = peak_index;
    delineation.specification = WaveSpecification.exist;

    auto peak_zcs_ids = PeakZCsIds(peak_zc_id, peak_zc_id, peak_zc_id);

    check_flexure_p(peak_zcs_ids, ecg_lead, qrs_id, zcs, delineation);

    check_left_biphasic_p(peak_zcs_ids, ecg_lead, zcs, delineation);

    define_p_onset_index(ecg_lead, delineation, zcs, peak_zcs_ids.left_zc_id, begin_index);
    define_p_offset_index(ecg_lead, delineation, zcs, peak_zcs_ids.right_zc_id, end_index);

    check_for_atrial_fibrillation(delineation, zcs);


    return delineation;
}
