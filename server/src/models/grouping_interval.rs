pub enum GroupingInterval {
    Hour,
    Day,
    Month,
    Year,
}

impl GroupingInterval {
    pub fn to_sql(&self) -> &'static str {
        match self {
            GroupingInterval::Hour => "date_trunc('hour', time)",
            GroupingInterval::Day => "date_trunc('day', time)",
            GroupingInterval::Month => "date_trunc('month', time)",
            GroupingInterval::Year => "date_trunc('year', time)",
        }
    }

    // From string
    pub fn from_str(s: &str) -> Option<Self> {
        match s {
            "hour" => Some(GroupingInterval::Hour),
            "day" => Some(GroupingInterval::Day),
            "month" => Some(GroupingInterval::Month),
            "year" => Some(GroupingInterval::Year),
            _ => None,
        }
    }
}
