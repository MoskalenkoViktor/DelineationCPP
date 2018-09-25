#include "ecg.h"


ECG::ECG(const std::map<std::string, std::vector<double>>& data, 
    const std::string & name, const std::string & record, bool is_log)
{
    for (auto pair : data)
    {
        leads_.emplace_back(pair.first, pair.second, 500.0);
    }
}

void ECG::get_filtrated()
{
}

void ECG::get_delineation()
{
}

void ECG::get_morphology()
{
}

void ECG::get_characteristics()
{
}

void ECG::get_plot_data()
{
}
