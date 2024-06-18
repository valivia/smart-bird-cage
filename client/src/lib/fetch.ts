export async function fetchData(url: string) {
    const response = await fetch(url, {
        method: "GET",
        headers: {
            Authorization: import.meta.env.VITE_AUTHORIZATION,
        },
    });
    return response.json();
}
