use rocket_validation::Validate;
use serde::{Deserialize, Serialize};

#[derive(Debug, Serialize, Deserialize, Clone, Validate)]
#[serde(crate = "rocket::serde")]
pub struct SensorData {
    // Predefined ID on esp
    pub device_id: i64,

    // Movement score from 0 to 1
    #[validate(range(min = 0.0, max = 1.0))]
    pub movement: f64,

    // Temperature in Celsius, DHT 22 sensor
    #[validate(range(min = -40.0, max = 125.0))]
    pub temperature: f64,

    // Humidity in percentage
    #[validate(range(min = 0.0, max = 100.0))]
    pub humidity: f64,

    // Weight in grams, 5KG load cell
    #[validate(range(min = 0.0, max = 5000.0))]
    pub weight: Option<f64>,

    // Light in lux
    #[validate(range(min = 0))]
    pub light: i64,

    // Sound score
    #[validate(range(min = 0.0, max = 1.0))]
    pub sound: f64,
}