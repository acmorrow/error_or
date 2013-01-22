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

#include <iostream>

#include "../error_or.hpp"
#include "foo.hpp"

using namespace acm;

namespace {

    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template<typename T>
    error_code_or<T> maybe_make_a_t(int val) {
        if (val <= 0)
            return std::make_error_code(std::errc::invalid_argument);
        return T();
    }

    template<typename T>
    void use_a_t(int val) __attribute__((noinline));

    template<typename T>
    void use_a_t(int val) {
        std::printf("Making a %s value\n", typeid(T).name());
        auto result = maybe_make_a_t<T>(val);
        if (result) {
            T t = result.release_value();
        } else {
            std::printf("Failed to make: %s\n", result.error().message().c_str());
        }
    }

    template<typename T>
    error_code_or_unique<T> maybe_make_a_new_t(int val) {
        if (val <= 0)
            return std::make_error_code(std::errc::invalid_argument);
        return make_unique<T>();
    }

    template<typename T>
    void use_a_new_t(int val) __attribute__((noinline));

    template<typename T>
    void use_a_new_t(int val) {
        std::printf("Making a heap %s\n", typeid(T).name());
        auto result = maybe_make_a_new_t<T>(val);
        if (result) {
            std::unique_ptr<T> new_t = result.release_value();
        } else {
            std::printf("Failed to make: %s\n", result.error().message().c_str());
        }
    }

} // namespace

int main(int argc, char* argv[]) {

    int arg = std::atoi(argv[1]);

    use_a_t<Foo>(arg);
    use_a_new_t<Foo>(arg);
    use_a_t<FooNoExcept>(arg);
    use_a_new_t<FooNoExcept>(arg);
    use_a_t<FooInline>(arg);
    use_a_new_t<FooInline>(arg);
    use_a_t<FooInlineNoExcept>(arg);
    use_a_new_t<FooInlineNoExcept>(arg);

    return EXIT_SUCCESS;
}
