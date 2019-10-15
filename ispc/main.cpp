
#include "Camera.h"
#include "Hittable.h"
#include "Material.h"
#include "Random.h"

#include <cstdlib>
#include <iostream>
#include <vector>

typedef std::vector<ispc::Hittable> HittableList;

HittableList RandomWorld()
{
	Random random(42);  // NOLINT

	HittableList world;

	world.push_back(Hittable::Sphere(Vec3(0, -1000, 0), 1000, Material::Lambertian(Vec3(0.5f, 0.5f, 0.5f))));

	for (int a = -11; a < 11; ++a)
	{
		for (int b = -11; b < 11; ++b)
		{
			const float chooseMat = Uniform(random);
			const Vec3 center(a + 0.9f*Uniform(random), 0.2f, b + 0.9f*Uniform(random));

			if (Length(center - Vec3(4, 0.2f, 0)) > 0.9)
			{
				if (chooseMat < 0.8f) // Diffuse
				{
					world.push_back(Hittable::Sphere(center, 0.2f, Material::Lambertian(Vec3(
						Uniform(random)*Uniform(random), 
						Uniform(random)*Uniform(random), 
						Uniform(random)*Uniform(random)))));
				}
				else if (chooseMat < 0.95f) // Metal
				{
					world.push_back(Hittable::Sphere(center, 0.2f, Material::Metallic(
						Vec3(0.5f*(1 + Uniform(random)), 0.5f*(1 + Uniform(random)), 0.5f*(1 + Uniform(random))),
						0.5f*Uniform(random))));
				}
				else // Glass
				{
					world.push_back(Hittable::Sphere(center, 0.2f, Material::Dielectric(1.5f)));
				}
			}
		}
	}

	world.push_back(Hittable::Sphere(Vec3(0, 1, 0), 1.0f, Material::Dielectric(1.5f)));
	world.push_back(Hittable::Sphere(Vec3(-4, 1, 0), 1.0f, Material::Lambertian(Vec3(0.4f, 0.2f, 0.1f))));
	world.push_back(Hittable::Sphere(Vec3(4, 1, 0), 1.0f, Material::Metallic(Vec3(0.7f, 0.6f, 0.5f), 0.0f)));

	return world;
}

const char* GetTargetString(int target)
{
	switch (target)
	{
	case 0: return "SSE2";
	case 1: return "SSE4";
	case 2: return "AVX";
	case 3: return "AVX2";
	case 4: return "AVX512";
	default: return "Unknown";
	}
}

void Render(std::vector<Vec3>& vec3s, const HittableList& world, const ispc::Camera& camera, const int w, const int h, const int samples, const int bounces)
{
	ispc::Render(
		reinterpret_cast<ispc::float3*>(vec3s.data()),
		world.data(), static_cast<int>(world.size()),
		camera,
		w, h,
		samples, 
		bounces);
}


void OutputFramebuffer(const std::vector<Vec3>& buffer, const int w, const int h)
{
	std::cout << "P3\n" << w << " " << h << "\n255\n";

	for (int j = h; j--;)
	{
		for (int i = 0; i < w; ++i)
		{
			const Vec3& color = buffer[j * w + i];
			std::cout << static_cast<int>(color.x()) << " " << static_cast<int>(color.y()) << " " << static_cast<int>(color.z()) << "\n";
		}
	}
}

void Application()
{
	const int w = 3840/6;
	const int h = 2160/6;
	const int samples = 1;
	const int bounces = 16;

	const Vec3 lookFrom(13, 2, 3);
	const Vec3 lookAt(0, 0, 0);
	const Vec3 up(0, 1, 0);
	const float fov = 20;
	const float aspectRatio = float(w) / float(h);
	const float aperture = 0.1;
	const float focusDistance = 10.0f;

	const auto camera = Camera::LookAt(lookFrom, lookAt, up, fov, aspectRatio, aperture, focusDistance);
	const auto world = RandomWorld();
	std::vector<Vec3> buffer(w * h);

	std::cerr << "ISPC Target: " << GetTargetString(ispc::GetTarget()) << std::endl;
	std::cerr << "ISPC Width: " << ispc::GetWidth() << std::endl;

	Render(buffer, world, camera, w, h, samples, bounces);
	OutputFramebuffer(buffer, w, h);
}

int main()
{
	try
	{
		Application();

		return EXIT_SUCCESS;
	}

	catch (const std::exception& exception)
	{
		std::cerr << "ERROR: " << exception.what() << std::endl;
	}

	catch (...)
	{
		std::cerr << "ERROR: unhandled exception" << std::endl;
	}

	return EXIT_FAILURE;
}
