const baseURL = "https://bird.hootsifer.com/api/v1/data";

const currentTimeEpochSeconds = Math.round(Date.now() / 1000);
const secondsInDay = 24 * 60 * 60;
const daysInWeek = 7;
const daysInMonth = 30;
const daysInYear = 365;

export const API_URLS = {
    LAST: `${baseURL}/last`,
    PERIOD: (start: number, end?: number, avg?: boolean) => `${baseURL}?start=${start}${end ? `&end=${end}` : ""}${avg ? "&average=true" : ""}`,
    DAY: `${baseURL}?start=${currentTimeEpochSeconds - secondsInDay}`,
    WEEK: `${baseURL}?start=${currentTimeEpochSeconds - secondsInDay * daysInWeek}&group=day`,
    MONTH: `${baseURL}?start=${currentTimeEpochSeconds - secondsInDay * daysInMonth}&group=day`,
    YEAR: `${baseURL}?start=${currentTimeEpochSeconds - secondsInDay * daysInYear}&group=month`,
};
