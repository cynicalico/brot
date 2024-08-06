module;

#include <functional>
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

class engine {
public:

private:
    template<stage S>
    std::vector<std::pair<std::size_t, receiver<S>>> &receivers_();
};

template<stage S>
std::vector<std::pair<std::size_t, receiver<S>>> &engine::receivers_() {
    static std::vector<std::pair<std::size_t, receiver<S>>> r;
    return r;
}
} // namespace brot