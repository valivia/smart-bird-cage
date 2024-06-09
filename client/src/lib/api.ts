const baseURL = "https://bird.hootsifer.com/api/v1/data";

export const API_URLS = {
    LAST: `${baseURL}/last`,
    PERIOD: (start: number, end?: number, avg?: boolean) => `${baseURL}?start=${start}${end ? `&end=${end}` : ""}${avg ? "&average=true" : ""}`,
};
