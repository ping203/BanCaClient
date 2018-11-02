#ifndef _MESSAGE_BUFFER_H_
#define _MESSAGE_BUFFER_H_
#include <stdio.h>
#include <string>
#include "cocos2d.h"

class MessageBuffer{
private: 
	int capacity;
	int limit;
protected:
	char* buf;
	int pos;
public:
	MessageBuffer();
	MessageBuffer(int capacity);
	MessageBuffer(const char* data, int size);
	virtual ~MessageBuffer(void);
	MessageBuffer* putByte(int b);
	MessageBuffer* putBytes(const char* bytes, int size);
	MessageBuffer* putByteArray(const char* bytes, int size);
	MessageBuffer* putShort(int v);
	MessageBuffer* putInt(int v);
	MessageBuffer* putLong(long long v);
	MessageBuffer* putString(std::string str);

	void reload(const char* data, int size);
	char* getBytes(int size);
	char getByte();
    int getByteFix();
	short getShort();
	int getUnsignedShort();
	int getInt();
	long long getLong();
	bool getBool();
	long getDouble();
	char* getCharArray(int& size) ;
	std::string getString();
	inline int getCapacity() {
		return capacity;
	};
	inline int getLimit() {
		return limit;
	};
	inline int getPosition() {
		return pos;
	}
	inline void rewind() {
		pos = 0;
	};
};
#endif