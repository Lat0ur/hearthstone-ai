#pragma once

#include <stdexcept>
#include <string>
#include <utility>

namespace Utils
{
	template <template <int> typename DefaultInvoked>
	class StaticDispatcher
	{
	public:
		template <int Id> struct DispatchMap
		{
			using type = DefaultInvoked<Id>;
		};

		template <template <typename> class InvokerType, typename Ret, typename... Args>
		static Ret Invoke(int id, Args&&... args)
		{
#define INVOKE_CASE(n) \
			&InvokerType<typename DispatchMap<n>::type>::Invoke,

#define LOOP_X1(n) INVOKE_CASE(n)
#define LOOP_X2(n) LOOP_X1(n) LOOP_X1(n+1)
#define LOOP_X4(n) LOOP_X2(n) LOOP_X2(n+2)
#define LOOP_X8(n) LOOP_X4(n) LOOP_X4(n+4)
#define LOOP_X16(n) LOOP_X8(n) LOOP_X8(n+8)
#define LOOP_X32(n) LOOP_X16(n) LOOP_X16(n+16)
#define LOOP_X64(n) LOOP_X32(n) LOOP_X32(n+32)
#define LOOP_X128(n) LOOP_X64(n) LOOP_X64(n+64)
#define LOOP_X256(n) LOOP_X128(n) LOOP_X128(n+128)
#define LOOP_X512(n) LOOP_X256(n) LOOP_X256(n+256)
#define LOOP_X1024(n) LOOP_X512(n) LOOP_X512(n+512)
#define LOOP_X2048(n) LOOP_X1024(n) LOOP_X1024(n+1024)
#define LOOP_X4096(n) LOOP_X2048(n) LOOP_X2048(n+2048)

			using FuncPtr = Ret (*)(Args&&...);

			static FuncPtr const jump_table[] =
			{
				LOOP_X4096(0)
			};

			if (id < 0) throw std::runtime_error("Invalid id");
			constexpr int jump_table_size = sizeof(jump_table) / sizeof(*jump_table);
			if (id >= jump_table_size) throw std::runtime_error("Need a larger jump table");

			return (*jump_table[id])(std::forward<Args>(args)...);

#undef LOOP_X4096
#undef LOOP_X2048
#undef LOOP_X1024
#undef LOOP_X512
#undef LOOP_X256
#undef LOOP_X128
#undef LOOP_X64
#undef LOOP_X32
#undef LOOP_X16
#undef LOOP_X8
#undef LOOP_X4
#undef LOOP_X2
#undef LOOP_X1
#undef INVOKE_CASE
		}
	};
}
