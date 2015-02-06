#include "vesp/math/Matrix.hpp"

namespace vesp { namespace math {

	Mat4 DXPerspective(F32 fovY, F32 aspect, F32 zNear, F32 zFar)
	{
		assert(abs(aspect - std::numeric_limits<F32>::epsilon()) > 0.0f);
		assert(zFar > zNear);

		F32 const tanHalfFovY = -tan(fovY / 2.0f);

		Mat4 result(0.0f);
		result[0][0] = 1.0f / (aspect * tanHalfFovY);
		result[1][1] = 1.0f / tanHalfFovY;
		result[2][2] = (zFar) / (zFar - zNear);
		result[2][3] = 1.0f;
		result[3][2] = - (zFar * zNear) / (zFar - zNear);
		return result;
	}

} }