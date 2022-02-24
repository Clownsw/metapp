#include "../test.h"

#include "metapp/variant.h"
#include "metapp/metatypes/metatypes.h"

#include <iostream>

namespace {

std::string myFunc1(const int a, const std::string & b)
{
	return b + std::to_string(a);
}

TEST_CASE("metatypes, DefaultArgsFunction, myFunc1")
{
	metapp::DefaultArgsFunction<decltype(&myFunc1), 1> func(&myFunc1, { std::string("hello") });
	metapp::Variant v(func);
	REQUIRE(v.getMetaType()->getTypeKind() == metapp::tkDefaultArgsFunction);
	REQUIRE(v.getMetaType()->getUpType()->getTypeKind() == metapp::tkFunction);

	SECTION("Invoke with default arguments") {
		metapp::Variant arguments[] = { 5 };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(arguments, 1));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(nullptr, v, arguments, 1).get<std::string>() == "hello5");
	}
	SECTION("Invoke without default arguments") {
		metapp::Variant arguments[] = { 6, std::string("good") };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(arguments, 2));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(nullptr, v, arguments, 2).get<std::string>() == "good6");
	}
}

struct MyClass
{
	std::vector<std::string> myFunc2(
			const int a,
			const std::string b,
			const long c,
			const std::string d,
			const char e
		)
	{
		return {
			std::to_string(a + value),
			b,
			std::to_string(c),
			d,
			std::to_string(e),
		};
	}

	int value;
};

TEST_CASE("metatypes, DefaultArgsFunction, MyClass::myFunc2")
{
	metapp::DefaultArgsFunction<decltype(&MyClass::myFunc2), 3> func(&MyClass::myFunc2, {
		'a',
		std::string("default"),
		38
	});
	metapp::Variant v(func);
	MyClass obj { 3 };
	SECTION("Invoke with 3 default arguments") {
		metapp::Variant arguments[] = { 5, std::string("hello") };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(arguments, 2));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(&obj, v, arguments, 2).get<const std::vector<std::string> &>()
			== 
			std::vector<std::string> { "8", "hello", "38", "default", "97" }
		);
	}
	SECTION("Invoke with 2 default arguments") {
		metapp::Variant arguments[] = { 6, std::string("good"), 98 };
		REQUIRE(v.getMetaType()->getMetaCallable()->canInvoke(arguments, 3));
		REQUIRE(v.getMetaType()->getMetaCallable()->invoke(&obj, v, arguments, 3).get<const std::vector<std::string> &>()
			== 
			std::vector<std::string> { "9", "good", "98", "default", "97" }
		);
	}
}


} // namespace
