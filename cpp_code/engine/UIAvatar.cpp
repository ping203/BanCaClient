#include "UIAvatar.h"

using namespace std;

UIAvatar::UIAvatar(void) : _downloading(false),_img(NULL),downloader(NULL)
{

}


UIAvatar::~UIAvatar(void)
{
if(downloader)
{
	downloader->_callback = nullptr;
}
	
}


UIAvatar *UIAvatar::create(string id,string url,string defaultImg)
{
	UIAvatar *ret = new UIAvatar();
	ret->init();
	ret->autorelease();
	ret->initWithUrl(id,url,defaultImg);

	return ret;
}

UIAvatar* UIAvatar::create(string defaultImg)
{
	UIAvatar *ret = new UIAvatar();
	ret->init();
	ret->autorelease();
	ret->initUI(defaultImg);

	return ret;
}

string UIAvatar::createScreenshot(string name,string logostr)
{
	CCSize visibleSize = Director::getInstance()->getWinSize();
	RenderTexture* texture = RenderTexture::create((int)visibleSize.width, (int)visibleSize.height);

	Sprite *logo = Sprite::create(logostr.c_str());
	logo->setPosition(ccp(73,visibleSize.height - 80));
	logo->setScale(.65f);
	logo->setZOrder(10);
	Director::getInstance()->getRunningScene()->addChild(logo);

	texture->setPosition(ccp(visibleSize.width/2, visibleSize.height/2));
	texture->begin();
	Director::getInstance()->getRunningScene()->visit();
	texture->end();
	string path = "";
	bool result = texture->saveToFile(name, kCCImageFormatPNG);
	if (result)
	{
		CCLOG("save    screenshot");
		path = FileUtils::getInstance()->getWritablePath() + name;
	}
	logo->removeFromParentAndCleanup(true);
	delete texture;
	return path;

}

UIAvatar* UIAvatar::createWithMask(string defaultImg,string pathMask,string extraImg)
{
	UIAvatar *ret = new UIAvatar();
	ret->init();
	ret->autorelease();
	ret->initUI(defaultImg,pathMask,extraImg);

	return ret;
}

void UIAvatar::initUI(string defaultImg)
{
	_id = "";
	_defaultImg = defaultImg;
	_img = Sprite::create(_defaultImg.c_str());
	Node::addChild(_img,-1,-1);

	//this->updateChildrenDisplayedRGBA();

	//updateContentSizeWithTextureSize(_img->getContentSize());

	//setImage(defaultImg);
}

void UIAvatar::initUI(string defaultImg,string pathMask,string extraImg)
{
	_id = "";
	_defaultImg = defaultImg;
	_img = Sprite::create(_defaultImg.c_str());
	
	//setImage(path);

	ClippingNode* mCliper = ClippingNode::create();
	mCliper->setContentSize(_img->getContentSize());
	mCliper->setStencil(_img);
	mCliper->setAnchorPoint(ccp(0,0));

	ClippingNode *holesClipper = ClippingNode::create();
	// holesClipper->setInverted(0);
	holesClipper->setAlphaThreshold( 0.1 );
	holesClipper->addChild(_img);
	Sprite *holeStencil = Sprite::create(pathMask.c_str());
	holeStencil->setPosition(ccp(0,0) );

	holesClipper->setStencil( holeStencil);

	mCliper->addChild(holesClipper);
	Node::addChild(mCliper,-1,-1);

	if (extraImg.compare("") != 0)
	{
		Node::addChild(Sprite::create(extraImg.c_str()),1);
	}

}

void UIAvatar::initWithUrl(string id,string url,string defaultImg)
{
	_id = id;
	_url = url;
	_defaultImg = defaultImg;

	_img = Sprite::create(_defaultImg.c_str());
	Node::addChild(_img,-1,-1);

	//updateAnchorPoint();
	//updateRGBAToRenderer(_img);
}

void UIAvatar::setOpacity(GLubyte opac)
{
	_img->setOpacity(opac);
}

CCSize UIAvatar::getImageSize()
{
	return _img->getContentSize();
}

void UIAvatar::setDefaultImage()
{
	_img->setTexture(TextureCache::sharedTextureCache()->addImage(_defaultImg.c_str()));
}

void UIAvatar::setTexture(Texture2D *texture)
{
	_img->setTexture(texture);
}

