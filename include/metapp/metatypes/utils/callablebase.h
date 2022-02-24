#ifndef CALLABLEBASE_FUNCTION_H_969872685611
#define CALLABLEBASE_FUNCTION_H_969872685611

#include "metapp/metatype.h"
#include "metapp/metacallable.h"
#include "metapp/utility.h"

namespace metapp {

template <typename FT, typename C, typename RT, typename ...Args>
struct CallableBase
	: public DeclareMetaTypeBase <FT>
{
public:
	using FunctionType = FT;
	using ClassType = C;
	using ReturnType = RT;
	using ArgumentTypeList = TypeList<Args...>;
	static constexpr size_t argsCount = TypeListCount<ArgumentTypeList>::value;

	static const MetaCallable * getMetaCallable() {
		static const MetaCallable metaCallable(
			&getParameterCount,
			&rankInvoke,
			&canInvoke,
			&invoke
		);
		return &metaCallable;
	}

	static size_t getParameterCount()
	{
		return TypeListCount<ArgumentTypeList>::value;
	}

	static int rankInvoke(const Variant * arguments, const size_t argumentCount)
	{
		if(argumentCount != argsCount) {
			return 0;
		}
		return MetaFunctionInvokeChecker<ArgumentTypeList>::rankInvoke(arguments, argumentCount);
	}

	static bool canInvoke(const Variant * arguments, const size_t argumentCount)
	{
		if(argumentCount != argsCount) {
			return false;
		}
		return MetaFunctionInvokeChecker<ArgumentTypeList>::canInvoke(arguments, argumentCount);
	}

	static Variant invoke(void * instance, const Variant & func, const Variant * arguments, const size_t argumentCount)
	{
		if(argumentCount != argsCount) {
			errorIllegalArgument();
		}

		FunctionType f = func.get<FunctionType &>();
		return MetaFunctionInvoker<C, RT, ArgumentTypeList>::invoke(instance, f, arguments, argumentCount);
	}

};


} // namespace metapp


#endif

