#include "FileTransferData.h"
#include "PacketStructs.h"
bool FileTransferData::Initialize(const std::string & fileName, std::string & errMsg)
{
	if (m_transferInProgress) //If transfer already in progress?
	{
		errMsg = "File Transfer already in progress. File Transfer Request denied.";
		return false;
	}

	m_infileStream.open(fileName, std::ios::binary | std::ios::ate); //Open file to read in binary | ate mode. We use ate so we can use tellg to get file size. We use binary because we need to read bytes as raw data
	if (!m_infileStream.is_open()) //If file is not open? (Error opening file?)
	{
		errMsg = "Requested file: " + fileName + " does not exist or was not found.";
		return false;
	}

	m_fileName = fileName; //set file name just so we can print it out after done transferring
	m_fileSize = m_infileStream.tellg(); //Get file size
	m_infileStream.seekg(0); //Set cursor position in file back to offset 0 for when we read file
	m_fileOffset = 0; //Update file offset for knowing when we hit end of file
	m_transferInProgress = true;
}
std::shared_ptr<Packet> FileTransferData::getOutgoingBufferPacket()
{
	int remainingBytes = m_fileSize - m_fileOffset; //calculate remaining bytes
	if (remainingBytes > m_bufferSize) //if remaining bytes > max byte buffer
	{
		PS::FileDataBuffer fileData; //Create FileDataBuffer packet structure
		m_infileStream.read(fileData.m_databuffer, m_bufferSize); //read in max buffer size bytes
		fileData.m_size = m_bufferSize; //Set FileDataBuffer buffer size
		m_fileOffset += m_bufferSize; //increment fileoffset by # of bytes written
		return std::make_shared<Packet>(fileData.toPacket()); //Append fileData packet to packet manager
	}
	else
	{
		PS::FileDataBuffer fileData; //Create FileDataBuffer packet structure
		m_infileStream.read(fileData.m_databuffer, remainingBytes); //read in remaining bytes
		fileData.m_size = remainingBytes; //Set FileDataBuffer remaining bytes
		m_fileOffset += remainingBytes; //increment fileoffset by # of bytes written
		Packet outgoingPacket = fileData.toPacket();
		outgoingPacket.Append(PacketType::FileTransfer_EndOfFile);
		m_transferInProgress = false;
		return std::make_shared<Packet>(outgoingPacket); //Append fileData packet to packet manager
	}
}