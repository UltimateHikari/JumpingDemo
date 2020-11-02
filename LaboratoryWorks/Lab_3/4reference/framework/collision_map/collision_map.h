#pragma once 
#include "../entity/entity.h"
#include <string>
#include <map>
#include <tuple>
#include <type_traits>

template <typename T> struct function_traits;

template <typename R, typename... Args>
struct function_traits<std::function<R(Args...)>> {
    static const size_t nargs = sizeof...(Args);
    typedef R result_type;

    template <size_t i> struct arg {
        using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
    };
};

class CollisionMap {
    CollisionMap() {}
    ~CollisionMap() {}

    using HitFunctionPtr = std::function<void(PhysicalObject *, PhysicalObject *)>;
    using HitMap = std::map<std::pair<const void*, const void*>, HitFunctionPtr>;

    HitMap hitmap;

    void addEntry(const void* o1, const void* o2, 
        HitFunctionPtr handler) {
        hitmap[std::make_pair(o1, o2)] = handler;
    }

public:    
    static CollisionMap& getInstance() {
        static CollisionMap instance = CollisionMap{};
        return instance;
    }

    template <class... Args>
    void addCollisionHandler(std::function<void(Args...)> handler) {
        using arg1_type = std::decay_t<
			function_traits<decltype(handler)>::template arg<0>::template
			type>;
		using arg2_type = std::decay_t<
			function_traits<decltype(handler)>::template arg<1>::template
			type>;

        addEntry(&typeid(std::remove_pointer_t<arg1_type>),
           &typeid(std::remove_pointer_t<arg2_type>),
           [handler](PhysicalObject *o1, PhysicalObject *o2) {
                handler(dynamic_cast<arg1_type>(o1),
                        dynamic_cast<arg2_type>(o2));
           }
        );
    }

    template <typename... Args> 
    void addCollisionHandler(void (*handler)(Args...)) {
        addCollisionHandler(std::function<void(Args...)>(handler));
    }

    void removeHandler(const PhysicalObject* o1, const PhysicalObject* o2) {
        auto it = hitmap.find(std::make_pair(
            &typeid(*o1), 
            &typeid(*o2)
        ));
        if (it != hitmap.end())
            hitmap.erase(it);
    }

    HitFunctionPtr lookup(const PhysicalObject* o1, const PhysicalObject* o2) {
        auto it = hitmap.find(std::make_pair(
            &typeid(*o1), 
            &typeid(*o2)
        ));
        if (it == hitmap.end())
            return nullptr;
        return it->second;
    }
};

