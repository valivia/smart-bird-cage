<script lang="ts">
    import HumidityChart from "../components/HumidityChart.svelte";
    import TemperatureChart from "../components/TemperatureChart.svelte";
    import LightChart from "../components/LightChart.svelte";
    import SoundChart from "../components/SoundChart.svelte";
    import MovementChart from "../components/MovementChart.svelte";
    import WeightChart from "../components/WeightChart.svelte";

    import { onMount } from "svelte";

    interface SensorData{
        humidity: number;
        temperature: number;
        light: number;
        weight: number;
        movement: number;
        sound: number;
        time: string;
    }

    const baseURL = 'https://bird.hootsifer.com/api/v1/data';
    const fetchIntervalSeconds = 300;
    const secondsInDay = 24 * 60 * 60;
    const currentTimeEpochSeconds = Date.now() / 1000;

    let timeSeriesURL = `${baseURL}?start=${currentTimeEpochSeconds - secondsInDay}`
    let lastValueURL = `${baseURL}/last`

    let data: SensorData[];

    let humidityValues: number[];
    let temperatureValues: number[];
    let lightValues: number[];
    let weightValues: number[];
    let movementValues: number[];
    let soundValues: number[];
    let timeStamps: string[];
    let xaxis: string[];

    const fetchData = async (url: string) => {
        const response = await fetch(url, {
            method: "GET",
            headers: {
                "Authorization": "beepboopbeepbeep",
            },
        });
        return response.json();
    };

    const appendLastValue = (lastValue: SensorData) => {
        data = [...data.slice(1), lastValue]
    }

    onMount(() => {
        const initializeData = async () => {
            data = await fetchData(timeSeriesURL);

            const interval = setInterval(async () => {
                const lastValue = await fetchData(lastValueURL);
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
        humidityValues = data.map(({ humidity }) => humidity);
        temperatureValues = data.map(({ temperature }) => temperature);
        lightValues = data.map(({ light }) => light);
        weightValues = data.map(({ weight }) => weight);
        movementValues = data.map(({ movement }) => movement);
        soundValues = data.map(({ sound }) => sound);
        timeStamps = data.map(({ time }) => new Date(time).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' }));

        xaxis = getElements(6, timeStamps)
    }

    $: data && dataChanged();
</script>

<h1 class="text-white">Afgelopen 24 uur</h1>

<main class="grid gap-10 m-20 sm:grid-cols-1 md:grid-cols-2 lg:grid-cols-3">
    <HumidityChart values={humidityValues} {timeStamps} {xaxis}/>
    <TemperatureChart values={(temperatureValues)} {timeStamps} {xaxis}  />
    <LightChart values={lightValues} {timeStamps} {xaxis} />
    <WeightChart values={weightValues} {timeStamps} {xaxis} />
    <MovementChart values={movementValues} {timeStamps}  />
    <SoundChart values={soundValues} {timeStamps} />
</main>