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

#ifndef included_3052c160_4439_448e_8678_dfdc0172440e
#define included_3052c160_4439_448e_8678_dfdc0172440e

#include <functional>
#include <system_error>

#include "error_or.hpp"

namespace acm {

    template<typename T, typename ...Args>
    auto throw2return(T(may_throw_system_error)(Args...)) -> std::function<error_code_or<T>(Args...)> {
        return [may_throw_system_error](Args&&... args) -> error_code_or<T> {
            try {
                return may_throw_system_error(std::forward<Args>(args)...);
            } catch(std::system_error const& xcp) {
                return xcp.code();
            }
        };
    }

    template<typename T, typename ...Args>
    auto throw2return(std::function<T(Args...)> may_throw_system_error) -> std::function<error_code_or<T>(Args...)> {
        return [may_throw_system_error](Args&&... args) -> error_code_or<T> {
            try {
                return may_throw_system_error(std::forward<Args>(args)...);
            } catch(std::system_error const& xcp) {
                return xcp.code();
            }
        };
    }

    template<typename T, typename ...Args>
    auto return2throw(T(returns_error_code_or)(Args...)) -> std::function<T(Args...)> {
        return [returns_error_code_or](Args&&... args) -> T {
            error_code_or<T> result = returns_error_code_or(std::forward<Args>(args)...);
            if (!result)
                throw std::system_error(result.release_error());
            return result.release_value();
        };
    }

    template<typename T, typename ...Args>
    auto return2throw(std::function<error_code_or<T>(Args...)> returns_error_code_or) -> std::function<T(Args...)> {
        return [returns_error_code_or](Args&&... args) -> T {
            error_code_or<T> result = returns_error_code_or(std::forward<Args>(args)...);
            if (!result)
                throw std::system_error(result.release_error());
            return result.release_value();
        };
    }

} // namespace acm

#endif // included_3052c160_4439_448e_8678_dfdc0172440e
