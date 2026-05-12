#include <lucaria/core/framebuffer.hpp>

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
namespace {

    static void check_complete()
    {
#if LUCARIA_CONFIG_DEBUG
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            LUCARIA_RUNTIME_ERROR("Framebuffer incomplete")
        }
#endif
    }

}

namespace detail {

    framebuffer_implementation::framebuffer_implementation(framebuffer_implementation&& other)
    {
        implementation_opengl.is_owning = false;
        *this = std::move(other);
    }

    framebuffer_implementation& framebuffer_implementation::operator=(framebuffer_implementation&& other)
    {
        if (implementation_opengl.is_owning) {
            LUCARIA_RUNTIME_ERROR("Object already owning resources")
        }
        
		implementation_opengl.is_owning = other.implementation_opengl.is_owning;
		implementation_opengl.id = other.implementation_opengl.id;
        implementation_opengl.texture_color_id = other.implementation_opengl.texture_color_id;
        implementation_opengl.texture_depth_id = other.implementation_opengl.texture_depth_id;
        implementation_opengl.renderbuffer_color_id = other.implementation_opengl.renderbuffer_color_id;
        implementation_opengl.renderbuffer_depth_id = other.implementation_opengl.renderbuffer_depth_id;

        other.implementation_opengl.is_owning = false;
        return *this;
    }

    framebuffer_implementation::~framebuffer_implementation()
    {
        if (implementation_opengl.is_owning) {
            glDeleteFramebuffers(1, &implementation_opengl.id);
        }
    }

    framebuffer_implementation::framebuffer_implementation()
    {
        glGenFramebuffers(1, &implementation_opengl.id);
        glBindFramebuffer(GL_FRAMEBUFFER, implementation_opengl.id);
        GLenum _none = GL_NONE;
        glDrawBuffers(1, &_none);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        implementation_opengl.is_owning = true;
    }

    void framebuffer_implementation::use_default()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void framebuffer_implementation::use()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, implementation_opengl.id);
    }

    void framebuffer_implementation::bind_color(const detail::texture_implementation& color)
    {
        const glm::uint _texture_handle = color.get_handle();

        if (implementation_opengl.texture_color_id && implementation_opengl.texture_color_id.value() == _texture_handle) {
            return;
        }

        const GLenum _attachment = GL_COLOR_ATTACHMENT0;
        const GLuint _color_id = _texture_handle;
        implementation_opengl.texture_color_id = _color_id;
        implementation_opengl.renderbuffer_color_id = std::nullopt;
        glBindFramebuffer(GL_FRAMEBUFFER, implementation_opengl.id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _color_id, 0);
        glDrawBuffers(1, &_attachment);
        glReadBuffer(GL_COLOR_ATTACHMENT0);

        check_complete();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void framebuffer_implementation::bind_color(renderbuffer& color)
    {
        if (implementation_opengl.renderbuffer_color_id && implementation_opengl.renderbuffer_color_id.value() == color.get_handle()) {
            return;
        }

        const GLenum _attachment = GL_COLOR_ATTACHMENT0;
        const GLuint _color_id = color.get_handle();
        implementation_opengl.renderbuffer_color_id = _color_id;
        implementation_opengl.texture_color_id = std::nullopt;
        glBindFramebuffer(GL_FRAMEBUFFER, implementation_opengl.id);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _color_id);
        glDrawBuffers(1, &_attachment);
        glReadBuffer(GL_COLOR_ATTACHMENT0);

        check_complete();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void framebuffer_implementation::bind_depth(detail::texture_implementation& depth)
    {
        if (implementation_opengl.texture_depth_id && implementation_opengl.texture_depth_id.value() == depth.get_handle()) {
            return;
        }

        const GLuint _depth_id = depth.get_handle();
        implementation_opengl.texture_depth_id = _depth_id;
        implementation_opengl.renderbuffer_depth_id = std::nullopt;
        glBindFramebuffer(GL_FRAMEBUFFER, implementation_opengl.id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth_id, 0);

        if (!implementation_opengl.texture_color_id && !implementation_opengl.renderbuffer_color_id) {
            GLenum _none = GL_NONE;
            glDrawBuffers(1, &_none);
            glReadBuffer(GL_NONE);
        }

        check_complete();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void framebuffer_implementation::bind_depth(renderbuffer& depth)
    {
        if (implementation_opengl.renderbuffer_depth_id && implementation_opengl.renderbuffer_depth_id.value() == depth.get_handle()) {
            return;
        }

        const GLuint _depth_id = depth.get_handle();
        implementation_opengl.renderbuffer_depth_id = _depth_id;
        implementation_opengl.texture_depth_id = std::nullopt;
        glBindFramebuffer(GL_FRAMEBUFFER, implementation_opengl.id);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth_id);

        if (!implementation_opengl.texture_color_id && !implementation_opengl.renderbuffer_color_id) {
            GLenum _none = GL_NONE;
            glDrawBuffers(1, &_none);
            glReadBuffer(GL_NONE);
        }

        check_complete();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

}
}