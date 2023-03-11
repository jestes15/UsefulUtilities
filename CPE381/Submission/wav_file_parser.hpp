#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstdint>
#include <random>
#include <chrono>
#include <memory>

typedef float f32;
typedef double f64;

typedef int8_t i8;
typedef uint8_t u8;

typedef int16_t i16;
typedef uint16_t u16;

typedef int32_t i32;
typedef uint32_t u32;

typedef int64_t i64;
typedef uint64_t u64;

// http://soundfile.sapp.org/doc/WaveFormat/
struct WAV_HEADER
{
	/* RIFF Chunk Descriptor */
	u8 RIFF[4];	   // RIFF Header Magic header
	u32 ChunkSize; // RIFF Chunk Size
	u8 WAVE[4];	   // WAVE Header

	/* "fmt" sub-chunk */
	u8 fmt[4];		   // FMT header
	u32 Subchunk1Size; // Size of the fmt chunk
	u16 AudioFormat;   // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
	u16 numChannels;   // Number of channels 1=Mono 2=Sterio
	u32 SamplesPerSec; // Sampling Frequency in Hz
	u32 bytesPerSec;   // bytes per second
	u16 blockAlign;	   // 2=16-bit mono, 4=16-bit stereo
	u16 bitsPerSample; // Number of bits per sample

	/* "data" sub-chunk */
	u8 Subchunk2ID[4]; // "data"  string
	u32 Subchunk2Size; // Sampled data length

	friend std::ostream& operator <<(std::ostream& stream, WAV_HEADER &header) {
		stream << std::endl << "RIFF header:\t\t\t" << header.RIFF[0] << header.RIFF[1] << header.RIFF[2] << header.RIFF[3] << std::endl;
		stream << "FMT:\t\t\t\t" << header.fmt[0] << header.fmt[1] << header.fmt[2] << header.fmt[3] << std::endl;
		stream << "Data size:\t\t\t" << header.ChunkSize << std::endl;
		stream << "Sampling Rate:\t\t\t" << header.SamplesPerSec << " Hz" << std::endl;
		stream << "Number of bits used:\t\t" << header.bitsPerSample << std::endl;
		stream << "Number of channels:\t\t" << header.numChannels << std::endl;
		stream << "Number of bytes per second:\t" << header.bytesPerSec << std::endl;
		stream << "Data length:\t\t\t" << header.Subchunk2Size << std::endl;
		stream << "Audio Format:\t\t\t" << header.AudioFormat << std::endl;
		stream << "Block align:\t\t\t" << header.blockAlign << std::endl;
		stream << "Data string:\t\t\t" << header.Subchunk2ID[0] << header.Subchunk2ID[1] << header.Subchunk2ID[2] << header.Subchunk2ID[3] << std::endl;
		return stream;
	}
};

void print_data(WAV_HEADER &wavHeader, u64 numSamples)
{
	std::cout << wavHeader << "Number of samples:\t\t" << numSamples << std::endl;
}

void print_summary_text_file(WAV_HEADER &wavHeader,
							 std::chrono::system_clock::time_point start_time,
							 std::chrono::system_clock::time_point stop_time,
							 std::string file_name,
							 u64 numSamples,
							 i32 max_channel_1_pre_noise,
							 i32 max_channel_2_pre_noise,
							 i32 max_channel_1_post_noise,
							 i32 max_channel_2_post_noise)
{
	std::ofstream test_file("Estes_J_sum.txt", std::ios::out);
	test_file << std::left << std::setw(50) << "Name:"
			  << "Joshua Estes" << std::endl;
	test_file << std::left << std::setw(50) << "File Name:"
			  << file_name << std::endl;
	test_file << std::left << std::setw(50) << "Number of samples in total:" << numSamples << std::endl;
	// Assumes each sample is comprised of two samples which correspond to a channel
	test_file << std::left << std::setw(50) << "Number of samples:" << numSamples / 2 << std::endl;
	test_file << std::left << std::setw(50) << "Sampling Frequency:" << wavHeader.SamplesPerSec << std::endl;
	test_file << std::left << std::setw(50) << "Number of bits per sample:" << wavHeader.bitsPerSample << std::endl;
	test_file << std::left << std::setw(50) << "Record length:" << numSamples / (wavHeader.numChannels * wavHeader.SamplesPerSec) << std::endl;
	test_file << std::left << std::setw(50) << "Maximum value in channel 1 before noise:" << max_channel_1_pre_noise << std::endl;
	test_file << std::left << std::setw(50) << "Maximum value in channel 2 before noise:" << max_channel_2_pre_noise << std::endl;
	test_file << std::left << std::setw(50) << "Maximum value in channel 1 after noise:" << max_channel_1_post_noise << std::endl;
	test_file << std::left << std::setw(50) << "Maximum value in channel 2 after noise:" << max_channel_2_post_noise << std::endl;
	test_file << std::left << std::setw(50) << "Execution time in nanoseconds:" << std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count() << " nanoseconds" << std::endl;
	test_file << std::left << std::setw(50) << "Execution time in milliseconds:" << std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time).count() << " milliseconds";
}