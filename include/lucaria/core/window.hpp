#pragma once

namespace lucaria {
namespace detail {

    struct window_implementation {

        std::unique_ptr<ImFontAtlas> _shared_font_atlas = nullptr;
        glm::uint _shared_font_texture = 0;
        ImGuiContext* _screen_context = nullptr;

#if LUCARIA_PLATFORM_ANDROID
        android_app* g_app = nullptr;
        EGLDisplay g_display = EGL_NO_DISPLAY;
        EGLSurface g_surface = EGL_NO_SURFACE;
        EGLContext g_context = EGL_NO_CONTEXT;
        bool g_has_window = false;
        bool g_engine_initialized = false;
#endif

#if LUCARIA_PLATFORM_WIN32
        GLFWwindow* glfw_window = nullptr;
#endif
		

        ImGuiContext* _create_shared_context();
        void _reupload_shared_font_texture();
    };

}
}