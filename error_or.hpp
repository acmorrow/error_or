// Copyright 2012 Andrew C. Morrow
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

#ifndef included_108FBB04_F426_414D_9EDD_B4D2AF941E96
#define included_108FBB04_F426_414D_9EDD_B4D2AF941E96

#include <cassert>
#include <initializer_list>
#include <memory>
#include <system_error>
#include <type_traits>

#include "detail/is_nothrow_swappable.hpp"

namespace acm {

    template<typename E, typename T>
    class error_or final {

    public:
        using error_type = E;
        using value_type = T;

    private:
        static bool constexpr is_nothrow_swappable =
            detail::is_nothrow_swappable<error_type>::value and
            detail::is_nothrow_swappable<value_type>::value and
            std::is_nothrow_move_constructible<error_type>::value and
            std::is_nothrow_move_constructible<value_type>::value and
            std::is_nothrow_destructible<error_type>::value and
            std::is_nothrow_destructible<value_type>::value;

    public:
        inline error_or() noexcept(std::is_nothrow_default_constructible<value_type>::value) {
            new(&val_.value) value_type;
        }

        inline error_or(error_type error) noexcept(std::is_nothrow_move_constructible<error_type>::value)
            : ok_(false) {
            assert(error);
            new(&val_.error) error_type(std::move(error));
        }

        inline error_or(value_type value) noexcept(std::is_nothrow_move_constructible<value_type>::value) {
            new(&val_.value) value_type(std::move(value));
        }

        template<typename U = value_type>
        inline error_or(std::initializer_list<typename U::value_type> values) noexcept(std::is_nothrow_constructible<U, std::initializer_list<typename U::value_type>>::value) {
            new(&val_.value) value_type(values);
        }

        inline error_or(error_or const& other) noexcept(std::is_nothrow_copy_constructible<error_type>::value and
                                                        std::is_nothrow_copy_constructible<value_type>::value) {
            if (other.ok_)
                new(&val_.value) value_type(other.val_.value);
            else
                new(&val_.error) error_type(other.val_.error);
            ok_ = other.ok_;
        }

        inline error_or(error_or&& other) noexcept((std::is_nothrow_move_constructible<error_type>::value or
                                                    std::is_nothrow_copy_constructible<error_type>::value) and
                                                   (std::is_nothrow_move_constructible<value_type>::value or
                                                    std::is_nothrow_copy_constructible<value_type>::value)) {
            if (other.ok_)
                new(&val_.value) value_type(std::move_if_noexcept(other.val_.value));
            else
                new(&val_.error) error_type(std::move_if_noexcept(other.val_.error));
            ok_ = other.ok_;
        }

        template<typename U>
        inline error_or(error_or<error_type, U> const& other) noexcept(std::is_nothrow_copy_constructible<error_type>::value and
                                                                       std::is_nothrow_copy_constructible<value_type>::value) {
            if (other.ok_)
                new(&val_.value) value_type(other.val_.value);
            else
                new(&val_.error) error_type(other.val_.error);
            ok_ = other.ok_;
        }

        template<typename U>
        inline error_or(error_or<error_type, U>&& other) noexcept((std::is_nothrow_move_constructible<error_type>::value or
                                                                   std::is_nothrow_copy_constructible<error_type>::value) and
                                                                  (std::is_nothrow_constructible<value_type, typename std::add_rvalue_reference<U>::type>::value or
                                                                   std::is_nothrow_constructible<value_type, typename std::add_lvalue_reference<U>::type>::value)) {
            if (other.ok_)
                new(&val_.value) value_type(move_if_noexcept_from(other.val_.value));
            else
                new(&val_.error) error_type(std::move_if_noexcept(other.val_.error));
            ok_ = other.ok_;
        }

        void swap(error_or& other) noexcept(is_nothrow_swappable) {
            other.sfinae_swap(*this);
        }

        friend inline void swap(error_or& a, error_or& b) noexcept(is_nothrow_swappable) {
            a.swap(b);
        }

        inline error_or& operator=(error_type error) noexcept(std::is_nothrow_constructible<error_or, typename std::add_rvalue_reference<error_type>::type>::value) {
            error_or(std::move(error)).swap(*this);
            return *this;
        }

