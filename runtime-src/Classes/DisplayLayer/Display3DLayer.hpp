//
//  Display3DLayer.hpp
//  Fish3D
//
//  Created by Admin on 8/31/18.
//

#ifndef Display3DLayer_hpp
#define Display3DLayer_hpp

#include <stdio.h>
#include "AbtractDisplayLayer.h"


class Display3DLayer : public AbtractDisplayLayer
{
public:
    Display3DLayer();
    ~Display3DLayer();
    
    CREATE_FUNC(Display3DLayer);
    bool init() override;
	virtual void update(float dt);
public:
    Camera *mainCam;
    
    void setupLight();
    void setupScene();
    
};

#endif /* Display3DLayer_hpp */
