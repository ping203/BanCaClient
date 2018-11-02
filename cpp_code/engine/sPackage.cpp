
#include "sPackage.h"
#include "PacketHeaderAnalyze.h"

USING_NS_CC;

sPackage::sPackage()
{
    _size = 0;
    packageType = kTypeData;
    _data = NULL;
    retain();
}
sPackage::~sPackage()
{
}

bool sPackage::process(std::vector<char> buff, int size)
{
    const char* data = buff.data();
    int dataSize = PacketHeaderAnalyze::getDataSize(data);
    bool compress = PacketHeaderAnalyze::isCompress(data);
    bool bigSize = PacketHeaderAnalyze::isBigSize(data);
    
    int headerSize = NORMAL_HEADER_SIZE;
    if(bigSize)
        headerSize = BIG_HEADER_SIZE;
    if (compress){
        unsigned char *deflated;
        unsigned int sizeHint = 256 * 1024;
        unsigned char *buffer = (unsigned char *)&buff[headerSize];
        
        int inflatedLen = ZipUtils::ccInflateMemoryWithHint(buffer, dataSize, &deflated, sizeHint);
        _data = new char[inflatedLen];
        _size = inflatedLen;
        memcpy(_data, deflated, _size);
    }
    else
    {
        _size = dataSize;
        _data = new char[_size];
        memcpy(_data, data + headerSize, _size);
    }
    return true;
}
void sPackage::clean()
{
    release();
}