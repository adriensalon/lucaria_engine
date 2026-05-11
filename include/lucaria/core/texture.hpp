#pragma once

#include <lucaria/core/image.hpp>
#include <lucaria/core/resource.hpp>

namespace lucaria {

struct texture_object;

namespace detail {

    struct texture_cell {
        LUCARIA_DELETE_DEFAULT(texture_cell)
        texture_cell(const texture_cell& other) = delete;
        texture_cell& operator=(const texture_cell& other) = delete;
        texture_cell(texture_cell&& other);
        texture_cell& operator=(texture_cell&& other);
        ~texture_cell();

        texture_cell(const image& from);
        texture_cell(const glm::uvec2 size);
        void resize(const glm::uvec2 size);
        void update_pixels(const image& from, const glm::uvec2 size, const glm::uvec2 offset = { 0, 0 });
        [[nodiscard]] glm::uvec2 get_size() const;
        [[nodiscard]] glm::uint get_handle() const;

    private:
        bool _is_owning;
        glm::uvec2 _size;
        glm::uint _handle;
    };

    struct texture_manager {
        texture_object fetch(const std::filesystem::path& path, const std::optional<std::filesystem::path>& etc2_path, const std::optional<std::filesystem::path>& s3tc_path);
        texture_object create(const glm::uvec2 size);
        void reload(texture_object handle);

    private:
        resource_manager<texture_cell> _resources = {};
    };

    [[nodiscard]] fetched<texture_cell> fetch_texture_cell(const std::filesystem::path& path, const std::optional<std::filesystem::path>& etc2_path, const std::optional<std::filesystem::path>& s3tc_path);

}

struct texture_object {
	texture_object() = default;
	texture_object(const texture_object& other) = default;
	texture_object& operator=(const texture_object& other) = default;
	texture_object(texture_object&& other) = default;
	texture_object& operator=(texture_object&& other) = default;

    /// @brief Loads an image from a file asynchronously and uploads directly to the device,
    /// lets the runtime choose the best format it can use without downloading the others
    /// @param path path to load uncompressed image version from
    /// @param etc2_path path to load ETC2 compressed image version from
    /// @param s3tc_path path to load S3TC compressed image version from
    static texture_object fetch(const std::filesystem::path& path,
        const std::optional<std::filesystem::path>& etc2_path = std::nullopt,
        const std::optional<std::filesystem::path>& s3tc_path = std::nullopt);

    /// @brief Creates a new texture with the specified size
    /// @param size	the size of the texture to create
    /// @return a texture object with an empty texture of the specified size
    static texture_object create(const glm::uvec2 size);

    /// @brief Checks if the texture is ready to be used
    /// @return true if the texture is ready, false otherwise
    [[nodiscard]] bool has_value() const;

    /// @brief Resizes the texture to the specified size
    /// @param size the new size of the texture
    void resize(const glm::uvec2 size);

    /// @brief Updates the pixels of the texture with the specified image data, size and offset
    /// @param from the image data to update from
    /// @param size the size of the image data
    /// @param offset the offset within the texture to update
    void update_pixels(const image& from, const glm::uvec2 size, const glm::uvec2 offset = { 0, 0 });

    /// @brief Gets the size of the texture
    /// @return the size of the texture
    [[nodiscard]] glm::uvec2 get_size() const;

	[[nodiscard]] explicit operator bool() const
    {
        return has_value();
    }

private:
    detail::resource_cell<detail::texture_cell>* _cell = nullptr;
    explicit texture_object(detail::resource_cell<detail::texture_cell>* cell);
    friend struct program;
    friend struct framebuffer;
    friend struct detail::texture_manager;
    friend struct rendering_system;
};

}
