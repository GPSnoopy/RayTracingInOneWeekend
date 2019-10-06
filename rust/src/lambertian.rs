use super::hit_record::HitRecord;
use super::material::*;
use super::random::*;
use super::ray::Ray;
use super::vec3::Vec3;

pub struct Lambertian {
    pub albedo: Vec3,
}

impl Lambertian {
    pub fn new(albedo: Vec3) -> Lambertian {
        Lambertian { albedo }
    }
}

impl Material for Lambertian {
    fn scatter(&self, ray: &Ray, record: &HitRecord, random: &mut Random) -> Option<MaterialRay> {
        let target = record.point + record.normal + random_in_unit_sphere(random);
        Some(MaterialRay::new(
            Ray::new(record.point, target - record.point),
            self.albedo,
        ))
    }
}
