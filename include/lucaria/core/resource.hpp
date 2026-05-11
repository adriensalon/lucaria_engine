#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <lucaria/core/fetch.hpp>

namespace lucaria {
namespace detail {
	
    template <typename CellType>
    struct resource_cell {

        [[nodiscard]] bool is_ready() const
        {
            return _fetched.has_value();
        }

        [[nodiscard]] bool is_loading() const
        {
            return _fetched.is_loading();
        }

        [[nodiscard]] std::uint32_t get_version() const
        {
            return _current_version.load(std::memory_order_acquire);
        }

        void set(fetched<CellType>&& next_value, const std::optional<std::filesystem::path>& origin_path = std::nullopt)
        {
            std::lock_guard lock(_set_mutex);
            _fetched = std::move(next_value);
            _origin_path = origin_path;

            _fetched.on_ready([this](CellType&) {
                _current_version.fetch_add(1, std::memory_order_release);
            });
        }

        CellType& get()
        {
            return _fetched.value();
        }

        const CellType& get() const
        {
            return _fetched.value();
        }

        const std::optional<std::filesystem::path>& origin_path() const
        {
            return _origin_path;
        }

        void on_ready(std::function<void(CellType&)> callback) const
        {
            _fetched.on_ready(std::move(callback));
        }

    private:
        fetched<CellType> _fetched = {};
        std::optional<std::filesystem::path> _origin_path = {};
        std::atomic<std::uint32_t> _current_version = { 0 };
        std::mutex _set_mutex = {};
    };

    template <typename CellType>
    struct resource_manager {

        resource_cell<CellType>* create_cell()
        {
            std::unique_ptr<resource_cell<CellType>> _cell = std::make_unique<resource_cell<CellType>>();
            resource_cell<CellType>* _raw_cell = _cell.get();
            _cells.emplace_back(std::move(_cell));
            return _raw_cell;
        }

        resource_cell<CellType>* create_cell(fetched<CellType>&& value, std::optional<std::filesystem::path> origin_path = std::nullopt)
        {
            resource_cell<CellType>* _cell = create_cell();
            _cell->set(std::move(value), std::move(origin_path));
            return _cell;
        }

        [[nodiscard]] resource_cell<CellType>* find_by_path(const std::filesystem::path& path) const
        {
            auto _it = _cells_by_path.find(path);
            return _it == _cells_by_path.end() ? nullptr : _it->second;
        }

        [[nodiscard]] resource_cell<CellType>* get_or_create_by_path(const std::filesystem::path& path, std::function<fetched<CellType>()> create_fetch)
        {
            if (resource_cell<CellType>* _existing = find_by_path(path)) {
                return _existing;
            }

            resource_cell<CellType>* _cell = create_cell();
            _cell->set(create_fetch(), path);
            _cells_by_path.emplace(path, _cell);

            return _cell;
        }

        void clear()
        {
            _cells_by_path.clear();
            _cells.clear();
        }

    private:
        std::vector<std::unique_ptr<resource_cell<CellType>>> _cells;
        std::unordered_map<std::filesystem::path, resource_cell<CellType>*> _cells_by_path;
    };
}
}