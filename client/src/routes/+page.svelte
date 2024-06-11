<script lang="ts">
    import TestChart from "../components/TestChart.svelte";
    import { fetchData } from "$lib/fetch";
    import { onMount } from "svelte";
    import { API_URLS } from "$lib/api";
    import type { SensorData } from "$lib/interfaces/sensorData";


    let data: SensorData[];

    let timePeriod: string;

    $: timePeriod && fetchPeriod();

    const timePeriodOptions: object = {'Last 24H': 'day', 'Last week': 'week', 'Last month': 'month', 'Last year': 'year'}

    const fetchPeriod = async () => {
        console.log((timePeriodOptions[timePeriod]).toUpperCase())
        data = await fetchData(API_URLS.(timePeriodOptions[timePeriod]).toUpperCase());
        console.log(data)
    }

    onMount(() => {
        fetchPeriod();
    });
</script>


<main>
    <TestChart bind:timePeriod {timePeriodOptions} />
</main>