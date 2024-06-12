use chrono::Utc;
use rocket_validation::Validate;
use serde::{Deserialize, Serialize};
use sqlx::prelude::FromRow;

#[derive(Debug, Serialize, Deserialize, Clone, Validate)]
#[serde(crate = "rocket::serde")]
pub struct SensorData {
    // Predefined ID on esp
    pub device_id: i32,

    // Weight in grams, 5KG load cell
    #[validate(range(min = 0.0, max = 5000.0))]
    pub weight: Option<f32>,

    // Amount of movements
    #[validate(range(min = 0))]
    pub movement: i32,

    // Amount of sounds measured
    #[validate(range(min = 0))]
    pub sound: i32,

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
    pub sound: i32,
    pub weight: Option<f32>,
    pub temperature: f32,
    pub humidity: f32,
    pub light: i32,
}

#[derive(Debug, FromRow, Clone)]
pub struct DataPointAverage {
    pub time: Option<chrono::DateTime<Utc>>,
    pub movement: Option<i32>,
    pub sound: Option<i32>,
    pub weight: Option<f64>,
    pub temperature: Option<f64>,
    pub humidity: Option<f64>,
    pub light: Option<i32>,
}

// Convert from datapointaverage to datapoint
impl From<DataPointAverage> for Datapoint {
    fn from(average: DataPointAverage) -> Self {
        Datapoint {
            time: average.time.unwrap(),
            movement: average.movement.unwrap_or(0),
            sound: average.sound.unwrap_or(0),
            weight: average
                .weight
                .map(|w| w as f32)
                .map(|w| (w * 10.0).round() / 10.0),
            temperature: (average.temperature.unwrap_or(0.0) as f32 * 10.0).round() / 10.0,
            humidity: (average.humidity.unwrap_or(0.0) as f32 * 10.0).round() / 10.0,
            light: average.light.unwrap_or(0),
        }
    }
}
