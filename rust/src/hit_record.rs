use super::material::Material;
use super::vec3::Vec3;

pub struct HitRecord<'a> {
    pub t: f32,
    pub point: Vec3,
    pub normal: Vec3,
    pub material: &'a Material,
}

impl<'a> HitRecord<'a> {
    pub fn new(t: f32, point: Vec3, normal: Vec3, material: &Material) -> HitRecord {
        HitRecord { t, point, normal, material }
    }
}
