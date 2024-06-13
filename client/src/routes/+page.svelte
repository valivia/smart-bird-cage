<script lang="ts">
    import CustomChart from "../components/CustomChart.svelte";
    import { fetchData } from "$lib/fetch";
    import { onMount } from "svelte";
    import { API_URLS } from "$lib/api";
    import type { SensorData } from "$lib/interfaces/sensorData";
    import { Button, Dropdown, DropdownItem } from "flowbite-svelte";
    import { ChevronDownOutline } from "flowbite-svelte-icons";

    const fetchIntervalSeconds = 300;
    let data: SensorData[];

    const fetchPeriods = async () => {
        data = await fetchData(API_URLS.DAY);
        data = data.slice(-288)

        console.log(data)

        const interval = setInterval(async () => {
            const lastValue = await fetchData(API_URLS.LAST);
            data = [...data.slice(1), lastValue]
        }, fetchIntervalSeconds * 1000);

        return () => clearInterval(interval);
    }

    let timeStamps: string[];
    let humidityValues: number[];
    let temperatureValues: number[];
    let lightValues: number[];
    let weightValues: number[];
    let movementValues: number[];
    let soundValues: number[];

    let humidityDescription: string = 'Report helps navigate cumulative growth of community activities. Ideally, the chart should have a growing trend, as stagnating chart signifies a significant decrease of community activity.'

    const mapData = () => {
        timeStamps = data.map(({ time }) => new Date(time).toLocaleTimeString('en-US', {hour: '2-digit', minute: '2-digit', hour12: false}));
        humidityValues = data.map(({ humidity }) => humidity);
        temperatureValues = data.map(({ temperature }) => temperature);
        lightValues = data.map(({ light }) => light);
        weightValues = data.map(({ weight }) => weight);
        soundValues = data.map(({ sound }) => sound);
        movementValues = data.map(({ movement }) => movement);
    }

    $: data && mapData();

    onMount(() => {
        fetchPeriods();
    })
    
    let timePeriod: string = 'Last 24H'
    let dropdownOpen = false;
    const timePeriodOptions: object = {'Last 24H': 'day', 'Last week': 'week', 'Last month': 'month', 'Last year': 'year'}
    const timePeriodOptionsValues = Object.entries(timePeriodOptions).map(([k]) => `${k}`);

    const setTimePeriod = (e: Event) => {
        e.preventDefault();
        timePeriod = e.target.innerHTML;
        dropdownOpen = false;
    };
</script>

<main class="grid gap-10 m-20 sm:grid-cols-1 md:grid-cols-2 lg:grid-cols-3">
    <CustomChart name='Humidity' description={humidityDescription} unit='%' data={humidityValues} {timeStamps} {timePeriod} min={0} max={100} color='#1A56DB' />
    <CustomChart name='Temperature' unit='°C' data={temperatureValues} {timeStamps} {timePeriod}  color='#f05252' />
    <CustomChart name='Light' unit='LUX' data={lightValues} {timeStamps} {timePeriod} color='#6af7bb'  />
    <CustomChart name='Weight' unit='g' data={weightValues} {timeStamps} {timePeriod} color='#be8fff'  />
    <CustomChart name='Movement' data={movementValues} {timeStamps} {timePeriod} color='#fdba3c' />
    <CustomChart name='Sound' data={soundValues} {timeStamps} {timePeriod} color='#5deffe' />

    <div class="flex items-center justify-between pt-5">
        <Button class="inline-flex items-center py-0 text-sm font-medium text-center text-gray-500 bg-transparent dark:text-gray-400 hover:text-gray-900 dark:hover:text-white hover:bg-transparent dark:bg-transparent dark:hover:bg-transparent focus:ring-transparent dark:focus:ring-transparent">{timePeriod}<ChevronDownOutline class="w-2.5 m-2.5 ms-1.5" /></Button>
        <Dropdown class="w-40" offset={-6} bind:open={dropdownOpen}>
        {#each timePeriodOptionsValues as option, i}
            <DropdownItem on:click={setTimePeriod}>{option}</DropdownItem>
        {/each}
        </Dropdown>
    </div>
</main>