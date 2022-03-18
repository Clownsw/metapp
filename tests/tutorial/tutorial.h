// metapp library
// 
// Copyright (C) 2022 Wang Qi (wqking)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef TUTORIAL_H
#define TUTORIAL_H

#include "../catch.hpp"

#include <iostream>

#define RUN_TUTORIAL(func) \
TEST_CASE(# func) { \
	std::cout << # func << std::endl; \
	func(); \
	std::cout << std::endl; \
}

// Alias REQUIRE to ASSERT for easy understanding
// for users who are not familar with Catch2
#define ASSERT REQUIRE


#endif
