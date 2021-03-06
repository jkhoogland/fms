// concatenate.h - concatenate an enumerator and an iterator
#pragma once
#include "enumerator.h"

namespace iter {

	template<class I, class J,
		class V = typename std::common_type_t<
			typename std::iterator_traits<I>::value_type,
			typename std::iterator_traits<J>::value_type
		>
	>
	class concatenate_ : public enumerator<std::pair<I,J>, V, std::input_iterator_tag> {
	public:
		using enumerator<std::pair<I,J>, V, std::input_iterator_tag>::i;

		concatenate_()
		{ }
		concatenate_(I i, J j)
			: enumerator<std::pair<I,J>, V, std::input_iterator_tag>(std::make_pair(i,j))
		{ }

		operator bool() const
		{
			return i.first || i.second;
		}
		V operator*() const
		{
			return i.first ? *i.first : *i.second;
		}
		concatenate_& operator++()
		{
			if (i.first)
				++i.first;
			else
				++i.second;

			return *this;
		}
		concatenate_ operator++(int)
		{
			concatenate_ k(*this);

			operator++();

			return k;
		}
	};
	template<class I, class J,
		class V = typename std::common_type_t<
			typename std::iterator_traits<I>::value_type,
			typename std::iterator_traits<J>::value_type
		>
	>
	inline concatenate_<I,J,V> concatenate(I i, J j)
	{
		return concatenate_<I,J,V>(i, j);
	}

} // iter
/*
template<class I, class J,
	class T = typename std::iterator_traits<I>::value_type,
	class U = typename std::iterator_traits<I>::value_type,
	class V = typename std::common_type_t<T,U>
>
inline auto operator,(const iter::enumerator<I,T>& i, const iter::enumerator<J,U>& j)
{
	return iter::concatenate_<I,J,V>(i, j);
}
template<class T, class U,
	class V = typename std::common_type_t<T,U>
>
inline auto operator,(iter::enumerator<T*,T> i, iter::enumerator<U*,U> j)
{
	return iter::concatenate_<T*,U*,V>(i.iterator(), j.iterator());
}
*/

#ifdef _DEBUG
#include "include/ensure.h"
#include "enumerator/counted.h"
#include "enumerator/null.h"

using namespace iter;

inline void test_concatenate()
{
	int i[] = {1,2,0};
	int j[] = {3,4};

	{
		auto c = concatenate(ne(i), ce(j, 2));
		auto d(c);
		c = d;
		ensure (*d == *c);
		ensure (*c == 1);
		ensure (*++c == 2);
		ensure (*++c == 3);
		c++;
		ensure (*c == 4);
	}
	{
		auto c = concatenate(make_null_enumerator(i), make_counted_enumerator(j, 2));
		ensure (*c == 1);
		ensure (*++c == 2);
		ensure (*++c == 3);
		c++;
		ensure (*c == 4);
	}
/*	{
		auto c = operator,(ne(i),ce(j,2));
		ensure (*c == 1);
		ensure (*++c == 2);
		ensure (*++c == 3);
		c++;
		ensure (*c == 4);
	}
*/
}

#endif // _DEBUG