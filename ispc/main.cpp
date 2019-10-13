
#include "Camera.h"
#include "Dielectric.h"
#include "HittableList.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Random.h"
#include "Sphere.h"

#include <limits>
#include <functional>
#include <thread>

HittableList RandomWorld()
{
	Random random(42);  // NOLINT

	HittableList world;

	world.Add(new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5f, 0.5f, 0.5f))));

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
					world.Add(new Sphere(center, 0.2f, new Lambertian(Vec3(
						Uniform(random)*Uniform(random), 
						Uniform(random)*Uniform(random), 
						Uniform(random)*Uniform(random)))));
				}
				else if (chooseMat < 0.95f) // Metal
				{
					world.Add(new Sphere(center, 0.2f, new Metal(
						Vec3(0.5f*(1 + Uniform(random)), 0.5f*(1 + Uniform(random)), 0.5f*(1 + Uniform(random))),
						0.5f*Uniform(random))));
				}
				else // Glass
				{
					world.Add(new Sphere(center, 0.2f, new Dielectric(1.5f)));
				}
			}
		}
	}

	world.Add(new Sphere(Vec3(0, 1, 0), 1.0f, new Dielectric(1.5f)));
	world.Add(new Sphere(Vec3(-4, 1, 0), 1.0f, new Lambertian(Vec3(0.4f, 0.2f, 0.1f))));
	world.Add(new Sphere(Vec3(4, 1, 0), 1.0f, new Metal(Vec3(0.7f, 0.6f, 0.5f), 0.0f)));

	return world;
}

Vec3 Color(Random& random, const Hittable& world, const Ray& ray, const int depth, const int maxDepth)
{
	const auto record = world.Hit(ray, 0.001f, std::numeric_limits<float>::max());

	if (record)
	{
		const auto materialRay = record->Material->Scatter(ray, *record, random);

		return depth < maxDepth && materialRay 
			? materialRay->Attenuation * Color(random, world, materialRay->Scattered, depth + 1, maxDepth) 
			: Vec3(0);
	}

	const Vec3 direction = UnitVector(ray.Direction);
	const float t = 0.5f*(direction.y() + 1);
	return (1 - t)*Vec3(1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

void ThreadMainLoop(
	std::vector<Vec3>& buffer,
	const Camera& camera, const HittableList& world,
	const int w, const int h, const int samples, const int bounces, 
	const int numThreads, const int threadId)
{
	Random random(threadId + 1);

	for (int j = h - threadId - 1; j >= 0; j -= numThreads)
	{
		for (int i = 0; i < w; ++i)
		{
			Vec3 color(0);

			for (int s = 0; s < samples; ++s)
			{
				const float u = float(i + Uniform(random)) / float(w);
				const float v = float(j + Uniform(random)) / float(h);

				color += Color(random, world, camera.GetRay(random, u, v), 0, bounces);
			}

			buffer[j*w + i] = Sqrt(color / float(samples)) * 255.99f;
		}
	}
}

void MultiThreadedRayTracing(
	std::vector<Vec3>& buffer, 
	const HittableList& world, const Camera& camera, 
	const int w, const int h, const int samples, const int bounces)
{
	const int numThreads = std::max(1, static_cast<int>(std::thread::hardware_concurrency()) - 2);
	std::vector<std::thread> threads;

	std::cerr << "Ray-tracing using " << numThreads << " threads" << std::endl;

	for (int t = 0; t != numThreads; ++t)
	{
		threads.emplace_back([=, &buffer, &camera, &world]()
		{
			ThreadMainLoop(buffer, camera, world, w, h, samples, bounces, numThreads, t);
		});
	}	

	for (auto& thread : threads)
	{
		thread.join();
	}
}

void OutputFramebuffer(const std::vector<Vec3>& buffer, const int w, const int h)
{
	std::cout << "P3\n" << w << " " << h << "\n255\n";

	for (int j = h; j--;)
	{
		for (int i = 0; i < w; ++i)
		{
			const Vec3& color = buffer[j*w + i];
			std::cout << static_cast<int>(color.x()) << " " << static_cast<int>(color.y()) << " " << static_cast<int>(color.z()) << "\n";
		}
	}
}

void Application()
{
	const int w = 3840/6;//640;
	const int h = 2160/6;//360;
	const int samples = 1;// 1024;
	const int bounces = 16;

	const Vec3 lookFrom(13, 2, 3);
	const Vec3 lookAt(0, 0, 0);
	const Vec3 up(0, 1, 0);
	const float fov = 20;
	const float aspectRatio = float(w) / float(h);
	const float aperture = 0.1;
	const float focusDistance = 10.0f;

	const Camera camera = Camera::LookAt(lookFrom, lookAt, up, fov, aspectRatio, aperture, focusDistance);
	const HittableList world = RandomWorld();

	std::vector<Vec3> buffer(w * h);

	MultiThreadedRayTracing(buffer, world, camera, w, h, samples, bounces);
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
