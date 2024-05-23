use influxdb::{Client, ReadQuery};
use rocket::{serde::json::Json, State};
use serde::{Deserialize, Serialize};

use crate::{
    lib::env::Config,
    models::{data::Point, device::Device},
};

#[get("/device")]
pub async fn run(env: &State<Config>, db: &State<Client>, device_info: Device) -> String {
    let read_query = ReadQuery::new("SELECT * FROM sensor_data");

    // Deserialize the result into a vec<Point>
    let res = db
        .json_query(read_query)
        .await
        .unwrap()
        .

    res
}
