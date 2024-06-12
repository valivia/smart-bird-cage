use serde::Deserialize;

#[derive(Deserialize, Debug)]
#[serde(rename_all = "lowercase")]
pub struct Config {
    // Database.
    pub database_url: String,

    // Module urls.
    pub origin: String,
}

pub fn validate_env() -> Config {
    match envy::from_env::<Config>() {
        Ok(data) => data,
        Err(error) => {
            eprintln!("{:#?}", error);
            panic!();
        }
    }
}
