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

#ifndef included_1194ba38_e51a_4c64_b628_3f907e697307
#define included_1194ba38_e51a_4c64_b628_3f907e697307

#include <memory>
#include <cstdio>

#include "../detail/is_nothrow_swappable.hpp"

namespace acm {

    class __attribute__((visibility("default"))) Foo {
    public:
        std::unique_ptr<int> token;

        Foo();

        Foo(const Foo& other);
        Foo(Foo&& other);

        Foo& operator=(const Foo& other);
        Foo& operator=(Foo&& other);

        ~Foo();
    };
    static_assert(acm::detail::is_swappable<Foo>::value, "Foo should be swappable");
    static_assert(!acm::detail::is_nothrow_swappable<Foo>::value, "Foo should not be nothrow swappable");


    class __attribute__((visibility("default"))) FooNoExcept {
    public:
        std::unique_ptr<int> token;

        FooNoExcept() noexcept;

        FooNoExcept(const FooNoExcept& other) noexcept;
        FooNoExcept(FooNoExcept&& other) noexcept;

        FooNoExcept& operator=(const FooNoExcept& other) noexcept;
        FooNoExcept& operator=(FooNoExcept&& other) noexcept;

        ~FooNoExcept() noexcept;
    };
    static_assert(acm::detail::is_swappable<FooNoExcept>::value, "FooNoExcept should be swappable");
    static_assert(acm::detail::is_nothrow_swappable<FooNoExcept>::value, "FooNoExcept should be nothrow swappable");


    class __attribute__((visibility("default"))) FooInline {
    public:
        std::unique_ptr<int> token;

        FooInline();

        FooInline(const FooInline& other);
        FooInline(FooInline&& other);

        FooInline& operator=(const FooInline& other);
        FooInline& operator=(FooInline&& other);

        ~FooInline();
    };

    class __attribute__((visibility("default"))) FooInlineNoExcept {
    public:
        std::unique_ptr<int> token;

        FooInlineNoExcept() noexcept;

        FooInlineNoExcept(const FooInlineNoExcept& other) noexcept;
        FooInlineNoExcept(FooInlineNoExcept&& other) noexcept;

        FooInlineNoExcept& operator=(const FooInlineNoExcept& other) noexcept;
        FooInlineNoExcept& operator=(FooInlineNoExcept&& other) noexcept;

        ~FooInlineNoExcept() noexcept;
    };

    FooInline::FooInline()
        : token(new int(17)) {
        printf("X FooInline()\n");
    }

    FooInline::FooInline(const FooInline& other)
        : token(new(std::nothrow) int(*other.token.get())) {
        printf("X FooInline(const FooInline&)\n");
    }

    FooInline::FooInline(FooInline&& other)
        : token(std::move(other.token)) {
        printf("  FooInline(FooInline&&)\n");
    }

    FooInline& FooInline::operator=(const FooInline& other) {
        token.reset(new(std::nothrow) int(*other.token.get()));
        printf("X FooInline& operator=(const FooInline&)\n");
        return *this;
    }

    FooInline& FooInline::operator=(FooInline&& other) {
        token = std::move(other.token);
        printf("  FooInline& operator=(FooInline&&)\n");
        return *this;
    }

    FooInline::~FooInline() {
        if (token)
            printf("X");
        else
            printf(" ");
        printf(" ~FooInline()\n");
    }

    FooInlineNoExcept::FooInlineNoExcept() noexcept
    : token(new(std::nothrow) int(17)) {
        printf("X FooInlineNoExcept()\n");
    }

    FooInlineNoExcept::FooInlineNoExcept(const FooInlineNoExcept& other) noexcept
    : token(new(std::nothrow) int(*other.token.get())) {
        printf("X FooInlineNoExcept(const FooInlineNoExcept&)\n");
    }

    FooInlineNoExcept::FooInlineNoExcept(FooInlineNoExcept&& other) noexcept
    : token(std::move(other.token)) {
        printf("  FooInlineNoExcept(FooInlineNoExcept&&)\n");
    }

    FooInlineNoExcept& FooInlineNoExcept::operator=(const FooInlineNoExcept& other) noexcept {
        token.reset(new(std::nothrow) int(*other.token.get()));
        printf("X FooInlineNoExcept& operator=(const FooInlineNoExcept&)\n");
        return *this;
    }

    FooInlineNoExcept& FooInlineNoExcept::operator=(FooInlineNoExcept&& other) noexcept {
        token = std::move(other.token);
        printf("  FooInlineNoExcept& operator=(FooInlineNoExcept&&)\n");
        return *this;
    }

    FooInlineNoExcept::~FooInlineNoExcept() noexcept {
        if (token)
            printf("X");
        else
            printf(" ");
        printf(" ~FooInlineNoExcept()\n");
    }

} // namespace acm

#endif // included_1194ba38_e51a_4c64_b628_3f907e697307
