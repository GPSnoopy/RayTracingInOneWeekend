use super::hit_record::HitRecord;
use super::material::*;
use super::random::*;
use super::ray::Ray;
use super::vec3::*;

pub struct Metal {
    pub albedo: Vec3,
    pub fuzz: f32
}

impl Metal {
    pub fn new(albedo: Vec3, fuzz: f32) -> Metal {
        Metal {albedo, fuzz: fuzz.min(1.0).max(0.0) }
    }
}

impl Material for Metal {
    fn scatter(&self, ray: &Ray, record: &HitRecord, random: &mut Random) -> Option<MaterialRay> {
        let reflected = reflect(unit_vector(ray.direction), record.normal);

		if dot(reflected, record.normal) > 0.0
			{ Some(MaterialRay::new(Ray::new(record.point, reflected + self.fuzz*random_in_unit_sphere(random)), self.albedo)) } else
			{ None }
    }
}