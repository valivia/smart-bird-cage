
import type { PageLoad } from "./$types";


export const load = (async ({ url }) => {
    const server = `${url.origin}/api/v1/`;
    console.log(`Checking if server is available at ${server}`);
    const response = fetch(server);
    return { response };
}) satisfies PageLoad;
