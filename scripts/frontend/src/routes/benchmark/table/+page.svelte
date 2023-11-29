<script lang='ts'>
  import { fastapi } from '$lib/fastapi';
  import type { Metadata } from '$lib/metadata';
  import {onMount} from 'svelte';
  import ResultTable from './ResultTable.svelte';
  import { mdTableStore } from '$lib/store';

  let result_table: {table: string} = {table: 'not available'};
  mdTableStore.subscribe(value => {
    result_table = value;
  });

</script>


<h1>Web UI of uni-klee</h1>
<a href="/benchmark/graph">Goto graph</a>
<ResultTable data={result_table} />

