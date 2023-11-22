import { writable } from 'svelte/store';
import type { NodeType, EdgeType, GraphType } from './graph_api';
export const resultStore = writable({ table: '' });
export const graphStore = writable<GraphType>({ nodes: [], edges: [] });