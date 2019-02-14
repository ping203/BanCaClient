#include "base/ccConfig.h"
#ifndef __banca_engine_h__
#define __banca_engine_h__

#include "jsapi.h"
#include "jsfriendapi.h"

extern JSClass  *jsb_GSprite3D_class;
extern JSObject *jsb_GSprite3D_prototype;

bool js_banca_engine_GSprite3D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_banca_engine_GSprite3D_finalize(JSContext *cx, JSObject *obj);
void js_register_banca_engine_GSprite3D(JSContext *cx, JS::HandleObject global);
void register_all_banca_engine(JSContext* cx, JS::HandleObject obj);
bool js_banca_engine_GSprite3D_initFromPath(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GSprite3D_setTexture(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GSprite3D_create(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GSprite3D_GSprite3D(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_GEnvironment_class;
extern JSObject *jsb_GEnvironment_prototype;

bool js_banca_engine_GEnvironment_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_banca_engine_GEnvironment_finalize(JSContext *cx, JSObject *obj);
void js_register_banca_engine_GEnvironment(JSContext *cx, JS::HandleObject global);
void register_all_banca_engine(JSContext* cx, JS::HandleObject obj);
bool js_banca_engine_GEnvironment_isEnableCausticAnimation(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GEnvironment_getDeltaTimeAnim(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GEnvironment_setEnabledShadow(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GEnvironment_addCausticTexture(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GEnvironment_update(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GEnvironment_isEnabledShadow(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GEnvironment_isSupportDepthTexture(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GEnvironment_cleanCausticTexture(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GEnvironment_runCausticAnimation(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GEnvironment_setupLight(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GEnvironment_setupShadow(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GEnvironment_setLightUpdate(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GEnvironment_setCameraUpdate(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GEnvironment_getInstance(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GEnvironment_GEnvironment(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_GMesh_class;
extern JSObject *jsb_GMesh_prototype;

bool js_banca_engine_GMesh_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_banca_engine_GMesh_finalize(JSContext *cx, JSObject *obj);
void js_register_banca_engine_GMesh(JSContext *cx, JS::HandleObject global);
void register_all_banca_engine(JSContext* cx, JS::HandleObject obj);
bool js_banca_engine_GMesh_setMaterial(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GMesh_setGLProgramState(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GMesh_receivedShadow(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GMesh_create(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GMesh_GMesh(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_Path_class;
extern JSObject *jsb_Path_prototype;

bool js_banca_engine_Path_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_banca_engine_Path_finalize(JSContext *cx, JSObject *obj);
void js_register_banca_engine_Path(JSContext *cx, JS::HandleObject global);
void register_all_banca_engine(JSContext* cx, JS::HandleObject obj);
bool js_banca_engine_Path_addPathPoint(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path_getCurrentAngle(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path_calculate(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path_getLength(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path_getPositionFromTime(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path_autorelease(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path_release(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path_clean(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path_setPathListener(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path_getCurrentAngleRad(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path_Path(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_Path3D_class;
extern JSObject *jsb_Path3D_prototype;

bool js_banca_engine_Path3D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_banca_engine_Path3D_finalize(JSContext *cx, JSObject *obj);
void js_register_banca_engine_Path3D(JSContext *cx, JS::HandleObject global);
void register_all_banca_engine(JSContext* cx, JS::HandleObject obj);
bool js_banca_engine_Path3D_getTransformFromTimeline(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path3D_addPathPoint(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path3D_calculate(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path3D_getLength(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path3D_getDuration(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path3D_autorelease(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path3D_recalculateWithResolution(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path3D_clean(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path3D_release(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path3D_setDuration(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Path3D_Path3D(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_HoldFishInfo_class;
extern JSObject *jsb_HoldFishInfo_prototype;

bool js_banca_engine_HoldFishInfo_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_banca_engine_HoldFishInfo_finalize(JSContext *cx, JSObject *obj);
void js_register_banca_engine_HoldFishInfo(JSContext *cx, JS::HandleObject global);
void register_all_banca_engine(JSContext* cx, JS::HandleObject obj);
bool js_banca_engine_HoldFishInfo_setFish(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_HoldFishInfo_getFish(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_HoldFishInfo_autorelease(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_HoldFishInfo_getIsHolding(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_HoldFishInfo_getPlayer(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_HoldFishInfo_setIsHolding(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_HoldFishInfo_setPlayer(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_HoldFishInfo_HoldFishInfo(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_Entity_class;
extern JSObject *jsb_Entity_prototype;

bool js_banca_engine_Entity_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_banca_engine_Entity_finalize(JSContext *cx, JSObject *obj);
void js_register_banca_engine_Entity(JSContext *cx, JS::HandleObject global);
void register_all_banca_engine(JSContext* cx, JS::HandleObject obj);
bool js_banca_engine_Entity_setUserData(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_setUpdateFunc(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_getUserData(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_getBodyVelocity(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_setTransform(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_getType(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_update(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_autorelease(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_isNeedRemove(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_rotationFromVel(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_setContentSize(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_getBodyPosition(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_setNodeDisplay(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_release(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_setPosition(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_setFinishFunc(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_getNodeDisplay(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_setVelocity(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Entity_Entity(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_Fish_class;
extern JSObject *jsb_Fish_prototype;

bool js_banca_engine_Fish_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_banca_engine_Fish_finalize(JSContext *cx, JSObject *obj);
void js_register_banca_engine_Fish(JSContext *cx, JS::HandleObject global);
void register_all_banca_engine(JSContext* cx, JS::HandleObject obj);
bool js_banca_engine_Fish_enableCheckOutsite(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish_pause(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish_enableAutoDie(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish_startWithPath(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish_update(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish_isOutsite(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish_onNewCurve(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish_enableFlip(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish_Fish(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_Wall_class;
extern JSObject *jsb_Wall_prototype;

bool js_banca_engine_Wall_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_banca_engine_Wall_finalize(JSContext *cx, JSObject *obj);
void js_register_banca_engine_Wall(JSContext *cx, JS::HandleObject global);
void register_all_banca_engine(JSContext* cx, JS::HandleObject obj);
bool js_banca_engine_Wall_Wall(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_Bullet_class;
extern JSObject *jsb_Bullet_prototype;

bool js_banca_engine_Bullet_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_banca_engine_Bullet_finalize(JSContext *cx, JSObject *obj);
void js_register_banca_engine_Bullet(JSContext *cx, JS::HandleObject global);
void register_all_banca_engine(JSContext* cx, JS::HandleObject obj);
bool js_banca_engine_Bullet_getLive(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Bullet_getHoldInfo(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Bullet_setVelLength(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Bullet_update(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Bullet_setHoldInfo(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Bullet_Bullet(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_Fish3D_class;
extern JSObject *jsb_Fish3D_prototype;

bool js_banca_engine_Fish3D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_banca_engine_Fish3D_finalize(JSContext *cx, JSObject *obj);
void js_register_banca_engine_Fish3D(JSContext *cx, JS::HandleObject global);
void register_all_banca_engine(JSContext* cx, JS::HandleObject obj);
bool js_banca_engine_Fish3D_setPath3D(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish3D_enableCheckOutsite(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish3D_pause(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish3D_resume(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish3D_enableAutoDie(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish3D_update(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish3D_start(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish3D_setNodeDebug2D(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish3D_isOutsite(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Fish3D_Fish3D(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_Setting_class;
extern JSObject *jsb_Setting_prototype;

bool js_banca_engine_Setting_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_banca_engine_Setting_finalize(JSContext *cx, JSObject *obj);
void js_register_banca_engine_Setting(JSContext *cx, JS::HandleObject global);
void register_all_banca_engine(JSContext* cx, JS::HandleObject obj);
bool js_banca_engine_Setting_autorelease(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_Setting_Setting(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_GameManager_class;
extern JSObject *jsb_GameManager_prototype;

bool js_banca_engine_GameManager_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_banca_engine_GameManager_finalize(JSContext *cx, JSObject *obj);
void js_register_banca_engine_GameManager(JSContext *cx, JS::HandleObject global);
void register_all_banca_engine(JSContext* cx, JS::HandleObject obj);
bool js_banca_engine_GameManager_getFishByPos(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager_destroyAllEntity(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager_onRealDestroyFish(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager_shootBullet(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager__doUpdate(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager_createWall(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager_setFishDestroyDelegate(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager_BeginContact(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager_update(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager_createBodyForBullet(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager_autorelease(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager_initWorld(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager_destroyEntity(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager_createBodyForFish(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager_setEntityCollisionListener(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager_PreSolve(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager_setOnContactPreSolve(JSContext *cx, uint32_t argc, jsval *vp);
bool js_banca_engine_GameManager_GameManager(JSContext *cx, uint32_t argc, jsval *vp);

#endif // __banca_engine_h__
