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

#ifndef METAPP_METAMAPPABLE_H_969872685611
#define METAPP_METAMAPPABLE_H_969872685611

#include "metapp/variant.h"
#include "metapp/utilities/utility.h"

namespace metapp {

class MetaMappable
{
public:
	MetaMappable(
		std::pair<const MetaType *, const MetaType *> (*getValueType)(const Variant & mappable),
		Variant (*get)(const Variant & mappable, const Variant & key),
		void (*set)(const Variant & mappable, const Variant & key, const Variant & value)
	)
		:
			getValueType(getValueType),
			get(get),
			set(set)
	{
	}

	std::pair<const MetaType *, const MetaType *> (*getValueType)(const Variant & mappable);
	Variant (*get)(const Variant & mappable, const Variant & key);
	void (*set)(const Variant & mappable, const Variant & key, const Variant & value);
};

inline std::pair<const MetaType *, const MetaType *> mappableGetValueType(const Variant & mappable)
{
	const Variant ref = depointer(mappable);
	return getNonReferenceMetaType(ref)->getMetaMappable()->getValueType(ref);
}

inline Variant mappableGet(const Variant & mappable, const Variant & key)
{
	const Variant ref = depointer(mappable);
	return getNonReferenceMetaType(ref)->getMetaMappable()->get(ref, key);
}

inline void mappableSet(const Variant & mappable, const Variant & key, const Variant & value)
{
	const Variant ref = depointer(mappable);
	getNonReferenceMetaType(ref)->getMetaMappable()->set(ref, key, value);
}


} // namespace metapp

#endif
