use rocket::serde::json::Json;

use crate::models::device::Device;

#[get("/device")]
pub async fn run(device_info: Device) -> Json<Device> {
    Json(device_info)
}
