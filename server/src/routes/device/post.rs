use influxdb2::Client;
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

    let mut points = vec![
        Point::make_datapoint(&device_info, "light", input.light as f64),
        Point::make_datapoint(&device_info, "temperature", input.temperature),
        Point::make_datapoint(&device_info, "humidity", input.humidity),
        Point::make_datapoint(&device_info, "movement", input.movement),
        Point::make_datapoint(&device_info, "sound", input.sound),
    ];

    if input.weight.is_some() {
        points.push(Point::make_datapoint(
            &device_info,
            "weight",
            input.weight.unwrap(),
        ));
    }

    db.write(&env.db_bucket, stream::iter(points))
        .await
        .unwrap();

    format!("{:?}", input)
}
