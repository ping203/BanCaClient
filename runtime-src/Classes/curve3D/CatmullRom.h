#ifndef THREE_PP_CATMUL_H
#define THREE_PP_CATMUL_H
#include "math/Vector3.h"
#include "curve/Spline.h"

using namespace three::extras::curve;
using namespace three::math;
struct CatmullRomPoint
{
public:
	Vector3 position;
	Vector3 tangent;
	Vector3 normal;

	CatmullRomPoint(){

	}

	CatmullRomPoint(Vector3 position, Vector3 tangent,
		Vector3 normal)
	{
		this->position = position;
		this->tangent = tangent;
		this->normal = normal;
	}
};

class CatmullRomCal
{
public:
	CatmullRomCal(std::vector<Vector3> controlPoints, int resolution, bool closedLoop)
	{
		this->controlPoints = controlPoints;
		this->resolution = resolution;
		this->closedLoop = closedLoop;

		GenerateSplinePoints();
	}

public:
	int resolution;
	bool closedLoop;
	std::vector<CatmullRomPoint> splinePoints;
	std::vector<Vector3> controlPoints;
	std::vector<float> _cacheArcLengths;

	//Sets the length of the point array based on resolution/closed loop.
	void InitializeProperties()
	{
		int pointsToCreate;
		if (closedLoop)
		{
			pointsToCreate = resolution * controlPoints.size(); //Loops back to the beggining, so no need to adjust for arrays starting at 0
		}
		else
		{
			pointsToCreate = resolution * (controlPoints.size() - 1);
		}
		splinePoints.resize(pointsToCreate);
		_cacheArcLengths.resize(pointsToCreate);
	}

	//Math stuff to generate the spline points
	void GenerateSplinePoints()
	{
		InitializeProperties();

		Vector3 p0, p1; //Start point, end point
		Vector3 m0, m1; //Tangents

		// First for loop goes through each individual control point and connects it to the next, so 0-1, 1-2, 2-3 and so on
		int closedAdjustment = closedLoop ? 0 : 1;
		Vector3 checkPoint(controlPoints[0]);
		float sum = 0;
		for (int currentPoint = 0; currentPoint < controlPoints.size() - closedAdjustment; currentPoint++)
		{
			bool closedLoopFinalPoint = (closedLoop && currentPoint == controlPoints.size() - 1);

			p0 = controlPoints[currentPoint];

			if (closedLoopFinalPoint)
			{
				p1 = controlPoints[0];
			}
			else
			{
				p1 = controlPoints[currentPoint + 1];
			}

			// m0
			if (currentPoint == 0) // Tangent M[k] = (P[k+1] - P[k-1]) / 2
			{
				if (closedLoop)
				{
					m0 = p1 - controlPoints[controlPoints.size() - 1];
				}
				else
				{
					m0 = p1 - p0;
				}
			}
			else
			{
				m0 = p1 - controlPoints[currentPoint - 1];
			}

			// m1
			if (closedLoop)
			{
				if (currentPoint == controlPoints.size() - 1) //Last point case
				{
					m1 = controlPoints[(currentPoint + 2) % controlPoints.size()] - p0;
				}
				else if (currentPoint == 0) //First point case
				{
					m1 = controlPoints[currentPoint + 2] - p0;
				}
				else
				{
					m1 = controlPoints[(currentPoint + 2) % controlPoints.size()] - p0;
				}
			}
			else
			{
				if (currentPoint < controlPoints.size() - 2)
				{
					m1 = controlPoints[(currentPoint + 2) % controlPoints.size()] - p0;
				}
				else
				{
					m1 = p1 - p0;
				}
			}

			m0 *= 0.5f; //Doing this here instead of  in every single above statement
			m1 *= 0.5f;

			float pointStep = 1.0f / resolution;

			if ((currentPoint == controlPoints.size() - 2 && !closedLoop) || closedLoopFinalPoint) //Final point
			{
				pointStep = 1.0f / (resolution - 1);  // last point of last segment should reach p1
			}

			// Creates [resolution] points between this control point and the next
			for (int tesselatedPoint = 0; tesselatedPoint < resolution; tesselatedPoint++)
			{
				float t = tesselatedPoint * pointStep;

				CatmullRomPoint point = Evaluate(p0, p1, m0, m1, t);

				splinePoints[currentPoint * resolution + tesselatedPoint] = point;

				sum += point.position.distanceTo(checkPoint);
				checkPoint = point.position;

				_cacheArcLengths[currentPoint * resolution + tesselatedPoint] = sum;
			}
		}
	}

