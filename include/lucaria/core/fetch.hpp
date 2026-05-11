#pragma once

#include <filesystem>
#include <functional>
#include <future>
#include <optional>
#include <type_traits>
#include <vector>

#include <ozz/base/io/stream.h>

#include <lucaria/core/error.hpp>

namespace lucaria {

/// @brief Represents a fetched value
/// @tparam FetchedType type being fetched
template <typename FetchedType>
struct fetched {

	fetched() = default;

    fetched(FetchedType&& value)
        : _cache(std::move(value))
        , _callbacks_invoked(true)
    {
    }

    /// @brief Creates a fetched object from an existing std::future<FetchedType>
    /// @param future the future to create from
    fetched(std::future<FetchedType>&& future)
    {
        std::shared_ptr<std::future<FetchedType>> _shared_future = std::make_shared<std::future<FetchedType>>(std::move(future));
        _poll = [_shared_future]() -> bool {
            if (!_shared_future->valid()) {
                return false;
            }
            return _shared_future->wait_for(std::chrono::milliseconds(0)) == std::future_status::ready;
        };
        _get = [_shared_future]() -> FetchedType {
            return _shared_future->get();
        };
    }

    /// @brief Creates a fetched object from an existing std::future<OriginFetchedType> and a continuation from OriginFetchedType to FetchedType
    /// @tparam OriginFetchedType intermediate type to continuate
    /// @tparam ThenCallback function type for the continuation
    /// @param future the future to create from
    /// @param then the continuation
    template <typename OriginFetchedType, typename ThenCallback, typename = std::enable_if_t<std::is_invocable_r_v<FetchedType, const ThenCallback&, const OriginFetchedType&>>>
    fetched(std::future<OriginFetchedType>&& future, const ThenCallback& then)
    {
        std::shared_ptr<std::future<OriginFetchedType>> _shared_intermediate_future = std::make_shared<std::future<OriginFetchedType>>(std::move(future));
        std::shared_ptr<std::decay_t<ThenCallback>> _shared_decayed_then = std::make_shared<std::decay_t<ThenCallback>>(then);
        _poll = [_shared_intermediate_future]() -> bool {
            if (!_shared_intermediate_future->valid()) {
                return false;
            }
            return _shared_intermediate_future->wait_for(std::chrono::milliseconds(0)) == std::future_status::ready;
        };
        _get = [_shared_intermediate_future, _shared_decayed_then]() -> FetchedType {
            const OriginFetchedType _intermediate_value = _shared_intermediate_future->get();
            return std::invoke(*_shared_decayed_then, _intermediate_value);
        };
    }

    /// @brief Checks if the underlying std::future<FetchedType> has yet a value or is still computing
    /// @return true if the value is available
    [[nodiscard]] bool has_value() const
    {
        if (_cache) {
			_invoke_callbacks_once();
            return true;
        }
        if (_poll && _poll()) {
            _cache = std::move(_get());
            _poll = nullptr;
            _get = nullptr;
			_invoke_callbacks_once();
            return true;
        }
        return false;
    }

    /// @brief Gets the available value held by the underlying std::future<FetchedType>.
    /// @throws throws a lucaria::runtime_error if the std::future result is not available yet
    /// @return the available value
    [[nodiscard]] FetchedType& value()
    {
        if (!has_value()) {
            LUCARIA_RUNTIME_ERROR("Failed to get fetched value&, please check has_value() before trying to access it")
        }
        return _cache.value();
    }

    /// @brief Gets the available value held by the underlying std::future<FetchedType>.
    /// @throws throws a lucaria::runtime_error if the std::future result is not available yet
    /// @return the available value
    [[nodiscard]] const FetchedType& value() const
    {
        if (!has_value()) {
            LUCARIA_RUNTIME_ERROR("Failed to get fetched const value&, please check has_value() before trying to access it")
        }
        return _cache.value();
    }

	void on_ready(std::function<void(FetchedType&)> callback) const
    {
        if (has_value()) {
            callback(_cache.value());
            return;
        }

        _callbacks.emplace_back(std::move(callback));
    }

    void on_ready(std::function<void()> callback) const
    {
        on_ready([callback = std::move(callback)](FetchedType&) {
            callback();
        });
    }

    /// @brief Conversion operator for the has_value member function
    [[nodiscard]] explicit operator bool() const
    {
        return has_value();
    }

private:
    mutable std::function<bool()> _poll = nullptr;
    mutable std::function<FetchedType()> _get = nullptr;
    mutable std::optional<FetchedType> _cache = std::nullopt;
    mutable bool _callbacks_invoked = false;
    mutable std::vector<std::function<void(FetchedType&)>> _callbacks = {};

    void _invoke_callbacks_once() const
    {
        if (_callbacks_invoked || !_cache) {
            return;
        }
        _callbacks_invoked = true;

        for (std::function<void(FetchedType&)>& _callback : _callbacks) {
            _callback(_cache.value());
        }
        _callbacks.clear();
    }
};

/// @brief
/// @param fetch_path
void set_fetch_path(const std::filesystem::path& fetch_path);

/// @brief Gets the current count of fetched objects that still have a std::future waiting
/// @return the current count
[[nodiscard]] std::size_t get_fetches_waiting();

// Internal definitions
namespace _detail {

    struct bytes_streambuf : public std::streambuf {
        bytes_streambuf(const std::vector<char>& data);
    };

    struct bytes_stream : public std::istream {
        bytes_stream(const std::vector<char>& data);

    private:
        bytes_streambuf _buffer;
    };

    struct ozz_bytes_stream : public ozz::io::Stream {
        ozz_bytes_stream(const std::vector<char>& data);
        ~ozz_bytes_stream() override = default;

        bool opened() const override;
        std::size_t Read(void* buffer, std::size_t size) override;
        std::size_t Write(const void* buffer, std::size_t size) override;
        int Seek(int offset, Origin origin) override;
        int Tell() const override;
        std::size_t Size() const override;

    private:
        const std::vector<char>& _bytes;
        std::size_t _position;
    };

    template <typename FetchedType>
    struct fetched_container {

        void emplace(FetchedType& obj)
        {
            _ptr = &obj;
        }

        void emplace(fetched<FetchedType>& fut, const std::function<void()>& callback = nullptr)
        {
            _fut = &fut;
            _callback = callback;
        }

        [[nodiscard]] bool has_value() const
        {
            if (_ptr) {
                return true;
            }
            if (_fut && _fut->has_value()) {
                if (_callback && !_is_callback_invoked) {
                    _is_callback_invoked = true;
                    _callback();
                    _callback = nullptr;
                }
                return true;
            }
            return false;
        }

        [[nodiscard]] FetchedType& value()
        {
            return _ptr ? *_ptr : _fut->value();
        }

        [[nodiscard]] const FetchedType& value() const
        {
            return _ptr ? *_ptr : _fut->value();
        }

        [[nodiscard]] explicit operator bool() const { return has_value(); }

    private:
        FetchedType* _ptr = nullptr;
        fetched<FetchedType>* _fut = nullptr;
        mutable std::function<void()> _callback = nullptr;
        mutable bool _is_callback_invoked = false;
    };

}
}
