#include "wave.h"
#include "../error.h"

#include <iostream>

WaveFileLoader::WaveFileLoader(std::string _file_path) {
    this->file_path = _file_path;

    std::ifstream wave_file;
    WaveFileHeader header;
    RiffSubChunkHeader * curr_subchunk;

    constexpr int MAX_SUB_CHUNKS = 16;
    int sub_chunk_count = 0;

    wave_file.open (this->file_path);
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

    wave_file.close();
}

void
WaveFileLoader::read_data(std::ifstream & wave_file, WaveFileHeader & header)
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
    