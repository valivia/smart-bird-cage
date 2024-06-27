<script lang="ts">
  import { Card, Button, Dropdown, DropdownItem, Popover } from 'flowbite-svelte';
  import { InfoCircleSolid, ChevronDownOutline} from 'flowbite-svelte-icons';

  export let name: string;
  export let description: string | undefined;
  export let timePeriod: string;
  
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

<Card class="flex-col p-5 aspect-video">
    <h2 class="inline-flex items-center mb-10 text-3xl font-bold leading-none text-gray-900 dark:text-white">
      {name}
      {#if description}
      <InfoCircleSolid id={name} class="w-3 h-3 text-gray-400 cursor-pointer hover:text-gray-900 dark:hover:text-white ms-1" />
      <Popover triggeredBy={`#${name}`} class="z-10 text-sm text-gray-500 bg-white border border-gray-200 rounded-lg shadow-sm w-72 dark:bg-gray-800 dark:border-gray-600 dark:text-gray-400">
        <div class="p-3 space-y-2">
          <h3 class="font-semibold text-gray-900 dark:text-white">{name} Measurement</h3>
          <p>{description}</p>
      </Popover>
      {/if}
    </h2>
    <slot />
    <div class="grid items-center justify-between grid-cols-1 border-t border-gray-200 dark:border-gray-700">
        <div class="flex items-center justify-between pt-5">
          <Button class="inline-flex items-center py-0 text-sm font-medium text-center text-gray-500 bg-transparent dark:text-gray-400 hover:text-gray-900 dark:hover:text-white hover:bg-transparent dark:bg-transparent dark:hover:bg-transparent focus:ring-transparent dark:focus:ring-transparent">{timePeriod}<ChevronDownOutline class="w-2.5 m-2.5 ms-1.5" /></Button>
          <Dropdown classContainer='dark:bg-[#374151] dark:text-white overflow-hidden' class="w-40" offset={-6} bind:open={dropdownOpen}>
            {#each timePeriodOptionsValues as option, i}
                <DropdownItem defaultClass="dark:hover:bg-[#4b5563] w-full text-left px-2 first:pt-1 last:pb-1" on:click={setTimePeriod}>{option}</DropdownItem>
            {/each}
          </Dropdown>
        </div>
      </div>
  </Card>