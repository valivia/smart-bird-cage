use rocket::{serde::json::Json, State};

use crate::{
    db::Database,
    models::{data::Datapoint, device::Device},
};

#[get("/data")]
pub async fn run(db: &State<Database>, device_info: Device) -> Json<Vec<Datapoint>> {
    let data = sqlx::query_as!(
        Datapoint,
        r#"
        SELECT time, movement, sound, weight, temperature, humidity, light
        FROM data
        WHERE device_id = $1
        "#,
        device_info.id
    )
    .fetch_all(&db.pool)
    .await
    .unwrap();

    Json(data)
}
