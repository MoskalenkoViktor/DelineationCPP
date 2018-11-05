//
// Created by Blintsov Sergey on 15/10/2018.
//

// ЗДЕСЬ И ДАЛЕЕ ВМЕСТО !zcs используется zcs.empty()

#pragma once

#include <functional>
#include <numeric>
#include "gamma/gamma.h"
#include "beta/beta.h"
#include "fibrillation/fibrillation.h"
#include "../wave_delineation.h"

std::pair<std::vector<WaveDelineation>, std::vector<Morphology>> get_p_dels(
        const ECGLead& ecg_lead);

//ZeroCrossing qrs_zc = ZeroCrossing();
//std::vector<WaveDelineation> get_p_delineations(ECGLead& ecg_lead);
//bool check_for_atrial_fibrillation(WaveDelineation delineation, std::vector<ZeroCrossing> zcs);
//bool is_small_p(const ECGLead& ecg_lead, size_t qrs_id, std::vector<ZeroCrossing> zcs, size_t peak_zc_id);

// DEPRECATED methods:
//
//double Variance(std::vector<double> samples) {
//    int size = samples.size();
//
//    double variance = 0;
//    double t = samples[0];
//    for (int i = 1; i < size; i++)
//    {
//        t += samples[i];
//        double diff = ((i + 1) * samples[i]) - t;
//        variance += (diff * diff) / ((i + 1.0) *i);
//    }
//
//    return variance / (size - 1);
//}
//
//double StandardDeviation(std::vector<double> samples) {
//    return sqrt(Variance(samples));
//}
//
//


