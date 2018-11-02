#include "PacketFactory.h"

USING_NS_CC;

Packet* PacketFactory::analyzeData(const char* data, int length)
{
	char* lastData;
	int dataSize = 0;
	MessageBuffer* msg = new MessageBuffer(data,length);
	unsigned char header = msg->getByte();
	bool compress = getBit(header,5);
	bool bigSize = getBit(header,3);
	if(bigSize)
		dataSize = msg->getInt();
	else
		dataSize = msg->getShort();
		char *bytes = msg->getBytes(dataSize);
	if(compress){
		unsigned char *deflated;
		unsigned int sizeHint = 256*1024;
		unsigned char *buffer = new unsigned char[dataSize];
		
		memcpy(buffer,bytes,dataSize);
		int inflatedLen = ZipUtils::ccInflateMemoryWithHint(buffer, dataSize, &deflated, sizeHint);
		lastData = new char[inflatedLen];
		dataSize =inflatedLen;
		memcpy(lastData,deflated,dataSize);
		delete buffer;
	}else{
		lastData = new char[dataSize];
		memcpy(lastData,bytes,dataSize);
	}
	Packet* packet = new Packet(lastData,dataSize);
	packet->controllerId = packet->getByte();
	packet->cmdId = packet->getShort();
	delete bytes;
	delete msg;
	delete []lastData;
	return packet;
}
const char* PacketFactory::packPacket(Packet* packet,int& dataLength)
{
	
	bool bigSize = false;
	bool compress = false;
	char header = genHeader(bigSize,compress);

	int size = packet->getPosition()+3;
	int indexData = 1;
	char* dt;
	if(bigSize){
		dt = new char[size+5];
		putIntAt(dt,indexData,size);
	}else{
		dt = new char[size+3];
		putShortAt(dt,indexData,size);
	}
	dt[indexData]=packet->controllerId;
	indexData++;
	putShortAt(dt,indexData,packet->cmdId);
	//header
	dt[0] = header;
	for(int i = 0;i<packet->getPosition();i++){
		dt[indexData+i]=packet->getData()[i];
	}
	dataLength = size+indexData-3;
	return dt;
}
int PacketFactory::getValidSize(const char* data, int length)
{
	unsigned char header = data[0];
	bool bigSize = getBit(header,3);
	int dataSize = 0;
	int addSize = 0;
	if(bigSize)
	{
		if(length < 1 + 4)
			return -1;
		dataSize = getIntAt(data,1);
		addSize = 5;
	}
	else{
		if(length < 1 + 2)
			return -1;
		dataSize = getShortAt(data,1);
		addSize = 3;
	}
	return dataSize+addSize;
}
bool PacketFactory::getBit(unsigned char& input,int index){
	int result = input&(1<<index);
	return (result!=0);
}
char PacketFactory::genHeader(bool bigSize, bool compress)
{
	unsigned char header = 0;
	//set bit dau la binary hay ko
	setBit(header,7,true);
	//bit 2: ko ma hoa
	setBit(header,6,false);
	//bit 3: ko nen 
	setBit(header,5,compress);
	//bit 4: isBlueBoxed?
	setBit(header,4,true);
	//bit 5: isBigSize?
	setBit(header,3,bigSize);
	return header;
}
void PacketFactory::setBit(unsigned char& input,int index,bool hasBit){
	if(hasBit){
		input |= 1 << index;
	}else{
		input &= ~(1 << index);
	}
}
void PacketFactory::putIntAt(char* data,int& index,int value){
	data[index]=((value >> 24) & 0xFF);	
	data[index+1]=((value >> 16) & 0xFF);
	data[index+2]=((value >> 8) & 0xFF);
	data[index+3]=((value >> 0) & 0xFF);
	index+=4;
}
void PacketFactory::putShortAt(char* data,int& index,short value){
	data[index]=((value >> 8) & 0xFF);	
	data[index+1]=((value >> 0) & 0xFF);
	index += 2;
}
int PacketFactory::getIntAt(const char* data, int index)
{
	return ((data[index] & 255) << 24) +
		((data[index+1] & 255) << 16) +
		((data[index+2] & 255) << 8) +
		((data[index+3] & 255) << 0);
}
short PacketFactory::getShortAt(const char* data, int index)
{
	return (short) ((data[index] << 8) + (data[index + 1] & 255));
}