use chrono::DateTime;
use rocket::{serde::json::Json, State};

use crate::{
    db::Database,
    models::{data::DataPointAverage, data::Datapoint, device::Device},
};

#[get("/data?<start>&<end>&<average>")]
pub async fn run(
    db: &State<Database>,
    device_info: Device,
    start: Option<i64>,
    end: Option<i64>,
    average: Option<bool>,
) -> Json<Vec<Datapoint>> {
    let end = match end {
        Some(end) => DateTime::from_timestamp(end, 0).unwrap(),
        None => DateTime::<chrono::Utc>::MAX_UTC,
    };

    let average = average.unwrap_or(false);

    let data = match average {
        true => sqlx::query_as!(
            DataPointAverage,
            r#"
                SELECT 
                    time::date,
                    AVG(weight) as weight,
                    AVG(humidity) as humidity,
                    AVG(temperature) as temperature,
                    CAST(AVG(light) AS INTEGER) as light,
                    CAST(AVG(sound) AS INTEGER)  as sound,
                    CAST(AVG(movement) AS INTEGER) as movement

                FROM data
                WHERE device_id = $1 AND time >= $2 AND time <= $3
                GROUP BY time::date
                ORDER BY time::date
            "#,
            device_info.id,
            DateTime::from_timestamp(start.unwrap_or(0), 0).unwrap(),
            end,
        )
        .fetch_all(&db.pool)
        .await
        .unwrap()
        .into_iter()
        .map(|average| average.into())
        .collect(),

        false => sqlx::query_as!(
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
        .unwrap(),
    };

    Json(data)
}
