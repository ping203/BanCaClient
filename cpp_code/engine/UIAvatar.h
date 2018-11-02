#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include <string>
#include "AsyncDownloader.h"

// Default Img la anh ban dau` (kick thuoc ban dau`)
class UIAvatar : public ui::Widget
{
public:
	UIAvatar(void);
	~UIAvatar(void);

	static UIAvatar* create(std::string id, std::string url, std::string defaultImg);
	static UIAvatar* create(std::string defaultImg);
	static UIAvatar* createWithMask(std::string defaultImg, std::string pathMask, std::string extraImg = "");
	static std::string createScreenshot(std::string name, std::string logo);

	void setDefaultImage();
	void setTexture(Texture2D *texture);
	void setImage(std::string path);

	Size getImageSize();
	void callbackDownload(int ret, std::string path);

	void asyncExecute();
	void asyncExecuteWithUrl(std::string id, std::string url);

	void setOpacity(GLubyte opac);
protected:

	virtual void initUI(std::string defaultImg);
	virtual void initWithUrl(std::string id, std::string url, std::string defaultImg);
	virtual void initUI(std::string defaultImg, std::string pathMask, std::string extraImg = "");
	static std::string filePathFromUrl(const char* id);

	std::string _url;
	std::string _defaultImg;
	std::string _id;
	Sprite *_img;
	bool _downloading;
	AsyncDownloader * downloader;
};

class CircleMove : public ActionInterval
{
public:
    static CircleMove* create(float duration,float radius);
    
    virtual void update(float time) override;
    virtual void startWithTarget(Node *target) override;
protected:
    void initWithDuration(float duration,float radius);
    float radius;
    
};

class Loading: public Layer
{
public:
	static Loading* create(std::string particlePath, float duration, float radius);
    
protected:
    Sprite *particle;
    virtual void onEnter() override;
    
};

class TimeProgressEffect : public ProgressFromTo
{
public:
	TimeProgressEffect(void);
	~TimeProgressEffect(void);

	virtual void update(float time);
	static TimeProgressEffect* create(CCProgressTimer *timer,float duration,float percentFrom);
	void setNen(Sprite *);
protected:
	ProgressTimer *_timer;
	Sprite *_nen;

	float startPercent;
	float totalTime;
	float duration;
};

class MoveCircle : public ActionInterval
{
public:
	static MoveCircle* create(float duration,float radius,float alphaBegin,float alphaEnd);

	virtual void update(float time) override;
	virtual void startWithTarget(Node *target) override;
protected:
	void initWithDuration(float duration,float radius,float alphaBegin,float alphaEnd);
	float radius;
	float alphaBegin;
	float alphaEnd;

	Point startPos;

};

class CCShake : public ActionInterval
{
	// Code by Francois Guibert
	// Contact: www.frozax.com - http://twitter.com/frozax - www.facebook.com/frozax
public:
	CCShake();

	// Create the action with a time and a strength (same in x and y)
	static CCShake* actionWithDuration(float d, float strength );
	// Create the action with a time and strengths (different in x and y)
	static CCShake* actionWithDuration(float d, float strength_x, float strength_y );
	bool initWithDuration(float d, float strength_x, float strength_y );

	virtual void startWithTarget(CCNode *pTarget);
	virtual void update(float time);
	virtual void stop(void);

protected:
	// Initial position of the shaked node
	float _initial_x, _initial_y;
	// Strength of the action
	float _strength_x, _strength_y;
};

