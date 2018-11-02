#ifndef InPacket_h__
#define InPacket_h__

#include <string>
#include "cocos2d.h"
#include "sPackage.h"

class InPacket  : public Ref
{
public:
	InPacket();
	~InPacket();
	void init(sPackage* pkg);
public:
	int getCmdId();
	int getControllerId();
	int getError();
    
    void clean();
    
public:
	char* getBytes(int size);
	int getByte();
	int getShort();
	unsigned short getUnsignedShort();
	int getInt();
	long long getLong();
	bool getBool();
	double getDouble();
	float getFloat();
	char* getCharArray(int& size);
	std::string getString();
private:
	char parseByte();
	const char* _data;
	int _length;
	int _pos;
	int _cmdId;
	char _controllerId;
	char _error;
};

#endif // InPacket_h__
