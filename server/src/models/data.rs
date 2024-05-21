use chrono::{DateTime, FixedOffset};
use influxdb2::FromDataPoint;
use serde::Serialize;

use super::device::Device;

#[derive(Debug, FromDataPoint, Serialize)]
pub struct Point {
    pub measurement: String,
    pub value: f64,
    pub time: DateTime<FixedOffset>,
}

impl Default for Point {
    fn default() -> Self {
        Self {
            measurement: "unknown".to_string(),
            value: 0.0,
            time: DateTime::parse_from_rfc3339("1970-01-01T00:00:00Z").unwrap(),
        }
    }
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