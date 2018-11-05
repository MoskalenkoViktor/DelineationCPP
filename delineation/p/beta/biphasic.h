//
// Created by Blintsov Sergey on 22/10/2018.
//
#pragma once

#include "../../peaks_zcs_ids.h"
#include "../../wave_delineation.h"
#include "../../../params/params.h"

void check_left_biphasic_p(PeakZCsIds& triplet, const ECGLead& ecg_lead, std::vector<ZeroCrossing>& zcs, WaveDelineation& delineation) {
    size_t left_peak_zc_id = triplet.left_zc_id;
    ZeroCrossing left_peak_zc = zcs[left_peak_zc_id];

    size_t right_peak_zc_id = triplet.right_zc_id;
    ZeroCrossing right_peak_zc = zcs[right_peak_zc_id];

    double rate = ecg_lead.rate;

    auto biphasic_th_more_peak_zc = float(ALPHA_BIPHASE_AMPL_MORE) * abs(right_peak_zc.s_r_mm.value);
    auto biphasic_th_less_peak_zc = float(ALPHA_BIPHASE_AMPL_LESS) * abs(right_peak_zc.s_r_mm.value);

    if (biphasic_th_more_peak_zc < abs(left_peak_zc.s_l_mm.value) < biphasic_th_less_peak_zc) {

        if (is_prev_zc_exist(zcs, left_peak_zc_id, float(ALPHA_PEAK_BEGIN_SHIFT) * rate)) {
            size_t prev_zc_id = left_peak_zc_id - 1;
            ZeroCrossing prev_zc = zcs[prev_zc_id];

            auto biphasic_th_more_prev_zc = float(ALPHA_BIPHASE_AMPL_MORE) * abs(prev_zc.s_r_mm.value);
            auto biphasic_th_less_prev_zc = float(ALPHA_BIPHASE_AMPL_LESS) * abs(prev_zc.s_r_mm.value);

            if (biphasic_th_more_prev_zc < abs(prev_zc.s_l_mm.value) < biphasic_th_less_prev_zc) {

                double amplitude = abs(zcs[left_peak_zc_id].s_l_mm.value) + abs(zcs[right_peak_zc_id].s_r_mm.value);

                if (prev_zc.mm_amplitude > float(ALPHA_BIPHASE_AMPL) * amplitude) {
                    delineation.specification = WaveSpecification::BIPHASIC;

                    triplet.left_zc_id = prev_zc_id;
                }
            }
        }
    }
}


void check_right_biphasic_p(PeakZCsIds& triplet, const ECGLead& ecg_lead, std::vector<ZeroCrossing>& zcs, WaveDelineation& delineation) {
    size_t left_peak_zc_id = triplet.left_zc_id;
    ZeroCrossing left_peak_zc = zcs[left_peak_zc_id];

    size_t right_peak_zc_id = triplet.right_zc_id;
    ZeroCrossing right_peak_zc = zcs[right_peak_zc_id];

    double sampling_rate = ecg_lead.rate;

    auto biphasic_th_more_peak_zc = float(ALPHA_BIPHASE_AMPL_MORE) * abs(right_peak_zc.s_r_mm.value);
    auto biphasic_th_less_peak_zc = float(ALPHA_BIPHASE_AMPL_LESS) * abs(right_peak_zc.s_r_mm.value);

    if (biphasic_th_more_peak_zc < abs(left_peak_zc.s_l_mm.value) < biphasic_th_less_peak_zc) {

        if (is_next_zc_exist(zcs, right_peak_zc_id, float(ALPHA_PEAK_BEGIN_SHIFT) * sampling_rate)) {
            size_t next_zc_id = right_peak_zc_id + 1;
            ZeroCrossing next_zc = zcs[next_zc_id];

            auto biphasic_th_more_next_zc = float(ALPHA_BIPHASE_AMPL_MORE) * abs(next_zc.s_r_mm.value);
            auto biphasic_th_less_next_zc = float(ALPHA_BIPHASE_AMPL_LESS) * abs(next_zc.s_r_mm.value);

            if (biphasic_th_more_next_zc < abs(next_zc.s_l_mm.value) <= biphasic_th_less_next_zc) {
                double amplitude = abs(zcs[left_peak_zc_id].s_l_mm.value) + abs(zcs[right_peak_zc_id].s_r_mm.value);

                if (next_zc.mm_amplitude > float(ALPHA_BIPHASE_AMPL) * amplitude) {
                    delineation.specification = WaveSpecification::BIPHASIC;

                    triplet.right_zc_id = next_zc_id;
                }
            }
        }
    }
}


