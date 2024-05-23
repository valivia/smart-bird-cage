use influxdb::{Client, InfluxDbWriteable};
use rocket::{futures::stream, serde::json::Json, State};
use rocket_validation::Validated;

use crate::{
    lib::env::Config,
    models::{data::Point, device::Device, sensor::SensorData},
};

#[post("/device", data = "<input>")]
pub async fn run(
    env: &State<Config>,
    db: &State<Client>,
    device_info: Device,
    input: Validated<Json<SensorData>>,
) -> String {
    println!("Received: {:?}", input);
    let input = input.into_inner();
    println!("Device: {:?}", device_info);

    db.query(
        Point {
            time: chrono::Utc::now(),
            device_id: device_info.id,
            weight: input.weight,
            sound: input.sound,
            movement: input.movement,
            temperature: input.temperature,
            humidity: input.humidity,
            light: input.light,
        }
        .into_query("sensor_data"),
    ).await.unwrap();

    format!("{:?}", input)
}
