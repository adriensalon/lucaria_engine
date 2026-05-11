#include <lucaria/core/texture.hpp>

#if LUCARIA_PLATFORM_ANDROID
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#elif LUCARIA_PLATFORM_WEB
#include <GLES3/gl3.h>
#elif LUCARIA_PLATFORM_WIN32
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#endif

namespace lucaria {

extern bool _is_etc2_supported;
extern bool _is_s3tc_supported;

namespace detail {
    namespace {

        // constexpr static GLenum COMPRESSED_R11_EAC = 0x9270;
        // constexpr static GLenum COMPRESSED_SIGNED_R11_EAC = 0x9271;
        // constexpr static GLenum COMPRESSED_RG11_EAC = 0x9272;
        // constexpr static GLenum COMPRESSED_SIGNED_RG11_EAC = 0x9273;
        constexpr static GLenum COMPRESSED_RGB8_ETC2 = 0x9274;
        // constexpr static GLenum COMPRESSED_SRGB8_ETC2 = 0x9275;
        // constexpr static GLenum COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 = 0x9276;
        // constexpr static GLenum COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 = 0x9277;
        constexpr static GLenum COMPRESSED_RGBA8_ETC2_EAC = 0x9278;
        // constexpr static GLenum COMPRESSED_SRGB8_ALPHA8_ETC2_EAC = 0x9279;
        constexpr static GLenum COMPRESSED_RGB_S3TC_DXT1_EXT = 0x83F0;
        // constexpr static GLenum COMPRESSED_RGBA_S3TC_DXT1_EXT = 0x83F1;
        // constexpr static GLenum COMPRESSED_RGBA_S3TC_DXT3_EXT = 0x83F2;
        constexpr static GLenum COMPRESSED_RGBA_S3TC_DXT5_EXT = 0x83F3;

    }

    texture_implementation::texture_implementation(texture_implementation&& other)
    {
        _opengl_impl.is_owning = false;
        *this = std::move(other);
    }

    texture_implementation& texture_implementation::operator=(texture_implementation&& other)
    {
        if (_opengl_impl.is_owning) {
            LUCARIA_RUNTIME_ERROR("Object already owning resources")
        }

        _opengl_impl.is_owning = other._opengl_impl.is_owning;
        _opengl_impl.size = other._opengl_impl.size;
        _opengl_impl.id = other._opengl_impl.id;

        other._opengl_impl.is_owning = false;
        return *this;
    }

    texture_implementation::~texture_implementation()
    {
        if (_opengl_impl.is_owning) {
            glBindTexture(GL_TEXTURE_2D, 0);
            glDeleteTextures(1, &_opengl_impl.id);
        }
    }

    texture_implementation::texture_implementation(const image_implementation& from)
    {
        _opengl_impl.size = { from.data.width, from.data.height };
        glGenTextures(1, &_opengl_impl.id);
        glBindTexture(GL_TEXTURE_2D, _opengl_impl.id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        const GLsizei _pixels_count = static_cast<GLsizei>(from.data.pixels.size());
        const GLubyte* _pixels_ptr = from.data.pixels.data();

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        switch (from.data.channels) {
        case 3:
            if (from.data.is_compressed_etc && _is_etc2_supported) {
                glCompressedTexImage2D(GL_TEXTURE_2D, 0, COMPRESSED_RGB8_ETC2, from.data.width, from.data.height, 0, _pixels_count, _pixels_ptr);
            } else if (from.data.is_compressed_s3tc && _is_s3tc_supported) {
                glCompressedTexImage2D(GL_TEXTURE_2D, 0, COMPRESSED_RGB_S3TC_DXT1_EXT, from.data.width, from.data.height, 0, _pixels_count, _pixels_ptr);
            } else {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, from.data.width, from.data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, _pixels_ptr);
            }
            break;
        case 4:
            if (from.data.is_compressed_etc && _is_etc2_supported) {
                glCompressedTexImage2D(GL_TEXTURE_2D, 0, COMPRESSED_RGBA8_ETC2_EAC, from.data.width, from.data.height, 0, _pixels_count, _pixels_ptr);
            } else if (from.data.is_compressed_s3tc && _is_s3tc_supported) {
                glCompressedTexImage2D(GL_TEXTURE_2D, 0, COMPRESSED_RGBA_S3TC_DXT5_EXT, from.data.width, from.data.height, 0, _pixels_count, _pixels_ptr);
            } else {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, from.data.width, from.data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _pixels_ptr);
            }
            break;
        default:
            LUCARIA_RUNTIME_ERROR("Invalid texture channels count, must be 3 or 4")
            break;
        }
#if LUCARIA_CONFIG_DEBUG
        std::cout << "Created TEXTURE_2D buffer of size " << from.data.width << "x" << from.data.height << " with id " << _opengl_impl.id << std::endl;
#endif

        _opengl_impl.is_owning = true;
    }

    texture_implementation::texture_implementation(const glm::uvec2 size)
    {
        _opengl_impl.size = size;
        glGenTextures(1, &_opengl_impl.id);
        glBindTexture(GL_TEXTURE_2D, _opengl_impl.id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

#if LUCARIA_CONFIG_DEBUG
        std::cout << "Created EMPTY TEXTURE_2D buffer of size " << size.x << "x" << size.y << " with id " << _opengl_impl.id << std::endl;
#endif

        _opengl_impl.is_owning = true;
    }

    void texture_implementation::resize(const glm::uvec2 size)
    {
        if (_opengl_impl.size != size) {
            _opengl_impl.size = size;
            glBindTexture(GL_TEXTURE_2D, _opengl_impl.id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _opengl_impl.size.x, _opengl_impl.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        }
    }

    glm::uvec2 texture_implementation::get_size() const
    {
        return _opengl_impl.size;
    }

    glm::uint texture_implementation::get_handle() const
    {
        return _opengl_impl.id;
    }
}
}