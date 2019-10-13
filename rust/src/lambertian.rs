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
    fn scatter(&self, _ray: &Ray, hit: &HitRecord, random: &mut Random) -> Option<MaterialRay> {
        let target = hit.point + hit.normal + random_in_unit_sphere(random);
        Some(MaterialRay::new(
            Ray::new(hit.point, target - hit.point),
            self.albedo,
        ))
    }
}
