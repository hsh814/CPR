<script lang='ts'>
  import { fastapi } from '$lib/fastapi';
  import type { Metadata } from '$lib/metadata';
  import {onMount} from 'svelte';

  let message: string = "invalid string";
  let data: { id: number } = {id: 0};
  let meta_data: Metadata;
  let out_dirs: string[] = [];
  let user_prefix = "uni-m-out";

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
  const get_message = (data: {message: string}) => {
    message = data.message;
  }

  const get_out_dirs = (id: number, prefix: string) => {
    console.log("get_out_dirs" + id);
    fastapi("GET", "/meta-data/out-dir/", {id: id, prefix: prefix}, (dirs: string[]) => {
      console.log("get_out_dirs: " + JSON.stringify(dirs));
      out_dirs = dirs;
    }, handle_error);
  }

  // fastapi("GET", "/hello", {}, get_message, handle_error);
  // fastapi("GET", "/meta-data/list", {}, get_meta_data, handle_error);
  onMount(() => {
    const urlSearchParams = new URLSearchParams(window.location.search);
    data = {id: parseInt(urlSearchParams.get('id') || '0')};
    get_meta_data(data.id);
  })

  $: {
    if (meta_data) {
      console.log("meta_data is updated: " + meta_data.bug_id);
    }
  }

</script>

<h1>Web UI of uni-klee</h1>
<p> Test Test </p>
<h2>{message}</h2>
<h2>Data is {data.id}</h2>
<h3>Bug id is {meta_data ? meta_data.bug_id : ''}</h3>
<input type="text" bind:value={user_prefix} />
<button on:click={() => get_out_dirs(data.id, user_prefix)}>Get Out Dirs</button>
{#each out_dirs as out_dir}
  <p>{out_dir}</p>
{/each}


