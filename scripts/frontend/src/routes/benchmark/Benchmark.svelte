<script lang='ts'>
  import type { Metadata } from '$lib/metadata';
  import { fastapi } from '$lib/fastapi';
  import { onMount } from 'svelte';
  export let data: Metadata;
</script>

<p>{data.id} {data.benchmark}</p>
