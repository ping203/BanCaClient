LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2djs_shared

LOCAL_MODULE_FILENAME := libcocos2djs

ifeq ($(USE_ARM_MODE),1)
LOCAL_ARM_MODE := arm
endif

LOCAL_SRC_FILES := hellojavascript/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/G3D/GBundle3D.cpp \
                   ../../Classes/G3D/GEnvironment.cpp \
                   ../../Classes/G3D/GFrameBufferObject.cpp \
                   ../../Classes/G3D/GMainLight.cpp \
                   ../../Classes/G3D/GScene3D.cpp \
                   ../../Classes/G3D/GSprite3D.cpp \
                   ../../Classes/G3D/GMesh.cpp \
                   ../../Classes/G3D/GMaterial.cpp \
                   ../../Classes/G3D/GShadowMapTexture.cpp \
                   ../../Classes/G3D/GBatchMesh.cpp \
                   ../../Classes/G3D/GShader.cpp \
                   ../../Classes/GameLogic/Entity.cpp \
                   ../../Classes/GameLogic/GameManager.cpp \
                   ../../Classes/GameLogic/Path.cpp \
                   ../../Classes/core/Curve.cpp \
                   ../../Classes/core/Interpolations.cpp \
                   ../../Classes/core/CurvePath.cpp \
                   ../../Classes/math/Vector2.cpp \
                   ../../Classes/math/Vector3.cpp \
                   ../../Classes/math/Vector4.cpp \
                   ../../Classes/math/Math.cpp \
                   ../../Classes/math/Quaternion.cpp \
                   ../../Classes/math/Matrix4.cpp \
                   ../../Classes/math/Matrix3.cpp \
                   ../../Classes/math/Frustum.cpp \
                   ../../Classes/math/Circle3.cpp \
                   ../../Classes/math/Euler.cpp \
                   ../../Classes/math/Sphere.cpp \
                   ../../Classes/math/Box3.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes $(LOCAL_PATH)/../../Classes/G3D

LOCAL_STATIC_LIBRARIES := cocos2d_js_static

LOCAL_EXPORT_CFLAGS := -DCOCOS2D_DEBUG=2 -DCOCOS2D_JAVASCRIPT

include $(BUILD_SHARED_LIBRARY)


$(call import-module, scripting/js-bindings/proj.android)
