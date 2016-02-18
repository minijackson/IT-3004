#include <type_traits>

template <template<typename> class Trait, typename Head, typename... Tail>
struct check_all
  : std::integral_constant<bool, Trait<Head>::value && check_all<Trait, Tail...>::value> {};

template <template<typename> class Trait, typename Head>
struct check_all<Trait, Head> : std::integral_constant<bool, Trait<Head>::value> {};

template<template<typename> class Trait, typename... Args>
constexpr bool const check_all_v = check_all<Trait, Args...>::value;
