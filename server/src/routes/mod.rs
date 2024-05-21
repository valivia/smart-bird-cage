use rocket::{serde::json::Json, Build, Rocket, State};
use sled::Db;

use crate::{
    db::devices::{get_device_info, save_device_info},
    models::device::Device,
};
pub mod device;

#[get("/")]
pub async fn index(state: &State<Db>) -> Json<Device> {
    let device = Device {
        id: 2,
        token: "beepboop".to_string(),
        bird_name: "Tibu".to_string(),
    };

    save_device_info(state, &device);
    let result = get_device_info(state, device.id).unwrap();

    Json(result)
}

pub fn mount(rocket: Rocket<Build>) -> Rocket<Build> {
    let routes = routes![device::post::run, device::get::run, index];

    rocket.mount("/", routes)
}
