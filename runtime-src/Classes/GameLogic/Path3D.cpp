#include "Path3D.h"

#define RESOLUTION_DEFAULT 75

Path3D::Path3D(float duration)
{
	_catMullRom = NULL;
	__caculated = false;
	__calLength = false;
	_resolution = RESOLUTION_DEFAULT;

	setDuration(duration);
}


Path3D::~Path3D()
{
	clean();
}

void Path3D::calculate()
{
	if (__caculated)
		return;
	_catMullRom = new CatmullRomCal(_knotPoints, _resolution, false);
	__caculated = true;
}

void Path3D::recalculateWithResolution(int resolution)
{
	_resolution = resolution;
	__caculated = false;
	calculate();
}

void Path3D::clean()
{
	if (_catMullRom)
		delete _catMullRom;

	_catMullRom = NULL;
	__caculated = false;
	__calLength = false;

	_knotPoints.clear();
	_resolution = RESOLUTION_DEFAULT;
}

// just in case you need that function also
cocos2d::Quaternion CreateFromAxisAngleF(Vector3 axis, float angle)
{
	float halfAngle = angle * .5f;
	float s = (float)sinf(halfAngle);
	cocos2d::Quaternion q;
	q.x = axis.x() * s;
	q.y = axis.y() * s;
	q.z = axis.z() * s;
	q.w = (float)cosf(halfAngle);
	return q;
}
cocos2d::Quaternion LookAtF(Vector3 sourcePoint, Vector3 destPoint)
{
	Vector3 forwardVector = Vector3(destPoint - sourcePoint).normalized();

	float dot = three::math::dot(Vector3(0, 0, 1), (forwardVector));

	if (abs(dot - (-1.0f)) < 0.000001f)
	{
		return cocos2d::Quaternion(0, 1, 0, 3.1415926535897932f);
	}
	if (abs(dot - (1.0f)) < 0.000001f)
	{
		return cocos2d::Quaternion::identity();
	}

	float rotAngle = (float)acos(dot);
	Vector3 rotAxis = Vector3(0, 0, 1).cross(forwardVector);
	rotAxis = rotAxis.normalize();
	return CreateFromAxisAngleF(rotAxis, rotAngle);
}

Mat4 Path3D::getTransformFromTimeline(float time)
{
	calculate();
	Mat4 ret;
	if (_duration <= 0 || _knotPoints.size() <= 0)
		return ret;
	int size = _catMullRom->splinePoints.size();
	float percent = time * size/ _duration;
	int idx = (int)percent;
	if (idx >= size)
		idx = size - 1;

	Vector3 position = _catMullRom->splinePoints[idx].position;
	cocos2d::Quaternion rotation = LookAtF(position, position + _catMullRom->splinePoints[idx].tangent * 10);

	if (idx < (size - 1) && (percent - idx) > 0)
	{
		Vector3 position_1 = _catMullRom->splinePoints[idx+1].position;
		cocos2d::Quaternion rotation_1 = LookAtF(position_1, position_1 + _catMullRom->splinePoints[idx+1].tangent * 10);

		// interpolate for smooth
		position = (position_1 - position) * (percent - idx) + position;

		cocos2d::Quaternion::slerpForSquad(rotation, rotation_1, percent - idx, &rotation);
	}

	Mat4::createTranslation(Vec3(position.x(), position.y(), position.z()), &ret);
	ret.rotate(rotation);

	return ret;
}