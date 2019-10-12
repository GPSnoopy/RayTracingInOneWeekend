use super::hit_record::HitRecord;
use super::random::Random;
use super::ray::Ray;
use super::vec3::*;

pub struct MaterialRay {
    pub scattered: Ray,
    pub attenuation: Vec3,
}

impl MaterialRay {
    pub fn new(scattered: Ray, attenuation: Vec3) -> MaterialRay {
        MaterialRay {
            scattered,
            attenuation,
        }
    }
}

pub trait Material: Send + Sync {
    fn scatter(&self, ray: &Ray, hit: &HitRecord, random: &mut Random) -> Option<MaterialRay>;
}

pub fn reflect(v: Vec3, normal: Vec3) -> Vec3 {
    v - 2.0 * dot(v, normal) * normal
}

pub fn refract(v: Vec3, normal: Vec3, ni_over_nt: f32) -> Option<Vec3> {
    let uv = unit_vector(v);
    let dt = dot(uv, normal);
    let discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.0 - dt * dt);

    if discriminant > 0.0 {
        Some(ni_over_nt * (uv - normal * dt) - normal * discriminant.sqrt())
    } else {
        None
    }
}
