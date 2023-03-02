#include "wav_file_parser.hpp"

int main(int argc, char *argv[])
{
    WAV_HEADER wavHeader;
    int headerSize = sizeof(WAV_HEADER);
    std::string input;

    std::cout << "Input wav file: ";
    std::cin >> input;

    //FILE *wavFile = fopen(input.c_str(), "r");
    auto wavFile_input = std::ifstream(input, std::ios::binary);
    if (!wavFile_input)
    {
        std::cout << "Unable to open file: " << input << std::endl;
        return 1;
    }

    std::cout << "Output wav file: ";
    std::cin >> input;

    auto wavFile_output = std::ofstream(input, std::ios::binary);
    if (!wavFile_output)
    {
        std::cout << "Unable to open wave file: " << input << std::endl;
        return 1;
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    // Read the header
    char data[44];
    std::size_t bytesRead = wavFile_input.read(data, headerSize).gcount();
    wavFile_output.write(data, sizeof(data));
    std::cout << "Header Read " << bytesRead << " bytes." << std::endl;

    wavHeader = reinterpret_cast<WAV_HEADER &>(data);

    if (bytesRead > 0)
    {
        uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;
        uint64_t numSamples = wavHeader.Subchunk2Size / bytesPerSample;
        static const uint16_t DUAL_CHANNEL_SIZE = bytesPerSample;

        std::unique_ptr<int16_t[]> buffer = std::unique_ptr<int16_t[]>(new int16_t[DUAL_CHANNEL_SIZE]);
        std::unique_ptr<int16_t[]> result = std::unique_ptr<int16_t[]>(new int16_t[DUAL_CHANNEL_SIZE]);

        std::random_device device;

        std::uniform_real_distribution<> dist(0.0, std::nextafter(1.0, std::numeric_limits<float>::max()));

        int16_t max_channel_1;
        int16_t max_channel_2;

        while (wavFile_input.read(reinterpret_cast<char*>(buffer.get()), 4)) 
        {
            for (uint64_t index = 0; index < DUAL_CHANNEL_SIZE; ++index)
            {
                auto temp = dist(device);
                result[index] = buffer[index] + 300 * temp - 150;
            }

            if (abs(result[0]) > max_channel_1) {
                max_channel_1 = abs(result[0]);
            }
            max_channel_1 = std::max(max_channel_1, static_cast<int16_t>(abs(result[0])));
            max_channel_2 = std::max(max_channel_2, static_cast<int16_t>(abs(result[1])));

            wavFile_output.write(reinterpret_cast<const char *>(result.get()), sizeof(uint32_t));
        }

        auto stop_time = std::chrono::high_resolution_clock::now();
        print_data(wavHeader, std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count(), numSamples);
        print_summary_text_file(wavHeader, start_time, stop_time, numSamples, max_channel_1, max_channel_2);
    }
    
    wavFile_input.close();
    wavFile_output.close();
    return 0;
}