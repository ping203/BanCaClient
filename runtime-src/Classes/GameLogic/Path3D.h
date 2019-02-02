#pragma once
#include "cocos2d.h"
#include "curve3D/CatmullRom.h"

USING_NS_CC;


class Path3D : public Ref
{
public:
	Path3D(float duration);
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
	float getDuration(){
		return _duration;
	}

private:
	CatmullRomCal *_catMullRom;
	std::vector<Vector3> _knotPoints;
	float _duration;
	int _resolution;

	// flag
	bool __caculated;
	bool __calLength;

};

