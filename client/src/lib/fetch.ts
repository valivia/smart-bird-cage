import { env } from "$lib/env";

export async function fetchData(url: string) {
    const response = await fetch(url, {
        method: "GET",
        headers: {
            Authorization: env.AUTHORIZATION,
        },
    });
    return response.json();
}
