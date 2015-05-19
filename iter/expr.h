// expr.h - iter expressions
#pragma once
#include <functional>
#include "enumerator.h"

namespace iter {

	template<class O, class I, class J, class T, class U, class V>
	class binop : public enumerator<std::pair<I,J>,V,typename std::common_type_t<
			typename std::iterator_traits<I>::iterator_category,
			typename std::iterator_traits<J>::iterator_category>> {
		O o;
		std::pair<I,J> ij;
	public:
		typedef V value_type;
		binop(O o, I i, J j)
			: o(o), ij(std::make_pair(i, j))
		{ }

		operator bool() const
		{
			return ij.first && ij.second;
		}
		V operator*() const
		{
			return o(*ij.first, *ij.second);
		}
		binop& operator++()
		{
			++ij.first;
			++ij.second;

			return *this;
		}
		binop operator++(int)
		{
			binop a(*this);

			operator++();

			return a;
		}
	};
	template<class O, class I, class J, 
		class T = typename std::iterator_traits<I>::value_type, 
		class U = typename std::iterator_traits<J>::value_type,
		class V = std::common_type_t<T,U>>
	inline auto make_binop(O o, I i, J j)
	{
		return binop<O,I,J,T,U,V>(o, i, j);
	}

	template<class I, class J, class T, class U, class V>
	struct add : public binop<std::plus<V>,I,J,T,U,V> {
		add()
		{ }
		add(I i, J j)
			: binop<std::plus<V>,I,J,T,U,V>(std::plus<V>{}, i, j)
		{ }
	};
	template<class I, class J, 
		class T = typename std::iterator_traits<I>::value_type, 
		class U = typename std::iterator_traits<J>::value_type,
		class V = std::common_type_t<T,U>>
	inline auto make_add(I i, J j)
	{
		return add<I,J,T,U,V>(i, j);
	}


	template<class I, class J, class T, class U, class V>
	struct sub : public binop<std::minus<V>,I,J,T,U,V> {
		sub()
		{ }
		sub(I i, J j)
			: binop<std::minus<V>,I,J,T,U,V>(std::minus<V>{}, i, j)
		{ }
	};

	template<class I, class J, 
		class T = typename std::iterator_traits<I>::value_type, 
		class U = typename std::iterator_traits<J>::value_type,
		class V = std::common_type_t<T,U>>
	inline auto make_sub(I i, J j)
	{
		return sub<I,J,T,U,V>(i, j);
	}

	template<class I, class J, class T, class U, class V>
	struct mul : public binop<std::multiplies<V>,I,J,T,U,V> {
		mul()
		{ }
		mul(I i, J j)
			: binop<std::multiplies<V>,I,J,T,U,V>(std::multiplies<V>{}, i, j)
		{ }
	};
	template<class I, class J, 
		class T = typename std::iterator_traits<I>::value_type, 
		class U = typename std::iterator_traits<J>::value_type,
		class V = std::common_type_t<T,U>>
	inline auto make_mul(I i, J j)
	{
		return mul<I,J,T,U,V>(i, j);
	}

		template<class I, class J, class T, class U, class V>
	struct div : public binop<std::divides<V>,I,J,T,U,V> {
		div()
		{ }
		div(I i, J j)
			: binop<std::divides<V>,I,J,T,U,V>(std::divides<V>{}, i, j)
		{ }
	};
	template<class I, class J, 
		class T = typename std::iterator_traits<I>::value_type, 
		class U = typename std::iterator_traits<J>::value_type,
		class V = std::common_type_t<T,U>>
	inline auto make_div(I i, J j)
	{
		return div<I,J,T,U,V>(i, j);
	}
	template<class I, class J, 
		class T = typename std::iterator_traits<I>::value_type, 
		class U = typename std::iterator_traits<J>::value_type,
		class V = std::common_type_t<T,U>>
	struct equal_ : public binop<std::equal_to<V>,I,J,T,U,bool> {
		equal_()
		{ }
		equal_(I i, J j)
			: binop<std::equal_to<V>,I,J,T,U,bool>(std::equal_to<V>{}, i, j)
		{ }
	};
	template<class I, class J, 
		class T = typename std::iterator_traits<I>::value_type, 
		class U = typename std::iterator_traits<J>::value_type,
		class V = std::common_type_t<T,U>>
	inline auto equal(I i, J j)
	{
		return equal_<I,J,T,U,V>(i, j);
	}

} // iter


template<class I, class J,
	class T = typename std::iterator_traits<I>::value_type, 
	class U = typename std::iterator_traits<J>::value_type,
	class V = std::common_type_t<T,U>>
inline auto operator+(I i, J j)
{
	return iter::add<I,J,T,U,V>(i, j);
}
template<class I, class J,
	class T = typename std::iterator_traits<I>::value_type, 
	class U = typename std::iterator_traits<J>::value_type,
	class V = std::common_type_t<T,U>>
