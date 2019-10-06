use super::hit_record::HitRecord;
use super::material::*;
use super::random::*;
use super::ray::Ray;
use super::vec3::*;

pub struct Dielectric {
    pub refraction_index: f32,
}

impl Dielectric {
    pub fn new(refraction_index: f32) -> Dielectric {
        Dielectric { refraction_index }
    }
}

impl Material for Dielectric {
    fn scatter(&self, ray: &Ray, record: &HitRecord, random: &mut Random) -> Option<MaterialRay> {
        let dot = dot(ray.direction, record.normal);

        let outward_normal = if dot > 0.0 {-record.normal} else {record.normal};
        let ni_over_nt = if dot > 0.0 {self.refraction_index} else {1.0 / self.refraction_index};
        let cosine = if dot > 0.0 {self.refraction_index * dot / length(ray.direction)} else {-dot / length(ray.direction)};

        let refracted = refract(ray.direction, outward_normal, ni_over_nt);
        let reflect_prob = if refracted.is_some() { schlick(cosine, self.refraction_index) } else { 1.0 };

        if uniform(random) < reflect_prob {
            Some(MaterialRay::new(
                Ray::new(record.point, reflect(ray.direction, record.normal)),
                Vec3::one(),
            ))
        } else {
            Some(MaterialRay::new(
                Ray::new(record.point, refracted.unwrap()),
                Vec3::one(),
            ))
        }
    }
}

// Polynomial approximation by Christophe Schlick
fn schlick(cosine: f32, refraction_index: f32) -> f32 {
    let r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
    let r0 = r0 * r0;
    r0 + (1.0 - r0) * (1.0 - cosine).powi(5)
}
