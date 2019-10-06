use core::ops::*;

#[derive(Clone, Copy)]
pub struct Vec3 {
    x: f32,
    y: f32,
    z: f32
}

impl Vec3 {
    pub fn new(x: f32, y: f32, z: f32) -> Vec3 {
        Vec3 {x, y, z}
    }

    pub fn zero() -> Vec3 {
        Vec3 {x: 0.0, y: 0.0, z: 0.0}
    }

    pub fn x(&self) -> f32 {
        self.x
    }

    pub fn y(&self) -> f32 {
        self.y
    }

    pub fn z(&self) -> f32 {
        self.z
    }
}

// Add
impl Add for Vec3 {
    type Output = Vec3;
    fn add(self, other: Vec3) -> Vec3 {
        Vec3::new(self.x + other.x, self.y + other.y, self.z + other.z)
    }
}

impl Add<Vec3> for f32 {
    type Output = Vec3;
    fn add(self, other: Vec3) -> Vec3 {
        Vec3::new(self + other.x, self + other.y, self + other.z)
    }
}

impl Add<f32> for Vec3 {
    type Output = Vec3;
    fn add(self, other: f32) -> Vec3 {
        Vec3::new(self.x + other, self.y + other, self.z + other)
    }
}

// Sub
impl Sub for Vec3 {
    type Output = Vec3;
    fn sub(self, other: Vec3) -> Vec3 {
        Vec3::new(self.x - other.x, self.y - other.y, self.z - other.z)
    }
}

impl Sub<Vec3> for f32 {
    type Output = Vec3;
    fn sub(self, other: Vec3) -> Vec3 {
        Vec3::new(self - other.x, self - other.y, self - other.z)
    }
}

impl Sub<f32> for Vec3 {
    type Output = Vec3;
    fn sub(self, other: f32) -> Vec3 {
        Vec3::new(self.x - other, self.y - other, self.z - other)
    }
}

// Mul
impl Mul for Vec3 {
    type Output = Vec3;
    fn mul(self, other: Vec3) -> Vec3 {
        Vec3::new(self.x * other.x, self.y * other.y, self.z * other.z)
    }
}

impl Mul<Vec3> for f32 {
    type Output = Vec3;
    fn mul(self, other: Vec3) -> Vec3 {
        Vec3::new(self * other.x, self * other.y, self * other.z)
    }
}

impl Mul<f32> for Vec3 {
    type Output = Vec3;
    fn mul(self, other: f32) -> Vec3 {
        Vec3::new(self.x * other, self.y * other, self.z * other)
    }
}

// Div
impl Div for Vec3 {
    type Output = Vec3;
    fn div(self, other: Vec3) -> Vec3 {
        Vec3::new(self.x / other.x, self.y / other.y, self.z / other.z)
    }
}

impl Div<Vec3> for f32 {
    type Output = Vec3;
    fn div(self, other: Vec3) -> Vec3 {
        Vec3::new(self / other.x, self / other.y, self / other.z)
    }
}

impl Div<f32> for Vec3 {
    type Output = Vec3;
    fn div(self, other: f32) -> Vec3 {
        Vec3::new(self.x / other, self.y / other, self.z / other)
    }
}

pub fn cross(l: Vec3, r: Vec3) -> Vec3 {
    Vec3::new(
		l.y*r.z - l.z*r.y,
		l.z*r.x - l.x*r.z,
		l.x*r.y - l.y*r.x
    )
}

pub fn dot(l: Vec3, r: Vec3) -> f32 {
    l.x*r.x + l.y*r.y + l.z*r.z
}

pub fn squared_length(v: Vec3) -> f32 {
    dot(v, v)
}

pub fn length(v: Vec3) -> f32 {
    squared_length(v).sqrt()
}

pub fn unit_vector(v: Vec3) -> Vec3 {
    v / length(v)
}