//double calculateSD(std::vector<double> data) {
//    double sum = 0.0, mean, standardDeviation = 0.0;
//
//    int i;
//
//    for(i = 0; i < data.size()-1; ++i) {
//        sum += data[i];
//    }
//
//    mean = sum/data.size();
//
//    for(i = 0; i < data.size()-1; ++i)
//        standardDeviation += pow(data[i] - mean, 2);
//
//    return sqrt(standardDeviation / data.size());
//}
//
//std::vector<WaveDelineation> get_p_delineations(ECGLead& ecg_lead) {
//    std::vector<WaveDelineation> delineations;
//
//    for (size_t qrs_id = 1; qrs_id < count(ecg_lead.cur_qrs_dels_seq() ) ) {
//            WaveDelineation delineation = get_p_delineation(ecg_lead, qrs_id);
//
//            if (delineation.specification != WaveSpecification::ABSENCE)
//                    delineations.push_back(delineation);
//    }
//
//    return delineations;
//}
//
//
//WaveDelineation get_p_delineation(const ECGLead& ecg_lead, size_t qrs_id) {
//
//    WaveDelineation delineation = WaveDelineation();
//
//    if (ecg_lead.cur_qrs_dels_seq[qrs_id].specification == WaveSpecification::EXTRA)
//            return delineation;
//
//    double sampling_rate = ecg_lead.sampling_rate;
//
//    auto mm_window = int(float(MM_WINDOW) * sampling_rate);
//
//    std::vector<ZeroCrossing> zcs = get_p_zcs(ecg_lead, qrs_id, mm_window);
//
//    if (zcs.empty())
//            return delineation;
//
//    if (((zcs[-1].right_mm.index - zcs[-1].index) > int(float(RIGHT_MM_DIST) * sampling_rate)) || (abs(zcs[-1].right_mm.value) / abs(zcs[-1].left_mm.value) > float(OFFSET_MM_SHARPNESS))) {
//        zcs.pop_back();
//    }
//
//    if (zcs.empty())
//        return delineation;
//
//    if (((zcs[0].index - zcs[0].left_mm.index) > int(float(LEFT_MM_DIST) * sampling_rate)) || (abs(zcs[0].left_mm.value) / abs(zcs[0].right_mm.value) > float(ONSET_MM_SHARPNESS)))
//        zcs.erase(zcs.begin());
//
//    if (zcs.empty())
//        return delineation;
//
//    int window = get_window(ecg_lead, qrs_id);
//    size_t begin_index = get_p_begin_index(ecg_lead, qrs_id);
//    size_t end_index = get_p_end_index(ecg_lead, qrs_id);
//
//    if (window < int(float(ZCS_PEAK_SEARCHING_SHIFT) * sampling_rate))
//        return delineation;
//
//    if (!is_p_peak_zc_candidate_exist(ecg_lead, qrs_id, zcs))
//        return delineation;
//
//    size_t peak_zc_id = get_p_peak_zc_id(ecg_lead, qrs_id, zcs);
//
//    if (is_small_p(ecg_lead, qrs_id, zcs, peak_zc_id))
//        return delineation;
//
//    ZeroCrossing peak_zc = zcs[peak_zc_id];
//    size_t peak_index = peak_zc.index;
//    delineation.peak_index = peak_index;
//    delineation.specification = WaveSpecification::NORMAL;
//
//    std::vector<size_t> peak_zcs_ids = PeakZCsIds(peak_zc_id, peak_zc_id, peak_zc_id);
//
//    peak_zcs_ids.check_flexure_p(ecg_lead, qrs_id, zcs, delineation);
//
//    peak_zcs_ids.check_left_biphasic_p(ecg_lead, zcs, delineation);
//
//    define_p_onset_index(ecg_lead, delineation, zcs, peak_zcs_ids.left_zc_id, begin_index);
//    define_p_offset_index(ecg_lead, delineation, zcs, peak_zcs_ids.right_zc_id, end_index);
//
//    check_for_atrial_fibrillation(delineation, zcs);
//
//    return delineation;
//}
//
//bool check_for_atrial_fibrillation(WaveDelineation delineation, std::vector<ZeroCrossing> zcs) {
//    std::vector<double> zcs_amplitudes;
//    for (size_t zc = 0; zc < zcs.size() - 1; ++zc)
//        zcs_amplitudes.push_back(zcs[zc].mm_amplitude);
//
//    //zcs_amplitudes = np.asarray(zcs_amplitudes);
//
//    if (zcs.size() > FIB_NUM_ZCS) {
//        std::sort(zcs_amplitudes.begin(), zcs_amplitudes.end(), std::greater<int>());
//        std::vector<double> new_zcs_amplitudes;
//        // Analog for python [1:FIB_NUM_ZCS]
//        for (int zcs_id = 1; zcs_id < FIB_NUM_ZCS + 1; ++zcs_id)
//            new_zcs_amplitudes.push_back(zcs_amplitudes[zcs_id]);
//        zcs_amplitudes = new_zcs_amplitudes;
//
//        // Analog for np.mean:
//        double zcs_amplitudes_mean = std::accumulate(zcs_amplitudes.begin(), zcs_amplitudes.end(), 0,
//                                                     std::plus<double>());
//        zcs_amplitudes_mean = zcs_amplitudes_mean / zcs_amplitudes.size();
//        // Second variant :
//        //        double tmp_sum = .0;
//        //        for (int i = 0; i < zcs_amplitudes.size()-1; ++i) {
//        //            tmp_sum += zcs_amplitudes[i];
//        //        }
//        //        auto zcs_amplitudes_mean = tmp_sum / zcs_amplitudes.size();
//
// DEPRECATED:
//double zcs_amplitudes_std = StandardDeviation(zcs_amplitudes);
// Check answer here:
//        double zcs_amplitudes_std = calculateSD(zcs_amplitudes);
//
//        if (zcs_amplitudes_std < zcs_amplitudes_mean * float(FIB_STD))
//            delineation.specification = WaveSpecification::ATRIAL_FIBRILLATION;
//    }
//}
//
//
//bool is_small_p(const ECGLead& ecg_lead, size_t qrs_id, std::vector<ZeroCrossing> zcs, size_t peak_zc_id) {
//
//    bool answer = false;
//
//    int wdc_scale_id = get_p_wdc_scale_id(ecg_lead);
//    std::vector<double> wdc = ecg_lead.wdc[wdc_scale_id];
//    double p_amplitude = zcs[peak_zc_id].mm_amplitude;
//    size_t begin_qrs_index = ecg_lead.cur_qrs_dels_seq[qrs_id].onset_index;
//    size_t end_qrs_index = ecg_lead.cur_qrs_dels_seq[qrs_id].offset_index;
//    std::vector<ZeroCrossing> qrs_aux_zcs = get_zcs_with_global_mms(wdc, begin_qrs_index, end_qrs_index);
//    if (!qrs_aux_zcs.empty()) {
//        qrs_zc = qrs_aux_zcs[0];
//        for (size_t i = 1; i < qrs_aux_zcs.size() - 1; ++i)
//            if (qrs_aux_zcs[i].mm_amplitude > qrs_zc.mm_amplitude)
//                qrs_zc = qrs_aux_zcs[i];
//
//        double qrs_amplitude = qrs_zc.mm_amplitude;
//
//        if (p_amplitude < float(LOW_LIMIT_AMPLITUDE) * qrs_amplitude)
//            answer = true;
//    }
//    return answer;
//}





