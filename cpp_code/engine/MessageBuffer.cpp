#include "MessageBuffer.h"
using namespace std;
MessageBuffer::MessageBuffer(){
//	capacity = 1000;
	pos = 0;
//	buf = new char[capacity];
	buf = NULL;
}
MessageBuffer::MessageBuffer(int capacity){
	pos = 0;
	this->capacity = capacity;
	buf = new char[capacity];
}
MessageBuffer::MessageBuffer(const char* data, int size){
	
	pos = 0;
	capacity = size;
	limit = size;
	buf = new char[capacity];
	memcpy(buf,data,size);
}
void MessageBuffer::reload(const char* data, int size){
	if(buf)
		delete[] buf;
	pos = 0;
	capacity = size;
	limit = size;
	buf = new char[capacity];
	memcpy(buf,data,size);
}
MessageBuffer::~MessageBuffer(void){
	if(buf)
		delete []buf;
	buf=NULL;
};
MessageBuffer* MessageBuffer::putByte(int b){
	if(pos >= capacity)
	{
		//cocos2d::CCLog("putByte");
	}
	CCAssert(pos < capacity, "IndexOutOfBoundsException");
	buf[pos++] = (char) b;
	limit = (pos == capacity ? pos : pos + 1);
	return this;
}
MessageBuffer* MessageBuffer::putByteArray(const char* bytes, int size){
	CCAssert(pos + 2 + size <= capacity, "IndexOutOfBoundsException");
	putShort(size);
	putBytes(bytes,size);
	return this;
}

MessageBuffer* MessageBuffer::putBytes(const char* bytes, int size){
	CCAssert(pos +size <= capacity, "IndexOutOfBoundsException");
	for (int i = 0; i < size; i++) {
		putByte(bytes[i]);
	}
	return this;
}

MessageBuffer* MessageBuffer::putShort(int v) {
	CCAssert(pos +2 <= capacity, "IndexOutOfBoundsException");
	putByte((v >> 8) & 0xFF);
	putByte((v >> 0) & 0xFF);
	return this;
}
MessageBuffer* MessageBuffer::putInt(int v) {
	CCAssert(pos +4 <= capacity, "IndexOutOfBoundsException");
	putByte((v >> 24) & 0xff);
	putByte((v >> 16) & 0xff);
	putByte((v >> 8) & 0xff);
	putByte((v >> 0) & 0xff);
	return this;
}
MessageBuffer* MessageBuffer::putLong(long long v) {
	char* byteArray = reinterpret_cast<char*>(&v);
	int i;
	for(i = 0; i<8; i++)
	{
		putByte(byteArray[7-i]);
	}
	return this;
}
MessageBuffer* MessageBuffer::putString(string str) {
		// Note: code adapted from java.io.DataOutputStream.writeUTF
		int size = str.size();
		CCAssert(pos +size <= capacity, "IndexOutOfBoundsException");
		putByteArray(str.c_str(),size);
		return this;
	}
char MessageBuffer::getByte() {
	CCAssert(pos < limit, "IndexOutOfBoundsException");
	char b = buf[pos++];
	return b;
}

int MessageBuffer::getByteFix() {
    CCAssert(pos < limit, "IndexOutOfBoundsException");
    char b = buf[pos++];
    int a = b;
    if (a > 200)
    {
        a -= 256;
    }
#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8
    return b;
#endif
    return a;
}

bool MessageBuffer::getBool() {
	CCAssert(pos < limit, "IndexOutOfBoundsException");
	char b = buf[pos++];
	return b > 0;
}
char* MessageBuffer::getBytes(int size) {
	CCAssert(pos + size <= limit, "IndexOutOfBoundsException");
	char *bytes = new char[size];
	for (int i = 0; i < size; i++) {
		bytes[i] = getByte();
	}
	return bytes;
}
short MessageBuffer::getShort() {
	CCAssert(pos + 2 <= limit, "IndexOutOfBoundsException");
	if (pos + 2 > limit) {
		return 0;
	}
	return (short) ((getByte() << 8) + (getByte() & 255));
}
int MessageBuffer::getUnsignedShort() {
	CCAssert(pos + 2 <= limit, "IndexOutOfBoundsException");
	int a = (getByte() & 255) << 8;
	int b = (getByte() & 255) << 0;
	return a + b;
	// return ((getByte() & 255) << 8) + ((getByte() & 255) << 0);
}
int MessageBuffer::getInt() {
	CCAssert(pos + 4 <= limit, "IndexOutOfBoundsException");
	return ((getByte() & 255) << 24) +
		((getByte() & 255) << 16) +
		((getByte() & 255) << 8) +
		((getByte() & 255) << 0);
}
long long MessageBuffer::getLong(){
	CCAssert(pos + 8 <= limit, "IndexOutOfBoundsException");
	return ((long long) (getByte() & 255) << 56) +
		((long long) (getByte() & 255) << 48) +
		((long long) (getByte() & 255) << 40) +
		((long long) (getByte() & 255) << 32) +
		((long long) (getByte() & 255) << 24) +
		((long long) (getByte() & 255) << 16) +
		((long long) (getByte() & 255) << 8) +
		((long long) (getByte() & 255) << 0);
}

char* MessageBuffer::getCharArray(int& size) {
	size = getUnsignedShort();
	return getBytes(size);
}
string MessageBuffer::getString(){
	int size =0;
	char* out = getCharArray( size);
	std::string str(out,size);
	delete []out;
	return str;
}
