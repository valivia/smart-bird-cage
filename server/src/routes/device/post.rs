use rocket::serde::json::Json;
use rocket_validation::Validated;

use crate::models::data::SensorData;

#[post("/device", data = "<input>")]
pub async fn post(input: Validated<Json<SensorData>>) -> String {
    println!("Received: {:?}", input);
    "Hello, device!".to_string()
}
