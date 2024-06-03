use rocket::{serde::json::Json, State};
use rocket_validation::Validated;
use sqlx::Executor;

use crate::{
    db::Database,
    models::{data::SensorData, device::Device},
};

#[post("/data", data = "<input>")]
pub async fn run(db: &State<Database>, device_info: Device, input: Validated<Json<SensorData>>) {
    let input = input.into_inner();

    let query = sqlx::query!(
        r#"
        INSERT INTO data (device_id, time, movement, sound, weight, temperature, humidity, light)
        VALUES ($1, $2, $3, $4, $5, $6, $7, $8)
        "#,
        device_info.id,
        chrono::Utc::now(),
        input.movement,
        input.sound,
        input.weight,
        input.temperature,
        input.humidity,
        input.light
    );

    db.pool.execute(query).await.unwrap();
}
