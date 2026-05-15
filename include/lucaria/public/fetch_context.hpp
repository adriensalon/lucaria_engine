#pragma once

#include <filesystem>
#include <functional>
#include <future>
#include <optional>
#include <type_traits>
#include <vector>

#include <ozz/base/io/stream.h>

#include <lucaria/core/error.hpp>
#include <lucaria/core/math.hpp>

namespace lucaria {
	
void set_fetch_path(const std::filesystem::path& fetch_path);
[[nodiscard]] std::size_t get_fetches_waiting();

/// @brief Runtime API for asynchronous loading of objects that allows to configure the filesystem
struct fetch_context {

	/// @brief Sets a prefix for all the subsequent uses of the filesystem. You can use this if
	/// you build for multiple platforms and choose different storage path strategies.
	/// @param prefix_path the prefix path to use for subsequent filesystem accesses
	void set_prefix_path(const std::filesystem::path& prefix_path);

	/// @brief Gets the current count of fetched objects that are still loading. You can use this to
	/// check if all the async fetches you issued are complete for loading screens.
	/// @return the current count of waiting async fetches
	[[nodiscard]] uint32 async_fetches_waiting();

};

}
