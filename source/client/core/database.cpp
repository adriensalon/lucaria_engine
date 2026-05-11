#include <lucaria/core/database.hpp>

namespace lucaria {
namespace detail {

    asset_database& engine_assets()
    {
        static asset_database _database = {};
        return _database;
    }

}
}