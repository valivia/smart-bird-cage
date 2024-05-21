use influxdb2::{Client, RequestError};
use rocket::fairing::AdHoc;

pub fn init() -> AdHoc {
    AdHoc::on_ignite("Connecting to db", |rocket| async {
        match connect().await {
            Ok(database) => rocket.manage(database),
            Err(error) => {
                panic!("Cannot connect to instance:: {:?}", error)
            }
        }
    })
}

async fn connect() -> Result<Client, RequestError> {
    let host = std::env::var("db_host").unwrap();
    let org = std::env::var("db_org").unwrap();
    let token = std::env::var("db_token").unwrap();
    let client = Client::new(host, org, token);
    client.health().await?;
    Ok(client)
}