	float getUtoTmapping(float u, float distance = 0)
	{
		//std::vector<float> arcLengths = getLengths();

		float targetArcLength; // The targeted u distance value to get

		if (distance > 0) {
			targetArcLength = distance;
		}
		else {
			targetArcLength = u * _cacheArcLengths.back();
		}

		// binary search for the index with largest value smaller than target u distance
		float low = 0, high = _cacheArcLengths.size() - 1;
		while (low <= high) {

			int i = low + (high - low) / 2; // less likely to overflow

			float comparison = _cacheArcLengths[i] - targetArcLength;

			if (comparison < 0) {

				low = i + 1;
			}
			else if (comparison > 0) {

				high = i - 1;
			}
			else {

				high = i;
				break;
				// DONE
			}
		}

		int i = high;

		if (_cacheArcLengths[i] == targetArcLength) {

			return i * 1.0 / (_cacheArcLengths.size() - 1);
		}

		// we could get finer grain at lengths, or use simple interpolation between two points
		float lengthBefore = _cacheArcLengths[i];
		float lengthAfter = _cacheArcLengths[i + 1];

		float segmentLength = lengthAfter - lengthBefore;

		// determine where we are between the 'before' and 'after' points
		float segmentFraction = (targetArcLength - lengthBefore) / segmentLength;

		// add that fractional amount to t
		return (i + segmentFraction) / (_cacheArcLengths.size() - 1);
	}

	//Evaluates curve at t[0, 1]. Returns point/normal/tan struct. [0, 1] means clamped between 0 and 1.
	static CatmullRomPoint Evaluate(Vector3 start, Vector3 end, Vector3 tanPoint1, Vector3 tanPoint2, float t)
	{
		Vector3 position = CalculatePosition(start, end, tanPoint1, tanPoint2, t);
		Vector3 tangent = CalculateTangent(start, end, tanPoint1, tanPoint2, t);
		Vector3 normal = NormalFromTangent(tangent);

		return CatmullRomPoint(position, tangent, normal);
	}
	//Calculates curve position at t[0, 1]
	static Vector3 CalculatePosition(Vector3 start, Vector3 end, Vector3 tanPoint1, Vector3 tanPoint2, float t)
	{
		// Hermite curve formula:
		// (2t^3 - 3t^2 + 1) * p0 + (t^3 - 2t^2 + t) * m0 + (-2t^3 + 3t^2) * p1 + (t^3 - t^2) * m1
		Vector3 position = (2.0f * t * t * t - 3.0f * t * t + 1.0f) * start
			+ (t * t * t - 2.0f * t * t + t) * tanPoint1
			+ (-2.0f * t * t * t + 3.0f * t * t) * end
			+ (t * t * t - t * t) * tanPoint2;

		return position;
	}

	//Calculates tangent at t[0, 1]
	static Vector3 CalculateTangent(Vector3 start, Vector3 end, Vector3 tanPoint1, Vector3 tanPoint2, float t)
	{
		// Calculate tangents
		// p'(t) = (6t² - 6t)p0 + (3t² - 4t + 1)m0 + (-6t² + 6t)p1 + (3t² - 2t)m1
		Vector3 tangent = (6 * t * t - 6 * t) * start
			+ (3 * t * t - 4 * t + 1) * tanPoint1
			+ (-6 * t * t + 6 * t) * end
			+ (3 * t * t - 2 * t) * tanPoint2;

		return tangent.normalized();
	}

	//Calculates normal vector from tangent
	static Vector3 NormalFromTangent(Vector3 tangent)
	{
		return tangent.cross(Vector3(0, 1, 0)).normalized() / 2;

	}

};





#endif