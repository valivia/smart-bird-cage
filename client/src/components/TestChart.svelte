<script lang='ts'>
    export let name: 'Humidity' | 'Temperature' | 'Light' | 'Weight' | 'Movement' | 'Sound';
    export let unit: '%' | '°C' | 'LUX' | 'g' | undefined = undefined;
    export let min: number | undefined = undefined;
    export let max: number | undefined = undefined;
    export let stepSize: number | undefined = undefined;
    export let color: string;

    export let data: number[];
    export let timeStamps: string[];
    export let xaxis;

    import { Chart, Card, A, Button, Dropdown, DropdownItem, Popover } from 'flowbite-svelte';
    import { InfoCircleSolid, ChevronRightOutline, ChevronDownOutline, FileLinesSolid } from 'flowbite-svelte-icons';

    $: options = {
      chart: {
        height: '400px',
        width: '100%',
        type: 'line',
        fontFamily: 'Inter, sans-serif',
        foreColor: 'currentColor',
        dropShadow: {
          enabled: false
        },
        toolbar: {
          show: false
        }
      },
      tooltip: {
        enabled: true,
        x: {
          show: true
        }
      },
      dataLabels: {
        enabled: false
      },
      stroke: {
        width: 5,
        curve: 'smooth'
      },
      grid: {
        show: true,
        strokeDashArray: 4,
        padding: {
          left: 2,
          right: 2,
          top: -26
        }
      },
      series: [
        {
          name: `${name} (${unit})`,
          data,
          color
        },
      ],
      legend: {
        show: false
      },
      xaxis: {
        categories: timeStamps,
        overwriteCategories: xaxis,
        labels: {
          show: true,
          style: {
            fontFamily: 'Inter, sans-serif',
            cssClass: 'text-xs font-normal fill-gray-500 dark:fill-gray-400'
          }
        },
        axisBorder: {
          show: false
        },
        axisTicks: {
          show: false
        }
      },
      yaxis: {
        show: true,
        min,
        max,
        stepSize,
      }
    };

    let dropdownOpen = false;
    export let timePeriod: string = 'Last 24H';
    export let timePeriodOptions;
    let timePeriodOptionsValues = Object.entries(timePeriodOptions).map(([k]) => `${k}`);

    const setTimePeriod = (e) => {
        e.preventDefault();
        timePeriod = e.target.innerHTML;
        dropdownOpen = false;
    };
    
  </script>
  
  <Card class="flex-col p-5">
    <h5 class="inline-flex items-center mb-10 text-3xl font-bold leading-none text-gray-900 dark:text-white">
      {name}
      <InfoCircleSolid id="b1" class="w-3 h-3 text-gray-400 cursor-pointer hover:text-gray-900 dark:hover:text-white ms-1" />
      <Popover triggeredBy="#b1" class="z-10 text-sm text-gray-500 bg-white border border-gray-200 rounded-lg shadow-sm w-72 dark:bg-gray-800 dark:border-gray-600 dark:text-gray-400">
        <div class="p-3 space-y-2">
          <h3 class="font-semibold text-gray-900 dark:text-white">{name} Measurement</h3>
          <p>Report helps navigate cumulative growth of community activities. Ideally, the chart should have a growing trend, as stagnating chart signifies a significant decrease of community activity.</p>
      </Popover>
    </h5>
    <Chart {options} x="100" />
    <div class="grid items-center justify-between grid-cols-1 border-t border-gray-200 dark:border-gray-700">
        <div class="flex items-center justify-between pt-5">
          <Button class="inline-flex items-center py-0 text-sm font-medium text-center text-gray-500 bg-transparent dark:text-gray-400 hover:text-gray-900 dark:hover:text-white hover:bg-transparent dark:bg-transparent dark:hover:bg-transparent focus:ring-transparent dark:focus:ring-transparent">{timePeriod}<ChevronDownOutline class="w-2.5 m-2.5 ms-1.5" /></Button>
          <Dropdown class="w-40" offset={-6} bind:open={dropdownOpen}>
            {#each timePeriodOptionsValues as option, i}
                <DropdownItem on:click={setTimePeriod}>{option}</DropdownItem>
            {/each}
          </Dropdown>
        </div>
      </div>
  </Card>