        inline error_or& operator=(value_type value) noexcept(std::is_nothrow_constructible<error_or, typename std::add_rvalue_reference<value_type>::type>::value) {
            error_or(std::move(value)).swap(*this);
            return *this;
        }

        inline error_or& operator=(error_or other) noexcept(std::is_nothrow_move_constructible<error_or>::value) {
            error_or(std::move(other)).swap(*this);
            return *this;
        }

        template<typename U>
        inline error_or& operator=(error_or<error_type, U> other) noexcept(std::is_nothrow_constructible<error_or, error_or<E, U>&&>::value) {
            error_or(std::move(other)).swap(*this);
            return *this;
        }

        inline ~error_or() noexcept(std::is_nothrow_destructible<error_type>::value and
                                    std::is_nothrow_destructible<value_type>::value) {
            if (ok_)
                val_.value.~value_type();
            else
                val_.error.~error_type();
        }

        inline bool ok() const noexcept {
            return ok_;
        }

        inline explicit operator bool() const noexcept {
            return ok_;
        }

        inline error_type const& error() const noexcept {
            assert(!ok_);
            return val_.error;
        }

        inline value_type& value() noexcept {
            assert(ok_);
            return val_.value;
        }

        inline value_type const& value() const noexcept {
            assert(ok_);
            return val_.value;
        }

        inline error_type&& release_error() noexcept {
            assert(!ok_);
            return std::move(val_.error);
        }

        inline value_type&& release_value() noexcept {
            assert(ok_);
            return std::move(val_.value);
        }

    private:
        union val {
            // The lifecycle of members is handled by the enclosing class.
            inline val() noexcept {}
            inline ~val() noexcept {}
            value_type value;
            error_type error;
        } val_;

        bool ok_ = true;

        template<typename U = value_type>
        typename std::enable_if<error_or<error_type, U>::is_nothrow_swappable>::type sfinae_swap(error_or& other) noexcept {
            using std::swap;

            if (ok_ == other.ok_) {
                if (ok_)
                    swap(val_.value, other.val_.value);
                else
                    swap(val_.error, other.val_.error);
            } else {
                if (ok_) {
                    new(&val_.error) error_type(std::move(other.val_.error));
                    new(&other.val_.value) value_type(std::move(val_.value));
                    val_.value.~value_type();
                    other.val_.error.~error_type();
                } else {
                    new(&other.val_.error) error_type(std::move(val_.error));
                    new(&val_.value) value_type(std::move(other.val_.value));
                    other.val_.value.~value_type();
                    val_.error.~error_type();
                }
                swap(ok_, other.ok_);
            }
        }

        template<typename U>
        typename std::conditional
        <
            !std::is_nothrow_constructible<value_type, typename std::add_rvalue_reference<U>::type>::value and std::is_constructible<U, typename std::add_lvalue_reference<U>::type>::value,
            U const&,
            U&&
            >::type
        move_if_noexcept_from(U& u) {
            return std::move(u);
        }

    };

    template<typename E1, typename T1, typename E2, typename T2>
    bool operator==(error_or<E1, T1> const& lhs, error_or<E2, T2> const& rhs) noexcept(noexcept(lhs.value() == rhs.value()) and
                                                                                       noexcept(lhs.error() == rhs.error())) {
        if (lhs.ok() == rhs.ok()) {
            if (lhs.ok()) {
                return lhs.value() == rhs.value();
            } else {
                return lhs.error() == rhs.error();
            }
        }
        return false;
    }

    template<typename E1, typename T1, typename E2, typename T2>
    bool operator!=(error_or<E1, T1> const& lhs, error_or<E2, T2> const& rhs) noexcept(noexcept(lhs == rhs)) {
        return not lhs == rhs;
    }

    template<typename T>
    using error_code_or = error_or<std::error_code, T>;

    template<typename T>
    using error_condition_or = error_or<std::error_condition, T>;

    template<typename T>
    using error_code_or_unique = error_code_or<std::unique_ptr<T>>;

    template<typename T>
    using error_condition_or_unique = error_condition_or<std::unique_ptr<T>>;

} // namespace acm

#endif // included_108FBB04_F426_414D_9EDD_B4D2AF941E96
