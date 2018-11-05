#pragma once
#include "../../../params/params.h"
#include "../../wave_delineation.h"
double calculateSD(std::vector<double> data) {
    double sum = 0.0, mean, standardDeviation = 0.0;

    int i;

    for(i = 0; i < data.size()-1; ++i) {
        sum += data[i];
    }

    mean = sum/data.size();

    for(i = 0; i < data.size()-1; ++i)
        standardDeviation += pow(data[i] - mean, 2);

    return sqrt(standardDeviation / data.size());
}

bool check_for_atrial_fibrillation(WaveDelineation& delineation, std::vector<ZeroCrossing>& zcs) {
    std::vector<double> zcs_amplitudes;
    for (size_t zc = 0; zc < zcs.size() - 1; ++zc)
        zcs_amplitudes.push_back(zcs[zc].mm_amplitude);

    if (zcs.size() > FIB_NUM_ZCS) {
        std::sort(zcs_amplitudes.begin(), zcs_amplitudes.end(), std::greater<int>());
        std::vector<double> new_zcs_amplitudes;
        // Analog for python [1:FIB_NUM_ZCS]
        for (int zcs_id = 1; zcs_id < FIB_NUM_ZCS + 1; ++zcs_id)
            new_zcs_amplitudes.push_back(zcs_amplitudes[zcs_id]);
        zcs_amplitudes = new_zcs_amplitudes;

        // Analog for np.mean:
        double zcs_amplitudes_mean = std::accumulate(zcs_amplitudes.begin(), zcs_amplitudes.end(), 0, std::plus<double>());
        zcs_amplitudes_mean = zcs_amplitudes_mean / zcs_amplitudes.size();
        // Second variant :
        //        double tmp_sum = .0;
        //        for (int i = 0; i < zcs_amplitudes.size()-1; ++i) {
        //            tmp_sum += zcs_amplitudes[i];
        //        }
        //        auto zcs_amplitudes_mean = tmp_sum / zcs_amplitudes.size();
   //     double zcs_amplitudes_std = StandardDeviation(zcs_amplitudes);
 //Check answer here:
        double zcs_amplitudes_std = calculateSD(zcs_amplitudes);

        if (zcs_amplitudes_std < zcs_amplitudes_mean * float(FIB_STD))
            delineation.specification = WaveSpecification::ATRIAL_FIBRILLATION;
    }
}