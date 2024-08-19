module;

#include "fmt/core.h"
#include <functional>
#include <ranges>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <vector>

export module brot.engine;

export namespace brot {
enum class stage {
    update,
    draw,
    swap
};

template<stage S>
struct receiver_data {};

template<>
struct receiver_data<stage::update> {
    double dt;
};

template<stage S>
using receiver = std::function<void(const receiver_data<S> &)>;

using module_tag = std::string;

template<stage S>
using receiver_list = std::pair<module_tag, std::vector<receiver<S>>>;

class engine {
public:
    const std::string DEFAULT_GROUP = "__default";

    template<stage S>
    void declare_dependencies(const module_tag &t, const std::vector<module_tag> &deps);

    template<stage S>
    void register_receiver(const module_tag &t, receiver<S> &&r);

    template<stage S, typename... Args>
    void run_stage(Args &&...args);

private:
    template<stage S>
    std::vector<receiver_list<S>> &receivers_();

    template<stage S>
    std::unordered_map<module_tag, std::size_t> &tag_to_idx_();

    template<stage S>
    std::size_t find_or_insert_dep_idx_(const module_tag &t);
};
} // namespace brot

template<brot::stage S>
void brot::engine::declare_dependencies(const module_tag &t, const std::vector<module_tag> &deps) {
    auto &r_lists = receivers_<S>();

    std::unordered_map<module_tag, std::size_t> dep_idxs;
    for (const auto &dep: deps) {
        dep_idxs =
    }
}

template<brot::stage S>
void brot::engine::register_receiver(const module_tag &t, receiver<S> &&r) {
    auto &r_lists = receivers_<S>();

    // Check if anything has been registered before,
    // if not create the default group
    if (r_lists.empty()) {
        r_lists.emplace_back(DEFAULT_GROUP, std::vector<receiver<S>>{});
    }

    for (auto &r_list: r_lists) {
        // Either we find a dependency group or we got to the end
        if (r_list.first == t || r_list.first == DEFAULT_GROUP) {
            r_list.second.emplace_back(std::forward<receiver<S>>(r));
            break;
        }
    }
}

template<brot::stage S, typename... Args>
void brot::engine::run_stage(Args &&...args) {
    auto &r_lists = receivers_<S>();
    auto data = receiver_data<S>{std::forward<Args>(args)...};
    for (const auto &r_list: r_lists) {
        for (const auto &r: r_list.second) {
            r(data);
        }
    }
}

template<brot::stage S>
std::vector<brot::receiver_list<S>> &brot::engine::receivers_() {
    static std::vector<receiver_list<S>> r;
    return r;
}

template<brot::stage S>
std::unordered_map<brot::module_tag, std::size_t> &brot::engine::tag_to_idx_() {
    static std::unordered_map<brot::module_tag, std::size_t> um;
    return um;
}

template<brot::stage S>
std::size_t brot::engine::find_or_insert_dep_idx_(const module_tag &t) {
    auto &r_lists = receivers_<S>();
    auto &tag_to_idx = tag_to_idx_<S>();
    for (const auto &r_list: r_lists) {
        if (r_list.first != t) {
            tag_to_idx[r_list.first]++;
        }
    }

    r_lists.emplace(r_lists.begin(), t, std::vector<receiver<S>>{});
    return 0;
}