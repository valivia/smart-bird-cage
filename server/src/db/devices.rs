use rocket::fairing::AdHoc;
use sled::Db;

use crate::models::device::Device;

pub fn init() -> AdHoc {
    AdHoc::on_ignite("Loading device info...", |rocket| async {
        match sled::open("../database/devices") {
            Ok(database) => rocket.manage(database),
            Err(error) => {
                panic!("Cannot load device info:: {:?}", error)
            }
        }
    })
}

pub fn save_device_info(db: &Db, device: &Device) {
    let tree = db.open_tree("device").unwrap();
    let bytes = bincode::serialize(device).unwrap();
    tree.insert(device.id.to_be_bytes(), bytes).unwrap();
}

pub fn get_device_info(db: &Db, id: i64) -> Option<Device> {
    let tree = db.open_tree("device").unwrap();
    match tree.get(id.to_be_bytes()) {
        Ok(Some(bytes)) => {
            let device: Device = bincode::deserialize(&bytes).unwrap();
            Some(device)
        }
        _ => None,
    }
}
