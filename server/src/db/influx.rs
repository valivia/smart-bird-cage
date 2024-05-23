use influxdb::{Client, Error};
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

async fn connect() -> Result<Client, Error> {
    let host = std::env::var("db_host").unwrap();
    let client = Client::new(host, "sensor_data");
    client.ping().await?;
    Ok(client)
}
