#pragma once

//#include "canvas2d.h"
// #include "context2d.h"
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

#include "../Bind_GL.h"

namespace BKJSInternals {

    class Audio {};

    void initBakery();

    void createAudio();

    void loadFileLocal();

    void loadFileRemote();

    void getSystemInfo();

    void writeStorageLocal();

    void request();

    void addEventListener();
}  // namespace BKJSInternals
