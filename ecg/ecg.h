#pragma once
#include <vector>
#include <map>
#include "../ecg_lead/ecg_lead.h"


class ECG
{
public:
    ECG(const std::map<std::string, std::vector<double>>& data, const std::string& name = "", 
        const std::string& record = "", bool is_log = false);
    void get_filtrated();
    void get_delineation();
    void get_morphology();
    void get_characteristics();
    void get_plot_data();


private:
    std::vector<ECGLead> leads_;
};