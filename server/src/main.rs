#[macro_use]
extern crate rocket;

use dotenv::dotenv;
use lib::env::validate_env;
use rocket::serde::json::{json, Value};

mod db;
mod lib;
mod request_guards;
mod models;
mod routes;

#[catch(404)]
fn not_found() -> Value {
    json!({
        "status": 404,
        "message": "Resource was not found."
    })
}

#[launch]
fn rocket() -> _ {
    dotenv().ok();
    let env = validate_env();
    let r = rocket::build()
        .manage(env)
        .attach(db::initialize())
        .register("/", catchers![not_found])
        .register("/", catchers![rocket_validation::validation_catcher]);

    routes::mount(r)
}
