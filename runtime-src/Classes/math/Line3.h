//
// Created by byter on 10.09.17.
//

#ifndef THREEPP_LINE3_H
#define THREEPP_LINE3_H

#include "Vector3.h"
#include "Math.h"

namespace three {
namespace math {

class Line3
{
  Vector3 _start;
  Vector3 _end;

  float _closestPointToPoint(const Vector3 &point, bool clampToLine) const
  {
    Vector3 startP = point - _start;
    Vector3 startEnd = _end - _start;

    float startEnd2 = dot(startEnd, startEnd );
    float startEnd_startP = dot(startEnd, startP);

    float t = startEnd_startP / startEnd2;

    if ( clampToLine ) {
      t = math::clamp(t, 0.0f, 1.0f);
    }

    return t;
  }

public:
  Line3() = default;
  Line3(const Vector3 &start, const Vector3 &end) : _start(start), _end(end) {}

  const Vector3 &start() const {return _start;}
  const Vector3 &end() const {return _end;}

  Vector3 getCenter()
  {
    return (_start + _end) * 0.5;
  }

  Vector3 delta() const
  {
    return _end - _start;
  }

  float distanceSq() const
  {
    return _start.distanceToSquared(_end);
  }

  float distance() const
  {
    return _start.distanceTo( _end );
  }

  Vector3 at(unsigned t) const
  {
    return delta() * t + _start;
  }

  Vector3 closestPointToPoint( const Vector3 &point, bool clampToLine )
  {
    float t = _closestPointToPoint( point, clampToLine );

    return delta() * t  + _start;
  }

  Line3 &apply(const Matrix4 &matrix )
  {
    _start.apply( matrix );
    _end.apply( matrix );

    return *this;
  }

  bool operator ==(const Line3 &line ) const
  {
    return line._start == _start && line._end == _end;
  }
};

}
}
#endif //THREEPP_LINE3_H
