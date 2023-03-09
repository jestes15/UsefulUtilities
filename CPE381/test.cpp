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
		return 1;
	}

	// std::cout << "Output wav file: ";
	std::cin >> input_str;

	auto wavFile_output = std::ofstream(input_str, std::ios::binary);
	if (!wavFile_output)
	{
		std::cout << "Unable to open wave file: " << input_str << std::endl;
		return 1;
	}

	auto start_time = std::chrono::high_resolution_clock::now();

	std::size_t bytesRead = wavFile_input.read(reinterpret_cast<char *>(&wavHeader), sizeof(WAV_HEADER)).gcount();

	if (bytesRead == 0)
		return EXIT_FAILURE;

	wavFile_output.write(reinterpret_cast<char *>(&wavHeader), sizeof(WAV_HEADER));

	uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;
	uint64_t numSamples = wavHeader.Subchunk2Size / bytesPerSample;

	std::unique_ptr<i16[]> data = std::unique_ptr<i16[]>(new i16[2]);

	std::random_device device;
	std::uniform_real_distribution<> dist(0.0, std::nextafter(1.0, std::numeric_limits<float>::max()));

	i16 max_channel_1_pre_noise = -32000;
	i16 max_channel_2_pre_noise = -32000;

	i16 max_channel_1_post_noise = -32000;
	i16 max_channel_2_post_noise = -32000;

	i8 channel = 0;
	// 0 = left, 1 = right
	while (wavFile_input.read(reinterpret_cast<char *>(&data[0]), sizeof(i16)))
	{
		i32 temp = data[0] + 300ll * dist(device) - 150;

		if (temp > 32767)
			data[1] = 32767;
		else if (temp < static_cast<i16>(-32768))
			data[1] = static_cast<i16>(-32768);
		else
			data[1] = static_cast<i16>(temp);

		if (channel)
		{
			max_channel_2_pre_noise = std::max(max_channel_2_pre_noise, data[0]);
			max_channel_2_post_noise = std::max(max_channel_2_post_noise, data[1]);
		}
		else
		{
			max_channel_1_pre_noise = std::max(max_channel_1_pre_noise, data[0]);
			max_channel_1_post_noise = std::max(max_channel_1_post_noise, data[1]);
		}

		wavFile_output.write(reinterpret_cast<const char *>(&data[1]), sizeof(i16));
		channel ^= 0x1;
	}
	auto stop_time = std::chrono::high_resolution_clock::now();
	print_summary_text_file_2(wavHeader, start_time, stop_time, numSamples, max_channel_1_pre_noise, max_channel_2_pre_noise, max_channel_1_post_noise, max_channel_2_post_noise);

	wavFile_input.close();
	wavFile_output.close();
	return EXIT_SUCCESS;
}