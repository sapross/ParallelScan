#pragma once
#include <cmath> // abs
#include <cstdint>
#include <iterator>

namespace pad
{
// change name to transform_iterator
template<typename F> class transform_iterator
{
  private:
    ssize_t idx_;
    F       fun_;

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = decltype(typename std::remove_reference<F>::type{}(0));
    using pointer           = std::nullptr_t;
    using reference         = value_type;
    using difference_type   = ssize_t;

    inline transform_iterator() = delete;

    inline explicit transform_iterator(const ssize_t _idx, F&& _fun)
        : idx_{_idx}, fun_{std::forward<F>(_fun)}
    {
    }

    inline transform_iterator(const transform_iterator& other) = default;

    inline transform_iterator& operator=(const transform_iterator& other) = default;

    // Pointer like operators
    [[nodiscard]] inline reference operator*() { return fun_(idx_); }

    [[nodiscard]] inline reference operator->() { return this->operator*(); }

    // Increment / Decrement
    inline transform_iterator& operator++() noexcept
    {
        ++idx_;
        return *this;
    }

    inline transform_iterator& operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    // index access
    [[nodiscard]] inline reference operator[](size_t i) { return fun_(idx_ + i); }

    // Arithmetic operators
    [[nodiscard]] inline friend difference_type
    operator-(const transform_iterator& lhs, const transform_iterator& rhs) noexcept
    {
        return std::abs(lhs.idx_ - rhs.idx_);
    }

    [[nodiscard]] inline friend transform_iterator
    operator+(const transform_iterator& lhs, const ssize_t idx) noexcept
    {
        auto tmp = lhs;
        tmp.idx_ += idx;
        return tmp;
    }

    // Comparision operators
    [[nodiscard]] inline friend bool operator==(const transform_iterator& lhs,
                                                const transform_iterator& rhs) noexcept
    {
        return lhs.idx_ == rhs.idx_;
    }

    [[nodiscard]] inline friend bool operator!=(const transform_iterator& lhs,
                                                const transform_iterator& rhs) noexcept
    {
        return !(lhs == rhs);
    }
};
} // namespace pad
