use super::hit_record::HitRecord;
use super::ray::Ray;

pub trait Hittable {
    fn hit(&self, ray: &Ray, t_min: f32, t_max: f32) -> Option<HitRecord>;
}
