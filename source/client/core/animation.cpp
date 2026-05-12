#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <ozz/animation/runtime/track_sampling_job.h>
#include <ozz/base/io/archive.h>
#include <ozz/base/memory/allocator.h>

#include <lucaria/core/animation.hpp>
#include <lucaria/core/database.hpp>
#include <lucaria/core/error.hpp>
#include <lucaria/core/math.hpp>

namespace lucaria {

extern void _fetch_bytes(const std::filesystem::path& file_path, const std::function<void(const std::vector<char>&)>& callback, bool persist);

namespace detail {
    namespace {

        static void _load_animation_bytes(ozz::animation::Animation& handle, const std::vector<char>& bytes)
        {
            _detail::ozz_bytes_stream _ozz_stream(bytes);
            ozz::io::IArchive _ozz_archive(&_ozz_stream);
            if (!_ozz_archive.TestTag<ozz::animation::Animation>()) {
                LUCARIA_RUNTIME_ERROR("Failed to load animation, archive doesn't contain the expected object type")
            }
            _ozz_archive >> handle;
#if LUCARIA_CONFIG_DEBUG
            std::cout << "Loaded animation with " << handle.num_tracks() << " tracks" << std::endl;
#endif
        }

        static void _load_motion_track_bytes(ozz::animation::Float3Track& translation_handle, ozz::animation::QuaternionTrack& rotation_handle, const std::vector<char>& bytes)
        {
            _detail::ozz_bytes_stream _ozz_stream(bytes);
            ozz::io::IArchive _ozz_archive(&_ozz_stream);
            if (!_ozz_archive.TestTag<ozz::animation::Float3Track>()) {
                LUCARIA_RUNTIME_ERROR("Failed to load float3 track, archive doesn't contain the expected object type")
            }
            _ozz_archive >> translation_handle;
            if (!_ozz_archive.TestTag<ozz::animation::QuaternionTrack>()) {
                LUCARIA_RUNTIME_ERROR("Impossible to load quaternion track, archive doesn't contain the expected object type")
            }
            _ozz_archive >> rotation_handle;
#if LUCARIA_CONFIG_DEBUG
            std::cout << "Loaded motion track with position and rotation" << std::endl;
#endif
        }

        static void _load_event_track_bytes(event_track_data& data, const std::vector<char>& bytes)
        {
            _detail::bytes_stream _stream(bytes);
#if LUCARIA_JSON
            cereal::JSONInputArchive _archive(_stream);
#else
            cereal::PortableBinaryInputArchive _archive(_stream);
#endif
            _archive(data);
#if LUCARIA_CONFIG_DEBUG
            std::cout << "Loaded event track" << std::endl;
#endif
        }

        static async_container<animation_implementation> _fetch_animation_async(const std::filesystem::path& path)
        {
            std::shared_ptr<std::promise<animation_implementation>> _promise = std::make_shared<std::promise<animation_implementation>>();
            _fetch_bytes(path, [_promise](const std::vector<char>& _bytes) {
        animation_implementation _animation(_bytes);
        _promise->set_value(std::move(_animation)); }, true);

            // create animation on worker thread is ok
            return async_container<animation_implementation>(_promise->get_future());
        }

        static async_container<motion_track_implementation> _fetch_motion_track_async(const std::filesystem::path& path)
        {
            std::shared_ptr<std::promise<motion_track_implementation>> _promise = std::make_shared<std::promise<motion_track_implementation>>();
            _fetch_bytes(path, [_promise](const std::vector<char>& _bytes) {
        motion_track_implementation _motion_track(_bytes);
        _promise->set_value(std::move(_motion_track)); }, true);

            // create motion track on worker thread is ok
            return async_container<motion_track_implementation>(_promise->get_future());
        }

        static async_container<event_track_implementation> _fetch_event_track_async(const std::filesystem::path& path)
        {
            std::shared_ptr<std::promise<event_track_implementation>> _promise = std::make_shared<std::promise<event_track_implementation>>();
            _fetch_bytes(path, [_promise](const std::vector<char>& _bytes) {
        event_track_implementation _event_track(_bytes);
        _promise->set_value(std::move(_event_track)); }, true);

            // create event track on worker thread is ok
            return async_container<event_track_implementation>(_promise->get_future());
        }

    }

    animation_implementation::animation_implementation(const std::vector<char>& bytes)
    {
        _load_animation_bytes(animation, bytes);
    }

    animation_implementation::animation_implementation(ozz::animation::Animation&& animation)
        : animation(std::move(animation))
    {
    }

    motion_track_implementation::motion_track_implementation(const std::vector<char>& bytes)
    {
        _load_motion_track_bytes(translation_track, rotation_track, bytes);
    }

    motion_track_implementation::motion_track_implementation(ozz::animation::Float3Track&& translation_track, ozz::animation::QuaternionTrack&& rotation_track)
        : translation_track(std::move(translation_track))
        , rotation_track(std::move(rotation_track))
    {
    }

    glm::vec3 motion_track_implementation::get_total_translation() const
    {
        ozz::math::Float3 _position_start;
        ozz::math::Float3 _position_end;
        ozz::animation::Float3TrackSamplingJob _job_start {};
        _job_start.track = &translation_track;
        _job_start.ratio = 0.f;
        _job_start.result = &_position_start;
        _job_start.Run();
        ozz::animation::Float3TrackSamplingJob _job_end {};
        _job_end.track = &translation_track;
        _job_end.ratio = 1.f;
        _job_end.result = &_position_end;
        _job_end.Run();
        return convert(_position_end - _position_start);
    }

    event_track_implementation::event_track_implementation(const std::vector<char>& bytes)
    {
        _load_event_track_bytes(data, bytes);
    }

    event_track_implementation::event_track_implementation(event_track_data&& data)
        : data(std::move(data))
    {
    }

}

animation_object animation_object::fetch(const std::filesystem::path& path)
{
    detail::resource_container<detail::animation_implementation>* _resource = detail::engine_assets().animations.get_or_create_by_path(path, [&] {
        return detail::_fetch_animation_async(path);
    });

    return animation_object { _resource };
}

bool animation_object::has_value() const
{
    return _resource && _resource->is_ready();
}

animation_object::operator bool() const
{
    return has_value();
}

animation_object::animation_object(detail::resource_container<detail::animation_implementation>* resource)
    : _resource(resource)
{
}

motion_track_object motion_track_object::fetch(const std::filesystem::path& path)
{
    detail::resource_container<detail::motion_track_implementation>* _resource = detail::engine_assets().motion_tracks.get_or_create_by_path(path, [&] {
        return detail::_fetch_motion_track_async(path);
    });

    return motion_track_object { _resource };
}

bool motion_track_object::has_value() const
{
    return _resource && _resource->is_ready();
}

motion_track_object::operator bool() const
{
    return has_value();
}

motion_track_object::motion_track_object(detail::resource_container<detail::motion_track_implementation>* resource)
    : _resource(resource)
{
}

event_track_object event_track_object::fetch(const std::filesystem::path& path)
{
    detail::resource_container<detail::event_track_implementation>* _resource = detail::engine_assets().event_tracks.get_or_create_by_path(path, [&] {
        return detail::_fetch_event_track_async(path);
    });

    return event_track_object { _resource };
}

bool event_track_object::has_value() const
{
    return _resource && _resource->is_ready();
}

event_track_object::operator bool() const
{
    return has_value();
}

event_track_object::event_track_object(detail::resource_container<detail::event_track_implementation>* resource)
    : _resource(resource)
{
}

}
