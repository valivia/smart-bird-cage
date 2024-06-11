<script lang="ts">
    import { onMount } from "svelte";

    import type { SensorData } from "$lib/interfaces/sensorData";
    import { fetchData } from "$lib/fetch";
    import { API_URLS } from "$lib/api";

    import LineChart from "../../components/LineChart.svelte";
    import ColumnChart from "../../components/ColumnChart.svelte";

    const fetchIntervalSeconds = 300;
    const secondsInDay = 24 * 60 * 60;
    const currentTimeEpochSeconds = Date.now() / 1000;

    let data: SensorData[];

    let humidityValues: number[];
    let temperatureValues: number[];
    let lightValues: number[];
    let weightValues: number[];
    let movementValues: object[];
    let soundValues: object[];
    let timeStamps: string[];
    let xaxis: string[];

    const appendLastValue = (lastValue: SensorData) => {
        data = [...data.slice(1), lastValue]
    }

    onMount(() => {
        const initializeData = async () => {
            data = await fetchData(API_URLS.PERIOD(currentTimeEpochSeconds - secondsInDay));

            const interval = setInterval(async () => {
                const lastValue = await fetchData(API_URLS.LAST);
                appendLastValue(lastValue);
            }, fetchIntervalSeconds * 1000);

            return () => clearInterval(interval);
        };

        initializeData().catch(console.error);
    });

    const getElements = (count: number, timeStamps: any[]): any[] => {
        const step = Math.floor((timeStamps.length - 1) / (count - 1));
        const result = Array.from({ length: count }, (_, index) => timeStamps[index * step]);
        return result;
    }

    const dataChanged = () => {
        timeStamps = data.map(({ time }) => new Date(time).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' }));
        humidityValues = data.map(({ humidity }) => humidity);
        temperatureValues = data.map(({ temperature }) => temperature);
        lightValues = data.map(({ light }) => light);
        weightValues = data.map(({ weight }) => weight);
        soundValues = data.map(({ sound }) => sound);
        // soundValues = data.map(({ sound }, index) => ({y: sound, x: timeStamps[index]}));
        movementValues = data.map(({ movement }, index) => ({y: movement, x: timeStamps[index]}));

        xaxis = getElements(6, timeStamps)
    }

    $: data && dataChanged();
</script>

<h1 class="text-white">Last 24 hours</h1>

<main class="grid gap-10 m-20 sm:grid-cols-1 md:grid-cols-2 lg:grid-cols-3">
    <LineChart name='Humidity' unit='%' data={humidityValues} {timeStamps} {xaxis} min={0} max={100} color='#1A56DB'/>
    <LineChart name='Temperature' unit='°C' data={temperatureValues} {timeStamps} {xaxis} color='#f05252' />
    <LineChart name='Light' unit='LUX' data={lightValues} {timeStamps} {xaxis} color='#6af7bb' />
    <LineChart name='Weight' unit='g' data={weightValues} {timeStamps} {xaxis} color='#be8fff' />
    <LineChart name='Sound' unit='x' data={soundValues} {timeStamps} {xaxis} min={0} max={10} color='#5deffe' />

    <!-- <ColumnChart name='Movement' data={movementValues} {timeStamps} {xaxis} color='#fdba3c' /> -->
    <!-- <ColumnChart name='Sound' data={soundValues} {timeStamps} {xaxis} color='#5deffe' /> -->
</main>