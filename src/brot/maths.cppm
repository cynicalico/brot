module;

export module brot.maths;

export namespace brot {
template<typename T>
T add(T a, T b);
} // namespace brot

namespace brot {
template<typename T>
T add(T a, T b) {
    return a + b;
}
} // namespace brot
