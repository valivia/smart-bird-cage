use rocket::fairing::AdHoc;

use crate::db::Database;

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
