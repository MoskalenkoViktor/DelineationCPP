//
// Created by Blintsov Sergey on 15/10/2018.
//

#pragma once

#include ""
#include "../../wave_delineation.h"
#include "../../../ecg_lead/ecg_lead.h"

Morphology get_p_morph(const ECGLead& ecg_lead, int del_id,
                              WaveDelineation* delineation)
{

    return { del_id, points, degree, branch_id };
}
