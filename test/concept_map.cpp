// Copyright Louis Dionne 2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include "testing.hpp"

#include <te/concept.hpp>
#include <te/concept_map.hpp>
using namespace te::literals;


struct A : decltype(te::requires(
  "f"_s = te::function<int (te::T&)>
)) { };

struct B : decltype(te::requires(
  A{},
  "g"_s = te::function<int (te::T&)>
)) { };

struct C : decltype(te::requires(B{})) { };

struct Foo { };

template <>
auto const te::concept_map<A, Foo> = te::make_concept_map<A, Foo>(
  "f"_s = [](Foo&) { return 222; }
);

template <>
auto const te::concept_map<B, Foo> = te::make_concept_map<B, Foo>(
  "g"_s = [](Foo&) { return 333; }
);

int main() {
  Foo foo;

  TE_CHECK(te::concept_map<C, Foo>["f"_s](foo) == 222);
  TE_CHECK(te::concept_map<C, Foo>["g"_s](foo) == 333);

  TE_CHECK(te::concept_map<B, Foo>["f"_s](foo) == 222);
  TE_CHECK(te::concept_map<B, Foo>["g"_s](foo) == 333);

  TE_CHECK(te::concept_map<A, Foo>["f"_s](foo) == 222);
}
