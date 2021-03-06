// accumulate.h - std::accumulate for enumerations
// accumulate, sum = accumulate<plus>, prod = accumulate<multiplies>
#pragma once
#include <algorithm>
#include <iterator>
#include <type_traits>
#include "enumerator/counted.h"

namespace iter {

	// o(t,i[0]), o(o(t,i[0]), i[1]), ...
	template<class O, class I, 
		class T = typename std::iterator_traits<I>::value_type,
		class C = typename std::iterator_traits<I>::interator_category
	>
	class accumulate_ : public enumerator<I,T,C> {
//		O o;
		std::function<T(const T&,const T&)> o;
		T t;
	public:
		using enumerator<I,T,C>::i;
		typedef typename enumerator_traits<I>::is_counted is_counted;

		accumulate_()
		{ }
		accumulate_(O o, I i, T t)
			: enumerator<I,T,C>(i), o(o), t(i ? o(t,*i) : t)
		{ }

		I iterator()
		{
			return i;
		}
		operator bool() const
		{
			return i;
		}
		T operator*() const
		{
			return t;
		}
		accumulate_& operator++()
		{
			if (i)
				t = o(t, *++i);

			return *this;
		}
		accumulate_ operator++(int)
		{
			accumulate_ a(*this);

			operator++();

			return a;
		}
	};
	template<class O, class I, 
		class T = typename std::iterator_traits<I>::value_type,
		class C = typename std::iterator_traits<I>::iterator_category
	>
	inline auto accumulate(O o, I i, T t)
	{
		return accumulate_<O,I,T,C>(o, i, t);
	}

	// running sum of enumerator values
	template<class I, 
		class T = typename std::iterator_traits<I>::value_type,
		class C = typename std::iterator_traits<I>::iterator_category
	>
	inline auto sum(I i, T t = T(0))
	{
		return accumulate_<std::plus<T>,I,T,C>(std::plus<T>{}, i, t);
	}

	// running product of  enumerator values
	template<class I, 
		class T = typename std::iterator_traits<I>::value_type,
		class C = typename std::iterator_traits<I>::iterator_category
	>
	inline auto prod(I i, T t = T(1))
	{
		return accumulate_<std::multiplies<T>,I,T,C>(std::multiplies<T>{}, i, t);
	}

} // iter

#ifdef _DEBUG
#include "include/ensure.h"

inline void test_accumulate()
{
	int a[] = {1,2,3};
	{	
		auto b = accumulate(std::plus<int>{}, ce(a,3), 0);
		auto c(b);
		b = c;
		ensure (*b++ == 1);
		ensure (*b == 3);
		ensure (*++b == 6);
		ensure (!++b);
	}
	{	
		auto b = sum(ce(a,3));
		auto c(b);
		b = c;
		ensure (*b++ == 1);
		ensure (*b == 3);
		ensure (*++b == 6);
		ensure (!++b);
	}
	{	
		auto b = prod(ce(a,3));
		auto c(b);
		b = c;
		ensure (*b++ == 1);
		ensure (*b == 2);
		ensure (*++b == 6);
		ensure (!++b);
	}
}

#endif // _DEBUG