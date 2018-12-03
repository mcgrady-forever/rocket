// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#ifndef ROCKET_BASE_MACRO_H_
#define ROCKET_BASE_MACRO_H_

// Define macro to check C++11
#if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus == 201103L
#define ROCKET_CXX11_ENABLED 1
#endif


//  Private copy constructor and copy assignment ensure classes derived from
//  class Uncopyable cannot be copied.

/// The macro way
#ifdef ROCKET_CXX11_ENABLED
#define ROCKET_DECLARE_UNCOPYABLE(Class) \
private: \
    Class(const Class&) = delete; \
    Class& operator=(const Class&) = delete
#else
#define ROCKET_DECLARE_UNCOPYABLE(Class) \
    Class(const Class&); \
    Class& operator=(const Class&)
#endif

/*

Usage:

class Foo {
public:
    Foo();
    ~Foo();
    
private:
    ROCKET_DECLARE_UNCOPYABLE(Foo);
};

*/

/// The private base class way
namespace uncopyable_details  // protection from unintended ADL
{
class Uncopyable
{
    ROCKET_DECLARE_UNCOPYABLE(Uncopyable);
protected:
    Uncopyable() {}
    ~Uncopyable() {}
};
} // namespace uncopyable_details

typedef uncopyable_details::Uncopyable Uncopyable;

/*

Usage:

class Foo : private Uncopyable
{
};

*/

#endif // ROCKET_BASE_MACRO_H_
