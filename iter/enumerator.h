// enumerator.h - iter::enumerator with operator bool
#pragma once
#include <cmath>
#include <functional>
#include <iterator>
#include <type_traits>
#include "input.h"

namespace iter {

	// read-only input iterator with sentinel
	template<class I, class T = typename std::iterator_traits<I>::value_type>
	struct enumerator_base : public std::iterator<std::input_iterator_tag, T> {
	public:
		enumerator_base()
		{ }
		~enumerator_base()
		{ }
/*
		operator I()
		{
			return I::operator I();
		}
*/		// return false when done
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
		I i;
	public:
		enumerator()
		{ }
		enumerator(I i)
			: i(i)
		{ }
/*		enumerator(const enumerator&) = default;
		enumerator(enumerator&&) = default;
		enumerator& operator=(enumerator&&) = default;
		enumerator& operator=(const enumerator&) = default;
*/		~enumerator()
		{ }

		operator I() const
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
	template<class I, class T = typename std::iterator_traits<I>::value_type>
	inline enumerator<I,T> make_enumerator(I i)
	{
		return enumerator<I,T>(i);
	}

	// null terminated enumerator
	template<class I, class T = typename std::iterator_traits<I>::value_type>
	class null_enumerator : public enumerator_base<I,T> {
	I i;
	public:
		null_enumerator()
		{ }
		null_enumerator(I i)
			: i(i)
		{ }

		operator I() const
		{
			return i;
		}
		operator bool() const
		{
			return *i != 0;
		}
		T operator*() const
		{
			return *i;
		}
		null_enumerator& operator++()
		{
			++i;

			return *this;
		}
		null_enumerator operator++(int)
		{
			null_enumerator e(*this);

			operator++();

			return e;
		}
	};
	template<class I, class T = typename std::iterator_traits<I>::value_type>
	inline null_enumerator<I,T> make_null_enumerator(I i)
	{
		return null_enumerator<I,T>(i);
	}
	// shorthand
	template<class I, class T = typename std::iterator_traits<I>::value_type>
	inline null_enumerator<I,T> e(I i)
	{
		return null_enumerator<I,T>(i);
	}
	// specialize for doubles
	template<class I>
	class null_enumerator<I,double> : public enumerator_base<I,double> {
		I i;
	public:
		null_enumerator()
		{ }
		null_enumerator(I i)
			: i(i)
		{ }

		operator I() const
		{
			return i;
		}
		operator bool() const
		{
			return std::isnormal(*i) && *i != 0;
		}
		double operator*() const
		{
			return *i;
		}
		null_enumerator& operator++()
		{
			++i;

			return *this;
		}
		null_enumerator operator++(int)
		{
			null_enumerator e(*this);

			operator++();

			return e;
		}
	};
	// specialize for floats
	template<class I>
	class null_enumerator<I,float> : public enumerator_base<I,float> {
		I i;
	public:
		null_enumerator()
		{ }
		null_enumerator(I i)
			: i(i)
		{ }

		operator I() const
		{
			return i;
		}
		operator bool() const
		{
			return std::isnormal(*i) && *i != 0;
		}
		float operator*() const
		{
			return *i;
		}
		null_enumerator& operator++()
		{
			++i;

			return *this;
		}
		null_enumerator operator++(int)
		{
			null_enumerator e(*this);

			operator++();

			return e;
		}
	};

	// counted enumerator
	template<class I, class T = typename std::iterator_traits<I>::value_type>
	class counted_enumerator : public enumerator_base<I,T> {
	I i;
	mutable size_t n;
	public:
		counted_enumerator()
		{ }
		counted_enumerator(I i, size_t n)
			: i(i), n(n)
		{ }
/*		counted_enumerator(const counted_enumerator&) = default;
		counted_enumerator(counted_enumerator&&) = default;
		counted_enumerator& operator=(counted_enumerator&&) = default;
		counted_enumerator& operator=(const counted_enumerator&) = default;
*/		~counted_enumerator()
		{ }

