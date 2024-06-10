<script lang="ts">
    import { onMount } from "svelte";

    import type { SensorData } from "$lib/interfaces/sensorData";
    import { fetchData } from "$lib/fetch";
    import { API_URLS } from "$lib/api";

    import LineChart from "../../components/LineChart.svelte";
    import ColumnChart from "../../components/ColumnChart.svelte";

    let data: SensorData[];

    let humidityValues: object[];
    let temperatureValues: number[];
    let lightValues: number[];
    let weightValues: number[];
    let movementValues: object[];
    let soundValues: object[];
    let timeStamps: string[];
    let xaxis: string[];
    
    const today = new Date();
    today.setHours(0, 0, 0, 0);

    const startOfDayEpoch = Math.floor(today.getTime() / 1000);

    const days = ['mon', 'tue', 'wen', 'thu', 'fri', 'sat', 'sun']

    onMount(() => {
        const initializeData = async () => {
            data = await fetchData(API_URLS.PERIOD((startOfDayEpoch - 86400 * 7), startOfDayEpoch, true));
        };

        initializeData().catch(console.error);
    });

    const getElements = (count: number, timeStamps: any[]): any[] => {
        const step = Math.floor((timeStamps.length - 1) / (count - 1));
        const result = Array.from({ length: count }, (_, index) => timeStamps[index * step]);
        return result;
    }

    const dataChanged = () => {
        timeStamps = data.map(({ time }) => days[new Date(time).getDay()]);
        humidityValues = data.map(({ humidity }, index) => ({y: humidity.toPrecision(3), x: timeStamps[index]}));
        temperatureValues = data.map(({ temperature }) => temperature);
        lightValues = data.map(({ light }) => light);
        weightValues = data.map(({ weight }) => weight);
        
        soundValues = data.map(({ sound }, index) => ({y: sound, x: timeStamps[index]}));
        movementValues = data.map(({ movement }, index) => ({y: movement, x: timeStamps[index]}));

        xaxis = getElements(6, timeStamps)
    }

    $: data && dataChanged();

</script>

<h1 class="text-white">Last week</h1>

<main class="grid gap-10 m-20 sm:grid-cols-1 md:grid-cols-2 lg:grid-cols-3">
    <ColumnChart name='Humidity' unit='%' data={humidityValues} {timeStamps} min={0} max={100} color='#1A56DB'/>
    <!-- <LineChart name='Temperature' unit='°C' data={temperatureValues} {timeStamps} {xaxis} color='#f05252' />
    <LineChart name='Light' unit='LUX' data={lightValues} {timeStamps} {xaxis} color='#6af7bb' />
    <LineChart name='Weight' unit='g' data={weightValues} {timeStamps} {xaxis} color='#be8fff' />
    <ColumnChart name='Movement' data={movementValues} {timeStamps} {xaxis} color='#fdba3c' />
    <ColumnChart name='Sound' data={soundValues} {timeStamps} {xaxis} color='#5deffe' /> -->
</main>