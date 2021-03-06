// Copyright Louis Dionne 2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include "testing.hpp"

#include <te/concept.hpp>
#include <te/concept_map.hpp>
using namespace te::literals;


struct Concept : decltype(te::requires(
  "f"_s = te::function<int (te::T&)>,
  "g"_s = te::function<int (te::T&)>
)) { };

// Since the definition of `f` would be invalid, we need to use a generic
// lambda to delay the instantiation of the body. This test makes sure that
// this "workaround" works.
template <typename T>
static auto const te::default_concept_map<Concept, T> = te::make_default_concept_map<Concept, T>(
  "f"_s = [](auto& t) { t.invalid(); return 222; },
  "g"_s = [](auto& t) { t.valid(); return 333; }
);

struct Foo {
  void invalid() = delete;
  void valid() { }
};

template <>
auto const te::concept_map<Concept, Foo> = te::make_concept_map<Concept, Foo>(
  "f"_s = [](Foo&) { return 444; }
);

int main() {
  Foo foo;
  TE_CHECK(te::concept_map<Concept, Foo>["f"_s](foo) == 444);
  TE_CHECK(te::concept_map<Concept, Foo>["g"_s](foo) == 333);
}
