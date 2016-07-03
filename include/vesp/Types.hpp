#pragma once

#include <glm/fwd.hpp>
#include <memory>
#include <cstdint>

namespace vesp
{
	// Integer types
	typedef uint8_t				U8;
	typedef int8_t				S8;

	typedef uint16_t			U16;
	typedef int16_t				S16;

	typedef uint32_t			U32;
	typedef int32_t				S32;

	typedef uint64_t			U64;
	typedef int64_t				S64;

	// Floating point types
	typedef float				F32;
	typedef double				F64;

	// String types (assume UTF-8)
	typedef char				StringByte;
	typedef StringByte const*	RawStringPtr;

	// Vector types
	typedef glm::vec2			Vec2;
	typedef glm::vec3			Vec3;
	typedef glm::vec4			Vec4;

	typedef glm::ivec2			IVec2;
	typedef glm::ivec3			IVec3;
	typedef glm::ivec4			IVec4;

	// Matrix types
	typedef glm::mat2			Mat2;
	typedef glm::mat3			Mat3;
	typedef glm::mat4			Mat4;

	// Quaternion types
	typedef glm::quat			Quat;

	template <typename T, typename... Args>
	using UniquePtr = std::unique_ptr<T, Args...>;

	template<class T>
	struct AlignedDeleter
	{
		void operator()(T* data) const
		{
			data->~T();
			_aligned_free(data);
		}
	};

	template <typename T>
	using AlignedUniquePtr = UniquePtr<T, AlignedDeleter<T>>;

	template <typename T, typename... Args>
	AlignedUniquePtr<T> MakeAlignedUnique(Args&&... args)
	{
		auto ptr = reinterpret_cast<T*>(_aligned_malloc(sizeof(T), alignof(T)));
		new (ptr) T(args...);
		return AlignedUniquePtr<T> {ptr};
	}
}

#define UniquePtrWithDeleter(T, ptr, deleter) \
	std::unique_ptr<T, decltype(&(deleter))>((ptr), &(deleter))