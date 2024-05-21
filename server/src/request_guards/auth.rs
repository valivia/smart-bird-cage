use rocket::http::Status;
use rocket::request::{FromRequest, Outcome, Request};
use rocket::State;
use sled::Db;

use crate::db::devices::get_device_info;
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
        let mut headers = req.headers().get("Authorization");

        // Extract and parse the ID
        let id = headers
            .next()
            .and_then(|auth| auth.parse::<i64>().ok())
            .ok_or(AuthError::Missing)
            .and_then(|id| {
                if id > 0 {
                    Ok(id)
                } else {
                    Err(AuthError::Invalid)
                }
            });

        // Extract the token
        let token = headers.next().ok_or(AuthError::Missing);

        match (id, token) {
            (Ok(id), Ok(token)) => {
                let devices = match req.guard::<&State<Db>>().await {
                    Outcome::Success(devices) => devices,
                    _ => return Outcome::Error((Status::InternalServerError, AuthError::Server)),
                };

                match get_device_info(devices, id) {
                    Some(device) if device.token == token => Outcome::Success(device),
                    _ => Outcome::Error((Status::Unauthorized, AuthError::Invalid)),
                }
            }
            (Err(AuthError::Missing), _) | (_, Err(AuthError::Missing)) => {
                Outcome::Error((Status::Unauthorized, AuthError::Missing))
            }
            _ => Outcome::Error((Status::Unauthorized, AuthError::Invalid)),
        }
    }
}
