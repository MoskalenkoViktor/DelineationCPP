//
// Created by Blintsov Sergey on 22/10/2018.
//

#pragma once;

#include "../../../params/params.h";
#include "../../../ecg_lead/ecg_lead.h"
#include "../../../modulus_maxima/routines.h"
#include "../../../delineation/p/routines.h"

bool is_small_p(const ECGLead& ecg_lead, size_t qrs_id, ZeroCrossing zcs, size_t peak_zc_id) {

    bool result = false;

    int wdc_scale_id = get_p_wdc_scale_id(ecg_lead);
    std::vector<double> wdc = ecg_lead.wdc[wdc_scale_id];
    double p_amplitude = zcs[peak_zc_id].mm_amplitude;
    size_t begin_qrs_index = ecg_lead.qrs_dels[qrs_id].onset_index;
    size_t end_qrs_index = ecg_lead.qrs_dels[qrs_id].offset_index;
    auto qrs_aux_zcs = get_zcs_with_global_mms(wdc, begin_qrs_index, end_qrs_index);
    if (qrs_aux_zcs) {
        ZeroCrossing qrs_zc = qrs_aux_zcs[0];

        for (size_t qrs_zc_candidate_it = 1; qrs_zc_candidate_it< qrs_aux_zcs.size(); qrs_zc_candidate_it++) {
            if (qrs_aux_zcs[qrs_zc_candidate_it].mm_amplitude > qrs_zc.mm_amplitude)
                qrs_zc = qrs_aux_zcs[qrs_zc_candidate_it];
        }

        double qrs_amplitude = qrs_zc.mm_amplitude;

        if (p_amplitude < float(ALPHA_PEAK_SMALL_AMPL) * qrs_amplitude)
            result = true;
    }
    return result;
}

bool is_p_peak_zc_candidate_exist(const ECGLead& ecg_lead, size_t qrs_id, ZeroCrossing zcs) {

    double rate = ecg_lead.rate;

    size_t begin_index = get_p_begin_index(ecg_lead, qrs_id);
    size_t end_index = get_p_end_index(ecg_lead, qrs_id);

    size_t zcs_peaks_candidates_begin_index = begin_index + int(float(ALPHA_PEAK_BEGIN_SHIFT) * rate);
    size_t zcs_peaks_candidates_end_index = end_index;

    bool result = false;

    for (size_t zc_it;zc_it<zcs.size();zc_it++) {
        if (zcs_peaks_candidates_begin_index < zcs[zc_it].index < zcs_peaks_candidates_end_index) {
            result = true;
            break;
        }
    }

    return result;
}

size_t get_p_peak_zc_id(const ECGLead& ecg_lead, size_t qrs_id, ZeroCrossing zcs) {

    double rate = ecg_lead.rate;

    size_t begin_index = get_p_begin_index(ecg_lead, qrs_id);
    size_t end_index = get_p_end_index(ecg_lead, qrs_id);

    size_t zcs_peaks_cands_begin_index = begin_index + int(float(ALPHA_PEAK_BEGIN_SHIFT) * rate);
    size_t zcs_peaks_cands_end_index = end_index;

    std::vector<size_t> cands_zcs_ids;
    for(size_t cand_zc_id = 0; cand_zc_id< zcs.size(); cand_zc_id++) {
        if (zcs_peaks_cands_begin_index < zcs[cand_zc_id].index < zcs_peaks_cands_end_index)
            cands_zcs_ids.push_back(cand_zc_id);
    }

    if (cands_zcs_ids.size() == 1) {
        size_t peak_zc_id = cands_zcs_ids[0];
        return peak_zc_id;
    }

    size_t zcs_cands_begin_index = zcs[cands_zcs_ids[0]].index - int((zcs[cands_zcs_ids[0]].index - zcs_peaks_cands_begin_index) * ALPHA_LEFT_ZC_PART_SHIFT);
    size_t zcs_cands_end_index = zcs[cands_zcs_ids[-1]].index + int((zcs_peaks_cands_end_index - zcs[cands_zcs_ids[-1]].index) * ALPHA_RIGHT_ZC_PART_SHIFT);
    size_t zcs_peaks_cands_window = zcs_cands_end_index - zcs_cands_begin_index;
    auto begin_part = float(ALPHA_PEAK_ADAPT_BEGIN_PART);
    double end_part = 1.0 - begin_part;
    auto zcs_peaks_candidates_point = zcs_cands_begin_index + zcs_peaks_cands_window * begin_part;

    size_t peak_zc_id = 0;
    double peak_zc_mm_amplitude = 0.0;

    for(size_t cand_zc_id = 0; cand_zc_id < cands_zcs_ids.size(); cand_zc_id++) {

        auto shift_percentage = float(zcs[cand_zc_id].index - zcs_peaks_candidates_point);
        if (shift_percentage < 0.0)
            shift_percentage = abs(shift_percentage) / (zcs_peaks_cands_window * begin_part);
        else
            shift_percentage = float(abs(shift_percentage) / (zcs_peaks_cands_window * end_part));

        double amplitude_part = 1.0 - pow(shift_percentage, float(ALPHA_PEAK_ADAPT_POW)) * float(ALPHA_PEAK_ADAPT_AMPL);

        if (zcs[cand_zc_id].mm_amplitude * amplitude_part >= peak_zc_mm_amplitude) {
            peak_zc_id = cand_zc_id;
            peak_zc_mm_amplitude = zcs[cand_zc_id].mm_amplitude * amplitude_part;
        }
    }
    return peak_zc_id;
}


