<script lang='ts'>
  import type { ResultType, AnalysisTableType, DirDataType } from '$lib/store';
  import { dirDataStore } from '$lib/store';
  import { onMount } from 'svelte';
  import { fastapi } from '$lib/fastapi';
  export let result_data: ResultType;
  export let table: AnalysisTableType;
  let dirData: DirDataType;
  dirDataStore.subscribe(value => {
    dirData = value;
  });
  $: localTable = table;
  $: selected_input = -1;
  let selected_feasibility: boolean = false;
  let remaining_inputs: Map<number, boolean[]> = new Map();
  let remaining_patches: Set<number> = new Set();
  const rebuild_table = () => {
    // Rebuild table based on remaining_inputs, remaining_patches
    let new_columns = [...remaining_patches].sort((a, b) => a - b);
    let new_rows: {base: number, row: boolean[]}[] = [];
    for (const [key, value] of remaining_inputs) {
      new_rows.push({base: key, row: value});
    }
    localTable = {columns: new_columns, rows: new_rows.sort((a, b) => a.base - b.base)};
  }
  const get_input_trace = () => {
    const params = dirData;
    console.log("get_input_trace: " + JSON.stringify(params));
    fastapi("POST", "/meta-data/data-log-parser/explain", params, (data: {trace: string[], input: object}) => {
      console.log("get_input_trace: " + JSON.stringify(data));
    }, (error: any) => {
      console.log(error);
    });
  }
  const select_input = () => {
    const params = dirData;
    console.log("select_input: " + JSON.stringify(params));
    fastapi("POST", "/meta-data/data-log-parser/select", params, (data: {selected_input: number, remaining_patches: number[], remaining_inputs: number[]}) => {
      console.log("select_input: " + JSON.stringify(data));
      dirData.inputs.push(data.selected_input);
      selected_input = data.selected_input;
      // Update remaining_inputs, remaining_patches
      const remaining_inputs_filter = new Set(data.remaining_inputs);
      for (const key of remaining_inputs.keys()) {
        if (!remaining_inputs_filter.has(key)) {
          remaining_inputs.delete(key);
        }
      }
      const remaining_patches_filter = new Set(data.remaining_patches);
      for (const key of remaining_patches) {
        if (!remaining_patches_filter.has(key)) {
          remaining_patches.delete(key);
        }
      }
    }, (error: any) => {
      console.log(error);
    });
  }
  onMount(() => {
    console.log("AnalysisTable onMount");
    // Initialize remaining_inputs, remaining_patches
    localTable.rows.forEach((value, index) => {
      remaining_inputs.set(value.base, value.row);
    });
    localTable.columns.forEach((value, index) => {
      remaining_patches.add(value);
    });
    select_input();
  });
</script>

{#if localTable.columns.length > 0}
  <h2>Analysis Table</h2>
  {#if selected_input > 0}
    <div>Current input: {selected_input}</div>
  {/if}
  <table>
    <thead>
      <tr></tr>
    </thead>
  </table>
  <button on:click={() => get_input_trace()}> Get input trace </button>
  <input type="checkbox" bind:checked={selected_feasibility} />
  <button on:click={() => select_input()}> Send feasiblity, get new input </button>
  <table>
    <thead>
      <tr>
        <th>Input</th>
        {#each localTable.columns as column}
          <th>p{column}</th>
        {/each}
      </tr>
    </thead>
    <tbody>
      {#each localTable.rows as row, index}
        <tr>
          <td>i{row.base}</td>
          {#each row.row as value}
            <td>{value ? 'X' : 'O'}</td>
          {/each}
        </tr>
      {/each}
    </tbody>
  </table>
{/if}

<style>
  table {
    border-collapse: collapse;
    width: 100%;
  }

  th, td {
    border: 1px solid #ddd;
    padding: 8px;
    text-align: left;
  }

  th {
    background-color: #f2f2f2;
  }
</style>
