use serde::{Deserialize, Serialize};
use sqlx::prelude::FromRow;
use validator::Validate;

#[derive(Debug, Serialize, Deserialize, FromRow, Clone, Validate)]
pub struct Device {
    pub id: i32,
    pub token: String,
    pub bird_name: Option<String>,
}
