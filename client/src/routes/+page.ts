
import type { PageLoad } from "./$types";


export const load = (async () => {
    const response = await fetch("http://localhost:82/api/v1/");
    return { canConnect: response.status === 200 };
}) satisfies PageLoad;
