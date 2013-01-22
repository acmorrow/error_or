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

#include "foo.hpp"

namespace acm {

    Foo::Foo()
        : token(new int(17)) {
        printf("X Foo()\n");
    }

    Foo::Foo(const Foo& other)
        : token(new(std::nothrow) int(*other.token.get())) {
        printf("X Foo(const Foo&)\n");
    }

    Foo::Foo(Foo&& other)
        : token(std::move(other.token)) {
        printf("  Foo(Foo&&)\n");
    }

    Foo& Foo::operator=(const Foo& other) {
        token.reset(new(std::nothrow) int(*other.token.get()));
        printf("X Foo& operator=(const Foo&)\n");
        return *this;
    }

    Foo& Foo::operator=(Foo&& other) {
        token = std::move(other.token);
        printf("  Foo& operator=(Foo&&)\n");
        return *this;
    }

    Foo::~Foo() {
        if (token)
            printf("X");
        else
            printf(" ");
        printf(" ~Foo()\n");
    }

    FooNoExcept::FooNoExcept() noexcept
    : token(new(std::nothrow) int(17)) {
        printf("X FooNoExcept()\n");
    }

    FooNoExcept::FooNoExcept(const FooNoExcept& other) noexcept
    : token(new(std::nothrow) int(*other.token.get())) {
        printf("X FooNoExcept(const FooNoExcept&)\n");
    }

    FooNoExcept::FooNoExcept(FooNoExcept&& other) noexcept
    : token(std::move(other.token)) {
        printf("  FooNoExcept(FooNoExcept&&)\n");
    }

    FooNoExcept& FooNoExcept::operator=(const FooNoExcept& other) noexcept {
        token.reset(new(std::nothrow) int(*other.token.get()));
        printf("X FooNoExcept& operator=(const FooNoExcept&)\n");
        return *this;
    }

    FooNoExcept& FooNoExcept::operator=(FooNoExcept&& other) noexcept {
        token = std::move(other.token);
        printf("  FooNoExcept& operator=(FooNoExcept&&)\n");
        return *this;
    }

    FooNoExcept::~FooNoExcept() noexcept {
        if (token)
            printf("X");
        else
            printf(" ");
        printf(" ~FooNoExcept()\n");
    }

} // namespace acm
