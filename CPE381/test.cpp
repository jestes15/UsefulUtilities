//Phase I 
//Hugh Vessels



#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <climits>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>


using namespace std;
using namespace std::chrono;

struct wav_header
{
	char chunkID[4];
    
	int32_t chunkSize;
	int32_t fmat;
	
	char SubChunk1ID[4];
	
	int32_t SubChunk1Size;
	
	int16_t Audio_fmat;
	
	int16_t num_channels;
	
	int32_t Fs;    //sample rate
	
	int32_t byteRate;
	
	int16_t blockAlign;
	
	int16_t bitsPerSample;
	
	char SubChunk2ID[4];
	
	int32_t SubChunk2Size;
	
};
int main(int argc, char* argv[])
{
	auto start = high_resolution_clock::now();
	
	char input_file_name[1000];
	//char buffer[100];
	//char h_buffer[4];
	char header[44];
	
	FILE * iFile;
	FILE * outWavFile;
	
	cout << "Type the name of your input file: " << endl;
	cin >> input_file_name;
	
	
	iFile = fopen(input_file_name, "rb");
	outWavFile = fopen("project_output.wav","wb");
	
	
	
	if(iFile == NULL)
	{
		cout << "File failed to open" << endl;
		return 1;
	}
	if(outWavFile == NULL)
	{
		cout << "Output wav file failed to open" << endl;
		return 1;
	}
	float left_c;
	float right_c;
	
	
	struct wav_header wh1;
	
	fread(&header, 1, sizeof(header), iFile);
	
	wh1 = reinterpret_cast<wav_header &>(header);
	
	//cout << endl << header << endl;
	//wh1.chunkID = header[]; 
	cout << endl << wh1.chunkID << endl;
	
	
	//char chunkID[10];
	//fread(&chunkID, sizeof(chunkID), 1, iFile);
	//fwrite(&chunkID, sizeof(chunkID), 1, outWavFile);
	cout << "Chunk ID " << wh1.chunkSize << " " << endl << endl;
	
	 //long int chunkSize;
	//fread(&chunkSize, sizeof(chunkSize), 1, iFile);
	//fwrite(&chunkSize, sizeof(chunkSize), 1, outWavFile);
	cout << "Chunk Size " << wh1.fmat << " " << endl  << endl;
	
	 //long int Format;
	//fread(&Format, sizeof(Format), 1, iFile);
	//fwrite(&Format, sizeof(Format), 1, outWavFile);
	cout << "Format " << wh1.SubChunk1ID << " " << endl << endl;
	
	 //long int subChunk1ID;
	//fread(&subChunk1ID, sizeof(subChunk1ID), 1, iFile);
	//fwrite(&subChunk1ID, sizeof(subChunk1ID), 1, outWavFile);
	cout << "Sub Chunk 1 ID " << wh1.SubChunk1Size << " " << endl  << endl;
	
	 //long int subChunk1size;
	//fread(&subChunk1size, sizeof(subChunk1size), 1, iFile);
	//fwrite(&subChunk1size, sizeof(subChunk1size), 1, outWavFile);
	cout << "Sub Chunk 1 Size " << wh1.Audio_fmat << " " << endl << endl;
	
	 //int audio_Format;
	//fread(&audio_Format, sizeof(audio_Format), 1, iFile);
	//fwrite(&audio_Format, sizeof(audio_Format), 1, outWavFile);
	cout << "Audio Format " << wh1.num_channels << " " << endl << endl;
	
	 //int num_channels;
	//fread(&num_channels, sizeof(num_channels), 1, iFile);
	//fwrite(&num_channels, sizeof(num_channels), 1, outWavFile);
	cout << "Number of channels " << wh1.Fs << " " << endl << endl;
	
	 //long int sampleRate;
	//fread(&sampleRate, sizeof(sampleRate), 1, iFile);
	//fwrite(&sampleRate, sizeof(sampleRate), 1, outWavFile);
	cout << "Sample Rate " << wh1.byteRate << " " << endl  << endl;
	
	 //long int byteRate;
	//fread(&byteRate, sizeof(byteRate), 1, iFile);
	//fwrite(&byteRate, sizeof(byteRate), 1, outWavFile);
	cout << "Byte rate " << wh1.blockAlign << " " << endl << endl;
	
	 //int blockAlign;
	//fread(&blockAlign, sizeof(blockAlign), 1, iFile);
	//fwrite(&blockAlign, sizeof(blockAlign), 1, outWavFile);
	cout << "Block Align " << wh1.bitsPerSample << " " << endl << endl;
	
	// int bits_per_sample;
	//fread(&bits_per_sample, sizeof(bits_per_sample), 1, iFile);
	//fwrite(&bits_per_sample, sizeof(bits_per_sample), 1, outWavFile);
	cout << "Bits per sample " << wh1.SubChunk2ID << " " << endl << endl;
	
	 //long int subChunk2ID;
	//fread(&subChunk2ID, sizeof(subChunk2ID), 1, iFile);
	//fwrite(&subChunk2ID, sizeof(subChunk2ID), 1, outWavFile);
	cout << "Sub Chunk 2 ID " << wh1.SubChunk2Size << " " << endl << endl;
	
	 //int subChunk2size; 
	//fread(&subChunk2size, sizeof(subChunk2size), 1, iFile);
	//fwrite(&subChunk2size, sizeof(subChunk2size), 1, outWavFile);
	//cout << "Sub Chunk 2 Size " << wh1.fmat << " " << endl << endl;
	

	
	
	
	int n_left;
	int n_right;
	float count = 0;
	while(!feof(iFile))
	{
		fread(&left_c, sizeof(left_c), 1, iFile);
		if(left_c > 32767)
		{
			left_c = 32767;
		}
		else if(left_c < -32768)
		{
			left_c = -32768;
		}
		//cout  << "last left sample"<< left_c << endl;
		
		
		fread(&right_c, sizeof(right_c), 1, iFile);
		if(right_c > 32767)
		{
			right_c = 32767;
		}
		else if(right_c < -32768)
		{
			right_c = -32768;
		}
		//cout  << "last right sample"<< right_c << endl;
		
		
		left_c = left_c + 300*rand()/RAND_MAX - 150;
		
		n_left = (int) left_c;
		//fwrite();
		fwrite(&n_left, sizeof(n_left), 1, outWavFile);
		
		
		right_c = right_c + 300*rand()/RAND_MAX - 150;
		
		n_right = (int) right_c;
		fwrite(&n_right, sizeof(n_right), 1, outWavFile);
		
		
		count = count + 2;
		
	}
	
	
	//cout  << "last left sample"<< n_left << endl;
	//cout  << "last right sample"<< n_right << endl;
	
	fclose(iFile);
	fclose(outWavFile);
	
	cout << endl << "Number of samples in data portion: "  << count << endl << endl;
	
	auto stop = high_resolution_clock::now();
	
	
	
	
	auto duration = duration_cast<microseconds>(stop-start);
	
	
	cout << "duration: " << duration.count() << " microseconds" << endl;
	return 0;
}