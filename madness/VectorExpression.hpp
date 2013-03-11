#pragma once

#include "madness/types.hpp"


template < typename Operation, typename Va>
struct VectorExpression < Operation, Va >
{
    friend Va;
    typedef VectorExpression< Operation, Va>  This;
    typedef typename Va::ValueType            ValueType;
    const Operation& op;
    const Va& vector;

    inline VectorExpression ( Operation oper, const Va& first) : op(oper), vector(first) {};

    inline auto size() const ->decltype( vector.size() ) {
        return vector.size();
    };

    inline auto operator[]( size_t i ) const 
        ->decltype( vector[i] ) {
        return vector[i];
        };
};

template < typename Operation, typename Va, typename ... Vs >
struct VectorExpression <Operation, Va, Vs...> : protected VectorExpression< Operation, Vs...>
{
    friend Va;
    const Operation op;
    const Va& vector;
    typedef VectorExpression< Operation, Va, Vs...>     This;
    typedef VectorExpression< Operation, Vs... >        Base;
    typedef decltype( op( typename Va::ValueType(), typename Base::ValueType() ) )    ValueType;


    inline VectorExpression ( Operation oper, const Va& first, const Vs&... vs ) : VectorExpression< Operation, Vs...>(oper, vs...), op(oper), vector(first) {};

    inline auto size() const ->decltype( vector.size() ) {
        return vector.size();
    };

    inline auto operator[]( size_t i ) const 
        ->decltype( op( vector[i], this->Base::operator[](i) ) ) {
        return op( vector[i], Base::operator[](i) );
        };
};

template < typename R, typename L >
struct add {
inline auto operator()( const R& a, const L& b) const -> decltype( a + b )
{
    return a + b;
}};

template < typename R, typename L >
struct sub {
inline auto operator()( const R& a, const L& b) const -> decltype( a - b )
{
    return a - b;
}};

template < typename R, typename L >
struct mult {
inline auto operator()( const R& a, const L& b) const -> decltype( a * b )
{
    return a * b;
}};

//Add using VectorExpression:
template < typename T, typename T2 >
VectorExpression< add < typename T::ValueType, typename T2::ValueType >, T, T2>
inline operator+( const T& rhs, const T2& lhs ) 
{
    return VectorExpression<  add< typename T::ValueType, typename T2::ValueType >, T, T2 >
        ( add< typename T::ValueType, typename T2::ValueType >() , rhs, lhs );
};

template < typename T, typename T2 >
VectorExpression< sub< typename T::ValueType, typename T2::ValueType >, T, T2>
inline operator-( const T& rhs, const T2& lhs ) 
{
    return VectorExpression<  sub< typename T::ValueType, typename T2::ValueType >, T, T2 >
        ( sub< typename T::ValueType, typename T2::ValueType >() , rhs, lhs );
};

template < typename T, typename T2 >
VectorExpression< mult< typename T::ValueType, typename T2::ValueType >, T, T2>
inline operator*( const T& rhs, const T2& lhs ) 
{
    return VectorExpression<  mult< typename T::ValueType, typename T2::ValueType >, T, T2 >
        ( mult< typename T::ValueType, typename T2::ValueType >() , rhs, lhs );
};