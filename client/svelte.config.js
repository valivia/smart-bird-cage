import adapter from "@sveltejs/adapter-node";
import { vitePreprocess } from "@sveltejs/vite-plugin-svelte";

/** @type {import('@sveltejs/kit').Config} */
const config = {
	preprocess: vitePreprocess({ style: true, script: false }),
	compilerOptions: {
		runes: true,
	},

	kit: {
		adapter: adapter(),
		alias: {
			"styles": "./src/styles",
			"components": "./src/components",
			"lib": "./src/lib",
			"stores": "./src/stores",
			"routes": "./src/routes",
		}
	}
};

export default config;
