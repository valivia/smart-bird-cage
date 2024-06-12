use rocket::http::Status;
use rocket::request::{FromRequest, Outcome, Request};
use rocket::State;

use crate::db::Database;
use crate::models::device::Device;

#[derive(Debug)]
pub enum AuthError {
    Missing,
    Invalid,
    Server,
}

#[rocket::async_trait]
impl<'r> FromRequest<'r> for Device {
    type Error = AuthError;

    async fn from_request(req: &'r Request<'_>) -> Outcome<Self, Self::Error> {
        let token = match req.headers().get_one("Authorization") {
            Some(token) => token,
            None => return Outcome::Error((Status::Unauthorized, AuthError::Missing)),
        };

        let db = match req.guard::<&State<Database>>().await {
            Outcome::Success(devices) => devices,
            _ => return Outcome::Error((Status::InternalServerError, AuthError::Server)),
        };

        match sqlx::query_as!(
            Device,
            r#"
                    SELECT *
                    FROM device
                    WHERE token = $1
                    "#,
            token
        )
        .fetch_one(&db.pool)
        .await
        {
            Ok(device) => Outcome::Success(device),
            _ => Outcome::Error((Status::Unauthorized, AuthError::Invalid)),
        }
    }
}
