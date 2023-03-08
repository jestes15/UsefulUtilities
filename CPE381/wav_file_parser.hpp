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
	u16 NumOfChan;	   // Number of channels 1=Mono 2=Sterio
	u32 SamplesPerSec; // Sampling Frequency in Hz
	u32 bytesPerSec;   // bytes per second
	u16 blockAlign;	   // 2=16-bit mono, 4=16-bit stereo
	u16 bitsPerSample; // Number of bits per sample

	/* "data" sub-chunk */
	u8 Subchunk2ID[4]; // "data"  string
	u32 Subchunk2Size; // Sampled data length
};

void print_data(WAV_HEADER &wavHeader,
				u64 time_delta,
				u64 numSamples)
{
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

void print_summary_text_file(WAV_HEADER &wavHeader,
							 std::chrono::steady_clock::time_point start_time,
							 std::chrono::steady_clock::time_point stop_time,
							 u64 numSamples,
							 i32 max_channel_1_pre_noise,
							 i32 max_channel_2_pre_noise,
							 i32 max_channel_1_post_noise,
							 i32 max_channel_2_post_noise)
{
	std::ofstream test_file("Estes_J_sim.txt", std::ios::out);
	test_file << std::left << std::setw(50) << "Name:"
			  << "Joshua Estes" << std::endl;
	test_file << std::left << std::setw(50) << "File Name:"
			  << "Estes_J_sim.txt" << std::endl;
	// Assumes each sample is comprised of two samples which correspond to a channel
	test_file << std::left << std::setw(50) << "Number of samples:" << numSamples / 2 << std::endl;
	test_file << std::left << std::setw(50) << "Sampling Frequency:" << wavHeader.SamplesPerSec << std::endl;
	test_file << std::left << std::setw(50) << "Number of bits per sample:" << wavHeader.bitsPerSample << std::endl;
	test_file << std::left << std::setw(50) << "Record length:" << numSamples / (wavHeader.NumOfChan * wavHeader.SamplesPerSec) << std::endl;
	test_file << std::left << std::setw(50) << "Maximum value in channel 1 before noise:" << max_channel_1_pre_noise << std::endl;
	test_file << std::left << std::setw(50) << "Maximum value in channel 2 before noise:" << max_channel_2_pre_noise << std::endl;
	test_file << std::left << std::setw(50) << "Maximum value in channel 1 after noise:" << max_channel_1_post_noise << std::endl;
	test_file << std::left << std::setw(50) << "Maximum value in channel 2 after noise:" << max_channel_2_post_noise << std::endl;
	test_file << std::left << std::setw(50) << "Execution time in nanoseconds:" << std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count() << " nanoseconds" << std::endl;
	test_file << std::left << std::setw(50) << "Execution time in milliseconds:" << std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time).count() << " milliseconds";
}