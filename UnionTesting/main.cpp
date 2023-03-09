#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <random>
#include <chrono>

// http://soundfile.sapp.org/doc/WaveFormat/
union WAV_HEADER
{
    /* variable to put data in originally */
    char data[44];

    struct
    {
        /* RIFF Chunk Descriptor */
        u8 RIFF[4];    // RIFF Header Magic header
        u32 ChunkSize; // RIFF Chunk Size
        u8 WAVE[4];    // WAVE Header

        /* "fmt" sub-chunk */
        u8 fmt[4];         // FMT header
        u32 Subchunk1Size; // Size of the fmt chunk
        uint16_t AudioFormat;   // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
        uint16_t numChannels;     // Number of channels 1=Mono 2=Sterio
        u32 SamplesPerSec; // Sampling Frequency in Hz
        u32 bytesPerSec;   // bytes per second
        uint16_t blockAlign;    // 2=16-bit mono, 4=16-bit stereo
        uint16_t bitsPerSample; // Number of bits per sample

        /* "data" sub-chunk */
        u8 Subchunk2ID[4]; // "data"  string
        u32 Subchunk2Size; // Sampled data length
    };
};

int main()
{
    WAV_HEADER wavHeader = {
        .data = {
            0x52, 0x49, 0x46, 0x46, 0x00, 0x09, 0x6b, 0xdc, 0x57, 0x41, 0x56, 0x45, 0x66, 0x6d, 0x74, 0x20,
            0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x2b, 0x11, 0x00, 0x00, 0xac, 0x44,
            0x00, 0x04, 0x00, 0x10, 0x64, 0x61, 0x74, 0x61, 0x00, 0x09, 0x6b, 0xb8}};

    std::cout << "RIFF header:\t\t\t" << wavHeader.RIFF[0] << wavHeader.RIFF[1] << wavHeader.RIFF[2] << wavHeader.RIFF[3] << std::endl;
    std::cout << "WAVE header:\t\t\t" << wavHeader.WAVE[0] << wavHeader.WAVE[1] << wavHeader.WAVE[2] << wavHeader.WAVE[3] << std::endl;
    std::cout << "FMT:\t\t\t\t" << wavHeader.fmt[0] << wavHeader.fmt[1] << wavHeader.fmt[2] << wavHeader.fmt[3] << std::endl;
    std::cout << "Data size:\t\t\t" << wavHeader.ChunkSize << std::endl;

    // Display the sampling Rate from the header
    std::cout << "Sampling Rate:\t\t\t" << wavHeader.SamplesPerSec << " Hz" << std::endl;
    std::cout << "Number of bits used:\t\t" << wavHeader.bitsPerSample << std::endl;
    std::cout << "Number of channels:\t\t" << wavHeader.numChannels << std::endl;
    std::cout << "Number of bytes per second:\t" << wavHeader.bytesPerSec << std::endl;
    std::cout << "Data length:\t\t\t" << wavHeader.Subchunk2Size << std::endl;
    std::cout << "Audio Format:\t\t\t" << wavHeader.AudioFormat << std::endl;

    std::cout << "Block align:\t\t\t" << wavHeader.blockAlign << std::endl;
    std::cout << "Data string:\t\t\t" << wavHeader.Subchunk2ID[0] << wavHeader.Subchunk2ID[1] << wavHeader.Subchunk2ID[2] << wavHeader.Subchunk2ID[3] << std::endl;
}