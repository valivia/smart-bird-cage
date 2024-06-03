#[macro_use]
extern crate rocket;

use dotenv::dotenv;
use rocket::serde::json::{json, Value};
use utils::env::validate_env;

mod db;
mod fairings;
mod models;
mod request_guards;
mod routes;
mod utils;

#[catch(404)]
fn not_found() -> Value {
    json!({
        "status": 404,
        "message": "Resource was not found."
    })
}

#[options("/<_..>")]
fn all_options() {
    /* Intentionally left empty */
}

#[launch]
fn rocket() -> _ {
    dotenv().ok();
    let env = validate_env();
    let r = rocket::build()
        .manage(env)
        .attach(fairings::db::initialize())
        .attach(fairings::cors::Cors)
        .mount("/", routes![all_options])
        .register("/", catchers![not_found])
        .register("/", catchers![rocket_validation::validation_catcher]);

    routes::mount(r)
}
