#include <lucaria/core/database.hpp>

namespace lucaria {
namespace detail {

    resource_database& engine_resources()
    {
        static resource_database _database = {};
        return _database;
    }

}
}