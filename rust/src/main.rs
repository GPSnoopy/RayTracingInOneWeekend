
mod camera;
mod random;
mod ray;
mod vec3;

use camera::Camera;
use random::*;
use ray::Ray;
use vec3::*;

fn main() {
    const WIDTH: usize = 640;
    const HEIGHT: usize = 480;
    const SAMPLES: usize = 4;
    const BOUNCES: usize = 16;

    let look_from = Vec3::new(13.0, 2.0, 3.0);
    let look_at = Vec3::new(0.0, 0.0, 0.0);
    let up = Vec3::new(0.0, 1.0, 0.0);

    const FOV: f32 = 20.0;
    const ASPECT_RATIO: f32 = WIDTH as f32 / HEIGHT as f32;
    const APERTURE: f32 = 0.1;
    const FOCUS_DISTANCE: f32 = 10.0;

    let buffer: Vec<Vec3> = vec![Vec3::zero(); WIDTH*HEIGHT];

    output_frame_buffer(&buffer, WIDTH, HEIGHT);
}

fn render_scene() {

}

fn output_frame_buffer(buffer: &Vec<Vec3>, w: usize, h: usize) {
    println!("P3\n{} {}\n255\n", w, h);

    for j in (0..h).rev() {
        for i in 0..w {
            let color = &buffer[j*w + i];
            println!("{} {} {}", color.x() as i32, color.y() as i32, color.z() as i32);
        }
    }
}

fn render_scene_per_thread(
    buffer: &Vec<Vec3>,
    camera: &Camera,
    w: usize, h: usize, samples: usize, bounces: usize) 
    {
    let mut random = Random::new(1);

    for j in (0..h).rev() {
        for i in 0..w {
            
            let mut color = Vec3::zero();

            for s in 0..samples {
                let u = (i as f32 + uniform(&mut random)) / w as f32;
                let v = (j as f32 + uniform(&mut random)) / h as f32;
                let ray = camera.get_ray(&mut random, u , v);

                color += get_color(&mut random, &ray, 0, bounces);
            }
        }
    }
}

fn get_color(random: &mut Random, ray: &Ray, depth: usize, max_depth: usize) -> Vec3 {
    Vec3::zero()
}
