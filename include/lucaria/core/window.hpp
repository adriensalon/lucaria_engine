#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>

#include <imgui.h>

#include <lucaria/core/texture.hpp>
#include <lucaria/core/input.hpp>

#if LUCARIA_PLATFORM_ANDROID
#include <lucaria/core/platform/android/window_android.hpp>
#endif

#if LUCARIA_PLATFORM_WEB
#include <lucaria/core/platform/web/window_web.hpp>
#endif

#if LUCARIA_PLATFORM_WIN32
#include <lucaria/core/platform/glfw/window_glfw.hpp>
#endif

namespace lucaria {
namespace detail {

    struct window_implementation {
        LUCARIA_DELETE_DEFAULT(window_implementation)
        window_implementation(const window_implementation& other) = delete;
        window_implementation& operator=(const window_implementation& other) = delete;
        window_implementation(window_implementation&& other);
        window_implementation& operator=(window_implementation&& other);
        ~window_implementation();

        window_implementation(const std::function<void()>& update_callback, const std::function<void()>& teardown_callback = nullptr);
        [[nodiscard]] ImGuiContext* create_shared_imgui_context();
        void reupload_shared_imgui_font_texture();
        void initialize_imgui();
        void destroy_imgui();
        void initialize_openal();
        void destroy_openal();

#if LUCARIA_PLATFORM_ANDROID
        window_implementation_android implementation_android = {};
#endif

#if LUCARIA_PLATFORM_WEB
        window_implementation_web implementation_web = {};
#endif

#if LUCARIA_PLATFORM_WIN32
        window_implementation_glfw implementation_glfw = {};
#endif

        bool is_touch_supported = false;
        bool is_mouse_supported = false;
        bool is_keyboard_supported = false;
        bool is_etc2_supported = false;
        bool is_s3tc_supported = false;

        bool is_fullscreen = false;
        uint32x2 screen_size = uint32x2(0);
        float64 time_delta_seconds = 0.f;
        std::function<void()> stored_update_callback = nullptr;
        std::unordered_map<uint32, float32x2> pointer_accumulators = {};
		std::unordered_map<uint32, pointer_event> pointer_events = {};
		std::unordered_map<input_key, key_event> key_events = {};
        std::unique_ptr<ImFontAtlas> shared_font_atlas = nullptr;
        std::optional<texture_implementation> shared_font_texture = std::nullopt;
        ImGuiContext* screen_context = nullptr;
    };

}
}