mod camera;
mod dielectric;
mod hit_record;
mod hittable;
mod hittable_list;
mod lambertian;
mod material;
mod metal;
mod random;
mod ray;
mod sphere;
mod vec3;

use camera::Camera;
use dielectric::Dielectric;
use hittable::Hittable;
use hittable_list::HittableList;
use lambertian::Lambertian;
use metal::Metal;
use random::*;
use ray::Ray;
use sphere::Sphere;
use vec3::*;

 use std::io::Write;

fn main() {
    const WIDTH: usize = 3840;
    const HEIGHT: usize = 2160;
    const SAMPLES: usize = 1024;
    const BOUNCES: usize = 16;

    let look_from = Vec3::new(13.0, 2.0, 3.0);
    let look_at = Vec3::new(0.0, 0.0, 0.0);
    let up = Vec3::new(0.0, 1.0, 0.0);

    const FOV: f32 = 20.0;
    const ASPECT_RATIO: f32 = WIDTH as f32 / HEIGHT as f32;
    const APERTURE: f32 = 0.1;
    const FOCUS_DISTANCE: f32 = 10.0;

    let world = std::sync::Arc::new(random_world());
    let camera = Camera::look_at(look_from, look_at, up, FOV, ASPECT_RATIO, APERTURE, FOCUS_DISTANCE);
    let mut buffer: Vec<Vec3> = vec![Vec3::zero(); WIDTH * HEIGHT];

    render_scene(&mut buffer, &world, camera, WIDTH, HEIGHT, SAMPLES, BOUNCES);
    output_frame_buffer(&buffer, WIDTH, HEIGHT);
}

fn random_world() -> HittableList {

    let mut random_instance = Random::new(42);
    let mut world = HittableList::new();
    let random = &mut random_instance;

    world.add(Box::new(Sphere::new(Vec3::new(0.0, -1000.0, 0.0), 1000.0, Box::new(Lambertian::new(Vec3::new(0.5, 0.5, 0.5))))));

    for a in -11..11 {
        for b in -11..11 {
			let choose_mat = uniform(random);
			let center = Vec3::new(a as f32 + 0.9*uniform(random), 0.2, b as f32 + 0.9*uniform(random));

			if length(center - Vec3::new(4.0, 0.2, 0.0)) > 0.9 {
				if choose_mat < 0.8 { // Diffuse
					world.add(Box::new(Sphere::new(center, 0.2, Box::new(Lambertian::new(Vec3::new(
						uniform(random)*uniform(random), 
						uniform(random)*uniform(random), 
						uniform(random)*uniform(random)))))));
				}
				else if choose_mat < 0.95 { // Metal
					world.add(Box::new(Sphere::new(center, 0.2, Box::new(Metal::new(
						Vec3::new(0.5*(1.0 + uniform(random)), 0.5*(1.0 + uniform(random)), 0.5*(1.0 + uniform(random))),
						0.5*uniform(random))))));
				}
				else { // Glass
					world.add(Box::new(Sphere::new(center, 0.2, Box::new(Dielectric::new(1.5)))));
				}
			}
		}
	}

	world.add(Box::new(Sphere::new(Vec3::new(0.0, 1.0, 0.0), 1.0, Box::new(Dielectric::new(1.5)))));
	world.add(Box::new(Sphere::new(Vec3::new(-4.0, 1.0, 0.0), 1.0, Box::new(Lambertian::new(Vec3::new(0.4, 0.2, 0.1))))));
	world.add(Box::new(Sphere::new(Vec3::new(4.0, 1.0, 0.0), 1.0, Box::new(Metal::new(Vec3::new(0.7, 0.6, 0.5), 0.0)))));

	world
}

fn render_scene(
    buffer: &mut Vec<Vec3>,
    world: &std::sync::Arc<HittableList>,
    camera: Camera,
    w: usize,
    h: usize,
    samples: usize,
    bounces: usize,
) {
    let num_threads = num_cpus::get() - 2;
    let mut threads: Vec<std::thread::JoinHandle<_>> = Vec::with_capacity(num_threads);
    let buffer_ptr = buffer.as_mut_ptr() as usize;

    eprintln!("Ray-tracing using {} threads", num_threads);

    for thread_id in 0..num_threads {
        let world = std::sync::Arc::clone(world);
        threads.push(std::thread::spawn(move || {
            render_scene_per_thread(buffer_ptr, world.as_ref(), camera, w, h, samples, bounces, num_threads, thread_id)
        }));
    }

    for thread in threads {
        thread.join().unwrap();
    }
}

fn render_scene_per_thread(
    buffer_ptr: usize,
    world: &HittableList,
    camera: Camera,
    w: usize,
    h: usize,
    samples: usize,
    bounces: usize,
    num_threads: usize,
    thread_id: usize,
) {
    let mut random = Random::new(thread_id as u64 + 1);

    for j in (thread_id..h).step_by(num_threads).rev() {
        for i in 0..w {
            let mut color = Vec3::zero();

            for _s in 0..samples {
                let u = (i as f32 + uniform(&mut random)) / w as f32;
                let v = (j as f32 + uniform(&mut random)) / h as f32;
                let ray = camera.get_ray(&mut random, u, v);

                color += get_color(&mut random, world, &ray, 0, bounces);
            }

            unsafe 
            {
                // https://internals.rust-lang.org/t/shouldnt-pointers-be-send-sync-or/8818
                let color = sqrt(color / samples as f32) * 255.99;
                let ptr = (buffer_ptr as *mut Vec3).add(j*w + i);
                
                *ptr = color;
            }
        }
    }
}

fn get_color(random: &mut Random, world: &HittableList, ray: &Ray, depth: usize, max_depth: usize) -> Vec3 {
	if let Some(hit) = world.hit(ray, 0.001, std::f32::MAX) {
        if depth < max_depth {
            let material_ray = hit.material.scatter(ray, &hit, random);
            return match material_ray {
                Some(scatter) => {scatter.attenuation * get_color(random, world, &scatter.scattered, depth + 1, max_depth)} 
                None => { Vec3::zero() }
            }
        }
	}

	let direction = unit_vector(ray.direction);
	let t = 0.5*(direction.y() + 1.0);

    (1.0 - t)*Vec3::one() + t * Vec3::new(0.5, 0.7, 1.0)
}

fn output_frame_buffer(buffer: &[Vec3], w: usize, h: usize) {
    // println! is really slow, lock stdout and use it directly.
    let stdout = std::io::stdout();
    let lock = stdout.lock();
    let mut writer = std::io::BufWriter::new(lock);

    writer.write_fmt(format_args!("P3\n{} {}\n255\n", w, h)).unwrap();

    for j in (0..h).rev() {
        for i in 0..w {
            let color = &buffer[j * w + i];
            writer.write_fmt(
                format_args!(
                    "{} {} {}\n",
                    color.x() as i32,
                    color.y() as i32,
                    color.z() as i32
                )
            ).unwrap();
        }
    }
}
