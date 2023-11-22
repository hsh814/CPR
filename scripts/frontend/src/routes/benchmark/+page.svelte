<script lang='ts'>
  import { fastapi } from '$lib/fastapi';
  import type { Metadata } from '$lib/metadata';
  import { graphStore, resultStore } from '$lib/store';
  import type { NodeType, EdgeType, GraphType } from '$lib/graph_api';
  interface dir_type { id: string, full: string };
  interface log_parser_result {
    table: string, 
    fork_graph: GraphType,
    input_graph: GraphType,
  };
  const urlSearchParams = new URLSearchParams(window.location.search);
  let data = {id: parseInt(urlSearchParams.get('id') || '0')};
  let meta_data: Metadata;
  let out_dirs: dir_type[] = [];
  let user_prefix = "uni-m-out";
  let show_result_table = false;

  const get_meta_data = (id: number) => {
    console.log("get_meta_data" + id);
    fastapi("GET", "/meta-data/info/" + id, {}, (data: {meta: Metadata, conf: object, meta_program: object}) => {
      console.log("get_meta_data: " + JSON.stringify(data));
      meta_data = data.meta;
    }, handle_error);
  }

  const handle_error = (error: any) => {
    console.log(error);
  }

  const get_out_dirs = (id: number, prefix: string) => {
    console.log("get_out_dirs" + id);
    fastapi("GET", "/meta-data/out-dir/", {id: id, prefix: prefix}, (dirs: dir_type[]) => {
      console.log("get_out_dirs: " + JSON.stringify(dirs));
      out_dirs = dirs;
    }, handle_error);
  }

  const handle_click_out_dir = (full_path: string) => {
    const data_log_parser_url = "/meta-data/data-log-parser";
    const params = { dir: full_path };
    fastapi("GET", data_log_parser_url, params, handle_log_parser_response, handle_error)
  }

  const handle_log_parser_response = (result: log_parser_result) => {
    resultStore.set({table: result.table});
    graphStore.set(result.input_graph);
    show_result_table = true;
  }

  get_meta_data(data.id);

</script>

<style>
  /* Add your styling here */
  .button-list {
    list-style: none;
    padding: 0;
    margin: 0;
  }

  .button-list-item {
    margin-bottom: 8px;
  }

  .button-list-item button {
    display: block;
    width: 100%;
    padding: 8px;
    text-align: left;
    background-color: #f0f0f0; /* Background color */
    border: 1px solid #ddd;   /* Border color */
    border-radius: 4px;       /* Rounded corners */
    cursor: pointer;
  }

  .button-list-item button:hover {
    background-color: #ddd;   /* Change background color on hover */
  }
</style>

<h1>Web UI of uni-klee</h1>
<h2>Id: {data.id}</h2>
<h3>Bug id is {meta_data ? meta_data.bug_id : ''}</h3>
{#if show_result_table}
  <div class="result-table">
    <a href="/benchmark/table">Goto result table</a>
  </div>
  <div class="graph">
    <a href="/benchmark/graph">Goto graph</a>
  </div>
{/if}

<input type="text" bind:value={user_prefix} />
<button on:click={() => get_out_dirs(data.id, user_prefix)}>Get Out Dirs</button>
<ul class="button-list">
  {#each out_dirs as out_dir}
    <li class="button-list-item">
      <button on:click={() => handle_click_out_dir(out_dir.full)}>{out_dir.id}</button>
    </li>
  {/each}
</ul>



