//
// Created by Blintsov Sergey on 22/10/2018.
//

#pragma once;

#include "../../params/params.h";
#include "../../modulus_maxima/modulus_maxima.h"
#include "../../threshold_crossings/routines.h"


int get_p_wdc_scale_id(const ECGLead& ecg_lead) {

    int num_wdc_scales = ecg_lead.wdc.size();

    int wdc_scale_id = int(WDC_SCALE_ID);

    if (wdc_scale_id > num_wdc_scales - 1) {
        throw "Wrong wdc scale id for p";
    }

    return wdc_scale_id;
}

int get_window(const ECGLead& ecg_lead, size_t qrs_id) {

    double rate = ecg_lead.rate;

    std::vector<WaveDelineation> qrs_dels = ecg_lead.qrs_dels;

    size_t qrs_gap = qrs_dels[qrs_id].onset_index - qrs_dels[qrs_id - 1].offset_index;

    int window_candidate_1 = int(qrs_gap * float(ALPHA_ZCS_QRS_GAP));
    int window_candidate_2 = int(rate * float(ALPHA_ZCS_WINDOW));

    int window = std::min(window_candidate_1, window_candidate_2);

    std::vector<WaveDelineation> t_dels = ecg_lead.t_dels;

    if (t_dels) {
        unsigned long corr_t_id = std::min(qrs_id - 1, t_dels.size() - 1);
        size_t left_diff = qrs_dels[qrs_id].onset_index - t_dels[corr_t_id].offset_index;

        while (left_diff < 0 && corr_t_id > 0) {
            corr_t_id -= 1;
            left_diff = qrs_dels[qrs_id].onset_index - t_dels[corr_t_id].offset_index;
        }

        if (left_diff > 0) {
            int window_candidate_3 = left_diff;
            window = std::min(window, window_candidate_3);
        }
    }
    return window;
}

        def get_p_begin_index(ecg_lead, qrs_id):

"""
Define begin index for P delineation
:param ecg_lead: certain ECG lead
:param qrs_id: id of QRS, to the left of which we delineate P
:return begin_index: begin index for P delineation
"""

qrs_dels = ecg_lead.qrs_dels

window = get_window(ecg_lead, qrs_id)
begin_index = qrs_dels[qrs_id].onset_index - window

return begin_index


        def get_p_end_index(ecg_lead, qrs_id):

"""
Define end index for P delineation
:param ecg_lead: certain ECG lead
:param qrs_id: id of QRS, to the left of which we delineate P
:return end_index: end index for P delineation
"""

qrs_dels = ecg_lead.qrs_dels
wdc_scale_id = get_p_wdc_scale_id(ecg_lead)
wdc = ecg_lead.wdc[wdc_scale_id]

window = get_window(ecg_lead, qrs_id)
begin_index = qrs_dels[qrs_id].onset_index - window
tmp_mm = find_left_mm(qrs_dels[qrs_id].onset_index, wdc)
end_index_candidate_1 = tmp_mm.index
end_index_candidate_2 = find_left_thc_index(wdc, qrs_dels[qrs_id].onset_index, begin_index, 0.0)
end_index = max(end_index_candidate_1, end_index_candidate_2 - 1)

return end_index