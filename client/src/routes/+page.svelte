<script lang="ts">
    import HumidityChart from "../components/HumidityChart.svelte";
    import TemperatureChart from "../components/TemperatureChart.svelte";
    import LightChart from "../components/LightChart.svelte";
    import SoundChart from "../components/SoundChart.svelte";
    import MovementChart from "../components/MovementChart.svelte";
    import WeightChart from "../components/WeightChart.svelte";

    import { onMount } from "svelte";
    
    let url = 'https://bird.hootsifer.com/api/v1/data';

    let data = [];

    let humidityValues = [];
    let temperatureValues = [];
    let lightValues = [];
    let weightValues = [];
    let movementValues = [];
    let soundValues = [];
    let timeStamps = [];
    let test = [];

    const intervalInSeconds = 300
    const amountOfValues = 24 * 60 * 60 / intervalInSeconds

    const getElements = (count: number, array: []) => {
    // Calculate the step size based on the desired count and the length of the array
        const step = Math.floor((array.length - 1) / (count - 1));
        // Create a new array with the selected elements
        const result = Array.from({ length: count }, (_, index) => array[index * step]);
        return result;
    }

    onMount(async () => {
        const response = await fetch(url, {
            method: "GET",
            headers: {
                "Authorization": "beepboopbeepbeep",
            },
        });
        data = await response.json();

        humidityValues = data.map(({ humidity }) => humidity);
        temperatureValues = data.map(({ temperature }) => temperature);
        lightValues = data.map(({ light }) => light);
        weightValues = data.map(({ weight }) => weight);
        movementValues = data.map(({ movement }) => movement);
        soundValues = data.map(({ sound }) => sound);
        timeStamps = data.map(({ time }) => new Date(time).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' }));

        console.log(timeStamps)

        test = getElements(6, timeStamps)

        console.log(test)

    });

    const get24HData = (values) => {
        return values.slice(Math.max(values.length - amountOfValues, 0))
    }
</script>

<h1 class="text-white">24H + data date</h1>

<main class="grid gap-10 m-20 sm:grid-cols-1 md:grid-cols-2 lg:grid-cols-3">
    <HumidityChart values={get24HData(humidityValues)} timeStamps={get24HData((timeStamps))} tests={test} />
    <TemperatureChart values={get24HData(temperatureValues)} timeStamps={get24HData((timeStamps))}  />
    <LightChart values={get24HData(lightValues)} timeStamps={get24HData((timeStamps))} />
    <WeightChart values={get24HData(weightValues)} timeStamps={get24HData((timeStamps))} />
    <MovementChart values={get24HData(movementValues)} timeStamps={get24HData((timeStamps))}  />
    <SoundChart values={get24HData(soundValues)} timeStamps={get24HData((timeStamps))} />
</main>