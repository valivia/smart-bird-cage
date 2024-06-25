import { PUBLIC_AUTHORIZATION } from "$env/static/public";

export async function fetchData(url: string) {
    const response = await fetch(url, {
        method: "GET",
        headers: {
            Authorization: PUBLIC_AUTHORIZATION,
        },
    });
    return response.json();
}
