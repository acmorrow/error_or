// Copyright 2013 Andrew C. Morrow
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef included_6603F051_ED25_4096_8DCF_0F4A5829CACD
#define included_6603F051_ED25_4096_8DCF_0F4A5829CACD

#include <type_traits>
#include <utility>

namespace acm {
    namespace detail  {

        namespace adl_swap_ns {
            using std::swap;

            template<typename T>
            class is_swappable_test {

                struct swap_not_found_type {};

                template<typename U>
                static auto test(U& u) -> decltype(swap(u, u));

                template<typename U>
                static auto test(...) -> swap_not_found_type;

                using test_type = decltype(test<T>(std::declval<T&>()));

            public:
                static constexpr bool value = !std::is_same<test_type, swap_not_found_type>::value;
            };

            template<bool, typename T>
            struct is_nothrow_swappable_test :
                std::integral_constant<bool, noexcept(swap(std::declval<T&>(), std::declval<T&>()))> {
            };

            template<typename T>
            struct is_nothrow_swappable_test<false, T> :
                std::false_type {
            };

        } // namespace adl_swap_ns

        template<typename T>
        struct is_swappable :
            std::integral_constant<bool, adl_swap_ns::is_swappable_test<T>::value> {
        };

        // This really should be part of C++
        template<typename T>
        struct is_nothrow_swappable
            : std::integral_constant<bool, adl_swap_ns::is_nothrow_swappable_test<is_swappable<T>::value, T>::value> {
        };

    } // namespace detail
} // namespace acm

#endif // included_6603F051_ED25_4096_8DCF_0F4A5829CACD
