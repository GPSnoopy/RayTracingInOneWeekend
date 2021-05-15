
#include "Camera.h"
#include "Hittable.h"
#include "Material.h"
#include "Random.h"

#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

typedef std::vector<ispc::Hittable> HittableList;
typedef std::vector<ispc::Material> MaterialList;

void AddSphere(HittableList& hittables, MaterialList& materials, const Vec3& centre, const float radius, const ispc::Material& material)
{
	materials.push_back(material);
	hittables.push_back(Hittable::Sphere(centre, radius, static_cast<int>(materials.size() - 1)));
}

std::tuple<HittableList, MaterialList> RandomWorld()
{
	Random random(42);  // NOLINT

	HittableList hittables;
	MaterialList materials;

	AddSphere(hittables, materials, Vec3(0, -1000, 0), 1000, Material::Lambertian(Vec3(0.5f, 0.5f, 0.5f)));

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
					AddSphere(hittables, materials, center, 0.2f, Material::Lambertian(Vec3(
						Uniform(random)*Uniform(random), 
						Uniform(random)*Uniform(random), 
						Uniform(random)*Uniform(random))));
				}
				else if (chooseMat < 0.95f) // Metal
				{
					AddSphere(hittables, materials, center, 0.2f, Material::Metallic(
						Vec3(0.5f*(1 + Uniform(random)), 0.5f*(1 + Uniform(random)), 0.5f*(1 + Uniform(random))),
						0.5f*Uniform(random)));
				}
				else // Glass
				{
					AddSphere(hittables, materials, center, 0.2f, Material::Dielectric(1.5f));
				}
			}
		}
	}

	AddSphere(hittables, materials, Vec3(0, 1, 0), 1.0f, Material::Dielectric(1.5f));
	AddSphere(hittables, materials, Vec3(-4, 1, 0), 1.0f, Material::Lambertian(Vec3(0.4f, 0.2f, 0.1f)));
	AddSphere(hittables, materials, Vec3(4, 1, 0), 1.0f, Material::Metallic(Vec3(0.7f, 0.6f, 0.5f), 0.0f));

	return std::forward_as_tuple(hittables, materials);
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
	case 100: return "NEON";
	default: return "Unknown";
	}
}

void Render(
	std::vector<Vec3>& buffer,
	const HittableList& world,
	const MaterialList& materials,
	const ispc::Camera& camera, 
	const int width, const int height, 
	const int samples, 
	const int bounces)
{
	const int numThreads = std::max(1, static_cast<int>(std::thread::hardware_concurrency()) - 2);
	std::vector<std::thread> threads;

	std::cerr << "Ray-tracing using " << numThreads << " threads" << std::endl;

	const int tileSize = 16;
	const int numTilesX = (width + tileSize - 1) / tileSize;
	const int numTilesY = (height + tileSize - 1) / tileSize;
	const int numTiles = numTilesX * numTilesY;

	for (int threadIndex = 0; threadIndex != numThreads; ++threadIndex)
	{
		threads.emplace_back([=, &buffer, &camera, &world]()
		{
			for (int tile = threadIndex; tile < numTiles; tile += numThreads)
			{
				const int x0 = (tile % numTilesX) * tileSize;
				const int y0 = (tile / numTilesX) * tileSize;

				const int x1 = std::min(x0 + tileSize, width);
				const int y1 = std::min(y0 + tileSize, height);
				
				ispc::RenderTile(
					reinterpret_cast<ispc::float3*>(buffer.data()),
					world.data(), static_cast<int>(world.size()),
					materials.data(),
					camera,
					x0, x1,
					y0, y1,
					width, height,
					samples,
					bounces);
			}
		});
	}

	for (auto& thread : threads)
	{
		thread.join();
	}
}


void OutputFramebuffer(const std::vector<Vec3>& buffer, const int width, const int height)
{
	std::cout << "P3\n" << width << " " << height << "\n255\n";

	for (int j = height; j--;)
	{
		for (int i = 0; i < width; ++i)
		{
			const Vec3& color = buffer[j * width + i];
			std::cout << static_cast<int>(color.x()) << " " << static_cast<int>(color.y()) << " " << static_cast<int>(color.z()) << "\n";
		}
	}
}

void Application()
{
	const int w = 3840;
	const int h = 2160;
	const int samples = 1024;
	const int bounces = 16;

	const Vec3 lookFrom(13, 2, 3);
	const Vec3 lookAt(0, 0, 0);
	const Vec3 up(0, 1, 0);
	const float fov = 20;
	const float aspectRatio = float(w) / float(h);
	const float aperture = 0.1;
	const float focusDistance = 10.0f;

	const auto camera = Camera::LookAt(lookFrom, lookAt, up, fov, aspectRatio, aperture, focusDistance);
	const auto [hittables, materials] = RandomWorld();
	std::vector<Vec3> buffer(w * h);

	std::cerr << "ISPC Version: " << ispc::GetVersionMajor() << "." << ispc::GetVersionMinor() << std::endl;
	std::cerr << "ISPC Target: " << GetTargetString(ispc::GetTarget()) << std::endl;
	std::cerr << "ISPC Width: " << 
		ispc::GetTargetWidth() * ispc::GetTargetElementWidth() * 8 << " bits (" <<
		ispc::GetTargetWidth() << " x " << ispc::GetTargetElementWidth() << " bytes)" << std::endl;

	Render(buffer, hittables, materials, camera, w, h, samples, bounces);
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
