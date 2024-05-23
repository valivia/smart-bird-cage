use rocket::fairing::AdHoc;
use sqlx::{postgres::PgPoolOptions, Error, Pool, Postgres};

pub fn initialize() -> AdHoc {
    AdHoc::on_ignite("Connecting to db", |rocket| async {
        match Database::new().await {
            Ok(database) => rocket.manage(database),
            Err(error) => {
                panic!("Cannot connect to instance:: {:?}", error)
            }
        }
    })
}

pub struct Database {
    pub pool: Pool<Postgres>,
}

// Database  constructor that makes a new connection to the database
impl Database {
    pub async fn new() -> Result<Self, Error> {
        let host = std::env::var("DATABASE_URL").unwrap();
        let pool = PgPoolOptions::new().max_connections(5).connect(&host).await?;
        Ok(Self { pool })
    }
}
