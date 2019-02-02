#include "scripting/js-bindings/auto/jsb_banca_auto.hpp"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "GSprite3D.h"
#include "GEnvironment.h"
#include "GMesh.h"
#include "Entity.h"
#include "GameManager.h"

template<class T>
static bool dummy_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS_ReportError(cx, "Constructor for the requested class is not available, please refer to the API reference.");
    return false;
}

static bool empty_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
    return false;
}

static bool js_is_native_obj(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    args.rval().setBoolean(true);
    return true;
}
JSClass  *jsb_GSprite3D_class;
JSObject *jsb_GSprite3D_prototype;

bool js_banca_engine_GSprite3D_initFromPath(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GSprite3D* cobj = (GSprite3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GSprite3D_initFromPath : Invalid Native Object");
    if (argc == 2) {
        std::string arg0;
        bool arg1;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        arg1 = JS::ToBoolean(args.get(1));
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GSprite3D_initFromPath : Error processing arguments");
        bool ret = cobj->initFromPath(arg0, arg1);
        JS::RootedValue jsret(cx);
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GSprite3D_initFromPath : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_banca_engine_GSprite3D_setTexture(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GSprite3D* cobj = (GSprite3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GSprite3D_setTexture : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GSprite3D_setTexture : Error processing arguments");
        cobj->setTexture(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GSprite3D_setTexture : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GSprite3D_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GSprite3D_create : Error processing arguments");

        auto ret = GSprite3D::create(arg0);
        js_type_class_t *typeClass = js_get_type_from_native<GSprite3D>(ret);
        JS::RootedObject jsret(cx, jsb_ref_autoreleased_create_jsobject(cx, ret, typeClass, "GSprite3D"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        bool arg1;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        arg1 = JS::ToBoolean(args.get(1));
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GSprite3D_create : Error processing arguments");

        auto ret = GSprite3D::create(arg0, arg1);
        js_type_class_t *typeClass = js_get_type_from_native<GSprite3D>(ret);
        JS::RootedObject jsret(cx, jsb_ref_autoreleased_create_jsobject(cx, ret, typeClass, "GSprite3D"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_banca_engine_GSprite3D_create : wrong number of arguments");
    return false;
}

bool js_banca_engine_GSprite3D_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    GSprite3D* cobj = new (std::nothrow) GSprite3D();

    js_type_class_t *typeClass = js_get_type_from_native<GSprite3D>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "GSprite3D"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


extern JSObject *jsb_cocos2d_Sprite3D_prototype;

void js_register_banca_engine_GSprite3D(JSContext *cx, JS::HandleObject global) {
    jsb_GSprite3D_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_GSprite3D_class->name = "GSprite3D";
    jsb_GSprite3D_class->addProperty = JS_PropertyStub;
    jsb_GSprite3D_class->delProperty = JS_DeletePropertyStub;
    jsb_GSprite3D_class->getProperty = JS_PropertyStub;
    jsb_GSprite3D_class->setProperty = JS_StrictPropertyStub;
    jsb_GSprite3D_class->enumerate = JS_EnumerateStub;
    jsb_GSprite3D_class->resolve = JS_ResolveStub;
    jsb_GSprite3D_class->convert = JS_ConvertStub;
    jsb_GSprite3D_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("initFromPath", js_banca_engine_GSprite3D_initFromPath, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setTexture", js_banca_engine_GSprite3D_setTexture, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_banca_engine_GSprite3D_create, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JS::RootedObject parent_proto(cx, jsb_cocos2d_Sprite3D_prototype);
    jsb_GSprite3D_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_GSprite3D_class,
        js_banca_engine_GSprite3D_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_GSprite3D_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "GSprite3D"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<GSprite3D>(cx, jsb_GSprite3D_class, proto, parent_proto);
}

JSClass  *jsb_GEnvironment_class;
JSObject *jsb_GEnvironment_prototype;

bool js_banca_engine_GEnvironment_isEnableCausticAnimation(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GEnvironment* cobj = (GEnvironment *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GEnvironment_isEnableCausticAnimation : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->isEnableCausticAnimation();
        JS::RootedValue jsret(cx);
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GEnvironment_isEnableCausticAnimation : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_GEnvironment_getDeltaTimeAnim(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GEnvironment* cobj = (GEnvironment *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GEnvironment_getDeltaTimeAnim : Invalid Native Object");
    if (argc == 0) {
        double ret = cobj->getDeltaTimeAnim();
        JS::RootedValue jsret(cx);
        jsret = DOUBLE_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GEnvironment_getDeltaTimeAnim : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_GEnvironment_setEnabledShadow(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GEnvironment* cobj = (GEnvironment *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GEnvironment_setEnabledShadow : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GEnvironment_setEnabledShadow : Error processing arguments");
        cobj->setEnabledShadow(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GEnvironment_setEnabledShadow : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GEnvironment_addCausticTexture(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GEnvironment* cobj = (GEnvironment *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GEnvironment_addCausticTexture : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GEnvironment_addCausticTexture : Error processing arguments");
        cobj->addCausticTexture(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GEnvironment_addCausticTexture : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GEnvironment_isEnabledShadow(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GEnvironment* cobj = (GEnvironment *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GEnvironment_isEnabledShadow : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->isEnabledShadow();
        JS::RootedValue jsret(cx);
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GEnvironment_isEnabledShadow : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_GEnvironment_isSupportDepthTexture(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GEnvironment* cobj = (GEnvironment *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GEnvironment_isSupportDepthTexture : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->isSupportDepthTexture();
        JS::RootedValue jsret(cx);
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GEnvironment_isSupportDepthTexture : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_GEnvironment_cleanCausticTexture(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GEnvironment* cobj = (GEnvironment *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GEnvironment_cleanCausticTexture : Invalid Native Object");
    if (argc == 0) {
        cobj->cleanCausticTexture();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GEnvironment_cleanCausticTexture : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_GEnvironment_runCausticAnimation(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GEnvironment* cobj = (GEnvironment *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GEnvironment_runCausticAnimation : Invalid Native Object");
    if (argc == 2) {
        bool arg0;
        double arg1 = 0;
        arg0 = JS::ToBoolean(args.get(0));
        ok &= JS::ToNumber( cx, args.get(1), &arg1) && !std::isnan(arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GEnvironment_runCausticAnimation : Error processing arguments");
        cobj->runCausticAnimation(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GEnvironment_runCausticAnimation : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_banca_engine_GEnvironment_setupLight(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GEnvironment* cobj = (GEnvironment *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GEnvironment_setupLight : Invalid Native Object");
    if (argc == 7) {
        cocos2d::Vec3 arg0;
        cocos2d::Vec3 arg1;
        double arg2 = 0;
        double arg3 = 0;
        double arg4 = 0;
        double arg5 = 0;
        double arg6 = 0;
        ok &= jsval_to_vector3(cx, args.get(0), &arg0);
        ok &= jsval_to_vector3(cx, args.get(1), &arg1);
        ok &= JS::ToNumber( cx, args.get(2), &arg2) && !std::isnan(arg2);
        ok &= JS::ToNumber( cx, args.get(3), &arg3) && !std::isnan(arg3);
        ok &= JS::ToNumber( cx, args.get(4), &arg4) && !std::isnan(arg4);
        ok &= JS::ToNumber( cx, args.get(5), &arg5) && !std::isnan(arg5);
        ok &= JS::ToNumber( cx, args.get(6), &arg6) && !std::isnan(arg6);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GEnvironment_setupLight : Error processing arguments");
        cobj->setupLight(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GEnvironment_setupLight : wrong number of arguments: %d, was expecting %d", argc, 7);
    return false;
}
bool js_banca_engine_GEnvironment_setupShadow(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GEnvironment* cobj = (GEnvironment *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GEnvironment_setupShadow : Invalid Native Object");
    if (argc == 4) {
        cocos2d::Node* arg0 = nullptr;
        int arg1 = 0;
        int arg2 = 0;
        int arg3 = 0;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Node*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
        ok &= jsval_to_int32(cx, args.get(3), (int32_t *)&arg3);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GEnvironment_setupShadow : Error processing arguments");
        cobj->setupShadow(arg0, arg1, arg2, arg3);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GEnvironment_setupShadow : wrong number of arguments: %d, was expecting %d", argc, 4);
    return false;
}
bool js_banca_engine_GEnvironment_setLightUpdate(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GEnvironment* cobj = (GEnvironment *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GEnvironment_setLightUpdate : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GEnvironment_setLightUpdate : Error processing arguments");
        cobj->setLightUpdate(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GEnvironment_setLightUpdate : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GEnvironment_setCameraUpdate(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GEnvironment* cobj = (GEnvironment *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GEnvironment_setCameraUpdate : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GEnvironment_setCameraUpdate : Error processing arguments");
        cobj->setCameraUpdate(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GEnvironment_setCameraUpdate : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GEnvironment_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = GEnvironment::getInstance();
        js_type_class_t *typeClass = js_get_type_from_native<GEnvironment>(ret);
        JS::RootedObject jsret(cx, jsb_ref_get_or_create_jsobject(cx, ret, typeClass, "GEnvironment"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_banca_engine_GEnvironment_getInstance : wrong number of arguments");
    return false;
}

bool js_banca_engine_GEnvironment_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    GEnvironment* cobj = new (std::nothrow) GEnvironment();

    js_type_class_t *typeClass = js_get_type_from_native<GEnvironment>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "GEnvironment"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


void js_register_banca_engine_GEnvironment(JSContext *cx, JS::HandleObject global) {
    jsb_GEnvironment_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_GEnvironment_class->name = "GEnvironment";
    jsb_GEnvironment_class->addProperty = JS_PropertyStub;
    jsb_GEnvironment_class->delProperty = JS_DeletePropertyStub;
    jsb_GEnvironment_class->getProperty = JS_PropertyStub;
    jsb_GEnvironment_class->setProperty = JS_StrictPropertyStub;
    jsb_GEnvironment_class->enumerate = JS_EnumerateStub;
    jsb_GEnvironment_class->resolve = JS_ResolveStub;
    jsb_GEnvironment_class->convert = JS_ConvertStub;
    jsb_GEnvironment_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("isEnableCausticAnimation", js_banca_engine_GEnvironment_isEnableCausticAnimation, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getDeltaTimeAnim", js_banca_engine_GEnvironment_getDeltaTimeAnim, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setEnabledShadow", js_banca_engine_GEnvironment_setEnabledShadow, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("addCausticTexture", js_banca_engine_GEnvironment_addCausticTexture, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("isEnabledShadow", js_banca_engine_GEnvironment_isEnabledShadow, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("isSupportDepthTexture", js_banca_engine_GEnvironment_isSupportDepthTexture, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("cleanCausticTexture", js_banca_engine_GEnvironment_cleanCausticTexture, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("runCausticAnimation", js_banca_engine_GEnvironment_runCausticAnimation, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setupLight", js_banca_engine_GEnvironment_setupLight, 7, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setupShadow", js_banca_engine_GEnvironment_setupShadow, 4, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setLightUpdate", js_banca_engine_GEnvironment_setLightUpdate, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setCameraUpdate", js_banca_engine_GEnvironment_setCameraUpdate, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("getInstance", js_banca_engine_GEnvironment_getInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_GEnvironment_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_GEnvironment_class,
        js_banca_engine_GEnvironment_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_GEnvironment_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "GEnvironment"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<GEnvironment>(cx, jsb_GEnvironment_class, proto, JS::NullPtr());
}

JSClass  *jsb_GMesh_class;
JSObject *jsb_GMesh_prototype;

bool js_banca_engine_GMesh_setMaterial(JSContext *cx, uint32_t argc, jsval *vp)
{
    bool ok = true;
    GMesh* cobj = nullptr;

    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx);
    obj.set(args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    cobj = (GMesh *)(proxy ? proxy->ptr : nullptr);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GMesh_setMaterial : Invalid Native Object");
    do {
        if (argc == 2) {
            GMaterial* arg0 = nullptr;
            do {
                if (args.get(0).isNull()) { arg0 = nullptr; break; }
                if (!args.get(0).isObject()) { ok = false; break; }
                js_proxy_t *jsProxy;
                JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
                jsProxy = jsb_get_js_proxy(tmpObj);
                arg0 = (GMaterial*)(jsProxy ? jsProxy->ptr : NULL);
                JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
            } while (0);
            if (!ok) { ok = true; break; }
            bool arg1;
            arg1 = JS::ToBoolean(args.get(1));
            cobj->setMaterial(arg0, arg1);
            args.rval().setUndefined();
            return true;
        }
    } while(0);

    do {
        if (argc == 1) {
            GMaterial* arg0 = nullptr;
            do {
                if (args.get(0).isNull()) { arg0 = nullptr; break; }
                if (!args.get(0).isObject()) { ok = false; break; }
                js_proxy_t *jsProxy;
                JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
                jsProxy = jsb_get_js_proxy(tmpObj);
                arg0 = (GMaterial*)(jsProxy ? jsProxy->ptr : NULL);
                JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
            } while (0);
            if (!ok) { ok = true; break; }
            cobj->setMaterial(arg0);
            args.rval().setUndefined();
            return true;
        }
    } while(0);

    JS_ReportError(cx, "js_banca_engine_GMesh_setMaterial : wrong number of arguments");
    return false;
}
bool js_banca_engine_GMesh_setGLProgramState(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GMesh* cobj = (GMesh *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GMesh_setGLProgramState : Invalid Native Object");
    if (argc == 1) {
        cocos2d::GLProgramState* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::GLProgramState*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GMesh_setGLProgramState : Error processing arguments");
        cobj->setGLProgramState(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GMesh_setGLProgramState : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GMesh_receivedShadow(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GMesh* cobj = (GMesh *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GMesh_receivedShadow : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GMesh_receivedShadow : Error processing arguments");
        cobj->receivedShadow(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GMesh_receivedShadow : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GMesh_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 3) {
        std::string arg0;
        cocos2d::MeshIndexData* arg1 = nullptr;
        cocos2d::MeshSkin* arg2 = nullptr;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        do {
            if (args.get(1).isNull()) { arg1 = nullptr; break; }
            if (!args.get(1).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(1).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (cocos2d::MeshIndexData*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        do {
            if (args.get(2).isNull()) { arg2 = nullptr; break; }
            if (!args.get(2).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(2).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg2 = (cocos2d::MeshSkin*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg2, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GMesh_create : Error processing arguments");

        auto ret = GMesh::create(arg0, arg1, arg2);
        js_type_class_t *typeClass = js_get_type_from_native<GMesh>(ret);
        JS::RootedObject jsret(cx, jsb_ref_autoreleased_create_jsobject(cx, ret, typeClass, "GMesh"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_banca_engine_GMesh_create : wrong number of arguments");
    return false;
}

bool js_banca_engine_GMesh_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    GMesh* cobj = new (std::nothrow) GMesh();

    js_type_class_t *typeClass = js_get_type_from_native<GMesh>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "GMesh"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


extern JSObject *jsb_cocos2d_Mesh_prototype;

void js_register_banca_engine_GMesh(JSContext *cx, JS::HandleObject global) {
    jsb_GMesh_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_GMesh_class->name = "GMesh";
    jsb_GMesh_class->addProperty = JS_PropertyStub;
    jsb_GMesh_class->delProperty = JS_DeletePropertyStub;
    jsb_GMesh_class->getProperty = JS_PropertyStub;
    jsb_GMesh_class->setProperty = JS_StrictPropertyStub;
    jsb_GMesh_class->enumerate = JS_EnumerateStub;
    jsb_GMesh_class->resolve = JS_ResolveStub;
    jsb_GMesh_class->convert = JS_ConvertStub;
    jsb_GMesh_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("setMaterial", js_banca_engine_GMesh_setMaterial, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setGLProgramState", js_banca_engine_GMesh_setGLProgramState, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("receivedShadow", js_banca_engine_GMesh_receivedShadow, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_banca_engine_GMesh_create, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JS::RootedObject parent_proto(cx, jsb_cocos2d_Mesh_prototype);
    jsb_GMesh_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_GMesh_class,
        js_banca_engine_GMesh_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_GMesh_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "GMesh"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<GMesh>(cx, jsb_GMesh_class, proto, parent_proto);
}

JSClass  *jsb_Path_class;
JSObject *jsb_Path_prototype;

bool js_banca_engine_Path_addPathPoint(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path* cobj = (Path *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path_addPathPoint : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Vec2 arg0;
        ok &= jsval_to_vector2(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Path_addPathPoint : Error processing arguments");
        cobj->addPathPoint(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path_addPathPoint : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Path_getCurrentAngle(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path* cobj = (Path *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path_getCurrentAngle : Invalid Native Object");
    if (argc == 0) {
        double ret = cobj->getCurrentAngle();
        JS::RootedValue jsret(cx);
        jsret = DOUBLE_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path_getCurrentAngle : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Path_calculate(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path* cobj = (Path *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path_calculate : Invalid Native Object");
    if (argc == 0) {
        cobj->calculate();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path_calculate : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Path_getLength(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path* cobj = (Path *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path_getLength : Invalid Native Object");
    if (argc == 0) {
        double ret = cobj->getLength();
        JS::RootedValue jsret(cx);
        jsret = DOUBLE_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path_getLength : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Path_getPositionFromTime(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path* cobj = (Path *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path_getPositionFromTime : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Path_getPositionFromTime : Error processing arguments");
        cocos2d::Vec2 ret = cobj->getPositionFromTime(arg0);
        JS::RootedValue jsret(cx);
        jsret = vector2_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path_getPositionFromTime : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Path_autorelease(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path* cobj = (Path *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path_autorelease : Invalid Native Object");
    if (argc == 0) {
        cocos2d::Ref* ret = cobj->autorelease();
        JS::RootedValue jsret(cx);
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<cocos2d::Ref>(cx, (cocos2d::Ref*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path_autorelease : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Path_release(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path* cobj = (Path *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path_release : Invalid Native Object");
    if (argc == 0) {
        cobj->release();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path_release : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Path_clean(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path* cobj = (Path *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path_clean : Invalid Native Object");
    if (argc == 0) {
        cobj->clean();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path_clean : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Path_setPathListener(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path* cobj = (Path *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path_setPathListener : Invalid Native Object");
    if (argc == 1) {
        std::function<void (three::extras::Curve *)> arg0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](three::extras::Curve* larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            if (larg0) {
		            largv[0] = OBJECT_TO_JSVAL(js_get_or_create_jsobject<three::extras::Curve>(cx, (three::extras::Curve*)larg0));
		        } else {
		            largv[0] = JSVAL_NULL;
		        };
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Path_setPathListener : Error processing arguments");
        cobj->setPathListener(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path_setPathListener : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Path_getCurrentAngleRad(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path* cobj = (Path *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path_getCurrentAngleRad : Invalid Native Object");
    if (argc == 0) {
        double ret = cobj->getCurrentAngleRad();
        JS::RootedValue jsret(cx);
        jsret = DOUBLE_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path_getCurrentAngleRad : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Path_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    double arg0 = 0;
    ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Path_constructor : Error processing arguments");
    Path* cobj = new (std::nothrow) Path(arg0);

    js_type_class_t *typeClass = js_get_type_from_native<Path>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "Path"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}
static bool js_banca_engine_Path_ctor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    bool ok = true;
    double arg0 = 0;
    ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_Path_ctor : Error processing arguments");
    Path *nobj = new (std::nothrow) Path(arg0);
    js_proxy_t* p = jsb_new_proxy(nobj, obj);
    jsb_ref_init(cx, &p->obj, nobj, "Path");
    bool isFound = false;
    if (JS_HasProperty(cx, obj, "_ctor", &isFound) && isFound)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", args);
    args.rval().setUndefined();
    return true;
}


    
void js_register_banca_engine_Path(JSContext *cx, JS::HandleObject global) {
    jsb_Path_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_Path_class->name = "Path";
    jsb_Path_class->addProperty = JS_PropertyStub;
    jsb_Path_class->delProperty = JS_DeletePropertyStub;
    jsb_Path_class->getProperty = JS_PropertyStub;
    jsb_Path_class->setProperty = JS_StrictPropertyStub;
    jsb_Path_class->enumerate = JS_EnumerateStub;
    jsb_Path_class->resolve = JS_ResolveStub;
    jsb_Path_class->convert = JS_ConvertStub;
    jsb_Path_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("addPathPoint", js_banca_engine_Path_addPathPoint, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCurrentAngle", js_banca_engine_Path_getCurrentAngle, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("calculate", js_banca_engine_Path_calculate, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getLength", js_banca_engine_Path_getLength, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getPositionFromTime", js_banca_engine_Path_getPositionFromTime, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("autorelease", js_banca_engine_Path_autorelease, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("release", js_banca_engine_Path_release, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("clean", js_banca_engine_Path_clean, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setPathListener", js_banca_engine_Path_setPathListener, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCurrentAngleRad", js_banca_engine_Path_getCurrentAngleRad, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("ctor", js_banca_engine_Path_ctor, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    jsb_Path_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_Path_class,
        js_banca_engine_Path_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_Path_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "Path"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<Path>(cx, jsb_Path_class, proto, JS::NullPtr());
    anonEvaluate(cx, global, "(function () { engine.Path.extend = cc.Class.extend; })()");
}

JSClass  *jsb_Path3D_class;
JSObject *jsb_Path3D_prototype;

bool js_banca_engine_Path3D_getTransformFromTimeline(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path3D* cobj = (Path3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path3D_getTransformFromTimeline : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Path3D_getTransformFromTimeline : Error processing arguments");
        cocos2d::Mat4 ret = cobj->getTransformFromTimeline(arg0);
        JS::RootedValue jsret(cx);
        jsret = matrix_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path3D_getTransformFromTimeline : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Path3D_addPathPoint(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path3D* cobj = (Path3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path3D_addPathPoint : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Vec3 arg0;
        ok &= jsval_to_vector3(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Path3D_addPathPoint : Error processing arguments");
        cobj->addPathPoint(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path3D_addPathPoint : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Path3D_calculate(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path3D* cobj = (Path3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path3D_calculate : Invalid Native Object");
    if (argc == 0) {
        cobj->calculate();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path3D_calculate : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Path3D_getDuration(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path3D* cobj = (Path3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path3D_getDuration : Invalid Native Object");
    if (argc == 0) {
        double ret = cobj->getDuration();
        JS::RootedValue jsret(cx);
        jsret = DOUBLE_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path3D_getDuration : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Path3D_autorelease(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path3D* cobj = (Path3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path3D_autorelease : Invalid Native Object");
    if (argc == 0) {
        cocos2d::Ref* ret = cobj->autorelease();
        JS::RootedValue jsret(cx);
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<cocos2d::Ref>(cx, (cocos2d::Ref*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path3D_autorelease : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Path3D_recalculateWithResolution(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path3D* cobj = (Path3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path3D_recalculateWithResolution : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Path3D_recalculateWithResolution : Error processing arguments");
        cobj->recalculateWithResolution(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path3D_recalculateWithResolution : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Path3D_clean(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path3D* cobj = (Path3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path3D_clean : Invalid Native Object");
    if (argc == 0) {
        cobj->clean();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path3D_clean : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Path3D_release(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path3D* cobj = (Path3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path3D_release : Invalid Native Object");
    if (argc == 0) {
        cobj->release();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path3D_release : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Path3D_setDuration(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Path3D* cobj = (Path3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Path3D_setDuration : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Path3D_setDuration : Error processing arguments");
        cobj->setDuration(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Path3D_setDuration : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Path3D_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    double arg0 = 0;
    ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Path3D_constructor : Error processing arguments");
    Path3D* cobj = new (std::nothrow) Path3D(arg0);

    js_type_class_t *typeClass = js_get_type_from_native<Path3D>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "Path3D"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}
static bool js_banca_engine_Path3D_ctor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    bool ok = true;
    double arg0 = 0;
    ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_Path3D_ctor : Error processing arguments");
    Path3D *nobj = new (std::nothrow) Path3D(arg0);
    js_proxy_t* p = jsb_new_proxy(nobj, obj);
    jsb_ref_init(cx, &p->obj, nobj, "Path3D");
    bool isFound = false;
    if (JS_HasProperty(cx, obj, "_ctor", &isFound) && isFound)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", args);
    args.rval().setUndefined();
    return true;
}


    
void js_register_banca_engine_Path3D(JSContext *cx, JS::HandleObject global) {
    jsb_Path3D_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_Path3D_class->name = "Path3D";
    jsb_Path3D_class->addProperty = JS_PropertyStub;
    jsb_Path3D_class->delProperty = JS_DeletePropertyStub;
    jsb_Path3D_class->getProperty = JS_PropertyStub;
    jsb_Path3D_class->setProperty = JS_StrictPropertyStub;
    jsb_Path3D_class->enumerate = JS_EnumerateStub;
    jsb_Path3D_class->resolve = JS_ResolveStub;
    jsb_Path3D_class->convert = JS_ConvertStub;
    jsb_Path3D_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("getTransformFromTimeline", js_banca_engine_Path3D_getTransformFromTimeline, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("addPathPoint", js_banca_engine_Path3D_addPathPoint, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("calculate", js_banca_engine_Path3D_calculate, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getDuration", js_banca_engine_Path3D_getDuration, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("autorelease", js_banca_engine_Path3D_autorelease, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("recalculateWithResolution", js_banca_engine_Path3D_recalculateWithResolution, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("clean", js_banca_engine_Path3D_clean, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("release", js_banca_engine_Path3D_release, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setDuration", js_banca_engine_Path3D_setDuration, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("ctor", js_banca_engine_Path3D_ctor, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    jsb_Path3D_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_Path3D_class,
        js_banca_engine_Path3D_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_Path3D_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "Path3D"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<Path3D>(cx, jsb_Path3D_class, proto, JS::NullPtr());
    anonEvaluate(cx, global, "(function () { engine.Path3D.extend = cc.Class.extend; })()");
}

JSClass  *jsb_HoldFishInfo_class;
JSObject *jsb_HoldFishInfo_prototype;

bool js_banca_engine_HoldFishInfo_setFish(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    HoldFishInfo* cobj = (HoldFishInfo *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_HoldFishInfo_setFish : Invalid Native Object");
    if (argc == 1) {
        Entity* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (Entity*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_HoldFishInfo_setFish : Error processing arguments");
        cobj->setFish(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_HoldFishInfo_setFish : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_HoldFishInfo_getFish(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    HoldFishInfo* cobj = (HoldFishInfo *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_HoldFishInfo_getFish : Invalid Native Object");
    if (argc == 0) {
        Entity* ret = cobj->getFish();
        JS::RootedValue jsret(cx);
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<Entity>(cx, (Entity*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_HoldFishInfo_getFish : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_HoldFishInfo_autorelease(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    HoldFishInfo* cobj = (HoldFishInfo *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_HoldFishInfo_autorelease : Invalid Native Object");
    if (argc == 0) {
        cocos2d::Ref* ret = cobj->autorelease();
        JS::RootedValue jsret(cx);
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<cocos2d::Ref>(cx, (cocos2d::Ref*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_HoldFishInfo_autorelease : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_HoldFishInfo_getIsHolding(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    HoldFishInfo* cobj = (HoldFishInfo *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_HoldFishInfo_getIsHolding : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->getIsHolding();
        JS::RootedValue jsret(cx);
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_HoldFishInfo_getIsHolding : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_HoldFishInfo_getPlayer(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    HoldFishInfo* cobj = (HoldFishInfo *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_HoldFishInfo_getPlayer : Invalid Native Object");
    if (argc == 0) {
        cocos2d::Node* ret = cobj->getPlayer();
        JS::RootedValue jsret(cx);
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<cocos2d::Node>(cx, (cocos2d::Node*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_HoldFishInfo_getPlayer : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_HoldFishInfo_setIsHolding(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    HoldFishInfo* cobj = (HoldFishInfo *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_HoldFishInfo_setIsHolding : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_HoldFishInfo_setIsHolding : Error processing arguments");
        cobj->setIsHolding(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_HoldFishInfo_setIsHolding : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_HoldFishInfo_setPlayer(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    HoldFishInfo* cobj = (HoldFishInfo *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_HoldFishInfo_setPlayer : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Node* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Node*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_HoldFishInfo_setPlayer : Error processing arguments");
        cobj->setPlayer(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_HoldFishInfo_setPlayer : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_HoldFishInfo_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    HoldFishInfo* cobj = new (std::nothrow) HoldFishInfo();

    js_type_class_t *typeClass = js_get_type_from_native<HoldFishInfo>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "HoldFishInfo"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}
static bool js_banca_engine_HoldFishInfo_ctor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    HoldFishInfo *nobj = new (std::nothrow) HoldFishInfo();
    js_proxy_t* p = jsb_new_proxy(nobj, obj);
    jsb_ref_init(cx, &p->obj, nobj, "HoldFishInfo");
    bool isFound = false;
    if (JS_HasProperty(cx, obj, "_ctor", &isFound) && isFound)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", args);
    args.rval().setUndefined();
    return true;
}


    
void js_register_banca_engine_HoldFishInfo(JSContext *cx, JS::HandleObject global) {
    jsb_HoldFishInfo_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_HoldFishInfo_class->name = "HoldFishInfo";
    jsb_HoldFishInfo_class->addProperty = JS_PropertyStub;
    jsb_HoldFishInfo_class->delProperty = JS_DeletePropertyStub;
    jsb_HoldFishInfo_class->getProperty = JS_PropertyStub;
    jsb_HoldFishInfo_class->setProperty = JS_StrictPropertyStub;
    jsb_HoldFishInfo_class->enumerate = JS_EnumerateStub;
    jsb_HoldFishInfo_class->resolve = JS_ResolveStub;
    jsb_HoldFishInfo_class->convert = JS_ConvertStub;
    jsb_HoldFishInfo_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("setFish", js_banca_engine_HoldFishInfo_setFish, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getFish", js_banca_engine_HoldFishInfo_getFish, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("autorelease", js_banca_engine_HoldFishInfo_autorelease, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getIsHolding", js_banca_engine_HoldFishInfo_getIsHolding, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getPlayer", js_banca_engine_HoldFishInfo_getPlayer, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setIsHolding", js_banca_engine_HoldFishInfo_setIsHolding, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setPlayer", js_banca_engine_HoldFishInfo_setPlayer, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("ctor", js_banca_engine_HoldFishInfo_ctor, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    jsb_HoldFishInfo_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_HoldFishInfo_class,
        js_banca_engine_HoldFishInfo_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_HoldFishInfo_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "HoldFishInfo"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<HoldFishInfo>(cx, jsb_HoldFishInfo_class, proto, JS::NullPtr());
    anonEvaluate(cx, global, "(function () { engine.HoldFishInfo.extend = cc.Class.extend; })()");
}

JSClass  *jsb_Entity_class;
JSObject *jsb_Entity_prototype;

bool js_banca_engine_Entity_setFinishFunc(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Entity* cobj = (Entity *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Entity_setFinishFunc : Invalid Native Object");
    if (argc == 1) {
        std::function<void (Entity *)> arg0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](Entity* larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            if (larg0) {
		            largv[0] = OBJECT_TO_JSVAL(js_get_or_create_jsobject<Entity>(cx, (Entity*)larg0));
		        } else {
		            largv[0] = JSVAL_NULL;
		        };
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Entity_setFinishFunc : Error processing arguments");
        cobj->setFinishFunc(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Entity_setFinishFunc : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Entity_setUpdateFunc(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Entity* cobj = (Entity *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Entity_setUpdateFunc : Invalid Native Object");
    if (argc == 1) {
        std::function<void (float)> arg0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](float larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = DOUBLE_TO_JSVAL(larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Entity_setUpdateFunc : Error processing arguments");
        cobj->setUpdateFunc(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Entity_setUpdateFunc : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Entity_getBodyVelocity(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Entity* cobj = (Entity *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Entity_getBodyVelocity : Invalid Native Object");
    if (argc == 0) {
        cocos2d::Vec2 ret = cobj->getBodyVelocity();
        JS::RootedValue jsret(cx);
        jsret = vector2_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Entity_getBodyVelocity : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Entity_setTransform(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Entity* cobj = (Entity *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Entity_setTransform : Invalid Native Object");
    if (argc == 2) {
        cocos2d::Vec2 arg0;
        double arg1 = 0;
        ok &= jsval_to_vector2(cx, args.get(0), &arg0);
        ok &= JS::ToNumber( cx, args.get(1), &arg1) && !std::isnan(arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Entity_setTransform : Error processing arguments");
        cobj->setTransform(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Entity_setTransform : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_banca_engine_Entity_getType(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Entity* cobj = (Entity *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Entity_getType : Invalid Native Object");
    if (argc == 0) {
        int ret = (int)cobj->getType();
        JS::RootedValue jsret(cx);
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Entity_getType : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Entity_update(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Entity* cobj = (Entity *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Entity_update : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Entity_update : Error processing arguments");
        cobj->update(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Entity_update : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Entity_autorelease(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Entity* cobj = (Entity *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Entity_autorelease : Invalid Native Object");
    if (argc == 0) {
        cocos2d::Ref* ret = cobj->autorelease();
        JS::RootedValue jsret(cx);
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<cocos2d::Ref>(cx, (cocos2d::Ref*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Entity_autorelease : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Entity_isNeedRemove(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Entity* cobj = (Entity *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Entity_isNeedRemove : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->isNeedRemove();
        JS::RootedValue jsret(cx);
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Entity_isNeedRemove : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Entity_rotationFromVel(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Entity* cobj = (Entity *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Entity_rotationFromVel : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Vec2 arg0;
        ok &= jsval_to_vector2(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Entity_rotationFromVel : Error processing arguments");
        double ret = cobj->rotationFromVel(arg0);
        JS::RootedValue jsret(cx);
        jsret = DOUBLE_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Entity_rotationFromVel : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Entity_setContentSize(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Entity* cobj = (Entity *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Entity_setContentSize : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Size arg0;
        ok &= jsval_to_ccsize(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Entity_setContentSize : Error processing arguments");
        cobj->setContentSize(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Entity_setContentSize : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Entity_getBodyPosition(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Entity* cobj = (Entity *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Entity_getBodyPosition : Invalid Native Object");
    if (argc == 0) {
        cocos2d::Vec2 ret = cobj->getBodyPosition();
        JS::RootedValue jsret(cx);
        jsret = vector2_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Entity_getBodyPosition : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Entity_setNodeDisplay(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Entity* cobj = (Entity *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Entity_setNodeDisplay : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Node* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Node*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Entity_setNodeDisplay : Error processing arguments");
        cobj->setNodeDisplay(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Entity_setNodeDisplay : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Entity_release(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Entity* cobj = (Entity *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Entity_release : Invalid Native Object");
    if (argc == 0) {
        cobj->release();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Entity_release : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Entity_setPosition(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Entity* cobj = (Entity *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Entity_setPosition : Invalid Native Object");
    if (argc == 2) {
        double arg0 = 0;
        double arg1 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        ok &= JS::ToNumber( cx, args.get(1), &arg1) && !std::isnan(arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Entity_setPosition : Error processing arguments");
        cobj->setPosition(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Entity_setPosition : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_banca_engine_Entity_getNodeDisplay(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Entity* cobj = (Entity *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Entity_getNodeDisplay : Invalid Native Object");
    if (argc == 0) {
        cocos2d::Node* ret = cobj->getNodeDisplay();
        JS::RootedValue jsret(cx);
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<cocos2d::Node>(cx, (cocos2d::Node*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Entity_getNodeDisplay : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Entity_setVelocity(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Entity* cobj = (Entity *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Entity_setVelocity : Invalid Native Object");
    if (argc == 2) {
        double arg0 = 0;
        double arg1 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        ok &= JS::ToNumber( cx, args.get(1), &arg1) && !std::isnan(arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Entity_setVelocity : Error processing arguments");
        cobj->setVelocity(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Entity_setVelocity : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_banca_engine_Entity_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    Entity* cobj = new (std::nothrow) Entity();

    js_type_class_t *typeClass = js_get_type_from_native<Entity>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "Entity"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}
static bool js_banca_engine_Entity_ctor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    Entity *nobj = new (std::nothrow) Entity();
    js_proxy_t* p = jsb_new_proxy(nobj, obj);
    jsb_ref_init(cx, &p->obj, nobj, "Entity");
    bool isFound = false;
    if (JS_HasProperty(cx, obj, "_ctor", &isFound) && isFound)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", args);
    args.rval().setUndefined();
    return true;
}


    
void js_register_banca_engine_Entity(JSContext *cx, JS::HandleObject global) {
    jsb_Entity_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_Entity_class->name = "Entity";
    jsb_Entity_class->addProperty = JS_PropertyStub;
    jsb_Entity_class->delProperty = JS_DeletePropertyStub;
    jsb_Entity_class->getProperty = JS_PropertyStub;
    jsb_Entity_class->setProperty = JS_StrictPropertyStub;
    jsb_Entity_class->enumerate = JS_EnumerateStub;
    jsb_Entity_class->resolve = JS_ResolveStub;
    jsb_Entity_class->convert = JS_ConvertStub;
    jsb_Entity_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("setFinishFunc", js_banca_engine_Entity_setFinishFunc, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setUpdateFunc", js_banca_engine_Entity_setUpdateFunc, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getBodyVelocity", js_banca_engine_Entity_getBodyVelocity, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setTransform", js_banca_engine_Entity_setTransform, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getType", js_banca_engine_Entity_getType, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("update", js_banca_engine_Entity_update, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("autorelease", js_banca_engine_Entity_autorelease, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("isNeedRemove", js_banca_engine_Entity_isNeedRemove, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("rotationFromVel", js_banca_engine_Entity_rotationFromVel, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setContentSize", js_banca_engine_Entity_setContentSize, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getBodyPosition", js_banca_engine_Entity_getBodyPosition, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setNodeDisplay", js_banca_engine_Entity_setNodeDisplay, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("release", js_banca_engine_Entity_release, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setPosition", js_banca_engine_Entity_setPosition, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getNodeDisplay", js_banca_engine_Entity_getNodeDisplay, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setVelocity", js_banca_engine_Entity_setVelocity, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("ctor", js_banca_engine_Entity_ctor, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    jsb_Entity_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_Entity_class,
        js_banca_engine_Entity_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_Entity_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "Entity"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<Entity>(cx, jsb_Entity_class, proto, JS::NullPtr());
    anonEvaluate(cx, global, "(function () { engine.Entity.extend = cc.Class.extend; })()");
}

JSClass  *jsb_Fish_class;
JSObject *jsb_Fish_prototype;

bool js_banca_engine_Fish_enableCheckOutsite(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Fish* cobj = (Fish *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Fish_enableCheckOutsite : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Fish_enableCheckOutsite : Error processing arguments");
        cobj->enableCheckOutsite(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Fish_enableCheckOutsite : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Fish_pause(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Fish* cobj = (Fish *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Fish_pause : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Fish_pause : Error processing arguments");
        cobj->pause(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Fish_pause : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Fish_enableAutoDie(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Fish* cobj = (Fish *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Fish_enableAutoDie : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Fish_enableAutoDie : Error processing arguments");
        cobj->enableAutoDie(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Fish_enableAutoDie : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Fish_startWithPath(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Fish* cobj = (Fish *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Fish_startWithPath : Invalid Native Object");
    if (argc == 2) {
        Path* arg0 = nullptr;
        double arg1 = 0;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (Path*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        ok &= JS::ToNumber( cx, args.get(1), &arg1) && !std::isnan(arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Fish_startWithPath : Error processing arguments");
        cobj->startWithPath(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Fish_startWithPath : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_banca_engine_Fish_update(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Fish* cobj = (Fish *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Fish_update : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Fish_update : Error processing arguments");
        cobj->update(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Fish_update : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Fish_isOutsite(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Fish* cobj = (Fish *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Fish_isOutsite : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->isOutsite();
        JS::RootedValue jsret(cx);
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Fish_isOutsite : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Fish_onNewCurve(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Fish* cobj = (Fish *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Fish_onNewCurve : Invalid Native Object");
    if (argc == 1) {
        three::extras::Curve* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (three::extras::Curve*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Fish_onNewCurve : Error processing arguments");
        cobj->onNewCurve(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Fish_onNewCurve : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Fish_enableFlip(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Fish* cobj = (Fish *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Fish_enableFlip : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Fish_enableFlip : Error processing arguments");
        cobj->enableFlip(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Fish_enableFlip : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Fish_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    Fish* cobj = new (std::nothrow) Fish();

    js_type_class_t *typeClass = js_get_type_from_native<Fish>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "Fish"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}
static bool js_banca_engine_Fish_ctor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    Fish *nobj = new (std::nothrow) Fish();
    js_proxy_t* p = jsb_new_proxy(nobj, obj);
    jsb_ref_init(cx, &p->obj, nobj, "Fish");
    bool isFound = false;
    if (JS_HasProperty(cx, obj, "_ctor", &isFound) && isFound)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", args);
    args.rval().setUndefined();
    return true;
}


extern JSObject *jsb_Entity_prototype;

    
void js_register_banca_engine_Fish(JSContext *cx, JS::HandleObject global) {
    jsb_Fish_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_Fish_class->name = "Fish";
    jsb_Fish_class->addProperty = JS_PropertyStub;
    jsb_Fish_class->delProperty = JS_DeletePropertyStub;
    jsb_Fish_class->getProperty = JS_PropertyStub;
    jsb_Fish_class->setProperty = JS_StrictPropertyStub;
    jsb_Fish_class->enumerate = JS_EnumerateStub;
    jsb_Fish_class->resolve = JS_ResolveStub;
    jsb_Fish_class->convert = JS_ConvertStub;
    jsb_Fish_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("enableCheckOutsite", js_banca_engine_Fish_enableCheckOutsite, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("pause", js_banca_engine_Fish_pause, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("enableAutoDie", js_banca_engine_Fish_enableAutoDie, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("startWithPath", js_banca_engine_Fish_startWithPath, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("update", js_banca_engine_Fish_update, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("isOutsite", js_banca_engine_Fish_isOutsite, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onNewCurve", js_banca_engine_Fish_onNewCurve, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("enableFlip", js_banca_engine_Fish_enableFlip, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("ctor", js_banca_engine_Fish_ctor, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    JS::RootedObject parent_proto(cx, jsb_Entity_prototype);
    jsb_Fish_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_Fish_class,
        js_banca_engine_Fish_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_Fish_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "Fish"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<Fish>(cx, jsb_Fish_class, proto, parent_proto);
    anonEvaluate(cx, global, "(function () { engine.Fish.extend = cc.Class.extend; })()");
}

JSClass  *jsb_Wall_class;
JSObject *jsb_Wall_prototype;

bool js_banca_engine_Wall_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    Wall* cobj = new (std::nothrow) Wall();

    js_type_class_t *typeClass = js_get_type_from_native<Wall>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "Wall"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}
static bool js_banca_engine_Wall_ctor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    Wall *nobj = new (std::nothrow) Wall();
    js_proxy_t* p = jsb_new_proxy(nobj, obj);
    jsb_ref_init(cx, &p->obj, nobj, "Wall");
    bool isFound = false;
    if (JS_HasProperty(cx, obj, "_ctor", &isFound) && isFound)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", args);
    args.rval().setUndefined();
    return true;
}


extern JSObject *jsb_Entity_prototype;

    
void js_register_banca_engine_Wall(JSContext *cx, JS::HandleObject global) {
    jsb_Wall_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_Wall_class->name = "Wall";
    jsb_Wall_class->addProperty = JS_PropertyStub;
    jsb_Wall_class->delProperty = JS_DeletePropertyStub;
    jsb_Wall_class->getProperty = JS_PropertyStub;
    jsb_Wall_class->setProperty = JS_StrictPropertyStub;
    jsb_Wall_class->enumerate = JS_EnumerateStub;
    jsb_Wall_class->resolve = JS_ResolveStub;
    jsb_Wall_class->convert = JS_ConvertStub;
    jsb_Wall_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("ctor", js_banca_engine_Wall_ctor, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    JS::RootedObject parent_proto(cx, jsb_Entity_prototype);
    jsb_Wall_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_Wall_class,
        js_banca_engine_Wall_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_Wall_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "Wall"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<Wall>(cx, jsb_Wall_class, proto, parent_proto);
    anonEvaluate(cx, global, "(function () { engine.Wall.extend = cc.Class.extend; })()");
}

JSClass  *jsb_Bullet_class;
JSObject *jsb_Bullet_prototype;

bool js_banca_engine_Bullet_getLive(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Bullet* cobj = (Bullet *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Bullet_getLive : Invalid Native Object");
    if (argc == 0) {
        int ret = cobj->getLive();
        JS::RootedValue jsret(cx);
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Bullet_getLive : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Bullet_getHoldInfo(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Bullet* cobj = (Bullet *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Bullet_getHoldInfo : Invalid Native Object");
    if (argc == 0) {
        HoldFishInfo* ret = cobj->getHoldInfo();
        JS::RootedValue jsret(cx);
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<HoldFishInfo>(cx, (HoldFishInfo*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Bullet_getHoldInfo : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Bullet_setVelLength(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Bullet* cobj = (Bullet *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Bullet_setVelLength : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Bullet_setVelLength : Error processing arguments");
        cobj->setVelLength(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Bullet_setVelLength : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Bullet_update(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Bullet* cobj = (Bullet *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Bullet_update : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Bullet_update : Error processing arguments");
        cobj->update(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Bullet_update : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Bullet_setHoldInfo(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Bullet* cobj = (Bullet *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Bullet_setHoldInfo : Invalid Native Object");
    if (argc == 1) {
        HoldFishInfo* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (HoldFishInfo*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Bullet_setHoldInfo : Error processing arguments");
        cobj->setHoldInfo(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Bullet_setHoldInfo : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Bullet_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Bullet_constructor : Error processing arguments");
    Bullet* cobj = new (std::nothrow) Bullet(arg0);

    js_type_class_t *typeClass = js_get_type_from_native<Bullet>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "Bullet"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}
static bool js_banca_engine_Bullet_ctor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_Bullet_ctor : Error processing arguments");
    Bullet *nobj = new (std::nothrow) Bullet(arg0);
    js_proxy_t* p = jsb_new_proxy(nobj, obj);
    jsb_ref_init(cx, &p->obj, nobj, "Bullet");
    bool isFound = false;
    if (JS_HasProperty(cx, obj, "_ctor", &isFound) && isFound)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", args);
    args.rval().setUndefined();
    return true;
}


extern JSObject *jsb_Entity_prototype;

    
void js_register_banca_engine_Bullet(JSContext *cx, JS::HandleObject global) {
    jsb_Bullet_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_Bullet_class->name = "Bullet";
    jsb_Bullet_class->addProperty = JS_PropertyStub;
    jsb_Bullet_class->delProperty = JS_DeletePropertyStub;
    jsb_Bullet_class->getProperty = JS_PropertyStub;
    jsb_Bullet_class->setProperty = JS_StrictPropertyStub;
    jsb_Bullet_class->enumerate = JS_EnumerateStub;
    jsb_Bullet_class->resolve = JS_ResolveStub;
    jsb_Bullet_class->convert = JS_ConvertStub;
    jsb_Bullet_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("getLive", js_banca_engine_Bullet_getLive, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getHoldInfo", js_banca_engine_Bullet_getHoldInfo, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setVelLength", js_banca_engine_Bullet_setVelLength, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("update", js_banca_engine_Bullet_update, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setHoldInfo", js_banca_engine_Bullet_setHoldInfo, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("ctor", js_banca_engine_Bullet_ctor, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    JS::RootedObject parent_proto(cx, jsb_Entity_prototype);
    jsb_Bullet_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_Bullet_class,
        js_banca_engine_Bullet_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_Bullet_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "Bullet"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<Bullet>(cx, jsb_Bullet_class, proto, parent_proto);
    anonEvaluate(cx, global, "(function () { engine.Bullet.extend = cc.Class.extend; })()");
}

JSClass  *jsb_Fish3D_class;
JSObject *jsb_Fish3D_prototype;

bool js_banca_engine_Fish3D_setPath3D(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Fish3D* cobj = (Fish3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Fish3D_setPath3D : Invalid Native Object");
    if (argc == 1) {
        Path3D* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (Path3D*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Fish3D_setPath3D : Error processing arguments");
        cobj->setPath3D(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Fish3D_setPath3D : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Fish3D_enableCheckOutsite(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Fish3D* cobj = (Fish3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Fish3D_enableCheckOutsite : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Fish3D_enableCheckOutsite : Error processing arguments");
        cobj->enableCheckOutsite(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Fish3D_enableCheckOutsite : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Fish3D_pause(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Fish3D* cobj = (Fish3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Fish3D_pause : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Fish3D_pause : Error processing arguments");
        cobj->pause(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Fish3D_pause : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Fish3D_resume(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Fish3D* cobj = (Fish3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Fish3D_resume : Invalid Native Object");
    if (argc == 0) {
        cobj->resume();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Fish3D_resume : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Fish3D_enableAutoDie(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Fish3D* cobj = (Fish3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Fish3D_enableAutoDie : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Fish3D_enableAutoDie : Error processing arguments");
        cobj->enableAutoDie(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Fish3D_enableAutoDie : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Fish3D_update(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Fish3D* cobj = (Fish3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Fish3D_update : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Fish3D_update : Error processing arguments");
        cobj->update(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Fish3D_update : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Fish3D_start(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Fish3D* cobj = (Fish3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Fish3D_start : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_Fish3D_start : Error processing arguments");
        cobj->start(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Fish3D_start : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_Fish3D_isOutsite(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Fish3D* cobj = (Fish3D *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Fish3D_isOutsite : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->isOutsite();
        JS::RootedValue jsret(cx);
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Fish3D_isOutsite : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Fish3D_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    Fish3D* cobj = new (std::nothrow) Fish3D();

    js_type_class_t *typeClass = js_get_type_from_native<Fish3D>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "Fish3D"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}
static bool js_banca_engine_Fish3D_ctor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    Fish3D *nobj = new (std::nothrow) Fish3D();
    js_proxy_t* p = jsb_new_proxy(nobj, obj);
    jsb_ref_init(cx, &p->obj, nobj, "Fish3D");
    bool isFound = false;
    if (JS_HasProperty(cx, obj, "_ctor", &isFound) && isFound)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", args);
    args.rval().setUndefined();
    return true;
}


extern JSObject *jsb_Entity_prototype;

    
void js_register_banca_engine_Fish3D(JSContext *cx, JS::HandleObject global) {
    jsb_Fish3D_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_Fish3D_class->name = "Fish3D";
    jsb_Fish3D_class->addProperty = JS_PropertyStub;
    jsb_Fish3D_class->delProperty = JS_DeletePropertyStub;
    jsb_Fish3D_class->getProperty = JS_PropertyStub;
    jsb_Fish3D_class->setProperty = JS_StrictPropertyStub;
    jsb_Fish3D_class->enumerate = JS_EnumerateStub;
    jsb_Fish3D_class->resolve = JS_ResolveStub;
    jsb_Fish3D_class->convert = JS_ConvertStub;
    jsb_Fish3D_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("setPath3D", js_banca_engine_Fish3D_setPath3D, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("enableCheckOutsite", js_banca_engine_Fish3D_enableCheckOutsite, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("pause", js_banca_engine_Fish3D_pause, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("resume", js_banca_engine_Fish3D_resume, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("enableAutoDie", js_banca_engine_Fish3D_enableAutoDie, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("update", js_banca_engine_Fish3D_update, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("start", js_banca_engine_Fish3D_start, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("isOutsite", js_banca_engine_Fish3D_isOutsite, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("ctor", js_banca_engine_Fish3D_ctor, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    JS::RootedObject parent_proto(cx, jsb_Entity_prototype);
    jsb_Fish3D_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_Fish3D_class,
        js_banca_engine_Fish3D_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_Fish3D_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "Fish3D"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<Fish3D>(cx, jsb_Fish3D_class, proto, parent_proto);
    anonEvaluate(cx, global, "(function () { engine.Fish3D.extend = cc.Class.extend; })()");
}

JSClass  *jsb_Setting_class;
JSObject *jsb_Setting_prototype;

bool js_banca_engine_Setting_autorelease(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Setting* cobj = (Setting *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_Setting_autorelease : Invalid Native Object");
    if (argc == 0) {
        cocos2d::Ref* ret = cobj->autorelease();
        JS::RootedValue jsret(cx);
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<cocos2d::Ref>(cx, (cocos2d::Ref*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_Setting_autorelease : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_Setting_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    Setting* cobj = new (std::nothrow) Setting();

    js_type_class_t *typeClass = js_get_type_from_native<Setting>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "Setting"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


void js_register_banca_engine_Setting(JSContext *cx, JS::HandleObject global) {
    jsb_Setting_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_Setting_class->name = "Setting";
    jsb_Setting_class->addProperty = JS_PropertyStub;
    jsb_Setting_class->delProperty = JS_DeletePropertyStub;
    jsb_Setting_class->getProperty = JS_PropertyStub;
    jsb_Setting_class->setProperty = JS_StrictPropertyStub;
    jsb_Setting_class->enumerate = JS_EnumerateStub;
    jsb_Setting_class->resolve = JS_ResolveStub;
    jsb_Setting_class->convert = JS_ConvertStub;
    jsb_Setting_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("autorelease", js_banca_engine_Setting_autorelease, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    jsb_Setting_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_Setting_class,
        js_banca_engine_Setting_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_Setting_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "Setting"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<Setting>(cx, jsb_Setting_class, proto, JS::NullPtr());
}

JSClass  *jsb_GameManager_class;
JSObject *jsb_GameManager_prototype;

bool js_banca_engine_GameManager_getFishByPos(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager_getFishByPos : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Vec2 arg0;
        ok &= jsval_to_vector2(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GameManager_getFishByPos : Error processing arguments");
        Fish* ret = cobj->getFishByPos(arg0);
        JS::RootedValue jsret(cx);
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<Fish>(cx, (Fish*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager_getFishByPos : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GameManager_destroyAllEntity(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager_destroyAllEntity : Invalid Native Object");
    if (argc == 0) {
        cobj->destroyAllEntity();
        args.rval().setUndefined();
        return true;
    }
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GameManager_destroyAllEntity : Error processing arguments");
        cobj->destroyAllEntity(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager_destroyAllEntity : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_GameManager_onRealDestroyFish(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager_onRealDestroyFish : Invalid Native Object");
    if (argc == 1) {
        Entity* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (Entity*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GameManager_onRealDestroyFish : Error processing arguments");
        cobj->onRealDestroyFish(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager_onRealDestroyFish : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GameManager_shootBullet(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager_shootBullet : Invalid Native Object");
    if (argc == 3) {
        Entity* arg0 = nullptr;
        cocos2d::Vec2 arg1;
        cocos2d::Vec2 arg2;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (Entity*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        ok &= jsval_to_vector2(cx, args.get(1), &arg1);
        ok &= jsval_to_vector2(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GameManager_shootBullet : Error processing arguments");
        Entity* ret = cobj->shootBullet(arg0, arg1, arg2);
        JS::RootedValue jsret(cx);
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<Entity>(cx, (Entity*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager_shootBullet : wrong number of arguments: %d, was expecting %d", argc, 3);
    return false;
}
bool js_banca_engine_GameManager__doUpdate(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager__doUpdate : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GameManager__doUpdate : Error processing arguments");
        cobj->_doUpdate(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager__doUpdate : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GameManager_createWall(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager_createWall : Invalid Native Object");
    if (argc == 1) {
        Entity* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (Entity*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GameManager_createWall : Error processing arguments");
        Entity* ret = cobj->createWall(arg0);
        JS::RootedValue jsret(cx);
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<Entity>(cx, (Entity*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager_createWall : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GameManager_setFishDestroyDelegate(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager_setFishDestroyDelegate : Invalid Native Object");
    if (argc == 1) {
        std::function<void (Entity *)> arg0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](Entity* larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            if (larg0) {
		            largv[0] = OBJECT_TO_JSVAL(js_get_or_create_jsobject<Entity>(cx, (Entity*)larg0));
		        } else {
		            largv[0] = JSVAL_NULL;
		        };
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GameManager_setFishDestroyDelegate : Error processing arguments");
        cobj->setFishDestroyDelegate(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager_setFishDestroyDelegate : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GameManager_BeginContact(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager_BeginContact : Invalid Native Object");
    if (argc == 1) {
        b2Contact* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (b2Contact*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GameManager_BeginContact : Error processing arguments");
        cobj->BeginContact(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager_BeginContact : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GameManager_update(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager_update : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GameManager_update : Error processing arguments");
        cobj->update(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager_update : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GameManager_createBodyForBullet(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager_createBodyForBullet : Invalid Native Object");
    if (argc == 2) {
        Entity* arg0 = nullptr;
        cocos2d::Vec2 arg1;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (Entity*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        ok &= jsval_to_vector2(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GameManager_createBodyForBullet : Error processing arguments");
        Entity* ret = cobj->createBodyForBullet(arg0, arg1);
        JS::RootedValue jsret(cx);
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<Entity>(cx, (Entity*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager_createBodyForBullet : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_banca_engine_GameManager_autorelease(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager_autorelease : Invalid Native Object");
    if (argc == 0) {
        cocos2d::Ref* ret = cobj->autorelease();
        JS::RootedValue jsret(cx);
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<cocos2d::Ref>(cx, (cocos2d::Ref*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager_autorelease : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_GameManager_initWorld(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager_initWorld : Invalid Native Object");
    if (argc == 0) {
        cobj->initWorld();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager_initWorld : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_banca_engine_GameManager_destroyEntity(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager_destroyEntity : Invalid Native Object");
    if (argc == 1) {
        Entity* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (Entity*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GameManager_destroyEntity : Error processing arguments");
        cobj->destroyEntity(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager_destroyEntity : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GameManager_createBodyForFish(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager_createBodyForFish : Invalid Native Object");
    if (argc == 2) {
        Entity* arg0 = nullptr;
        cocos2d::Vec2 arg1;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (Entity*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        ok &= jsval_to_vector2(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GameManager_createBodyForFish : Error processing arguments");
        Entity* ret = cobj->createBodyForFish(arg0, arg1);
        JS::RootedValue jsret(cx);
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<Entity>(cx, (Entity*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager_createBodyForFish : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_banca_engine_GameManager_setEntityCollisionListener(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager_setEntityCollisionListener : Invalid Native Object");
    if (argc == 1) {
        std::function<void (Entity *, Entity *, cocos2d::Vec2)> arg0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](Entity* larg0, Entity* larg1, cocos2d::Vec2 larg2) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[3];
		            if (larg0) {
		            largv[0] = OBJECT_TO_JSVAL(js_get_or_create_jsobject<Entity>(cx, (Entity*)larg0));
		        } else {
		            largv[0] = JSVAL_NULL;
		        };
		            if (larg1) {
		            largv[1] = OBJECT_TO_JSVAL(js_get_or_create_jsobject<Entity>(cx, (Entity*)larg1));
		        } else {
		            largv[1] = JSVAL_NULL;
		        };
		            largv[2] = vector2_to_jsval(cx, larg2);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(3, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GameManager_setEntityCollisionListener : Error processing arguments");
        cobj->setEntityCollisionListener(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager_setEntityCollisionListener : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GameManager_PreSolve(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager_PreSolve : Invalid Native Object");
    if (argc == 2) {
        b2Contact* arg0 = nullptr;
        const b2Manifold* arg1 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (b2Contact*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        #pragma warning NO CONVERSION TO NATIVE FOR b2Manifold*
		ok = false;
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GameManager_PreSolve : Error processing arguments");
        cobj->PreSolve(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager_PreSolve : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_banca_engine_GameManager_setOnContactPreSolve(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    GameManager* cobj = (GameManager *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_banca_engine_GameManager_setOnContactPreSolve : Invalid Native Object");
    if (argc == 1) {
        std::function<bool (Entity *, Entity *, cocos2d::Vec2)> arg0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](Entity* larg0, Entity* larg1, cocos2d::Vec2 larg2) -> bool {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[3];
		            if (larg0) {
		            largv[0] = OBJECT_TO_JSVAL(js_get_or_create_jsobject<Entity>(cx, (Entity*)larg0));
		        } else {
		            largv[0] = JSVAL_NULL;
		        };
		            if (larg1) {
		            largv[1] = OBJECT_TO_JSVAL(js_get_or_create_jsobject<Entity>(cx, (Entity*)larg1));
		        } else {
		            largv[1] = JSVAL_NULL;
		        };
		            largv[2] = vector2_to_jsval(cx, larg2);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(3, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		            bool ok = true;
		            bool ret;
		            ret = JS::ToBoolean(rval);
		            JSB_PRECONDITION2(ok, cx, false, "lambda function : Error processing return value with type bool");
		            return ret;
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GameManager_setOnContactPreSolve : Error processing arguments");
        cobj->setOnContactPreSolve(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_banca_engine_GameManager_setOnContactPreSolve : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_banca_engine_GameManager_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    Setting* arg0 = nullptr;
    do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (Setting*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
    JSB_PRECONDITION2(ok, cx, false, "js_banca_engine_GameManager_constructor : Error processing arguments");
    GameManager* cobj = new (std::nothrow) GameManager(arg0);

    js_type_class_t *typeClass = js_get_type_from_native<GameManager>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "GameManager"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}
static bool js_banca_engine_GameManager_ctor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    bool ok = true;
    Setting* arg0 = nullptr;
    do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (Setting*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
    JSB_PRECONDITION2(ok, cx, false, "js_GameManager_ctor : Error processing arguments");
    GameManager *nobj = new (std::nothrow) GameManager(arg0);
    js_proxy_t* p = jsb_new_proxy(nobj, obj);
    jsb_ref_init(cx, &p->obj, nobj, "GameManager");
    bool isFound = false;
    if (JS_HasProperty(cx, obj, "_ctor", &isFound) && isFound)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", args);
    args.rval().setUndefined();
    return true;
}


    
void js_register_banca_engine_GameManager(JSContext *cx, JS::HandleObject global) {
    jsb_GameManager_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_GameManager_class->name = "GameManager";
    jsb_GameManager_class->addProperty = JS_PropertyStub;
    jsb_GameManager_class->delProperty = JS_DeletePropertyStub;
    jsb_GameManager_class->getProperty = JS_PropertyStub;
    jsb_GameManager_class->setProperty = JS_StrictPropertyStub;
    jsb_GameManager_class->enumerate = JS_EnumerateStub;
    jsb_GameManager_class->resolve = JS_ResolveStub;
    jsb_GameManager_class->convert = JS_ConvertStub;
    jsb_GameManager_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("getFishByPos", js_banca_engine_GameManager_getFishByPos, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("destroyAllEntity", js_banca_engine_GameManager_destroyAllEntity, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onRealDestroyFish", js_banca_engine_GameManager_onRealDestroyFish, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("shootBullet", js_banca_engine_GameManager_shootBullet, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("_doUpdate", js_banca_engine_GameManager__doUpdate, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("createWall", js_banca_engine_GameManager_createWall, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setFishDestroyDelegate", js_banca_engine_GameManager_setFishDestroyDelegate, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("BeginContact", js_banca_engine_GameManager_BeginContact, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("update", js_banca_engine_GameManager_update, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("createBodyForBullet", js_banca_engine_GameManager_createBodyForBullet, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("autorelease", js_banca_engine_GameManager_autorelease, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("initWorld", js_banca_engine_GameManager_initWorld, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("destroyEntity", js_banca_engine_GameManager_destroyEntity, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("createBodyForFish", js_banca_engine_GameManager_createBodyForFish, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setEntityCollisionListener", js_banca_engine_GameManager_setEntityCollisionListener, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("PreSolve", js_banca_engine_GameManager_PreSolve, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setOnContactPreSolve", js_banca_engine_GameManager_setOnContactPreSolve, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("ctor", js_banca_engine_GameManager_ctor, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    jsb_GameManager_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_GameManager_class,
        js_banca_engine_GameManager_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_GameManager_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "GameManager"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<GameManager>(cx, jsb_GameManager_class, proto, JS::NullPtr());
    anonEvaluate(cx, global, "(function () { engine.GameManager.extend = cc.Class.extend; })()");
}

void register_all_banca_engine(JSContext* cx, JS::HandleObject obj) {
    // Get the ns
    JS::RootedObject ns(cx);
    get_or_create_js_obj(cx, obj, "engine", &ns);

    js_register_banca_engine_Entity(cx, ns);
    js_register_banca_engine_Bullet(cx, ns);
    js_register_banca_engine_Wall(cx, ns);
    js_register_banca_engine_Fish3D(cx, ns);
    js_register_banca_engine_Fish(cx, ns);
    js_register_banca_engine_Path3D(cx, ns);
    js_register_banca_engine_GSprite3D(cx, ns);
    js_register_banca_engine_GMesh(cx, ns);
    js_register_banca_engine_GEnvironment(cx, ns);
    js_register_banca_engine_Setting(cx, ns);
    js_register_banca_engine_HoldFishInfo(cx, ns);
    js_register_banca_engine_Path(cx, ns);
    js_register_banca_engine_GameManager(cx, ns);
}

