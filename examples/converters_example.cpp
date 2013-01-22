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

#include "../converters.hpp"

using namespace acm;

namespace {

    int sometimes_throws_system_error(int val) {
        if (val == 0)
            throw std::system_error(std::make_error_code(std::errc::invalid_argument));
        return val;
    }

    error_code_or<int> sometimes_returns_error_code(int val) {
        if (val == 0)
            return std::make_error_code(std::errc::invalid_argument);
        return val;
    }

} // namespace

int main(int argc, char* argv[]) {

    int arg = std::atoi(argv[1]);

    {
        std::function<int(int)> stse = sometimes_throws_system_error;
        auto wrapped = throw2return(std::move(stse));
        error_code_or<int> result = wrapped(arg);
        if (result) {
            std::cout << "Wrapped sometimes_throws_system_error returned a value: " << result.value() << std::endl;
        } else {
            std::cout << "Wrapped sometimes_throws_system_error returned an error: " << result.error().message() << std::endl;
        }
    }

    {
        std::function<error_code_or<int>(int)> srec = sometimes_returns_error_code;
        auto wrapped = return2throw(std::move(srec));
        try {
            int result = wrapped(arg);
            std::cout << "Wrapped sometimes_returns_error_code returned a value: " << result << std::endl;
        }
        catch(const std::system_error& error) {
            std::cout << "Wrapped sometimes_returns_error_code threw an exception: " << error.code().message() << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
