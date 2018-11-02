//
//  Path.hpp
//  FishJS
//
//  Created by Admin on 9/14/18.
//

#ifndef Path_hpp
#define Path_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "curve/CubicBezier.h"
#include "curve/Line.h"
#include "curve/QuadraticBezier.h"
#include "core/Curve.h"

#include <vector>
using namespace std;
using namespace three::extras::curve;
USING_NS_CC;

class Path : public Ref
{
public:
    Path(float duration);
	~Path();
    
    void addPathPoint(Vec2 point);
    void clean();
    void calculate();
    float getLength();
    Vec2 getPositionFromTime(float t);
    float getCurrentAngle();
    float getCurrentAngleRad();
    
    virtual Ref*  autorelease(){return this;};
    void release(){delete this;};
public:
	vector<three::extras::Curve * > bezierData;
    vector<Vec2> pointDatas;
    float m_duration;
protected:
     three::extras::Curve* getCurveFromPercentTimeLine(float percent);
    void calculateLoop();
    float rotationFromVel(Vec2 vel);
    float rotationFromVelRad(Vec2 vel);
private:
    Vec2 _previousPos;
    Vec2 _currentPos;
    
    
    float length;
};

#endif /* Path_hpp */
