use serde::Deserialize;

#[derive(Deserialize, Debug)]
#[serde(rename_all = "lowercase")]
pub struct Config {
    // Database.
    pub db_host: String,
    pub db_org: String,
    pub db_token: String,
    pub db_bucket: String,

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