inline auto operator-(I i, J j)
{
	return make_sub(i, j);
}
template<class I, class J,
	class T = typename std::iterator_traits<I>::value_type, 
	class U = typename std::iterator_traits<J>::value_type,
	class V = std::common_type_t<T,U>>
inline auto operator*(I i, J j)
{
	return make_mul(i, j);
}
template<class I, class J,
	class T = typename std::iterator_traits<I>::value_type, 
	class U = typename std::iterator_traits<J>::value_type,
	class V = std::common_type_t<T,U>>
inline auto operator/(I i, J j)
{
	return make_div(i, j);
}
/*
#include "concatenate.h"
template<class I, class J,
	class T = typename std::iterator_traits<I>::value_type, 
	class U = typename std::iterator_traits<J>::value_type,
	class V = std::common_type_t<T,U>>
inline iter::concatenate_<I,J,V> operator,(I i, J j)
{
	return iter::concatenate_<I,J,V>(i,j);
}
*/
#ifdef _DEBUG
#include "include/ensure.h"

using namespace iter;

template<class T, class U, template<typename TU> class O>
inline void test_expr_binop()
{
	O<std::common_type_t<T,U>> o{};
	T a[] = {1,2,3};
	U b[] = {4,5,6};

	auto ab = make_binop(o, a, b);
	int i = 0;
	ensure (*ab == o(a[i],b[i]));
	ab++; i++;
	ensure (*ab == o(a[i],b[i]));
	++i;
	ensure (*++ab == o(a[i],b[i]));
}

inline void test_expr()
{
	test_expr_binop<int,int,std::plus>();
	test_expr_binop<int,double,std::plus>();
	test_expr_binop<double,int,std::plus>();
	test_expr_binop<double,double,std::plus>();

	test_expr_binop<int,int,std::minus>();
	test_expr_binop<int,double,std::minus>();
	test_expr_binop<double,int,std::minus>();
	test_expr_binop<double,double,std::minus>();

	test_expr_binop<int,int,std::multiplies>();
	test_expr_binop<int,double,std::multiplies>();
	test_expr_binop<double,int,std::multiplies>();
	test_expr_binop<double,double,std::multiplies>();

	test_expr_binop<int,int,std::divides>();
	test_expr_binop<int,double,std::divides>();
	test_expr_binop<double,int,std::divides>();
	test_expr_binop<double,double,std::divides>();

	int a[] = {1,2,3};

	{
		auto aa_ = make_add(e(a),e(a));
		auto aa = make_add(a,a);
		ensure (*aa == 1 + 1);
		ensure (*++aa == 2 + 2);
		aa++;
		ensure (*aa == 3 + 3);

		auto a1 = make_add(e(a), e(a));
		auto aaa = make_add(make_add(e(a),e(a)), e(a));
		ensure (*aaa == 3);
		ensure (*++aaa == 6);
		aaa++;
		ensure (*aaa == 9);

		auto a4 = make_add(a,a) + e(a);
		ensure (*a4 == 3);
		ensure (*++a4 == 6);
		a4++;
		ensure (*a4 == 9);

		auto ai = e(a);
		auto a5 = (ai + ai) + (ai + ai) + ai;
		int i = 0;
		ensure (*a5 == 5*a[i]);
		ensure (*++a5 == 5*a[++i]);
		a5++;
		ensure (*a5 == 5*a[++i]);
		{
			auto a6 = e(a) + c(1);// constant<int>(1);
			ensure (*a6 == a[i=0] + 1);
			ensure (*++a6 == a[++i] + 1);
			a6++; i++;
			ensure (*a6 == a[i] + 1);
		}
		{
			auto a6 = constant<int>(1) + e(a);
			ensure (*a6 == a[i=0] + 1);
			ensure (*++a6 == a[++i] + 1);
			a6++; i++;
			ensure (*a6 == a[i] + 1);
		}
	}
	{
		auto aa = make_mul(e(a),e(a));
		ensure (*aa == 1 * 1);
		ensure (*++aa == 2 * 2);
		aa++;
		ensure (*aa == 3 * 3);

		auto a1 = make_mul(e(a), e(a));
		auto aaa = make_mul(make_mul(e(a),e(a)), e(a));
		ensure (*aaa == 1);
		ensure (*++aaa == 8);
		aaa++;
		ensure (*aaa == 27);

		auto a4 = make_mul(a,a) * e(a);
		ensure (*a4 == 1);
		ensure (*++a4 == 8);
		a4++;
		ensure (*a4 == 27);
	}
	{
		int a[] = {1,2,3};
		auto b = equal(a,a);
		ensure (*b);
		ensure (*b++ && *b++ && *b++);

		auto c = equal(a, re(a + 3));
		ensure (!*c);
		ensure (*++c);
		c++;
		ensure (!*c);
		ensure (c && ++c);
	}
}
#endif // _DEBUG
