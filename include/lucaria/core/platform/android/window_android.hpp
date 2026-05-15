#pragma once

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <backends/imgui_impl_android.h>
#include <unistd.h>

namespace lucaria {
namespace detail {

	struct window_implementation_android {
        android_app* app = nullptr;
        EGLDisplay display = EGL_NO_DISPLAY;
        EGLSurface surface = EGL_NO_SURFACE;
        EGLContext context = EGL_NO_CONTEXT;
        bool has_window = false;
        bool is_engine_initialized = false;
		bool must_install_imgui_callbacks = true;
	};

}
}