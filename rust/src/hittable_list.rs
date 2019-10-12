use super::hit_record::HitRecord;
use super::hittable::Hittable;
use super::ray::Ray;

pub struct HittableList {
    list: Vec<Box<dyn Hittable>>,
}

impl HittableList {
    pub fn new() -> HittableList {
        HittableList {list: Vec::new()}
    }

    pub fn add(&mut self, hittable: Box<dyn Hittable>) {
        self.list.push(hittable);
    }
}

impl Hittable for HittableList {
    fn hit(&self, ray: &Ray, t_min: f32, t_max: f32) -> Option<HitRecord> {
        let mut record: Option<HitRecord> = None;
        let mut t: f32 = t_max;

        for hittable in self.list.iter() {
            if let Some(hit) = hittable.hit(ray, t_min, t) {
                t = hit.t;
                record = Some(hit);
            } 
        }

        record
    }
}