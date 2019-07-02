#include "v8.h"
#include "v8pp/call_from_v8.hpp"
#include "v8pp/call_v8.hpp"
#include "v8pp/class.hpp"
#include "v8pp/config.hpp"
#include "v8pp/context.hpp"
#include "v8pp/json.hpp"
#include "v8pp/module.hpp"
#include "v8pp/object.hpp"
#include "v8pp/persistent.hpp"
#include "v8pp/throw_ex.hpp"
#include "v8pp/utility.hpp"

using namespace v8;

inline void SetPrototypeMethod(v8::Local<v8::FunctionTemplate> recv, const char* name, FunctionCallback callback) {
    HandleScope scope;
    v8::Local<v8::FunctionTemplate> t = New<v8::FunctionTemplate>(callback, v8::Local<v8::Value>(), New<v8::Signature>(recv));
    v8::Local<v8::String> fn_name = New(name).ToLocalChecked();
    recv->PrototypeTemplate()->Set(fn_name, t);
    t->SetClassName(fn_name);
}

inline void SetAccessor(
    v8::Local<v8::ObjectTemplate> tpl, v8::Local<v8::String> name, GetterCallback getter, SetterCallback setter = 0, v8::Local<v8::Value> data = v8::Local<v8::Value>(),
    v8::AccessControl settings = v8::DEFAULT, v8::PropertyAttribute attribute = v8::None, imp::Sig signature = imp::Sig()) {
    HandleScope scope;

    imp::NativeGetter getter_ = imp::GetterCallbackWrapper;
    imp::NativeSetter setter_ = setter ? imp::SetterCallbackWrapper : 0;

    v8::Local<v8::ObjectTemplate> otpl = New<v8::ObjectTemplate>();
    otpl->SetInternalFieldCount(imp::kAccessorFieldCount);
    v8::Local<v8::Object> obj = NewInstance(otpl).ToLocalChecked();

    obj->SetInternalField(imp::kGetterIndex, New<v8::External>(reinterpret_cast<void*>(getter)));

    if (setter != 0) {
        obj->SetInternalField(imp::kSetterIndex, New<v8::External>(reinterpret_cast<void*>(setter)));
    }

    if (!data.IsEmpty()) {
        obj->SetInternalField(imp::kDataIndex, data);
    }

    tpl->SetAccessor(name, getter_, setter_, obj, settings, attribute, signature);
}

inline bool SetAccessor(
    v8::Local<v8::Object> obj, v8::Local<v8::String> name, GetterCallback getter, SetterCallback setter = 0, v8::Local<v8::Value> data = v8::Local<v8::Value>(),
    v8::AccessControl settings = v8::DEFAULT, v8::PropertyAttribute attribute = v8::None) {
    HandleScope scope;

    imp::NativeGetter getter_ = imp::GetterCallbackWrapper;
    imp::NativeSetter setter_ = setter ? imp::SetterCallbackWrapper : 0;

    v8::Local<v8::ObjectTemplate> otpl = New<v8::ObjectTemplate>();
    otpl->SetInternalFieldCount(imp::kAccessorFieldCount);
    v8::Local<v8::Object> dataobj = NewInstance(otpl).ToLocalChecked();

    dataobj->SetInternalField(imp::kGetterIndex, New<v8::External>(reinterpret_cast<void*>(getter)));

    if (!data.IsEmpty()) {
        dataobj->SetInternalField(imp::kDataIndex, data);
    }

    if (setter) {
        dataobj->SetInternalField(imp::kSetterIndex, New<v8::External>(reinterpret_cast<void*>(setter)));
    }

#if (NODE_MODULE_VERSION >= NODE_6_0_MODULE_VERSION)
    return obj->SetAccessor(GetCurrentContext(), name, getter_, setter_, dataobj, settings, attribute).FromMaybe(false);
#else
    return obj->SetAccessor(name, getter_, setter_, dataobj, settings, attribute);
#endif
}

#define NAN_METHOD(name) Nan::NAN_METHOD_RETURN_TYPE name(Nan::NAN_METHOD_ARGS_TYPE info)
#define NAN_GETTER(name) Nan::NAN_GETTER_RETURN_TYPE name(v8::Local<v8::String> property, Nan::NAN_GETTER_ARGS_TYPE info)
#define NAN_SETTER(name) Nan::NAN_SETTER_RETURN_TYPE name(v8::Local<v8::String> property, v8::Local<v8::Value> value, Nan::NAN_SETTER_ARGS_TYPE info)

namespace Nan {
    class ObjectWrap {};

    template <typename T>
    using Persistent = v8::Persistent<T>;

    v8pp::wrap_function_template()
}  // namespace Nan