use chrono::DateTime;
use rocket::{serde::json::Json, State};

use crate::{
    db::Database,
    models::{data::Datapoint, device::Device},
};

#[get("/data?<start>&<end>")]
pub async fn run(
    db: &State<Database>,
    device_info: Device,
    start: Option<i64>,
    end: Option<i64>,
) -> Json<Vec<Datapoint>> {
    // This is technically a more efficient query but man it's cursed

    // let data = match (start, end) {
    //     (Some(start), Some(end)) => sqlx::query_as!(
    //         Datapoint,
    //         r#"
    //             SELECT time, movement, sound, weight, temperature, humidity, light
    //             FROM data
    //             WHERE device_id = $1 AND time >= $2 AND time <= $3
    //         "#,
    //         device_info.id,
    //         DateTime::from_timestamp(start, 0),
    //         DateTime::from_timestamp(end, 0)
    //     )
    //     .fetch_all(&db.pool)
    //     .await
    //     .unwrap(),
    //     (Some(start), None) => sqlx::query_as!(
    //         Datapoint,
    //         r#"
    //             SELECT time, movement, sound, weight, temperature, humidity, light
    //             FROM data
    //             WHERE device_id = $1 AND time >= $2
    //         "#,
    //         device_info.id,
    //         DateTime::from_timestamp(start, 0)
    //     )
    //     .fetch_all(&db.pool)
    //     .await
    //     .unwrap(),
    //     (None, Some(end)) => sqlx::query_as!(
    //         Datapoint,
    //         r#"
    //             SELECT time, movement, sound, weight, temperature, humidity, light
    //             FROM data
    //             WHERE device_id = $1 AND time <= $2
    //         "#,
    //         device_info.id,
    //         DateTime::from_timestamp(end, 0)
    //     )
    //     .fetch_all(&db.pool)
    //     .await
    //     .unwrap(),
    //     _ => sqlx::query_as!(
    //         Datapoint,
    //         r#"
    //             SELECT time, movement, sound, weight, temperature, humidity, light
    //             FROM data
    //             WHERE device_id = $1
    //         "#,
    //         device_info.id
    //     )
    //     .fetch_all(&db.pool)
    //     .await
    //     .unwrap(),
    // };

    let end = match end {
        Some(end) => DateTime::from_timestamp(end, 0).unwrap(),
        None => DateTime::<chrono::Utc>::MAX_UTC,
    };

    let data = sqlx::query_as!(
        Datapoint,
        r#"
            SELECT time, movement, sound, weight, temperature, humidity, light
            FROM data
            WHERE device_id = $1 AND time >= $2 AND time <= $3
        "#,
        device_info.id,
        DateTime::from_timestamp(start.unwrap_or(0), 0).unwrap(),
        end
    )
    .fetch_all(&db.pool)
    .await
    .unwrap();

    Json(data)
}
