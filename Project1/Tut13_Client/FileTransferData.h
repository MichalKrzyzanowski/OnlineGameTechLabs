#pragma once
#include <fstream> //for std::ifstream and std::ofstream
struct FileTransferData
{
	static const int m_bufferSize = 16384;
	std::string m_fileName; //For when receiving files
	int m_bytesWritten; //For when receiving files
	int m_fileOffset; //For when reading a file
	int m_fileSize; //For when reading a file
	int m_remainingBytes; //For when reading a file
	std::ifstream m_infileStream; //For reading a file that is being sent
	std::ofstream m_outfileStream; //For writing a file that is being received
	bool m_transferInProgress = false;
	char m_buffer[m_bufferSize]; //buffer used for when sending or receiving to optimize constantly reallocating buffers
};