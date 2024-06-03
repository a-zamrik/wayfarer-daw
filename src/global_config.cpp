#include "global_config.h"
#include "error.h"


void 
GConfig::check_config()
{
    if (this->num_channels < 1) {
        critical_error_no_line_print("Need at least one channel");
    }

    if (this->sample_rate < 1) {
        critical_error_no_line_print("Sample rate is too low, must be above 1");
    }

    if (this->samples_per_frame < 1) {
        critical_error_no_line_print("samples per frame is too low, must be above 1");
    }
}