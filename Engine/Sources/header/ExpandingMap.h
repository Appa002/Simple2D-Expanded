#pragma once

#include <unordered_map>

namespace Simple2D {
    template <class Key_t, class Value_t>
    class ExpandingMap {
    private:
        std::unordered_map<Key_t, Value_t> map;

    public:
        Value_t& get(Key_t key) {
            if (map.find(key) == map.end())
                map[key] = Value_t(key);
            return map[key];
        }

    };
}