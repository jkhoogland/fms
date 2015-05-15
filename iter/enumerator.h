// enumerator.h - iter::enumerator with operator bool

#pragma once
#include <cmath>
#include <functional>
#include <iterator>
#include <type_traits>

namespace iter {

	// read-only input iterator with sentinel
	template<class I, class T = typename std::iterator_traits<I>::value_type>
	struct enumerator_base : public std::iterator<std::input_iterator_tag, T> {
	public:
		enumerator_base()
		{ }
		~enumerator_base()
		{ }

		// return false when done
		operator bool() const
		{
			return I::operator bool();
		}
		// not necessarily I::value_type
		T operator*(void) const
		{
			return I::operator*();
		}
		enumerator_base& operator++()
		{
			return I::operator++();
		}
		enumerator_base operator++(int)
		{
			return I::operator++(0);
		}
	};

	template<class I, class T = typename std::iterator_traits<I>::value_type>
	class enumerator : public enumerator_base<I,T> {
	protected:
		I i;
	public:
		enumerator()
		{ }
		enumerator(I i)
			: i(i)
		{ }
		template<class J>
		enumerator(J j)
			: i(j.i)
		{ }

		operator bool() const
		{
			return true; // infinite
		}
		T operator*() const
		{
			return *i;
		}
		enumerator& operator++()
		{
			++i;

			return *this;
		}
		enumerator operator++(int)
		{
			enumerator e(*this);

			operator++();

			return e;
		}
	};
	template<class I, class T = typename std::iterator_traits<I>::value_type>
	inline enumerator<I,T> make_enumerator(I i)
	{
		return enumerator<I,T>(i);
	}

	// specializations for pointers
	template<class T>
//	class enumerator<T*,T> {
	class enumerator<T*,T> : public enumerator_base<T*,T> {
	protected:
		T* i;
	public:
		enumerator()
		{ }
		enumerator(T* i)
			: i(i)
		{ }

		operator const T*() const
		{
			return i;
		}
		operator bool() const
		{
			return true; // infinite
		}
		T operator*() const
		{
			return *i;
		}
		enumerator& operator++()
		{
			++i;

			return *this;
		}
		enumerator operator++(int)
		{
			enumerator e(*this);

			operator++();

			return e;
		}
	};
/*	template<class T, size_t N>
	class enumerator<T(&)[N],T> {
		T* i;
		size_t n;
	public:
		enumerator()
		{ }
		enumerator(T (&i)[N])
			: i(i), n(N)
		{ }
		operator T*()
		{
			return i;
		}
		operator bool() const
		{
			return n != 0; // infinite
		}
		T operator*() const
		{
			return *i;
		}
		enumerator& operator++()
		{
			++i;
			--n;

			return *this;
		}
		enumerator operator++(int)
		{
			enumerator e(*this);

			operator++();

			return e;
		}
	};
*/
	// epsilon_iterator ???
} // iter

#ifdef _DEBUG
#include <cassert>
#include <vector>

using namespace iter;

inline void test_enumerator()
{
	int a[] = {1,2,0};

	{
		enumerator<int*> b(a), c;
		c = b;
//		assert (b == c);
		assert (*c == *b);
		assert (b);
		assert (*++b == 2);
		b++; // not really the end
		assert (b && c);
//		assert (b == c); // operator bool() => true == true
	}
	{
		auto e = make_enumerator(a);
		assert (e);
		assert (*++e == 2);
		e++;
	}
}

#endif // _DEBUG