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

#ifndef METAPP_VARIANTDATA_H_969872685611
#define METAPP_VARIANTDATA_H_969872685611

#include "metapp/exception.h"
#include "metapp/implement/internal/typeutil_i.h"
#include "metapp/implement/internal/construct_i.h"

#include <memory>
#include <array>
#include <type_traits>

namespace metapp {

class VariantData
{
private:
	static constexpr std::size_t bufferSize = internal_::maxOf(sizeof(long long), internal_::maxOf(sizeof(long double), sizeof(void *)));
	static_assert(bufferSize >= sizeof(int), "VariantData, wrong bufferSize");

	template <typename T>
	using FitBuffer = std::integral_constant<bool,
		std::is_trivial<T>::value
		&& std::is_standard_layout<T>::value
		&& sizeof(T) <= bufferSize
	>;

	static constexpr int storageNone = 0;
	static constexpr int storageObject = 1;
	static constexpr int storageBuffer = 2;
	static constexpr int storageSharedPtr = 3;
	static constexpr int storageReference = 4;

public:
	struct StorageTagObject {};
	struct StorageTagSharedPtr {};
	struct StorageTagReference {};

public:
	VariantData()
		: object(), buffer(), storageType(storageNone)
	{
	}

	template <typename T>
	VariantData(const T * copyFrom, const CopyStrategy copyStrategy)
		: object(), buffer(), storageType(storageNone)
	{
		doConstructOnObjectOrBuffer<T>(copyFrom, FitBuffer<T>(), copyStrategy);
	}

	VariantData(const std::shared_ptr<void> & obj, StorageTagObject)
		: object(obj), buffer(), storageType(storageObject)
	{
	}

	VariantData(const std::shared_ptr<void> & sharedPtr, StorageTagSharedPtr)
		: object(sharedPtr), buffer(), storageType(storageSharedPtr)
	{
	}

	VariantData(const void * copyFrom, StorageTagReference)
		: object(), buffer(), storageType(storageReference)
	{
		podAs<const void *>() = copyFrom;
	}

	void * getAddress() const;

	void swap(VariantData & other) noexcept {
		object.swap(other.object);
		buffer.swap(other.buffer);
	}

private:
	template <typename T>
	void doConstructOnObjectOrBuffer(const void * copyFrom, std::true_type, const CopyStrategy /*copyStrategy*/) {
		object.reset();

		setStorageType(storageBuffer);

		if(copyFrom == nullptr) {
			doConstructOnBufferDefault<T>(
				internal_::TrueFalse<std::is_default_constructible<T>::value && std::is_copy_assignable<T>::value>()
			);
		}
		else {
			doConstructOnBufferCopy<T>(copyFrom, std::is_copy_assignable<T>());
		}
	}

	template <typename T>
	void doConstructOnObjectOrBuffer(const void * copyFrom, std::false_type, const CopyStrategy copyStrategy) {
		setStorageType(storageObject);
		object = internal_::constructSharedPtr<T>(copyFrom, copyStrategy);
	}

	template <typename T>
	void doConstructOnBufferDefault(std::true_type) {
		podAs<T>() = T();
	}

	template <typename T>
	void doConstructOnBufferDefault(std::false_type) {
		raiseException<NotConstructibleException>();
	}

	template <typename T>
	void doConstructOnBufferCopy(const void * copyFrom, std::true_type) {
		podAs<T>() = *(T *)copyFrom;
	}

	template <typename T>
	void doConstructOnBufferCopy(const void * /*copyFrom*/, std::false_type) {
		// For POD, we don't need to check movable. If it's not copyable, just raise error.
		raiseException<NotConstructibleException>();
	}

	template <typename T>
	T & podAs() const {
		return *(T *)(buffer.data());
	}

	int getStorageType() const {
		return storageType;
	}

	void setStorageType(const int value) {
		storageType = value;
	}

private:
	std::shared_ptr<void> object;
	std::array<uint8_t, bufferSize> buffer;
	int storageType;
};

inline void swap(VariantData & a, VariantData & b) noexcept
{
	a.swap(b);
}


} // namespace metapp

#endif
