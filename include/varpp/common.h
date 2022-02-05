#ifndef COMMON_H_969872685611
#define COMMON_H_969872685611

#include <memory>

namespace varpp {

constexpr size_t podSize = sizeof(long long);

class VarType
{
private:
	using ValueType = uint16_t;

public:
	constexpr VarType() : value() {
	}
	
	explicit constexpr VarType(const ValueType value) : value(value) {
	}

	//constexpr VarType operator + (const ValueType otherValue) const {
	//	return VarType(value + otherValue);
	//}
	//
	//constexpr bool operator == (const VarType & other) const {
	//	return value == other.value;
	//}

	//constexpr bool operator != (const VarType & other) const {
	//	return value != other.value;
	//}

	//constexpr bool operator < (const VarType & other) const {
	//	return value < other.value;
	//}

	//constexpr bool operator <= (const VarType & other) const {
	//	return value <= other.value;
	//}

	//constexpr bool operator > (const VarType & other) const {
	//	return value > other.value;
	//}

	//constexpr bool operator >= (const VarType & other) const {
	//	return value >= other.value;
	//}

	constexpr operator ValueType() const {
		return value;
	}

private:
	ValueType value;
};

constexpr VarType vtEmpty(0);

constexpr VarType vtVoid(1);

constexpr VarType vtFundamentalBegin(vtVoid + 1);

constexpr VarType vtArithmeticBegin(vtVoid + 1); // 2
constexpr VarType vtIntegerBegin(vtArithmeticBegin); // 2
constexpr VarType vtBool(vtIntegerBegin);
constexpr VarType vtChar(vtIntegerBegin + 1);
constexpr VarType vtWchar(vtIntegerBegin + 2);
constexpr VarType vtSignedChar(vtIntegerBegin + 3);
constexpr VarType vtUnsignedChar(vtIntegerBegin + 4);
constexpr VarType vtShort(vtIntegerBegin + 5);
constexpr VarType vtUnsignedShort(vtIntegerBegin + 6);
constexpr VarType vtInt(vtIntegerBegin + 7);
constexpr VarType vtUnsignedInt(vtIntegerBegin + 8);
constexpr VarType vtLong(vtIntegerBegin + 9);
constexpr VarType vtUnsignedLong(vtIntegerBegin + 10);
constexpr VarType vtLongLong(vtIntegerBegin + 11);
constexpr VarType vtUnsignedLongLong(vtIntegerBegin + 12);
constexpr VarType vtIntegerEnd(vtUnsignedLongLong);

constexpr VarType vtRealBegin(vtIntegerEnd + 1); // 15
constexpr VarType vtFloat(vtRealBegin);
constexpr VarType vtDouble(vtRealBegin + 1);
constexpr VarType vtLongDouble(vtRealBegin + 2);
constexpr VarType vtRealEnd(vtLongDouble);
constexpr VarType vtArithmeticEnd(vtRealEnd);

constexpr VarType vtFundamentalEnd(vtLongDouble);

constexpr VarType vtString(vtFundamentalEnd + 1); // std::string
constexpr VarType vtWideString(vtString + 1); // std::wstring

constexpr VarType vtObject(31); // is equivalent to unkown type

class ExtendType
{
private:
	using ValueType = uint64_t;
public:
	constexpr ExtendType() : value(1) {
	}

	explicit constexpr ExtendType(std::nullptr_t) : value(0) {
	}

	constexpr bool operator == (const ExtendType & other) const {
		return value == other.value;
	}

	constexpr ExtendType operator << (const int bits) const {
		return ExtendType(value << bits);
	}

	constexpr ExtendType operator | (const ExtendType & other) const {
		return ExtendType(value | other.value);
	}

	constexpr ExtendType operator & (const ExtendType & other) const {
		return ExtendType(value & other.value);
	}

	constexpr operator bool() const {
		return value;
	}

private:
	constexpr ExtendType(const ValueType value) : value(value) {
	}

private:
	ValueType value;
};

constexpr ExtendType etNone(nullptr);
constexpr ExtendType etPointer;
constexpr ExtendType etReference = etPointer << 1;

struct VariantData
{
	uint8_t pod[podSize];
	std::shared_ptr<void> object;

	template <typename T>
	T & podAs() const {
		return *(T *)pod;
	}
};

inline bool isPointer(const ExtendType & et)
{
	return et & etPointer;
}

inline bool isReference(const ExtendType & et)
{
	return et & etReference;
}

inline bool isAnyExtension(const ExtendType & et)
{
	return et != etNone;
}


} // namespace varpp

#endif
