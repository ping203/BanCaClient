//
// Created by byter on 6/15/18.
//

#ifndef THREE_PP_CUBICBEZIER_H
#define THREE_PP_CUBICBEZIER_H

#include "../core/Curve.h"
#include "../core/Interpolations.h"
#include "../math/Vector2.h"


namespace three {
namespace extras {
namespace curve {

class CubicBezier : public Curve
{
public:
  math::Vector2 v0, v1, v2, v3;

  CubicBezier(const math::Vector2 &v0, const math::Vector2 &v1, const math::Vector2 &v2, const math::Vector2 &v3)
     : v0(v0), v1(v1), v2(v2), v3(v3) {}

public:
  using Ptr = std::shared_ptr<CubicBezier>;

  static Ptr make(const math::Vector2 &v0, const math::Vector2 &v1, const math::Vector2 &v2, const math::Vector2 &v3)
  {
    return Ptr(new CubicBezier(v0, v1, v2, v3));
  }

  math::Vector2 getPoint( float t ) override
  {
    return math::Vector2(
       interpolations::CubicBezier( t, v0.x(), v1.x(), v2.x(), v3.x() ),
       interpolations::CubicBezier( t, v0.y(), v1.y(), v2.y(), v3.y() )
    );
  }

  math::Vector2 start() const override {
    return v0;
  }
};

}
}
}

#endif //THREE_PP_CUBICBEZIER_H
