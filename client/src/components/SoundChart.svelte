<script>
  import { Chart, Card, A, Button, Dropdown, DropdownItem, Popover } from 'flowbite-svelte';
  import { UsersGroupOutline, ArrowUpOutline, ChevronDownOutline, ChevronRightOutline, InfoCircleSolid } from 'flowbite-svelte-icons';

  export let values;
  export let timeStamps;

  $: options = {
    chart: {
      type: 'bar',
      height: '400px',
      width: '100%',
      fontFamily: 'Inter, sans-serif',
      foreColor: 'currentColor',
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
    series: [
      {
        name: 'Sound',
        color: '#5deffe',
        data: [
          { x: 'Mon', y: 231 },
          { x: 'Tue', y: 122 },
          { x: 'Wed', y: 63 },
          { x: 'Thu', y: 421 },
          { x: 'Fri', y: 122 },
          { x: 'Sat', y: 323 },
          { x: 'Sun', y: 111 }
        ]
      },
    ],
    plotOptions: {
      bar: {
        horizontal: false,
        columnWidth: '70%',
        borderRadiusApplication: 'end',
        borderRadius: 8
      }
    },
    states: {
      hover: {
        filter: {
          type: 'darken',
          value: 1
        }
      }
    },
    stroke: {
      show: true,
      width: 0,
      colors: ['transparent']
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
    legend: {
      show: false
    },
    xaxis: {
      floating: false,
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
      show: true
    },
    fill: {
      opacity: 1
    }
  };
</script>

<Card class="flex-col p-5">
  <h5 class="inline-flex items-center mb-10 text-3xl font-bold leading-none text-gray-900 dark:text-white">
    Sound
    <InfoCircleSolid id="b1" class="w-3 h-3 text-gray-400 cursor-pointer hover:text-gray-900 dark:hover:text-white ms-1" />
    <Popover triggeredBy="#b1" class="z-10 text-sm text-gray-500 bg-white border border-gray-200 rounded-lg shadow-sm w-72 dark:bg-gray-800 dark:border-gray-600 dark:text-gray-400">
      <div class="p-3 space-y-2">
        <h3 class="font-semibold text-gray-900 dark:text-white">Sound Measurement</h3>
        <p>Report helps navigate cumulative growth of community activities. Ideally, the chart should have a growing trend, as stagnating chart signifies a significant decrease of community activity.</p>
    </Popover>
  </h5>
  <Chart {options} />
</Card>