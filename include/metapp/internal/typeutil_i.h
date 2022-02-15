#ifndef TYPEUTIL_I_H_969872685611
#define TYPEUTIL_I_H_969872685611

#include <type_traits>
#include <istream>
#include <ostream>

namespace metapp {

namespace internal_ {

template <typename T>
struct HasInputStreamOperator
{
	template <typename C> static std::true_type test(
		typename std::remove_reference<decltype(std::declval<std::istream>() >> std::declval<C &>())>::type *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <typename T>
struct HasOutputStreamOperator
{
	template <typename C> static std::true_type test(
		typename std::remove_reference<decltype(std::declval<std::ostream>() << std::declval<C>())>::type *);
	template <typename C> static std::false_type test(...);

	enum { value = !! decltype(test<T>(0))() };
};

template <size_t ...Ns>
struct MaxOfInt
{
};

template <size_t N1, size_t N2, size_t ...Ns>
struct MaxOfInt <N1, N2, Ns...>
{
	static constexpr size_t r = MaxOfInt<N2, Ns...>::value;
	static constexpr size_t value = (N1 > r ? N1 : r);
};

template <size_t N1>
struct MaxOfInt <N1>
{
	static constexpr size_t value = N1;
};

} // namespace internal_

} // namespace metapp

#endif
