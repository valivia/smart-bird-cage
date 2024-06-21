<script lang="ts">
    import CustomChart from "../components/CustomChart.svelte";
    import { fetchData } from "$lib/fetch";
    import { onMount } from "svelte";
    import { API_URLS } from "$lib/api";
    import type { sensorData } from "$lib/interfaces/sensorData";
    import { Button, Dropdown, DropdownItem } from "flowbite-svelte";
    import { ChevronDownOutline } from "flowbite-svelte-icons";

    const fetchIntervalSeconds = 300;
    let data: sensorData[];

    const fetchPeriods = async () => {
        data = await fetchData(API_URLS.DAY);

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
    
    let timePeriod: string = 'Last month'
    let dropdownOpen = false;
    const timePeriodOptions: object = {'Last 24H': 'day', 'Last week': 'week', 'Last month': 'month', 'Last year': 'year'}
    const timePeriodOptionsValues = Object.entries(timePeriodOptions).map(([k]) => `${k}`);

    const setTimePeriod = (e: Event) => {
        e.preventDefault();
        let event = e.target as HTMLElement
        timePeriod = event?.innerHTML;
        dropdownOpen = false;
    };
</script>

<main class="m-5 lg:m-20">
    <div class="flex items-center justify-between my-5">
        <h1 class="text-5xl text-white">Bird Health</h1>
        <Button class="inline-flex items-center py-0 text-sm font-medium text-center text-gray-500 bg-transparent dark:text-gray-400 hover:text-gray-900 dark:hover:text-white hover:bg-transparent dark:bg-transparent dark:hover:bg-transparent focus:ring-transparent dark:focus:ring-transparent">{timePeriod}<ChevronDownOutline class="w-2.5 m-2.5 ms-1.5" /></Button>
        <Dropdown classContainer='dark:bg-[#374151] dark:text-white overflow-hidden' class="w-40" offset={-6} bind:open={dropdownOpen}>
            {#each timePeriodOptionsValues as option, i}
                <DropdownItem defaultClass="dark:hover:bg-[#4b5563] w-full text-left px-2 first:pt-1 last:pb-1" on:click={setTimePeriod}>{option}</DropdownItem>
            {/each}
        </Dropdown>
    </div>
    <div class="flex justify-center w-full">
        <div class="grid gap-10 grid-cols-1 md:grid-cols-2 lg:grid-cols-3 max-w-[2560px] w-full">
            <CustomChart name='Humidity' description={humidityDescription} unit='%' data={humidityValues} {timeStamps} {timePeriod} min={0} max={100} color='#1A56DB' />
            <CustomChart name='Temperature' unit='°C' data={temperatureValues} {timeStamps} {timePeriod}  color='#f05252' />
            <CustomChart name='Light' unit='LUX' data={lightValues} {timeStamps} {timePeriod} color='#6af7bb'  />
            <CustomChart name='Weight' unit='g' data={weightValues} {timeStamps} {timePeriod} color='#be8fff'  />
            <CustomChart name='Movement' unit='amount' data={movementValues} {timeStamps} {timePeriod} color='#fdba3c' />
            <CustomChart name='Sound' unit='amount' data={soundValues} {timeStamps} {timePeriod} color='#5deffe' />
        </div>
    </div>
</main>