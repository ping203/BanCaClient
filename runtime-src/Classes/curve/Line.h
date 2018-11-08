//
// Created by byter on 6/15/18.
//

#ifndef THREEPP_EXTRAS_LINE_H
#define THREEPP_EXTRAS_LINE_H

#include "../core/Curve.h"

namespace three {
namespace extras {
namespace curve {

class Line : public Curve
{
public:
  math::Vector2 _v1, _v2;

public:
  Line(const math::Vector2 &v1, const math::Vector2 &v2) : _v1(v1), _v2(v2) {}

  unsigned pathResolution(unsigned divisions) const override
  {
    return 1;
  }

public:
  using Ptr = std::shared_ptr<Line>;

  static Ptr make(const math::Vector2 &v1, const math::Vector2 &v2)
  {
    return Ptr(new Line(v1, v2));
  }

  math::Vector2 getPoint(float t) override
  {
    if ( t == 1 ) {

      return _v2;
    }
    else {

      return (_v2 - _v1) * t + _v1;
    }
  };

  math::Vector2 start() const override {
    return _v1;
  }

  // Line curve is linear, so we can overwrite default getPointAt
  math::Vector2 getPointAt( float u ) override
  {
    return getPoint( u );
  }

  math::Vector2 getTangent(float t) override
  {
    return (_v2 - _v1).normalize();

  };
};

}
}
}
#endif //THREEPP_EXTRAS_LINE_H
