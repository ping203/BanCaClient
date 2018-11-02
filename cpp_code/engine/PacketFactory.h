#ifndef PacketFactory_h__
#define PacketFactory_h__
#include "Packet.h"
class PacketFactory
{
	public:
		static Packet* analyzeData(const char* data, int length);
		static const char* packPacket(Packet* packet, int& dataLength);
		static bool getBit(unsigned char& input,int index);
		//lay chieu dai thuc cua goi tin
		static int getValidSize(const char* data, int length);
private:
		static char genHeader(bool bigSize, bool compress);
		static void setBit(unsigned char& input,int index,bool hasBit);
		static void putIntAt(char* data,int& index,int value);
		static void putShortAt(char* data,int& index,short value);
		static int getIntAt(const char* data, int index);
		static short getShortAt(const char* data, int index);
};

#endif // PacketFactory_h__
