#pragma once
#include "../../wave_delineation.h"
#include "../../../ecg_lead/ecg_lead.h"


void define_qrs_peak_index(const ECGLead& ecg_lead, 
    WaveDelineation* delineation, const ZeroCrossing& qrs_zc)
{
    size_t qrs_peak_index = qrs_zc.index;
    delineation->peak_index = qrs_peak_index;
    delineation->specification = WaveSpecification::EXIST;
}
