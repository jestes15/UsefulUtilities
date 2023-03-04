#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <random>
#include <chrono>

// http://soundfile.sapp.org/doc/WaveFormat/
struct WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    uint8_t     RIFF[4];    // RIFF Header Magic header
    uint32_t    ChunkSize; // RIFF Chunk Size
    uint8_t     WAVE[4];    // WAVE Header

    /* "fmt" sub-chunk */
    uint8_t     fmt[4];         // FMT header
    uint32_t    Subchunk1Size; // Size of the fmt chunk
    uint16_t    AudioFormat;   // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t    NumOfChan;     // Number of channels 1=Mono 2=Sterio
    uint32_t    SamplesPerSec; // Sampling Frequency in Hz
    uint32_t    bytesPerSec;   // bytes per second
    uint16_t    blockAlign;    // 2=16-bit mono, 4=16-bit stereo
    uint16_t    bitsPerSample; // Number of bits per sample

    /* "data" sub-chunk */
    uint8_t     Subchunk2ID[4]; // "data"  string
    uint32_t    Subchunk2Size; // Sampled data length
};


void print_data(WAV_HEADER &wavHeader, uint_fast64_t time_delta, uint64_t numSamples) {
    std::cout << wavHeader.RIFF << std::endl;
        std::cout << "RIFF header:\t\t\t" << wavHeader.RIFF[0] << wavHeader.RIFF[1] << wavHeader.RIFF[2] << wavHeader.RIFF[3] << std::endl;
        std::cout << "WAVE header:\t\t\t" << wavHeader.WAVE[0] << wavHeader.WAVE[1] << wavHeader.WAVE[2] << wavHeader.WAVE[3] << std::endl;
        std::cout << "FMT:\t\t\t\t" << wavHeader.fmt[0] << wavHeader.fmt[1] << wavHeader.fmt[2] << wavHeader.fmt[3] << std::endl;
        std::cout << "Data size:\t\t\t" << wavHeader.ChunkSize << std::endl;

        std::cout << "Sampling Rate:\t\t\t" << wavHeader.SamplesPerSec << " Hz" << std::endl;
        std::cout << "Number of bits used:\t\t" << wavHeader.bitsPerSample << std::endl;
        std::cout << "Number of channels:\t\t" << wavHeader.NumOfChan << std::endl;
        std::cout << "Number of bytes per second:\t" << wavHeader.bytesPerSec << std::endl;
        std::cout << "Data length:\t\t\t" << wavHeader.Subchunk2Size << std::endl;
        std::cout << "Audio Format:\t\t\t" << wavHeader.AudioFormat << std::endl;

        std::cout << "Block align:\t\t\t" << wavHeader.blockAlign << std::endl;
        std::cout << "Data string:\t\t\t" << wavHeader.Subchunk2ID[0] << wavHeader.Subchunk2ID[1] << wavHeader.Subchunk2ID[2] << wavHeader.Subchunk2ID[3] << std::endl;

        std::cout << "Number of samples:\t\t" << numSamples << std::endl;

        std::cout << "Total time: " << time_delta << " ns" << std::endl;
        std::cout << "Seconds: " << (numSamples / 2) / wavHeader.SamplesPerSec << " s" << std::endl;
}

void print_summary_text_file(WAV_HEADER &wavHeader, std::chrono::steady_clock::time_point start_time, std::chrono::steady_clock::time_point stop_time, uint64_t numSamples, int16_t max_channel_1, int16_t max_channel_2) {
    std::ofstream test_file("Estes_J_sim.txt", std::ios::out);
    test_file << "Name:\t\t\t\t\t\tJoshua Estes" << std::endl;
    test_file << "File Name:\t\t\t\t\tEstes_J_sim.txt" << std::endl;
    test_file << "Number of samples:\t\t\t" << numSamples << std::endl;
    test_file << "Sampling Frequency:\t\t\t" << wavHeader.ChunkSize << std::endl;
    test_file << "Number of bits per sample:\t" << wavHeader.bitsPerSample << std::endl;
    test_file << "Record length:\t\t\t\t" << numSamples / (wavHeader.NumOfChan * wavHeader.SamplesPerSec) << std::endl;
    test_file << "Maximum absolute value in channel 1:\t" << abs(max_channel_1) << std::endl;
    test_file << "Maximum absolute value in channel 2:\t" << abs(max_channel_2) << std::endl;
    test_file << "Execution time:\t\t\t\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count() << " nanoseconds" << std::endl;
    test_file << "Execution time:\t\t\t\t" << std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time).count() << " milliseconds" << std::endl;
}