std::string UIAvatar::filePathFromUrl( const char* id)
{
	std::string dir = CCFileUtils::sharedFileUtils()->getWritablePath();
	char buff[200];
	sprintf(buff,"%s%s%s",dir.c_str(),id,".png");
	return buff;
}


bool replaceAlls(std::string& str,const std::string& from, const std::string& to){
	if(from.empty())
		return false;
	size_t start_pos = 0;
	while((start_pos = str.find(from,start_pos))!= std::string::npos){
		str.replace(start_pos,from.length(),to);
		start_pos +=to.length();
	}
}

void UIAvatar::asyncExecute()
{
	if(_url.compare("") == 0)
	{
		setDefaultImage();
		return;
	}
	std::string strUrl = _url;
	//if(strUrl.find("https://") != std::string::npos) {
	//	replaceAlls(strUrl, "https://", "https://");
	//}

	

	string filecheck = filePathFromUrl(_id.c_str());

	bool isExistFile =  CCFileUtils::sharedFileUtils()->isFileExist(filecheck);
	//download file ve
	if(!isExistFile && (strcmp("",strUrl.c_str()) != 0))
	{
		downloader = AsyncDownloader::create(strUrl.c_str(),filecheck,CC_CALLBACK_2(UIAvatar::callbackDownload,this));
		downloader->startDownload();
		_downloading = true;
	}
	else
	{
		setImage(filecheck);
	}
	
}

void UIAvatar::asyncExecuteWithUrl(string id,string url)
{
	if (!_downloading)
	{
		_id = id;
		_url = url;
		asyncExecute();
	}
}

void UIAvatar::setImage(string path)
{
	_img->setTexture(TextureCache::sharedTextureCache()->addImage(path.c_str()));
}


void UIAvatar::callbackDownload(int ret,string path)
{
	_downloading = false;
	downloader = NULL;
	if (ret == 0)
	{
		setImage(path);
		CCLOG("download avatar : %s",path.c_str());
	}
	else
	{
		setDefaultImage();
	}
}


CircleMove* CircleMove::create(float duration, float radius)
{
    CircleMove *pRet = new CircleMove();
    pRet->initWithDuration(duration,radius);
    pRet->autorelease();
    
    return pRet;
    
    
}

void CircleMove::initWithDuration(float duration, float radius)
{
    if(ActionInterval::initWithDuration(duration))
    {
        this->radius = radius;
    }
}

void CircleMove::startWithTarget(cocos2d::Node *target)
{
    ActionInterval::startWithTarget(target);
    Size winSize = Director::getInstance()->getWinSize();
    _target->setPositionX(winSize.width/2 - radius);
}

void CircleMove::update(float time)
{
    Size winSize = Director::getInstance()->getWinSize();
    float alpha = MATH_PIOVER2 + time * 2 * MATH_PIOVER2;
//    if(fabs(alpha - 2 * MATH_PIOVER2) < .5)
//    {
//        alpha = 2 * MATH_PIOVER2 + .5;
//    }
    if(time < 0.35)
    {
        _target->setPositionX(winSize.width/2 - cos(alpha) * radius - radius);
    }
	else if((time >= 0.35) && (time < 0.65))
	{
		float x = winSize.width/2 - cos(MATH_PIOVER2 + 0.35 * 2 * MATH_PIOVER2) * radius - radius;
		float x2 = winSize.width/2 + cos(MATH_PIOVER2 + 0.65 * 2 * MATH_PIOVER2) * radius + radius;

		float dx = fabs(x2-x);
		//CCLOG("%f  %f  %f",x,x2,dx);
		_target->setPositionX(x + (dx / 0.3) * (time-0.35) );
		
	}
    else
    {
        _target->setPositionX(winSize.width/2 + radius +  cos(alpha) * radius);
    }

    
}

TimeProgressEffect::TimeProgressEffect(void)
{
	_timer = NULL;
	startPercent = 0;
	totalTime =0;
	_nen = NULL;
}


TimeProgressEffect::~TimeProgressEffect(void)
{
}

void TimeProgressEffect::setNen(Sprite *sprite)
{

	_nen = sprite;
}

