use chrono::DateTime;
use rocket::{serde::json::Json, State};

use crate::{
    db::Database,
    models::{
        data::{DataPointAverage, Datapoint},
        device::Device,
        grouping_interval::GroupingInterval,
    },
};

#[get("/data?<start>&<end>&<group>")]
pub async fn get(
    db: &State<Database>,
    device_info: Device,
    start: Option<i64>,
    end: Option<i64>,
    group: Option<&str>,
) -> Json<Vec<Datapoint>> {
    let end = match end {
        Some(end) => DateTime::from_timestamp(end, 0).unwrap(),
        None => DateTime::<chrono::Utc>::MAX_UTC,
    };

    let group = group.and_then(GroupingInterval::from_str);

    let data = match group {
        Some(grouping_interval) => {
            let query = format!(
                r#"
                    SELECT 
                        {interval_str} AS time,
                        AVG(weight) as weight,
                        AVG(humidity) as humidity,
                        AVG(temperature) as temperature,
                        CAST(AVG(light) AS INTEGER) as light,
                        CAST(AVG(sound) AS INTEGER)  as sound,
                        CAST(AVG(movement) AS INTEGER) as movement
        
                    FROM data
                    WHERE device_id = $1 AND time >= $2 AND time <= $3
                    GROUP BY {interval_str}
                    ORDER BY {interval_str}
                "#,
                interval_str = grouping_interval.to_sql(),
            );

            sqlx::query_as(&query)
                .bind(device_info.id)
                .bind(DateTime::from_timestamp(start.unwrap_or(0), 0).unwrap())
                .bind(end)
                .fetch_all(&db.pool)
                .await
                .unwrap()
                .into_iter()
                .map(|average: DataPointAverage| average.into())
                .collect()
        }
        None => sqlx::query_as!(
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

#[get("/data/last")]
pub async fn get_last(db: &State<Database>, device_info: Device) -> Json<Datapoint> {
    let data = sqlx::query_as!(
        Datapoint,
        r#"
            SELECT time, movement, sound, weight, temperature, humidity, light
            FROM data
            WHERE device_id = $1
            ORDER BY time DESC
            LIMIT 1
        "#,
        device_info.id,
    )
    .fetch_one(&db.pool)
    .await
    .unwrap();

    Json(data)
}
