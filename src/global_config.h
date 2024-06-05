#ifndef GLOBAL_CONFIG_H
#define GLOBAL_CONFIG_H

#include <string>



class GConfig
{
    private:
        GConfig() {}                        // Constructor? (the {} brackets) are needed here.

        uint32_t sample_rate             = 44100; // Global sample rate
        uint16_t num_channels            = 2;     // Global number of channels
        uint16_t samples_per_frame       = 64;    // Global number of frames per buffer/window (used in stream outputs)

    public:
        static GConfig& get_instance()
            {
                static GConfig    instance; // Guaranteed to be destroyed.
                                            // Instantiated on first use.
                return instance;
            }
        
        GConfig(GConfig const&)         = delete;
        void operator=(GConfig const&)  = delete;

        inline void set_sample_rate(uint32_t _sample_rate) {this->sample_rate = _sample_rate;}
        inline void set_num_channels(uint32_t _num_channels) {this->num_channels = _num_channels;}
        inline void set_frames_per_buffer(uint32_t _frames_per_buffer) {this->samples_per_frame = _frames_per_buffer;}

        inline uint32_t get_sample_rate() const {return this->sample_rate;}  
        inline uint16_t get_num_channels()  const {return this->num_channels ;} 
        inline uint16_t get_frames_per_buffer() const {return this->samples_per_frame;} 

        inline void print_config() const {
            printf("GLOBAL CONFIG:\n  Sample Rate = %u\n  Number of Channels = %u\n  Frames per buffer = %u\n", 
                    this->sample_rate, 
                    this->num_channels, 
                    this->samples_per_frame);}

        void check_config();
};

#endif