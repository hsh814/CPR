<script lang='ts'>
  import type { ResultType, AnalysisTableType, DirDataType, InputSelectType } from '$lib/store';
  import { dirDataStore } from '$lib/store';
  import { onMount } from 'svelte';
  import { fastapi } from '$lib/fastapi';
  export let result_data: ResultType;
  export let table: AnalysisTableType;
  let dirData: DirDataType;
  dirDataStore.subscribe(value => {
    dirData = value;
  });
  let input_select_list: {input: number, feasibility: boolean, used: boolean}[] = [];
  let localTable: AnalysisTableType = {columns: [], rows: []};
  let selected_input = -1;
  let trace_data: {trace: string[], input: object} = {trace: [], input: {}};
  const original_columns: number[] = [...table.columns];
  const original_rows: {base: number, row: boolean[]}[] = [...table.rows];
  let remaining_inputs: Map<number, boolean[]> = new Map();
  let remaining_patches: Set<number> = new Set();
  let showOriginalTable: boolean = false;
  $: console.log("trace_data", trace_data);

  const get_table_header = (): number[] => {
    let header: number[] = [];
    for (const column of original_columns) {
      if (remaining_patches.has(column)) {
        header.push(column);
      }
    }
    return header;
  };

  const get_table_row = (input: number): boolean[] => {
    if (!remaining_inputs.has(input)) {
      return [];
    }
    const row = remaining_inputs.get(input);
    if (row && remaining_patches.size > 0) {
      const hasPatches = original_rows.some((r) => remaining_patches.has(r.base));
      if (hasPatches) {
        // Extract the corresponding row based on the input
        let result: boolean[] = [];
        row.map((value, index) => {
          if (remaining_patches.has(original_columns[index])) {
            result.push(value);
          }
        });
        return result;
      }
    }
    return [];
  }

  const rebuild_table = () => {
    // Rebuild table based on remaining_inputs, remaining_patches
    console.log("rebuild_table");
    console.log(remaining_inputs);
    console.log(remaining_patches);
    let new_columns = get_table_header();
    let new_rows: {base: number, row: boolean[]}[] = [];
    for (const [base, values] of remaining_inputs) {
      new_rows.push({base: base, row: get_table_row(base)});
    }
    localTable = {columns: new_columns, rows: new_rows.sort((a, b) => a.base - b.base)};
  };

  const get_input_trace = () => {
    const params: DirDataType = { dir: dirData.dir, inputs: input_select_list };
    console.log("get_input_trace: " + JSON.stringify(params));
    fastapi("POST", "/meta-data/data-log-parser/explain", params, (data: {trace: string[], input: object}) => {
      console.log("get_input_trace: " + JSON.stringify(data));
      if (data == null) {
        console.log("get_input_trace: data is null");
        return;
      }
      trace_data = data;
    }, (error: any) => {
      console.log(error);
    });
  };

  const select_input = () => {
    input_select_list.forEach((value, index) => {
      value.used = true;
    });
    const params: DirDataType = { dir: dirData.dir, inputs: input_select_list };
    console.log("select_input: " + JSON.stringify(params));
    fastapi("POST", "/meta-data/data-log-parser/select", params, (data: {selected_input: number, remaining_patches: number[], remaining_inputs: number[]}) => {
      console.log("select_input: " + JSON.stringify(data));
      const sel_in: InputSelectType = { input: data.selected_input, feasibility: true, used: false };
      input_select_list = [...input_select_list, sel_in];
      selected_input = data.selected_input;
      console.log("select result: ", input_select_list);
      // Update remaining_inputs, remaining_patches
      const remaining_inputs_filter = new Set(data.remaining_inputs);
      for (const key of remaining_inputs.keys()) {
        if (!remaining_inputs_filter.has(key)) {
          remaining_inputs.delete(key);
        }
      }
      remaining_patches = new Set(data.remaining_patches);
      console.log("remaining_inputs: " + JSON.stringify(remaining_inputs));
      console.log("remaining_patches: " + JSON.stringify(remaining_patches));
    }, (error: any) => {
      console.log(error);
    });
  };

  onMount(() => {
    console.log("AnalysisTable onMount");
    // Initialize remaining_inputs, remaining_patches
    table.rows.forEach((value, index) => {
      remaining_inputs.set(value.base, value.row);
    });
    table.columns.forEach((value, index) => {
      remaining_patches.add(value);
    });
    select_input();
  });
</script>


<h2>Analysis Table</h2>
<div class="trace-container">
  Selected input: {selected_input}
  <ul class="trace-list">
    {#each trace_data.trace as trace}
      <li>{trace}</li>
    {/each}
  </ul>
</div>
<table>
  <thead>
    <tr></tr>
  </thead>
</table>

<div class="sel-input-container">
  {#each input_select_list as input}
    <div class="sel-input-elem">
      {#if input.used}
        <input type="checkbox" bind:checked={input.feasibility} />
        <div class="sel-input-input">{input.input}</div>
      {:else}
        <input type="checkbox" bind:checked={input.feasibility} />
        <div class="sel-input-select">{input.input}</div>
      {/if}
    </div>
  {/each}
</div>

<button on:click={() => rebuild_table()}> Rebuild table </button>
<button on:click={() => get_input_trace()}> Get input trace </button>
<button on:click={() => select_input()}> Send feasiblity & get new input </button>

<div class="patch-container">
  {#each remaining_patches as patch}
    <div class="patch-elem">{patch}</div>
  {/each}
</div>
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
    {#each localTable.rows as row}
      <tr>
        <td>i{row.base}</td>
        {#each row.row as value}
          {#if remaining_inputs.has(row.base)}
            <td>{value ? 'O' : 'X'}</td>
          {/if}
        {/each}
      </tr>
    {/each}
  </tbody>
</table>

<button on:click={() => showOriginalTable = !showOriginalTable}> Show original table </button>
{#if showOriginalTable}
  <table>
  <thead>
    <tr>
      <th>Input</th>
      {#each original_columns as column}
        <th>p{column}</th>
      {/each}
    </tr>
  </thead>
  <tbody>
    {#each original_rows as row}
      <tr>
        <td>i{row.base}</td>
        {#each row.row as value}
          {#if remaining_inputs.has(row.base)}
            <td>{value ? 'O' : 'X'}</td>
          {/if}
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
  .trace-container {
    max-height: 200px; /* Set your desired maximum height */
    max-width: 80%;
    overflow-y: auto;
    border: 1px solid #3cc; /* Optional: Add a border for better visibility */
  }

  .trace-list {
    list-style-type: none;
    padding: 0;
    margin: 0;
  }

  .patch-container {
    max-height: 200px; /* Set your desired maximum height */
    max-width: 80%;
    overflow-x: auto;
    border: 1px solid #c3c; /* Optional: Add a border for better visibility */
    white-space: nowrap;
  }

  .patch-elem {
    display: inline-block;
    padding: 2px;
    border: 1px solid #33c;
  }

  .sel-input-container {
    max-height: 200px; /* Set your desired maximum height */
    max-width: 80%;
    overflow-y: auto;
    display: flex;
    border: 1px solid #3cc; /* Optional: Add a border for better visibility */
    padding: 10px;
  }

  .sel-input-elem {
    padding: 8px;
    margin-bottom: 5px;
    border: 1px solid #c0c; /* Optional: Add a border between list items for better visibility */
  }
  .sel-input-input {
    color: blue;
  }
  .sel-input-select {
    color: red;

  }

  li {
    padding: 8px;
    border-bottom: 1px solid #ddd; /* Optional: Add a border between list items for better visibility */
  }
</style>
