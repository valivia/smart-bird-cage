interface SensorData {
  // float, 80-120 Randomized
  weight: number;
  // float, 0-20 Randomized, day/night cycle dependent
  movement: number;
  // int, 0-20 Randomized, day/night cycle dependent
  sound: number;
  // float, 10-30 Randomized, day/night cycle dependent
  temperature: number;
  // float, 0-100 Randomized, day/night cycle dependent
  humidity: number;
  // int, 0-30000 Randomized, day/night cycle dependent
  light: number;
}

// Variables
const timeout = 5 * 1000; // 5 minutes
const host = "https://bird.hootsifer.com/api/v1"
const token = "testdata"

// State
let lastData: SensorData = {
  weight: 95,
  movement: 0,
  sound: 0,
  temperature: 20,
  humidity: 50,
  light: 1000,
}

function getRandomFloat(min: number, max: number): number {
  return parseFloat((Math.random() * (max - min) + min).toFixed(2));
}

function getRandomInt(min: number, max: number): number {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

function getTimeBasedValue(base: number, amplitude: number): number {
  // Normalize hour to range [0, 1]
  const normalizedHour = (new Date().getHours() + 12 % 24) / 24;
  console.log({ normalizedHour })
  // Use a sinusoidal function to simulate daily cycles
  const value = base + amplitude * Math.sin(2 * Math.PI * normalizedHour);
  return value;
}

async function main() {
  const baseMovement = 10;
  const amplitudeMovement = 10;
  const baseSound = 10;
  const amplitudeSound = 10;
  const baseTemperature = 20;
  const amplitudeTemperature = 10;
  const baseHumidity = 50;
  const amplitudeHumidity = 20;
  const baseLightDay = 20000;
  const amplitudeLightDay = 10000;
  const baseLightNight = 500;
  const amplitudeLightNight = 500;

  const body: SensorData = {
    weight: getRandomFloat(80, 120),
    movement: getRandomFloat(0, getTimeBasedValue(baseMovement, amplitudeMovement)),
    sound: getRandomInt(0, Math.round(getTimeBasedValue(baseSound, amplitudeSound))),
    temperature: getRandomFloat(10, getTimeBasedValue(baseTemperature, amplitudeTemperature)),
    humidity: getRandomFloat(0, getTimeBasedValue(baseHumidity, amplitudeHumidity)),
    light: 0
  };

  // fetch(`${host}/data`, {
  //   method: 'POST',
  //   headers: {
  //     'Content-Type': 'application/json',
  //     "authorization": token,
  //   },
  //   body: JSON.stringify(body),
  // })

  lastData = body;

  setTimeout(main, timeout);
}


main();
