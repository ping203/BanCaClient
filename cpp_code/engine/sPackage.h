#ifndef sPackage_h__
#define sPackage_h__

#include "cocos2d.h"
#include <vector>
/*
package type
*/
USING_NS_CC;
enum PackageType
{
	kTypeConnectionOpened = 0,
	kTypeData,
	kTypeConnectionClosed
};

class sPackage : public Ref
{
public:
	int _size;
	PackageType packageType;
	char* _data;
	bool result;
    void clean();
public:
	sPackage();
	~sPackage();
	bool process(std::vector<char> buff, int size);
};
#endif // sPackage_h__
