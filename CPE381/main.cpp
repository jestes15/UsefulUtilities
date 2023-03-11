#include "wav_file_parser.hpp"

int main()
{
	WAV_HEADER wavHeader;
	std::string input_str;

	// std::cout << "Input wav file: ";
	std::cin >> input_str;

	auto wavFile_input = std::ifstream(input_str, std::ios::binary);

	if (!wavFile_input)
	{
		std::cout << "Unable to open file: " << input_str << std::endl;
		return EXIT_FAILURE;
	}

	// std::cout << "Output wav file: ";
	std::cin >> input_str;

	auto wavFile_output = std::ofstream(input_str, std::ios::binary);
	if (!wavFile_output)
	{
		std::cout << "Unable to open wave file: " << input_str << std::endl;
		return EXIT_FAILURE;
	}

	auto start_time = std::chrono::high_resolution_clock::now();

	std::size_t bytesRead = wavFile_input.read(reinterpret_cast<char *>(&wavHeader), sizeof(WAV_HEADER)).gcount();

	if (bytesRead == 0)
		return EXIT_FAILURE;

	wavFile_output.write(reinterpret_cast<char *>(&wavHeader), sizeof(WAV_HEADER));

	uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;
	uint64_t numSamples = wavHeader.Subchunk2Size / bytesPerSample;
	static const uint16_t NUMBER_OF_CHANNELS = wavHeader.numChannels;

	std::unique_ptr<i16[]> input = std::unique_ptr<i16[]>(new i16[NUMBER_OF_CHANNELS]);
	std::unique_ptr<i32[]> buffer = std::unique_ptr<i32[]>(new i32[NUMBER_OF_CHANNELS]);
	std::unique_ptr<i16[]> result = std::unique_ptr<i16[]>(new i16[NUMBER_OF_CHANNELS]);

	std::random_device device;
	std::uniform_real_distribution<> dist(0.0, std::nextafter(1.0, std::numeric_limits<float>::max()));

	i32 max_channel_1_pre_noise = -65000;
	i32 max_channel_2_pre_noise = -65000;

	i32 max_channel_1_post_noise = -65000;
	i32 max_channel_2_post_noise = -65000;

	while (wavFile_input.read(reinterpret_cast<char *>(input.get()), 4))
	{
		for (u64 index = 0; index < NUMBER_OF_CHANNELS; ++index)
		{
			auto temp = dist(device);
			buffer[index] = static_cast<i32>(input[index] + 300 * temp - 150);

			if (buffer[index] > 0x7FFF)
				result[index] = 0x7FFF;
			else if (buffer[index] < static_cast<i16>(0x8000))
				result[index] = static_cast<i16>(0x8000);
			else
				result[index] = static_cast<i16>(buffer[index]);
		}

		max_channel_1_pre_noise = std::max(max_channel_1_pre_noise, static_cast<i32>(input[0]));
		max_channel_2_pre_noise = std::max(max_channel_2_pre_noise, static_cast<i32>(input[1]));
		max_channel_1_post_noise = std::max(max_channel_1_post_noise, static_cast<i32>(result[0]));
		max_channel_2_post_noise = std::max(max_channel_2_post_noise, static_cast<i32>(result[1]));

		wavFile_output.write(reinterpret_cast<const char *>(result.get()), sizeof(u32));
	}
	auto stop_time = std::chrono::high_resolution_clock::now();
	print_summary_text_file(wavHeader, start_time, stop_time, numSamples, max_channel_1_pre_noise, max_channel_2_pre_noise, max_channel_1_post_noise, max_channel_2_post_noise);

	wavFile_input.close();
	wavFile_output.close();
	return EXIT_SUCCESS;
}