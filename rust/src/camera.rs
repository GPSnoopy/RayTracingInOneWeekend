
use super::random::*;
use super::ray::*;
use super::vec3::*;

use std::f32;

pub struct Camera {
    pub origin: Vec3,
    pub lower_left_corner: Vec3,
    pub horizontal: Vec3,
    pub vertical: Vec3,
    pub lens_radius: f32,

    u: Vec3,
    v: Vec3, 
    w: Vec3
}

impl Camera {

    pub fn look_at(
        origin: Vec3,
        target: Vec3,
        up: Vec3,
        vertical_fov: f32,
        aspect_ratio: f32,
        aperture: f32,
        focus_distance: f32
    ) -> Camera {

        let theta = vertical_fov * (std::f32::consts::PI / 180.0);
        let half_height = (theta / 2.0).tan();
        let half_width = aspect_ratio * half_height;
        let lens_radius = aperture / 2.0;

        let w = unit_vector(origin - target);
        let u = unit_vector(cross(up, w));
        let v = cross(w, u);

        let lower_left_corner = origin - focus_distance * (half_width * u + half_height * v + w);
        let horizontal = 2.0 * half_width * focus_distance * u;
        let vertical = 2.0 * half_height * focus_distance * v;

        Camera {
            origin,
            lower_left_corner,
            horizontal,
            vertical,
            lens_radius,
            u, v, w
        }
    }

    pub fn get_ray(&self, random: &mut Random, s: f32, t: f32) -> Ray {
        let rd = self.lens_radius * random_in_unit_disk(random);
        let offset = self.u * rd.x() + self.v * rd.y();
        Ray::new(self.origin + offset, self.lower_left_corner + s * self.horizontal + t * self.vertical - self.origin - offset)
    }
}