use sqlx::{postgres::PgPoolOptions, Error, Pool, Postgres};

pub struct Database {
    pub pool: Pool<Postgres>,
}

// Database  constructor that makes a new connection to the database
impl Database {
    pub async fn new() -> Result<Self, Error> {
        let host = std::env::var("DATABASE_URL").unwrap();
        let pool = PgPoolOptions::new()
            .max_connections(5)
            .connect(&host)
            .await?;
        Ok(Self { pool })
    }
}
