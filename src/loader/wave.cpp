#include "wave.h"
#include "../error.h"

#include <iostream>
#include <windows.h>

std::shared_ptr<AudioTrack>
WaveFileLoader::load(std::string _file_path) {

    std::ifstream wave_file;
    WaveFileHeader header;
    RiffSubChunkHeader * curr_subchunk;
    std::vector<std::vector<float>> data;

    constexpr int MAX_SUB_CHUNKS = 16;
    int sub_chunk_count = 0;

    wave_file.open (_file_path, std::ios::in | std::ios::binary);
    if (wave_file.fail())
    {
        critical_error_no_line_print("Failed to open %s\n", _file_path.c_str());
    }

    wave_file.read(((char *) &header), sizeof(WaveFileHeader));

    curr_subchunk = &header.FirstSubChunk;

    // Need to find data
    while (strncmp(curr_subchunk->SubchunkID, "data", 4) && sub_chunk_count <= MAX_SUB_CHUNKS)
    {
       wave_file.seekg(curr_subchunk->SubchunkSize, std::ios_base::cur);
       wave_file.read(((char *) curr_subchunk), sizeof(RiffSubChunkHeader));

       sub_chunk_count++;
       printf("Found chunk");
    }

    // Handle waves with no subchunk
    if (sub_chunk_count >= MAX_SUB_CHUNKS)
    {
        critical_error_no_line_print("Wave file \"%s\" contains no data sub chunk. Something is wrong with the file.\n", _file_path.c_str());
    }

    WaveFileLoader::print_header(header);
    // wave_file is now looking pointed at data, need to load it

    // Check if we can handle the data format
    if (header.AudioFormat != WaveDataFormat::FLOAT_PCM && header.AudioFormat != WaveDataFormat::INTEGER_PCM) 
    {
        critical_error_no_line_print("Wave file \"%s\" data format is unkown. Can't read...\n", _file_path.c_str());
    }

    if (header.BitsPerSample > 32)
    {
        critical_error("Wave file \"%s\" data BitsPerSample is > 32. Need to implment double float reads\n", _file_path.c_str());
    }

    //WaveFileLoader::read_data(wave_file, header, data);
    int16_t i_sample = 0; // SET TO UNSIGNED TO CRUNCH THE FUCK OUT OF IT
    float    sample = 0;
    uint32_t current_channel = 0;

    bool reading_padding = true;

    for (int c=0; c < header.NumChannels; c++)
    {
        data.push_back(std::vector<float>(header.FirstSubChunk.SubchunkSize / (header.BitsPerSample / 8)));
    }


    size_t bytes_read = 0;
    while (bytes_read < header.FirstSubChunk.SubchunkSize)
    {

        sample = 0;
        
        if (header.AudioFormat == WaveDataFormat::INTEGER_PCM)
        {
            wave_file.read((reinterpret_cast<char*>( &i_sample)), header.BitsPerSample / 8);
            bytes_read += header.BitsPerSample / 8;
            sample = ((static_cast<float>(i_sample)) / 32768.0f);
            if( sample > 1 ) sample = 1.0f;
            if( sample < -1 ) sample = -1.0f;
            sample *= 0.75f; // TODO: MAKE THIS GAIN CONFIGURABLE
            
        }
        else
        {
            wave_file.read(reinterpret_cast<char*>(&sample), header.BitsPerSample / 8);
            bytes_read += header.BitsPerSample / 8;
        }




            
        data[current_channel].push_back(sample);
        
        

        current_channel++;
        if (current_channel == header.NumChannels)
        {
            current_channel = 0;
        }
    }

    std::cout << "Read " << data[0].size() << std::endl;

    if (bytes_read != header.FirstSubChunk.SubchunkSize)
    {
        critical_error("Did not read file properly, reached EOF before reading all bytes somehow. Read %zu, expected %u\n", bytes_read, header.FirstSubChunk.SubchunkSize);
    }

    wave_file.close();

    return std::shared_ptr<AudioTrack>(new AudioTrack(data, header.SampleRate));
}

void
WaveFileLoader::read_data(std::ifstream & wave_file, 
                          WaveFileHeader & header, 
                          std::vector<std::vector<float>> & data)
{
    

    
}

void
WaveFileLoader::print_header(WaveFileHeader & header)
{
    std::cout << "Audio Format: " << header.AudioFormat << std::endl;
    std::cout << "Mum Channels: " << header.NumChannels << std::endl;
    std::cout << "Sample Rage: " << header.SampleRate << std::endl;
    std::cout << "Bits Per Sample: " << header.BitsPerSample << std::endl;
    std::cout << "Subchunk 2: " << header.FirstSubChunk.SubchunkID << std::endl;
    std::cout << "Data Size: " << header.FirstSubChunk.SubchunkSize << std::endl;
}
    