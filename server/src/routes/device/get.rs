use influxdb2::{models::Query, Client};
use rocket::{serde::json::Json, State};

use crate::{
    lib::env::Config,
    models::{data::Point, device::Device},
};

#[get("/device")]
pub async fn run(env: &State<Config>, db: &State<Client>, device_info: Device) -> Json<Vec<Point>> {
    let qs = format!(
        "from(bucket: \"{}\") 
        |> range(start: -1w)
        |> filter(fn: (r) => r.device_id == \"{}\")
    ",
        env.db_bucket, device_info.id
    );

    let query = Query::new(qs.to_string());
    let res: Vec<Point> = db.query::<Point>(Some(query)).await.unwrap();

    Json(res)
}
