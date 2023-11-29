<script lang='ts'>
  import type { ResultType, AnalysisTableType } from '$lib/store';
  export let data: ResultType;
  export let table: AnalysisTableType;
  $: localTable = table;
  console.log('localTable:', localTable);
  console.log('data', data);
</script>

{#if localTable.columns.length > 0}
  <h2>Analysis Table</h2>
  <table>
    <thead>
      <tr>
        <th>Cluster</th>
        <th>Input</th>
        {#each localTable.columns as column}
          <th>p{column}</th>
        {/each}
      </tr>
    </thead>
    <tbody>
      {#each localTable.rows as sub_row, index}
        {#each sub_row as { base, row }}
          <tr>
            <td>{index}</td>
            <td>i{base}</td>
            {#each row as value}
              <td>{value ? 'O' : 'X'}</td>
            {/each}
          </tr>
        {/each}
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

<!-- {#if removed_if_feasible_rows.length > 0}
  <h2>Removed If Feasible Table</h2>
  <table>
    <thead>
      <tr>
        {#each removed_if_feasible_columns as { key, title }}
          <th>{title}</th>
        {/each}
      </tr>
    </thead>
    <tbody>
      {#each removed_if_feasible_rows as row}
        <tr>
          {#each removed_if_feasible_columns as { key }}
            <td>{row[key]}</td>
          {/each}
        </tr>
      {/each}
    </tbody>
  </table>
{/if}

{#if patch_analysis_rows.length > 0}
  <h2>Patch Analysis Table</h2>
  <table>
    <thead>
      <tr>
        {#each patch_analysis_columns as { key, title }}
          <th>{title}</th>
        {/each}
      </tr>
    </thead>
    <tbody>
      {#each patch_analysis_rows as row}
        <tr>
          {#each patch_analysis_columns as { key }}
            <td>{row[key]}</td>
          {/each}
        </tr>
      {/each}
    </tbody>
  </table>
{/if} -->
