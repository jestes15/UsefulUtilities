#include "wav_file_parser.hpp"

int main(int argc, char *argv[])
{
    WAV_HEADER wavHeader;
    int headerSize = sizeof(WAV_HEADER);
    std::string input;

    std::cout << "Input wav file: ";
    std::cin >> input;

    // FILE *wavFile = fopen(input.c_str(), "r");
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

    wavHeader = reinterpret_cast<WAV_HEADER &>(data);

    if (bytesRead > 0)
    {
        uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;
        uint64_t numSamples = wavHeader.Subchunk2Size / bytesPerSample;
        static const uint16_t DUAL_CHANNEL_SIZE = bytesPerSample;

        std::unique_ptr<int16_t[]> input = std::unique_ptr<int16_t[]>(new int16_t[DUAL_CHANNEL_SIZE]);
        std::unique_ptr<int32_t[]> buffer = std::unique_ptr<int32_t[]>(new int32_t[DUAL_CHANNEL_SIZE]);
        std::unique_ptr<int16_t[]> result = std::unique_ptr<int16_t[]>(new int16_t[DUAL_CHANNEL_SIZE]);

        std::random_device device;
        std::uniform_real_distribution<> dist(0.0, std::nextafter(1.0, std::numeric_limits<float>::max()));

        int32_t max_channel_1_pre_noise = -65000;
        int32_t max_channel_2_pre_noise = -65000;

        int32_t max_channel_1_post_noise = -65000;
        int32_t max_channel_2_post_noise = -65000;

        while (wavFile_input.read(reinterpret_cast<char *>(input.get()), 4))
        {

            max_channel_1_pre_noise = std::max(max_channel_1_pre_noise, abs(input[0]));
            max_channel_2_pre_noise = std::max(max_channel_2_pre_noise, abs(input[1]));

            for (uint64_t index = 0; index < DUAL_CHANNEL_SIZE; ++index)
            {
                auto temp = dist(device);
                buffer[index] = static_cast<int32_t>(buffer[index] + 300 * temp - 150);

                if (buffer[index] > 0x7FFF)
                    result[index] = 0x7FFF;
                else if (buffer[index] < static_cast<int16_t>(0x8000))
                    result[index] = static_cast<int16_t>(0x8000);
                else
                    result[index] = static_cast<int16_t>(buffer[index]);
            }

            max_channel_1_post_noise = std::max(max_channel_1_post_noise, static_cast<int32_t>(result[0]));
            max_channel_2_post_noise = std::max(max_channel_2_post_noise, static_cast<int32_t>(result[1]));

            wavFile_output.write(reinterpret_cast<const char *>(result.get()), sizeof(uint32_t));
        }

        auto stop_time = std::chrono::high_resolution_clock::now();
        print_data(wavHeader, std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count(), numSamples);
        print_summary_text_file(wavHeader, start_time, stop_time, numSamples, max_channel_1_pre_noise, max_channel_2_pre_noise, max_channel_1_post_noise, max_channel_2_post_noise);
    }

    wavFile_input.close();
    wavFile_output.close();
    return 0;
}