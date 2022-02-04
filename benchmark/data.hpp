#pragma once
#include <oneapi/tbb.h>
#include <memory>
namespace pad {

template <typename T, typename A = std::allocator<T>>
class default_init_allocator : public A {
	// Implementation taken from https://stackoverflow.com/a/21028912.
   public:
	using A::A;

	template <typename U>
	struct rebind {
		using other = default_init_allocator<
		    U,
		    typename std::allocator_traits<A>::template rebind_alloc<U>>;
	};

	template <typename U>
	void construct(U* ptr) noexcept(
	    std::is_nothrow_default_constructible<U>::value) {
		::new (static_cast<void*>(ptr)) U;
	}
	template <typename U, typename... ArgsT>
	void construct(U* ptr, ArgsT&&... args) {
		std::allocator_traits<A>::construct(
		    static_cast<A&>(*this), ptr, std::forward<ArgsT>(args)...);
	}
};

template <typename T>
class arrayDataOpenMP {
   private:
	std::vector<T, default_init_allocator<T>> a, b, c;

   public:
	arrayDataOpenMP(const arrayDataOpenMP&) = delete;
	arrayDataOpenMP(arrayDataOpenMP&&) = delete;
	arrayDataOpenMP() = delete;
	explicit arrayDataOpenMP(unsigned aSize,
	                         unsigned bSize = 0,
	                         unsigned cSize = 0)
	    : a(aSize), b(bSize), c(cSize) {
#pragma omp parallel for schedule(static)
		for (unsigned i = 0; i < aSize; ++i)
			a[i] = i;
#pragma omp parallel for schedule(static)
		for (unsigned i = 0; i < bSize; ++i)
			b[i] = 1 + i;
#pragma omp parallel for schedule(static)
		for (unsigned i = 0; i < cSize; ++i)
			c[i] = i;
	}
	auto get_range() {
		return std::make_tuple(std::make_tuple(a.begin(), a.end()),
		                       std::make_tuple(b.begin(), b.end()),
		                       std::make_tuple(c.begin(), c.end()));
	}
	auto get_ptr() {
		return std::make_tuple(a.data(), b.data(), c.data());
	}
};

template <typename T, typename Partitioner>
class arrayDataTBB {
   private:
	std::vector<T, default_init_allocator<T>> a, b, c;

   public:
	arrayDataTBB(const arrayDataTBB&) = delete;
	arrayDataTBB(arrayDataTBB&&) = delete;
	arrayDataTBB() = delete;
	explicit arrayDataTBB(unsigned aSize,
	                      unsigned bSize,
	                      unsigned cSize,
	                      Partitioner part)
	    : a(aSize), b(bSize), c(cSize) {
		oneapi::tbb::parallel_for(
		    oneapi::tbb::blocked_range<int>(0, aSize),
		    [&](oneapi::tbb::blocked_range<int>& r) {
			    for (int i = r.begin(); i != r.end(); ++i) {
				    a[i] = i;
			    }
		    },
		    part);
		oneapi::tbb::parallel_for(
		    oneapi::tbb::blocked_range<int>(0, bSize),
		    [&](oneapi::tbb::blocked_range<int>& r) {
			    for (int i = r.begin(); i != r.end(); ++i) {
				    b[i] = i + 1;
			    }
		    },
		    part);
		oneapi::tbb::parallel_for(
		    oneapi::tbb::blocked_range<int>(0, cSize),
		    [&](oneapi::tbb::blocked_range<int>& r) {
			    for (int i = r.begin(); i != r.end(); ++i) {
				    c[i] = i;
			    }
		    },
		    part);
	}
	auto get_range() {
		return std::make_tuple(std::make_tuple(a.begin(), a.end()),
		                       std::make_tuple(b.begin(), b.end()),
		                       std::make_tuple(c.begin(), c.end()));
	}
	auto get_ptr() {
		return std::make_tuple(a.data(), b.data(), c.data());
	}
};

} // namespace pad

