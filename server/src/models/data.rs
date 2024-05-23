use chrono::Utc;
use rocket_validation::Validate;
use serde::{Deserialize, Serialize};
use sqlx::prelude::FromRow;

#[derive(Debug, Serialize, Deserialize, Clone, Validate)]
#[serde(crate = "rocket::serde")]
pub struct SensorData {
    // Predefined ID on esp
    pub device_id: i32,

    // Sound score
    #[validate(range(min = 0.0, max = 1.0))]
    pub sound: f32,

    // Weight in grams, 5KG load cell
    #[validate(range(min = 0.0, max = 5000.0))]
    pub weight: Option<f32>,

    // Amount of movements
    #[validate(range(min = 0))]
    pub movement: i32,

    // Temperature in Celsius, DHT 22 sensor
    #[validate(range(min = -40.0, max = 125.0))]
    pub temperature: f32,

    // Humidity in percentage
    #[validate(range(min = 0.0, max = 100.0))]
    pub humidity: f32,

    // Light in lux
    #[validate(range(min = 0))]
    pub light: i32,
}

#[derive(Debug, FromRow, Serialize, Clone)]
pub struct Datapoint {
    pub time: chrono::DateTime<Utc>,
    pub movement: i32,
    pub sound: f32,
    pub weight: Option<f32>,
    pub temperature: f32,
    pub humidity: f32,
    pub light: i32,
}
