#[macro_use]
extern crate rocket;

mod routes;
mod models;

#[launch]
fn rocket() -> _ {
    let r = rocket::build();
    routes::mount(r)
}
