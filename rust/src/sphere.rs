use super::hit_record::HitRecord;
use super::hittable::Hittable;
use super::material::Material;
use super::ray::Ray;
use super::vec3::*;

pub struct Sphere {
    pub center: Vec3,
    pub radius: f32,
    pub material: Box<Material>,
}

impl Sphere {
    pub fn new(center: Vec3, radius: f32, material: Box<Material>) -> Sphere {
        Sphere { center, radius, material }
    }
}

impl Hittable for Sphere {
    fn hit(&self, ray: &Ray, t_min: f32, t_max: f32) -> Option<HitRecord> {
        // https://en.wikipedia.org/wiki/Quadratic_formula
        let oc = ray.origin - self.center;
        let a = dot(ray.direction, ray.direction);
        let b = dot(oc, ray.direction);
        let c = dot(oc, oc) - self.radius * self.radius;
        let discriminant = b * b - a * c;

        if discriminant >= 0.0 {
            let t1 = (-b - discriminant.sqrt()) / a;
            let t2 = (-b + discriminant.sqrt()) / a;

            if (t_min <= t1 && t1 < t_max) || (t_min <= t2 && t2 < t_max) {
                let t = if t_min <= t1 && t1 < t_max { t1 } else { t2 };
                let point = ray.point_at_parameter(t);
                let normal = (point - self.center) / self.radius;

                return Some(HitRecord::new(t, point, normal, &*self.material));
            }
        }

        None
    }
}
