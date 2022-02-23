#ifndef METAREPOBASE_I_H_969872685611
#define METAREPOBASE_I_H_969872685611

#include "metapp/metatype.h"
#include "metapp/methodlist.h"

#include <unordered_map>
#include <memory>

namespace metapp {

namespace internal_ {

template <typename Map>
auto getPointerFromMap(const Map & map, const std::string & name)
-> const typename Map::mapped_type *
{
	auto it = map.find(name);
	if(it == map.end()) {
		return nullptr;
	}
	return &it->second;
}

class MetaRepoBase
{
public:
	MetaRepoBase()
		:
			nameTypeMap(),
			kindTypeMap(),
			methodListMap(),
			fieldMap()
	{
	}

	void addType(const std::string & name, const MetaType * metaType) {
		nameTypeMap[name] = metaType;
		kindTypeMap[metaType->getTypeKind()] = metaType;
	}

	template <typename T>
	void addType(const std::string & name) {
		addType(name, getMetaType<T>());
	}

	const MetaType * getTypeByName(const std::string & name) const {
		auto it = nameTypeMap.find(name);
		if(it != nameTypeMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	const MetaType * getTypeByKind(const TypeKind kind) const {
		auto it = kindTypeMap.find(kind);
		if(it != kindTypeMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	void addMethod(const std::string & name, const Variant & method) {
		if(! method.getMetaType()->isCallable()) {
			return;
		}
		std::shared_ptr<MethodList> methodList;
		auto it = methodListMap.find(name);
		if(it == methodListMap.end()) {
			methodList = std::make_shared<MethodList>();
			methodListMap.insert(std::make_pair(name, methodList));
		}
		else {
			methodList = it->second;
		}
		methodList->addMethod(method);
	}

	const MethodList * getMethodList(const std::string & name) const {
		auto it = methodListMap.find(name);
		if(it == methodListMap.end()) {
			return nullptr;
		}
		return it->second.get();
	}

	void addField(const std::string & name, const Variant & field) {
		fieldMap[name]= field;
	}

	const Variant * getField(const std::string & name) const {
		return internal_::getPointerFromMap(fieldMap, name);
	}

private:
	std::unordered_map<std::string, const MetaType *> nameTypeMap;
	std::unordered_map<TypeKind, const MetaType *> kindTypeMap;
	std::unordered_map<std::string, std::shared_ptr<MethodList> > methodListMap;
	std::unordered_map<std::string, Variant> fieldMap;
};


} // namespace internal_

} // namespace metapp

#endif
