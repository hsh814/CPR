import { writable } from 'svelte/store';
import type { NodeType, EdgeType, GraphType } from './graph_api';
export interface ResultType {
  state_map: Record<number, any>
  removed_if_feasible: Record<number, Array<[number, number]>>
  removed_if_infeasible: Record<number, Array<[number, number]>>
  removed: Record<number, Array<[number, number]>>
  crash_id_to_state: Record<number, number>
  crash_test_result: Record<number, Array<number>>
  graph: { nodes: Set<number>, edges: Set<[number, number, string]> }
  patch_analysis: Record<number, Array<number>>
}
export const mdTableStore = writable({ table: '' });
export const graphStore = writable<GraphType>({ nodes: [], edges: [] });
export const resultStore = writable<ResultType>({ state_map: {}, removed_if_feasible: {}, removed_if_infeasible: {}, removed: {}, crash_id_to_state: {}, crash_test_result: {}, graph: { nodes: new Set(), edges: new Set() }, patch_analysis: {} });