		operator I() const
		{
			return i;
		}
		operator bool() const
		{
			return n != 0;
		}
		T operator*() const
		{
			return *i;
		}
		counted_enumerator& operator++()
		{
			++i;
			--n;

			return *this;
		}
		counted_enumerator operator++(int)
		{
			counted_enumerator e(*this);

			operator++();

			return e;
		}
	};
	template<class I, class T = typename std::iterator_traits<I>::value_type>
	inline counted_enumerator<I,T> make_counted_enumerator(I i, size_t n)
	{
		return counted_enumerator<I,T>(i, n);
	}
	// shorthand
	template<class I, class T = typename std::iterator_traits<I>::value_type>
	inline counted_enumerator<I,T> e(I i, size_t n)
	{
		return counted_enumerator<I,T>(i, n);
	}

	// end sentinel enumerator
	template<class I, class J = I, class T = typename std::iterator_traits<I>::value_type>
	class end_enumerator : public enumerator_base<I,T> {
		I i;
		J e;
	public:
		end_enumerator()
		{ }
		end_enumerator(I i, J e)
			: i(i), e(e)
		{ }
/*		end_enumerator(const end_enumerator&) = default;
		end_enumerator(end_enumerator&&) = default;
		end_enumerator& operator=(end_enumerator&&) = default;
		end_enumerator& operator=(const end_enumerator&) = default;
*/		~end_enumerator()
		{ }

		operator I() const
		{
			return i;
		}
		operator bool() const
		{
			return i != e;
		}
		T operator*() const
		{
			return *i;
		}
		end_enumerator& operator++()
		{
			++i;

			return *this;
		}
		end_enumerator operator++(int)
		{
			end_enumerator e(*this);

			operator++();

			return e;
		}
	};
	template<class I, class J = I, class T = typename std::iterator_traits<I>::value_type>
	inline end_enumerator<I,J,T> make_end_enumerator(I i, J e)
	{
		return end_enumerator<I,J,T>(i, e);
	}
	/*
	template<class I, class J = I, class T = typename std::iterator_traits<I>::value_type>
	inline end_enumerator<I,J,T> e(I i, J e)
	{
		return end_enumerator<I,J,T>(i, e);
	}
	*/
	/*
	template<class C,
		class I = typename C::iterator,
		class J = typename C::iterator,
		class T = typename C::value_type
	>
	inline end_enumerator<I,J,T> e(C c)
	{
		return end_enumerator<I,J,T>(std::begin(c), std::end(c));
	}
	*/
} // iter

#ifdef _DEBUG
#include <cassert>
#include <vector>

using namespace iter;

template<class E>
inline size_t count(E e) { 
	size_t c = 0; 
	while (e) { 
		++e; 
		++c; 
	} 
	return c; 
};

inline void test_enumerator()
{
	int a[] = {1,2,0};

	{
		enumerator<int*> b(a), c;
		c = b;
//		assert (b == c);
//		assert (*c == *b);
		assert (b);
		assert (*++b == 2);
		b++;
		assert (b && c);
//		assert (b != c);
	}
	{
		auto e = make_enumerator(a);
		assert (e);
		assert (*++e == 2);
		e++;
	}
	{
		auto e = make_null_enumerator("foo");
		assert (3 == count(e));
	}
	{
		auto e = make_counted_enumerator(a, 2);
		assert (e);
		assert (*++e == 2);
		e++;
		assert (!e);
	}
	{
		char foo[] = "foo";
		auto n = e(foo, 3);
		assert (*n++ == 'f' && *n++ == 'o' && *n++ == 'o' && !n);
	}
	{
		auto f = make_end_enumerator(std::begin(a), std::end(a));
		assert (f);
		assert (*++f == 2);
		f++;
		assert (*f++ == 0);
		assert (!f);
	}
}

#endif // _DEBUG