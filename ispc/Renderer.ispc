
#include "Material.ispc.h"

export uniform int GetTarget()
{
#if defined(ISPC_TARGET_SSE2)
	return 0;
#elif defined(ISPC_TARGET_SSE4)
	return 1;
#elif defined(ISPC_TARGET_AVX)
	return 2;
#elif defined(ISPC_TARGET_AVX2)
	return 3;
#elif defined(ISPC_TARGET_AVX2SKX)
	return 4;
#else
	return -1;
#endif
}

export uniform int GetWidth()
{
	return programCount;
}

export void Render(uniform int width, uniform int height)
{

}
