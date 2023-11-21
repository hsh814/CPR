<script lang='ts'>
  import { fastapi } from '$lib/fastapi';
  import type { Metadata } from '$lib/metadata';
  import {onMount} from 'svelte';
  import ResultTable from './ResultTable.svelte';
  import { resultStore } from '$lib/store';

  let result_table: {table: string} = {table: 'not available'};
  resultStore.subscribe(value => {
    result_table = value;
  });

</script>


<h1>Web UI of uni-klee</h1>

<ResultTable data={result_table} />

