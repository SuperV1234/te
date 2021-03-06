// Copyright Louis Dionne 2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#ifndef TE_EXPERIMENTAL_UNROLLED_VTABLE_HPP
#define TE_EXPERIMENTAL_UNROLLED_VTABLE_HPP

#include <te/concept.hpp>
#include <te/detail/erase_function.hpp>

#include <utility>


namespace te { namespace experimental {

namespace detail {
  template <typename Concept, typename ...Mappings>
  struct unrolled_vtable_impl;

  template <typename Concept>
  struct make_unrolled_vtable_impl {
    template <typename ...Mappings>
    using apply = detail::unrolled_vtable_impl<Concept, Mappings...>;
  };
} // end namespace detail

template <typename Concept>
using unrolled_vtable = te::unpack_vtable_layout<
  Concept, detail::make_unrolled_vtable_impl<Concept>::template apply
>;

/*
What follows is a script to generate the specializations of `unrolled_vtable_impl`.
Run the script as

  awk '/SCRIPTBEGIN/{flag=1;next}/SCRIPTEND/{flag=0}flag' ${thisfile} | erb

SCRIPTBEGIN
<% (0..10).each do |n| %>
template <typename Concept
  <%= (0...n).map {|i| ", typename Name#{i}, typename Fptr#{i}" }.join("\n  ") %>
> struct unrolled_vtable_impl<Concept
  <%= (0...n).map {|i| ", std::pair<Name#{i}, Fptr#{i}>" }.join("\n  ") %>
> {
  template <typename ConceptMap>
  constexpr explicit unrolled_vtable_impl(ConceptMap map)
    <%= ": " unless n == 0 %><%= (0...n).map {|i| "fptr#{i}_(te::detail::erase_function<typename decltype(Concept{}.get_signature(Name#{i}{}))::type>(map[Name#{i}{}]))" }.join("\n    , ") %>
  { }
  <%= (0...n).map {|i| "constexpr auto operator[](Name#{i}) const { return fptr#{i}_; };" }.join("\n  ") %>

private:
  <%= (0...n).map {|i| "Fptr#{i} fptr#{i}_;" }.join("\n  ") %>
};
<% end %>
SCRIPTEND */

namespace detail {
  ////////////////////////////////////////////////////////////////////////////
  // BEGIN GENERATED CODE
  ////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////
  // END GENERATED CODE
  ////////////////////////////////////////////////////////////////////////////
} // end namespace detail

}} // end namespace te::experimental

#endif // TE_EXPERIMENTAL_UNROLLED_VTABLE_HPP
