use rocket::{Build, Rocket};

pub mod data;
pub mod device;

pub fn mount(rocket: Rocket<Build>) -> Rocket<Build> {
    let routes = routes![
        data::post::run,
        data::get::run,
        device::post::run,
        device::get::run
    ];

    rocket.mount("/", routes)
}
