use rocket::{Build, Rocket};
pub mod device;

pub fn mount(rocket: Rocket<Build>) -> Rocket<Build> {
    let routes = routes![device::post::post];

    rocket.mount("/", routes)
}
