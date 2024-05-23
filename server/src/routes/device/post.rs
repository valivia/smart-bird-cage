use rocket::{serde::json::Json, State};
use rocket_validation::Validated;
use sqlx::Executor;

use crate::{db::Database, models::device::Device};

#[post("/device", data = "<input>")]
pub async fn run(db: &State<Database>, input: Validated<Json<Device>>) -> String {
    let input = input.into_inner();

    let query = sqlx::query!(
        r#"
        INSERT INTO device (id, token, bird_name)
        VALUES ($1, $2, $3)
        "#,
        input.id,
        input.token,
        input.bird_name
    );

    db.pool.execute(query).await.unwrap();

    format!("{:?}", input)
}
