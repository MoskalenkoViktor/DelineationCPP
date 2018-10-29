#pragma once

#include "../../params/params.h"
#include "../../ecg_lead/ecg_lead.h"
#include "../../modulus_maxima/routines.h"
#include "../../modulus_maxima/modulus_maxima.h"
#include "routines.h"


bool is_p_peak_zc_candidate_exist(const ECGLead& ecg_lead, size_t qrs_id, ZeroCrossing& zcs) {

    double rate = ecg_lead.rate;

    size_t begin_index = get_p_begin_index(ecg_lead, qrs_id);
    size_t end_index = get_p_end_index(ecg_lead, qrs_id);

    size_t zcs_peaks_candidates_begin_index = begin_index + int(float(ZCS_PEAK_SEARCHING_SHIFT) * rate);
    size_t zcs_peaks_candidates_end_index = end_index;

    bool result = false;

    for (size_t zc = 0; zc<zcs.size(); zc++)
        if ((zcs_peaks_candidates_begin_index < zcs[zc].index) && (zcs[zc].index < zcs_peaks_candidates_end_index)) {
            result = true;
            break;
        }

    return result;
}


size_t get_p_peak_zc_id(const ECGLead& ecg_lead, size_t qrs_id, ZeroCrossing& zcs) {

    double rate = ecg_lead.rate;

    size_t begin_index = get_p_begin_index(ecg_lead, qrs_id);
    size_t end_index = get_p_end_index(ecg_lead, qrs_id);

    size_t zcs_peaks_candidates_begin_index = begin_index + int(float(ZCS_PEAK_SEARCHING_SHIFT) * rate);
    size_t zcs_peaks_candidates_end_index = end_index;

    std::vector<size_t> candidates_zcs_ids;
    for (size_t candidate_zc_id = 0; candidate_zc_id < zcs.size(); candidate_zc_id++)
        if ((zcs_peaks_candidates_begin_index < zcs[candidate_zc_id].index) && (zcs[candidate_zc_id].index < zcs_peaks_candidates_end_index))
            candidates_zcs_ids.push_back(candidate_zc_id);

    if (candidates_zcs_ids.size() == 1) {
        size_t peak_zc_id = candidates_zcs_ids[0];
        return peak_zc_id;
    }

    size_t zcs_candidates_begin_index = zcs[candidates_zcs_ids[0]].index - int((zcs[candidates_zcs_ids[0]].index - zcs_peaks_candidates_begin_index) * PEAK_ZC_AMPLITUDE_DECREASING_LEFT_PRIVILEGE);
    size_t zcs_candidates_end_index = zcs[candidates_zcs_ids[-1]].index + int((zcs_peaks_candidates_end_index - zcs[candidates_zcs_ids[-1]].index) * PEAK_ZC_AMPLITUDE_DECREASING_RIGHT_PRIVILEGE);
    size_t zcs_peaks_candidates_window = zcs_candidates_end_index - zcs_candidates_begin_index;
    auto begin_part = float(PEAK_ZC_AMPLITUDE_DECREASING_BEGIN_PART);
    double end_part = 1.0 - begin_part;
    double zcs_peaks_candidates_point = zcs_candidates_begin_index + zcs_peaks_candidates_window * begin_part;

    size_t peak_zc_id = 0;
    double peak_zc_mm_amplitude = 0.0;

    for (size_t candidate_zc_id = 0; candidate_zc_id<candidates_zcs_ids.size(); candidate_zc_id++) {
        double shift_percentage = double(zcs[candidate_zc_id].index - zcs_peaks_candidates_point);
        if (shift_percentage < 0.0)
            shift_percentage = abs(shift_percentage) / (zcs_peaks_candidates_window * begin_part);
        else
            shift_percentage = abs(shift_percentage) / (zcs_peaks_candidates_window * end_part);

        double amplitude_part = 1.0 - pow(shift_percentage, float(PEAK_ZC_AMPLITUDE_DECREASING_POW)) * float(PEAK_ZC_AMPLITUDE_DECREASING);

        if (zcs[candidate_zc_id].mm_amplitude * amplitude_part >= peak_zc_mm_amplitude) {
            peak_zc_id = candidate_zc_id;
            peak_zc_mm_amplitude = zcs[candidate_zc_id].mm_amplitude * amplitude_part;
        }
    }
    return peak_zc_id;
}


size_t get_p_flexure_zc_id(const ECGLead& ecg_lead, size_t qrs_id, ZeroCrossing& zcs, size_t peak_zc_id) {

    auto qrs_dels = ecg_lead.qrs_dels;

    size_t flexure_zc_id = 0;

    auto rr = qrs_dels[qrs_id].peak_index - qrs_dels[qrs_id - 1].peak_index;

    for (size_t zc_id = 1; zc_id <zcs.size()-1; zc_id++) {
        if (abs(zcs[zc_id - 1].index - zcs[zc_id].index) < float(FLEXURE_SHIFT) * rr && \
            abs(zcs[zc_id + 1].index - zcs[zc_id].index) < float(FLEXURE_SHIFT) * rr && \
            zcs[zc_id].mm_amplitude < float(FLEXURE_AMPLITUDE_NEIGHBOUR) * zcs[zc_id - 1].mm_amplitude && \
            zcs[zc_id].mm_amplitude < float(FLEXURE_AMPLITUDE_NEIGHBOUR) * zcs[zc_id + 1].mm_amplitude && \
            zcs[zc_id - 1].mm_amplitude > float(FLEXURE_AMPLITUDE_OLD_ZC) * zcs[peak_zc_id].mm_amplitude && \
            zcs[zc_id + 1].mm_amplitude > float(FLEXURE_AMPLITUDE_OLD_ZC) * zcs[peak_zc_id].mm_amplitude) {
                flexure_zc_id = zc_id;
        }
    }

    return flexure_zc_id;
}