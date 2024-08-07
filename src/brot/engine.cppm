module;

#include <functional>
#include <string>
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
    template<stage S>
    void declare_dependencies(const module_tag &t, const std::vector<module_tag> &deps);

    template<stage S>
    void register_receiver(const module_tag &t, receiver<S> &&r);

    template<stage S, typename... Args>
    void run_stage(Args &&...args);

private:
    template<stage S>
    std::vector<receiver_list<S>> &receivers_();
};
} // namespace brot

template<brot::stage S>
void brot::engine::declare_dependencies(const module_tag &t, const std::vector<module_tag> &deps) {
    auto &rs = receivers_<S>();

    std::size_t min_idx = 0;
}

template<brot::stage S>
void brot::engine::register_receiver(const module_tag &t, receiver<S> &&r) {
    auto &r_lists = receivers_<S>();

    // Check if anything has been registered before
    if (r_lists.empty()) {
        r_lists.emplace_back("", std::vector<receiver<S>>{});
    }

    bool registered = false;
    for (auto &r_list: r_lists) {
        if (r_list.first == t) {
            r_list.second.emplace_back(std::forward<receiver<S>>(r));
            registered = true;
            break;
        }
    }

    // Final section is default if there is not a designated section
    if (!registered) {
        r_lists.back().second.emplace_back(std::forward<receiver<S>>(r));
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