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

	u32 firstByte = static_cast<u32>(data[40]);
	u32 secondByte = static_cast<u32>(data[41]);
	u32 thirdByte = static_cast<u32>(data[42]);
	u32 fourthByte = static_cast<u32>(data[43]);

	printf("\n\n%x\n\n", wavHeader.Subchunk2Size);

	printf("%x %x %x %x\n", data[43], data[42], data[41], data[40] & 0xFF);

	std::cout << static_cast<u32>(data[43] << 24 | data[42] << 16 | data[41] << 8 | (data[40] & 0xFF)) << std::endl;

	std::cout << std::endl
			  << std::string(78, '-') << std::endl;

	std::cout << std::setprecision(6);

	if (bytesRead > 0)
	{
		uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;
		uint64_t numSamples = wavHeader.Subchunk2Size / bytesPerSample;
		static const uint16_t DUAL_CHANNEL_SIZE = bytesPerSample;

		std::unique_ptr<i16[]> input = std::unique_ptr<i16[]>(new i16[DUAL_CHANNEL_SIZE]);
		std::unique_ptr<i32[]> buffer = std::unique_ptr<i32[]>(new i32[DUAL_CHANNEL_SIZE]);
		std::unique_ptr<i16[]> result = std::unique_ptr<i16[]>(new i16[DUAL_CHANNEL_SIZE]);

		std::random_device device;
		std::uniform_real_distribution<> dist(0.0, std::nextafter(1.0, std::numeric_limits<float>::max()));

		i32 max_channel_1_pre_noise = -65000;
		i32 max_channel_2_pre_noise = -65000;

		i32 max_channel_1_post_noise = -65000;
		i32 max_channel_2_post_noise = -65000;

		u64 bytes_processed = 0;

		while (wavFile_input.read(reinterpret_cast<char *>(input.get()), 4))
		{

			max_channel_1_pre_noise = std::max(max_channel_1_pre_noise, static_cast<i32>(input[0]));
			max_channel_2_pre_noise = std::max(max_channel_2_pre_noise, static_cast<i32>(input[1]));

			for (u64 index = 0; index < DUAL_CHANNEL_SIZE; ++index)
			{
				auto temp = dist(device);
				buffer[index] = static_cast<i32>(buffer[index] + 300 * temp - 150);

				if (buffer[index] > 0x7FFF)
					result[index] = 0x7FFF;
				else if (buffer[index] < static_cast<i16>(0x8000))
					result[index] = static_cast<i16>(0x8000);
				else
					result[index] = static_cast<i16>(buffer[index]);
			}

			max_channel_1_post_noise = std::max(max_channel_1_post_noise, static_cast<i32>(result[0]));
			max_channel_2_post_noise = std::max(max_channel_2_post_noise, static_cast<i32>(result[1]));

			wavFile_output.write(reinterpret_cast<const char *>(result.get()), sizeof(u32));
			bytes_processed += 4;

			float progress = 0.0;
			while (progress < 1.0)
			{
				int barWidth = 70;

				std::cout << "[";
				int pos = barWidth * (static_cast<f64>(bytes_processed) / static_cast<f64>(wavHeader.Subchunk2Size));
				for (int i = 0; i < barWidth; ++i)
				{
					if (i < pos)
						std::cout << "\033[42m.\033[0m";
					else if (i == pos)
						std::cout << ">";
					else
						std::cout << ".";
				}
				std::cout << "] " << static_cast<i16>((static_cast<f64>(bytes_processed) / static_cast<f64>(wavHeader.Subchunk2Size)) * 100) << " %\r";
				std::cout.flush();

				progress += 4; // for demonstration only
			}
			//std::cout << std::endl;

			//std::cout << "Processing data....." << (static_cast<f64>(bytes_processed) / static_cast<f64>(wavHeader.Subchunk2Size)) * 100 << std::string(10, ' ') << "\r";
			std::cout.flush();
		}

		std::cout << std::endl;

			std::cout << std::string(78, '-') << std::endl;

		auto stop_time = std::chrono::high_resolution_clock::now();
		print_data(wavHeader, std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count(), numSamples);
		print_summary_text_file(wavHeader,
								start_time,
								stop_time,
								numSamples,
								max_channel_1_pre_noise,
								max_channel_2_pre_noise,
								max_channel_1_post_noise,
								max_channel_2_post_noise);
	}

	wavFile_input.close();
	wavFile_output.close();
	return 0;
}