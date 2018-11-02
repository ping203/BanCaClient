//
//  Path.cpp
//  FishJS
//
//  Created by Admin on 9/14/18.
//



#include "Path.hpp"
#include "../math/Vector2.h"
// Path
using namespace three::math;
Path::Path(float duration)
{
    m_duration = duration;
    clean();
}

Path::~Path()
{
	for (int i = 0; i < bezierData.size(); i++)
	{
		delete bezierData[i];
	}
	bezierData.clear();
}

void Path::addPathPoint(cocos2d::Vec2 point)
{
    pointDatas.push_back(point);
}

void Path::clean()
{
    pointDatas.clear();
    bezierData.clear();
    
    _previousPos = Vec2(-1,-1);
    _currentPos = Vec2(-1,-1);
    length = -1;
}

void Path::calculate()
{
    int size = pointDatas.size();
    length = 0;
    if(size < 2)
		return;

	int count = 0;
	while (true)
	{
		if ((count + 3) < size)
		{
			Vector2 p0(pointDatas[count].x, pointDatas[count].y);
			Vector2 p1(pointDatas[count +1].x, pointDatas[count + 1].y);
			Vector2 p2(pointDatas[count + 2].x, pointDatas[count + 2].y);
			Vector2 p3(pointDatas[count + 3].x, pointDatas[count + 3].y);   

			CubicBezier *curve = new CubicBezier(p0, p1, p2, p3);
			curve->length_start = length;
			length += curve->getLength();
			bezierData.push_back(curve);
			count += 3;
		}
		else if ((count + 2) < size)
		{
			Vector2 p0(pointDatas[count].x, pointDatas[count].y);
			Vector2 p1(pointDatas[count + 1].x, pointDatas[count + 1].y);
			Vector2 p2(pointDatas[count + 2].x, pointDatas[count + 2].y);

			QuadraticBezier *curve = new  QuadraticBezier(p0, p1, p2);
			curve->length_start = length;
			length += curve->getLength();
			bezierData.push_back(curve);
			count += 2;
		}
		else if ((count + 1) < size)
		{
			Vector2 p0(pointDatas[count].x, pointDatas[count].y);
			Vector2 p1(pointDatas[count + 1].x, pointDatas[count + 1].y);


			Line* curve = new Line(p0, p1);
			curve->length_start = length;
			length += curve->getLength();
			bezierData.push_back(curve);
			count += 1;
		}
		else
			break;

	}


	int a = 0;
	
}

float Path::getLength()
{
    if(length >= 0)
    return length;
    calculate();
    return length;
}

  three::extras::Curve* Path::getCurveFromPercentTimeLine(float percent)
{
    int size = bezierData.size();
    for(int i=0;i<size;i++)
    {
        if(((bezierData[i]->getLength() + bezierData[i]->length_start) / length) >= percent)
        return bezierData[i];
    }
    return bezierData[size-1];
}

Vec2 Path::getPositionFromTime(float time)
{
    
    if(m_duration == 0 || length == 0)
    {
        return pointDatas[pointDatas.size()-1];
    }
    _previousPos.set(_currentPos.x, _currentPos.y);
    float t = time / m_duration;
    if(t > 1) t = 1;
    
	three::extras:: Curve* curve_choose = getCurveFromPercentTimeLine(t);
    float curve_duration = curve_choose->getLength() * m_duration / length;
    
    
    float curve_start_time = curve_choose->length_start * m_duration / length;
    float curve_end_time = curve_duration + curve_start_time;
    
    float tt = 0;
    if(time > curve_end_time)
    tt = 1;
    else if(time <= curve_start_time)
    tt = 0;
    else
    {
        tt = (time - curve_start_time) / curve_duration;
    }
    
    //CCLOG("%f   _  %f",tt,curve_choose.getUtoTmapping(tt));
    Vector2 pos = curve_choose->getPointAt(tt);
    Vec2 dis(pos.elements()[0],pos.elements()[1]);
    _currentPos.set(dis.x, dis.y);
    return dis;
}

float Path::getCurrentAngle()
{
    return rotationFromVel(_currentPos - _previousPos);
}

float Path::getCurrentAngleRad()
{
    return rotationFromVelRad(_currentPos - _previousPos);
}

float Path::rotationFromVel(Vec2 vel)
{
    if(abs(vel.y) < 0.000001)
    {
        return vel.x > 0?90:-90;
    }
    else if(vel.y > 0)
    {
        return atanf(vel.x / vel.y) * 180 / M_PI;
    }
    else
    {
        return atanf(vel.x / vel.y) * 180 / M_PI + 180;
    }
}

float Path::rotationFromVelRad(Vec2 vel)
{
	if (abs(vel.y) < 0.000001)
    {
		return vel.x > 0 ? M_PI / 2 : -M_PI/2;
    }
    else if(vel.y > 0)
    {
        return atanf(vel.x / vel.y);
    }
    else
    {
        return atanf(vel.x / vel.y) + M_PI;
    }
}


