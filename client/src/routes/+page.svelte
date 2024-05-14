<script lang="ts">
    let { data } = $props();
</script>

<h1>Smart bird cage</h1>
can connect:
{#await data.response}
    <span>loading...</span>
{:then response}
    <span>{response.status == 200}</span>
{:catch error}
    <span>false</span>
{/await}
