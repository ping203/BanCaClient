#pragma once
#include "cocos2d.h"
#include "curve3D/CatmullRom.h"
#include "curve/Spline.h"

USING_NS_CC;


class Path3D : public Ref
{
public:
	Path3D(float duration, bool constantTime);
	~Path3D();

	virtual Ref*  autorelease(){ return this; };
	void release(){ delete this; };

	
	/*
	* add control point for patch
	*/
	void addPathPoint(Vec3 point){
		_knotPoints.push_back(Vector3(point.x,point.y,point.z));
	}

	void setDuration(float duration){
		_duration = duration;
	}

	float getLength()
	{
		return _catMullRom->_cacheArcLengths.back();
	}
	float getDuration(){
		return _duration;
	}

	// tinh toan lai duong di voi resolution moi
	void recalculateWithResolution(int resolution);

	// clean list point and delete catmull
	void clean();

	/************************************************************************/
	/* calculate all point for this patch
	*/
	/************************************************************************/
	void calculate();

	Mat4 getTransformFromTimeline(float time);

private:
	CatmullRomCal *_catMullRom;
	Spline3D *_spline3D;
	std::vector<Vector3> _knotPoints;
	float _duration;
	int _resolution;
	bool _constantTime;

	// flag
	bool __caculated;
	bool __calLength;
	

};

