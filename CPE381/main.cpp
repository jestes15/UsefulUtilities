#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <random>
#include <chrono>

// http://soundfile.sapp.org/doc/WaveFormat/
typedef struct WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    uint8_t RIFF[4];    // RIFF Header Magic header
    uint32_t ChunkSize; // RIFF Chunk Size
    uint8_t WAVE[4];    // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t fmt[4];         // FMT header
    uint32_t Subchunk1Size; // Size of the fmt chunk
    uint16_t AudioFormat;   // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t NumOfChan;     // Number of channels 1=Mono 2=Sterio
    uint32_t SamplesPerSec; // Sampling Frequency in Hz
    uint32_t bytesPerSec;   // bytes per second
    uint16_t blockAlign;    // 2=16-bit mono, 4=16-bit stereo
    uint16_t bitsPerSample; // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t Subchunk2ID[4]; // "data"  string
    uint32_t Subchunk2Size; // Sampled data length
} wav_hdr;

// Function prototypes
int getFileSize(FILE *inFile);

int main(int argc, char *argv[])
{
    wav_hdr wavHeader;
    int headerSize = sizeof(wav_hdr), filelength = 0;

    std::string input;
    std::cout << "Input wave file name: ";
    std::cin >> input;
    std::cin.get();

    FILE *wavFile = fopen(input.c_str(), "r");
    if (wavFile == nullptr)
    {
        std::cout << "Unable to open wave file: " << input << std::endl;
        return 1;
    }

    std::cout << "Output wav file: ";
    std::cin >> input;
    std::cin.get();

    auto wavFile_output = std::ofstream(input, std::ios::binary);
    if (!wavFile_output)
    {
        std::cout << "Unable to open wave file: " << input << std::endl;
        return 1;
    }

    auto start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // Read the header
    std::size_t bytesRead = fread(&wavHeader, 1, headerSize, wavFile);
    wavFile_output.write(reinterpret_cast<const char *>(&wavHeader), sizeof(wavHeader));
    std::cout << "Header Read " << bytesRead << " bytes." << std::endl;

    if (bytesRead > 0)
    {
        // Read the data
        uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;      // Number     of bytes per sample
        uint64_t numSamples = wavHeader.ChunkSize / bytesPerSample; // How many samples are in the wav file?
        static const uint16_t BUFFER_SIZE = bytesPerSample;

        std::unique_ptr<int8_t[]> buffer = std::unique_ptr<int8_t[]>(new int8_t[BUFFER_SIZE]);
        std::unique_ptr<int8_t[]> result = std::unique_ptr<int8_t[]>(new int8_t[BUFFER_SIZE]);

        std::random_device device;
        std::uniform_real_distribution<> dist(0.0, std::nextafter(1.0, std::numeric_limits<float>::max()));

        while ((bytesRead = fread(buffer.get(), sizeof(buffer[0]), BUFFER_SIZE / (sizeof(buffer[0])), wavFile)) > 0)
        {
            for (uint64_t index = 0; index < BUFFER_SIZE / (sizeof(buffer[0])); ++index)
            {
                auto temp = dist(device);
                result[index] = buffer[index] + 300 * temp - 150;
            }
            wavFile_output.write(reinterpret_cast<const char *>(result.get()), sizeof(uint16_t));
        }
        auto stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        filelength = getFileSize(wavFile);

        std::cout << "File is:\t\t\t" << filelength << " bytes." << std::endl;
        std::cout << "RIFF header:\t\t\t" << wavHeader.RIFF[0] << wavHeader.RIFF[1] << wavHeader.RIFF[2] << wavHeader.RIFF[3] << std::endl;
        std::cout << "WAVE header:\t\t\t" << wavHeader.WAVE[0] << wavHeader.WAVE[1] << wavHeader.WAVE[2] << wavHeader.WAVE[3] << std::endl;
        std::cout << "FMT:\t\t\t\t" << wavHeader.fmt[0] << wavHeader.fmt[1] << wavHeader.fmt[2] << wavHeader.fmt[3] << std::endl;
        std::cout << "Data size:\t\t\t" << wavHeader.ChunkSize << std::endl;

        // Display the sampling Rate from the header
        std::cout << "Sampling Rate:\t\t\t" << wavHeader.SamplesPerSec << " Hz" << std::endl;
        std::cout << "Number of bits used:\t\t" << wavHeader.bitsPerSample << std::endl;
        std::cout << "Number of channels:\t\t" << wavHeader.NumOfChan << std::endl;
        std::cout << "Number of bytes per second:\t" << wavHeader.bytesPerSec << std::endl;
        std::cout << "Data length:\t\t\t" << wavHeader.Subchunk2Size << std::endl;
        std::cout << "Audio Format:\t\t\t" << wavHeader.AudioFormat << std::endl;
        // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM

        std::cout << "Block align:\t\t\t" << wavHeader.blockAlign << std::endl;
        std::cout << "Data string:\t\t\t" << wavHeader.Subchunk2ID[0] << wavHeader.Subchunk2ID[1] << wavHeader.Subchunk2ID[2] << wavHeader.Subchunk2ID[3] << std::endl;

        std::cout << "Number of samples:\t\t" << numSamples << std::endl;

        std::cout << "Total time: " << stop_time - start_time << " ms" << std::endl;
    }
    fclose(wavFile);
    return 0;
}

// find the file size
int getFileSize(FILE *inFile)
{
    int fileSize = 0;
    fseek(inFile, 0, SEEK_END);

    fileSize = ftell(inFile);

    fseek(inFile, 0, SEEK_SET);
    return fileSize;
}