<script lang='ts'>
  import { fastapi } from '$lib/fastapi';
  import {onMount} from 'svelte';
  import Graph from './Graph.svelte'
  import type { NodeType, EdgeType, GraphType } from '$lib/graph_api'
  import { graphStore } from '$lib/store';
  let graph_data: GraphType;
  graphStore.subscribe(value => {
    graph_data = value;
  });
</script>

<h1>Web UI of uni-klee</h1>
<a href="/benchmark/table">Goto table</a>

<Graph graph_data={graph_data} />

