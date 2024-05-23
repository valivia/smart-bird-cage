use chrono::DateTime;
use influxdb::InfluxDbWriteable;
use serde::{Deserialize, Serialize};

use super::device::Device;

#[derive(Debug, Serialize, Deserialize, InfluxDbWriteable)]
pub struct Point {
    pub time: DateTime<chrono::Utc>,

    #[influxdb(tag)]
    pub device_id: i64,

    pub weight: Option<f64>,
    pub sound: f64,
    pub movement: f64,
    pub temperature: f64,
    pub humidity: f64,
    pub light: i64,
}

impl Point {
    pub fn make_datapoint(device: &Device, name: &str, value: f64) -> influxdb2::models::DataPoint {
        influxdb2::models::DataPoint::builder(name)
            .field("value", value)
            .tag("device_id", device.id.to_string())
            .build()
            .unwrap()
    }
}
