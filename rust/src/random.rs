
use super::vec3::*;

use rand::{Rng, SeedableRng};
use rand_pcg;
use rand::distributions::Uniform;

pub struct Random {
    generator: rand_pcg::Pcg64
}

impl Random {
    pub fn new(seed: u64) -> Random {
        Random{ generator: rand_pcg::Pcg64::seed_from_u64(seed) }
    }
}

pub fn uniform(random: &mut Random) -> f32 {
    let u = Uniform::new::<f32, f32>(0.0, 1.0);
    random.generator.sample(u)
}

pub fn random_in_unit_disk(random: &mut Random) -> Vec3 {
    loop {
        let p = 2.0 * Vec3::new(uniform(random), uniform(random), 0.0) - Vec3::new(1.0, 1.0, 0.0);
        if squared_length(p) < 1.0 {
            return p;
        }
    }
}

pub fn random_in_unit_sphere(random: &mut Random) -> Vec3 {
    loop {
        let p = 2.0 * Vec3::new(uniform(random), uniform(random), uniform(random)) - 1.0;
        if squared_length(p) < 1.0 {
            return p;
        }
    }
}