TimeProgressEffect *TimeProgressEffect::create(ProgressTimer *timer,float duration,float percentFrom)
{
	TimeProgressEffect *_timeEff = new TimeProgressEffect();
	_timeEff->initWithDuration(duration,percentFrom * 100,0);
	_timeEff->autorelease();
	_timeEff->_timer = timer;
	_timeEff->startPercent = percentFrom;
	_timeEff->duration = duration;
	_timeEff->totalTime = duration / percentFrom;

	/*
	float checkMau = 1 - percentFrom; 
	if (checkMau <= .5f)
	{
		GLubyte r = (GLubyte)(checkMau * 510);
		ccColor3B color = ccc3(r,255,0);
		_timeEff->_timer->setColor(color);
	}
	else
	{
		GLubyte r = (GLubyte)(255 - (checkMau - .5f) * 510);
		ccColor3B color = ccc3(255,r,0);
		_timeEff->_timer->setColor(color);
	}
	if (_timeEff->_nen != NULL)
	{
		_timeEff->_nen->setOpacity(255);
		_timeEff->_nen->setColor(_timeEff->_timer->getColor());
	}
	*/
	return _timeEff;
}

void TimeProgressEffect::update(float time)
{
	ProgressFromTo::update(time);
	
	/*
	if (_timer!= NULL)
	{
		float checkTime = (1 - startPercent) + time * startPercent;
		if (checkTime <= .5f)
		{
			GLubyte r = (GLubyte)(checkTime * 510);
			ccColor3B color = ccc3(r,255,0);
			_timer->setColor(color);
		}
		else
		{
			GLubyte r = (GLubyte)(255 - (checkTime - .5f) * 510);
			ccColor3B color = ccc3(255,r,0);
			_timer->setColor(color);
		}
		if (_nen != NULL)
		{
			_nen->setOpacity((GLubyte)((1 - time) * 255) );
			_nen->setColor(_timer->getColor());
		}
	}
	*/
}



MoveCircle* MoveCircle::create(float duration, float radius,float alphaBegin,float alphaEnd)
{
	MoveCircle *pRet = new MoveCircle();
	pRet->initWithDuration(duration,radius,alphaBegin,alphaEnd);
	pRet->autorelease();

	return pRet;


}

void MoveCircle::initWithDuration(float duration, float radius,float alphaBegin,float alphaEnd)
{
	if(ActionInterval::initWithDuration(duration))
	{
		this->radius = radius;
		this->alphaBegin = alphaBegin;
		this->alphaEnd = alphaEnd;
	}
}

void MoveCircle::startWithTarget(cocos2d::Node *target)
{
	ActionInterval::startWithTarget(target);
	this->startPos = _target->getPosition();
	_target->setPosition(startPos.x + radius * sinf(alphaBegin),startPos.y + radius * cosf(alphaBegin));
}

void MoveCircle::update(float time)
{
	float alpha =(alphaEnd - alphaBegin)  * time + alphaBegin;
	_target->setPosition(startPos.x + radius * sinf(alpha),startPos.y + radius * cosf(alpha));
}

CCShake::CCShake() : _strength_x(0), _strength_y(0), _initial_x(0), _initial_y(0)
{
}

CCShake* CCShake::actionWithDuration( float d, float strength )
{
	// call other construction method with twice the same strength
	return actionWithDuration( d, strength, strength );
}

CCShake* CCShake::actionWithDuration(float duration, float strength_x, float strength_y)
{
	CCShake *p_action = new CCShake();
	p_action->initWithDuration(duration, strength_x, strength_y);
	p_action->autorelease();

	return p_action;
}

bool CCShake::initWithDuration(float duration, float strength_x, float strength_y)
{
	if (CCActionInterval::initWithDuration(duration))
	{
		_strength_x = strength_x;
		_strength_y = strength_y;
		return true;
	}

	return false;
}

// Helper function. I included it here so that you can compile the whole file
// it returns a random value between min and max included
float fgRangeRand( float min, float max )
{
	float rnd = ((float)rand()/(float)RAND_MAX);
	return rnd*(max-min)+min;
}

int countt = 0;
void CCShake::update(float time)
{
	float randx = fgRangeRand( -_strength_x, _strength_x );
	float randy = fgRangeRand( -_strength_y, _strength_y );

	// move the target to a shaked position
	countt++;
	if(countt % 2 == 0)
		_target->setPosition( ccp( randx, randy) );
}

void CCShake::startWithTarget(CCNode *pTarget)
{
	CCActionInterval::startWithTarget( pTarget );

	// save the initial position
	_initial_x = pTarget->getPosition().x;
	_initial_y = pTarget->getPosition().y;
}

void CCShake::stop(void)
{
	// Action is done, reset clip position
	_target->setPosition( ccp( _initial_x, _initial_y ) );

	CCActionInterval::stop();
}
