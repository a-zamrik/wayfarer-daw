#ifndef _WAVE_H_
#define _WAVE_H_

#include <cstdint>
#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include "../audio_track.h"

// Reference: http://soundfile.sapp.org/doc/WaveFormat/

enum WaveDataFormat
{
    INTEGER_PCM = 1,
    FLOAT_PCM   = 3
};

struct RiffSubChunkHeader
{
    char     SubchunkID[4];
    uint32_t SubchunkSize;
};

struct WaveFileHeader
{
    // RIFF PORTION
    char     ChunkId[4];
    uint32_t ChunkSize;
    char     Format[4];
    // FMT SUB CHUNK
    char     Subchunk1ID[4];
    uint32_t Subchunk1Size;
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BockAlign;
    uint16_t BitsPerSample;
    // DATA SUB CHUNK
    RiffSubChunkHeader FirstSubChunk;

    // Data follows
};


class WaveFileLoader
{
private:

    static void print_header(WaveFileHeader & header);
    
    static void
    read_data(std::ifstream & wave_file, WaveFileHeader & wave_header, std::vector<std::vector<float>> & );
    
    
public:
    static std::shared_ptr<AudioTrack> load(std::string _file_path);
    


};

#endif