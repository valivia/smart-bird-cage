<script lang='ts'>
    import { Chart } from 'flowbite-svelte';
    import Card from './Card.svelte';
    import { API_URLS } from '$lib/api';
    import { fetchData } from '$lib/fetch';
    import type { SensorData } from '$lib/interfaces/sensorData';
    import { onMount } from 'svelte';
    
    export let name: 'Humidity' | 'Temperature' | 'Light' | 'Weight' | 'Movement' | 'Sound';
    export let description: string | undefined = undefined;
    export let unit: '%' | '°C' | 'LUX' | 'g' | undefined = undefined;
    export let min: number | undefined = undefined;
    export let max: number | undefined = undefined;
    export let stepSize: number | undefined = undefined;
    export let color: string;
    
    export let data: number[];
    export let timeStamps: string[];
    export let timePeriod: string = "Last 24H";
    
    let weekData: SensorData[];
    let weekDataValues: object;
    let monthData: SensorData[];
    let monthDataValues: object;
    let yearData: SensorData[];
    let yearDataValues: object;

    onMount(() => {
      const fetchPeriods = async () => {
        weekData = await fetchData(API_URLS.WEEK);
        monthData = await fetchData(API_URLS.MONTH);
        yearData = await fetchData(API_URLS.YEAR);
        weekDataValues = mapData(weekData, 'day');
        monthDataValues = mapData(monthData, 'date');
        yearDataValues = mapData(yearData, 'month');
    }

      fetchPeriods()
    })

    const mapData = (data: SensorData[], group: string) => {
        let groupBy: Intl.DateTimeFormatOptions = group === 'day' ? { weekday: 'long' } : group === 'month' ? { month: 'long' } : group === 'date' && {day: 'numeric',month: 'short'}

        return data.map(item => ({x: new Date(item.time).toLocaleString('default', groupBy), y: item[(name.toLowerCase()) as keyof SensorData]}));
    };

    let timePeriodData: [];

    let overwriteData = () => {
      switch(timePeriod) {
        case 'Last 24H':
          timePeriodData = []
          break;
        case 'Last week':
          timePeriodData = weekDataValues
          break;
        case 'Last month':
          timePeriodData = monthDataValues
          break;
        case 'Last year':
          timePeriodData = yearDataValues
          break
      }
    }

    $: timePeriod && overwriteData()

    $: commonOptions = {
      chart: {
        height: '400px',
        width: '100%',
        fontFamily: 'Inter, sans-serif',
        foreColor: 'currentColor',
        toolbar: {
          show: false
        }
      },
      series: [
        {
          name: `${name} (${unit})`,
          data: timePeriodData.length ? timePeriodData : data,
          color,
        }
      ],
      tooltip: {
        enabled: true,
        x: {
          show: true
        }
      },
      dataLabels: {
        enabled: false
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
        tickAmount: 10,
        categories: timePeriod === "Last 24H" && timeStamps,
        labels: {
          trim: true,
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
        stepSize
      }
    };

    $: lineChartOptions = {
      ...commonOptions,
      chart: {
        ...commonOptions.chart,
        type: "line",
        dropShadow: {
          enabled: false
        }
      },
      stroke: {
        width: 5,
        curve: 'smooth'
      },
      
    };

    $: barChartOptions = {
      ...commonOptions,
      chart: {
        ...commonOptions.chart,
        type: "bar"
      },
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
      fill: {
        opacity: 1
      }
    };

</script>
  
  <Card {name} {description} bind:timePeriod>
    <Chart options={timePeriod === "Last 24H" ? lineChartOptions : barChartOptions}/>
  </Card>