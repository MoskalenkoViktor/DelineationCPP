#pragma once
#include "../../../ecg_lead/ecg_lead.h"
#include "data.h"
#include "../../morfology_point.h"


std::vector<Point> processing_default_morphology(const ECGLead& ecg_lead,
    const WaveDelineation& delineation, const QRSMorphologyData& qrs_morphology_data)
{
    return {};
}