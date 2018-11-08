#include "GScene3D.h"
#include "GSprite3D.h"
#include "GMainLight.h"
#include "GFrameBufferObject.h"
#include "GShadowMapTexture.h"
#include "GEnvironment.h"
#include "GShader.h"
#include "GMesh.h"
#include "GBatchMesh.h"


GScene3D::GScene3D()
{
}


GScene3D::~GScene3D()
{
}

Camera *depthCam;
Mat4 biasMatrix;

bool GScene3D::init()
{
	Layer::init();


	auto size = Director::getInstance()->getWinSize();
	mainCam = Camera::createPerspective(45, size.width / size.height, 0.1f, 1000);
	mainCam->setPosition3D(Vec3(55, 55, 55));
	mainCam->lookAt(Vec3(0, 0, 0),Vec3(0,0,1));
	mainCam->setCameraFlag(CameraFlag::USER1);
	addChild(mainCam);
// 
// 

	GLProgramState* sameState =  GLProgramState::create(GShader::getInstance()->getGLProgram(GShader::G_SHADER_DIFFUSE));
	GMaterial *sameMat = GMaterial::createWithGLProgramState(sameState);


	auto p = GSprite3D::create("sin/Banchoiupdate.c3t");
	p->setCameraMask(2);
	p->setScale(.1);
	addChild(p); 

// 	auto bundle = GBundle3D::createInstance("sin/Tham.c3t");
// 	MeshDatas datas;
// 	bundle->loadMeshDatas(datas);
// 
// 	GBatchMesh *batchMesh = GBatchMesh::create(*datas.meshDatas[0]);
// 	batchMesh->setCameraMask(2);
// 	addChild(batchMesh);
// 
// 	batchMesh->setScale(5);


	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesMoved = CC_CALLBACK_2(GScene3D::onTouchsMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_center = Vec3(0, -3, 0);

// 	GEnvironment::getInstance()->setupLight(Vec3(0, 15, 8), Vec3(1, 1, 1), 0.2, 0.1);
// 	GEnvironment::getInstance()->setupShadow(this, 1024, 1024, (int)CameraFlag::USER2);
// 	GEnvironment::getInstance()->setEnabledShadow(false);

	return true;
}

void GScene3D::setupLight()
{

}
void GScene3D::calculateArcBall(cocos2d::Vec3 & axis, float & angle, float p1x, float p1y, float p2x, float p2y)
{
	Mat4 rotation_matrix;
	Mat4::createRotation(_rotationQuat, &rotation_matrix);

	Vec3 uv = rotation_matrix * Vec3(0.0f, 1.0f, 0.0f); //rotation y
	Vec3 sv = rotation_matrix * Vec3(1.0f, 0.0f, 0.0f); //rotation x
	Vec3 lv = rotation_matrix * Vec3(0.0f, 0.0f, -1.0f);//rotation z

	Vec3 p1 = sv * p1x + uv * p1y - lv * projectToSphere(_radius, p1x, p1y); //start point screen transform to 3d
	Vec3 p2 = sv * p2x + uv * p2y - lv * projectToSphere(_radius, p2x, p2y); //end point screen transform to 3d

	Vec3::cross(p2, p1, &axis);  //calculate rotation axis
	axis.normalize();

	float t = (p2 - p1).length() / (2.0 * _radius);
	//clamp -1 to 1
	if (t > 1.0) t = 1.0;
	if (t < -1.0) t = -1.0;
	angle = asin(t);           //rotation angle
}

/* project an x,y pair onto a sphere of radius r or a
hyperbolic sheet if we are away from the center of the sphere. */
float GScene3D::projectToSphere(float r, float x, float y)
{
	float d, t, z;
	d = sqrt(x*x + y*y);
	if (d < r * 0.70710678118654752440)//inside sphere
	{
		z = sqrt(r*r - d*d);
	}
	else                               //on hyperbola
	{
		t = r / 1.41421356237309504880;
		z = t*t / d;
	}
	return z;
}

void GScene3D::updateCameraTransform()
{
	//return;
	Mat4 trans, rot, center;
	Mat4::createTranslation(Vec3(0.0f, _distanceZ / 2, _distanceZ), &trans);
	Mat4::createRotation(_rotationQuat, &rot);
	Mat4::createTranslation(_center, &center);
	Mat4 result = center * rot * trans;
	mainCam->setNodeToParentTransform(result);
	//_camera->lookAt(Vec3(0, 0, 0));

}


void GScene3D::onTouchsMoved(const std::vector<Touch*> &touchs, Event *event)
{
	if (!touchs.empty())
	{
		if (true)           //arc ball rotate
		{
			Size visibleSize = Director::getInstance()->getVisibleSize();
			Vec2 prelocation = touchs[0]->getPreviousLocationInView();
			Vec2 location = touchs[0]->getLocationInView();
			location.x = 2.0 * (location.x) / (visibleSize.width) - 1.0f;
			location.y = 2.0 * (visibleSize.height - location.y) / (visibleSize.height) - 1.0f;
			prelocation.x = 2.0 * (prelocation.x) / (visibleSize.width) - 1.0f;
			prelocation.y = 2.0 * (visibleSize.height - prelocation.y) / (visibleSize.height) - 1.0f;

			Vec3 axes;
			float angle;
			calculateArcBall(axes, angle, prelocation.x, prelocation.y, location.x, location.y);    //calculate  rotation quaternion parameters
			Quaternion quat(axes, angle);                                                           //get rotation quaternion
			_rotationQuat = quat * _rotationQuat;

			updateCameraTransform();                                                                //update camera Transform
		}
		else        //camera zoom 
		{
			Point newPos = touchs[0]->getPreviousLocation() - touchs[0]->getLocation();
			//_distanceZ -= newPos.y*0.1f;

			updateCameraTransform();
		}
	}
}