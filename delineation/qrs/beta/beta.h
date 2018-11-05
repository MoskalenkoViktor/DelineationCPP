#pragma once
#include "../../wave_delineation.h"
#include "peak.h"
#include "onset.h"
#include "offset.h"


WaveDelineation beta_processing(const ECGLead& ecg_lead, const ZeroCrossing& qrs_zc)
{
    WaveDelineation delineation;

    // define qrs peak index for WaveDelineation instance
    define_qrs_peak_index(ecg_lead, &delineation, qrs_zc);

    // define qrs onset index for WaveDelineation instance
    define_qrs_onset_index(ecg_lead, &delineation, qrs_zc);

    // define qrs offset index for WaveDelineation instance
    define_qrs_offset_index(ecg_lead, &delineation, qrs_zc);

    return delineation;
}
