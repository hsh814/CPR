<script lang='ts'>
  export let data: {
    removed_if_feasible: Record<number, Array<[number, number]>>, 
    patch_analysis: Record<number, Array<number>>
  };
  const removed_if_feasible_columns = [
    {key: 'base', title: 'crash input'},
    {key: 'patch', title: 'patch'},
    {key: 'state', title: 'state'},
  ];
  const removed_if_feasible_rows = Object.entries(data.removed_if_feasible).map(([base, values]) => {
    return values.map(([patch, state]) => {
      return {base, patch, state};
    });
  }).flat();
  const patch_analysis_columns = [
    {key: 'patch', title: 'patch'},
    {key: 'base', title: 'crash input'},
  ];
  const patch_analysis_rows = Object.entries(data.patch_analysis).map(([patch, values]) => {
    return values.map((base) => {
      return {patch, base};
    });
  }).flat();
  console.log(removed_if_feasible_rows);
  console.log(patch_analysis_rows);
</script>

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

{#if patch_analysis_rows.length > 0}
  <h2>Analysis Table</h2>
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
{/if}

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
