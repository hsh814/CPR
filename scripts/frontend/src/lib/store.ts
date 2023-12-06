import { writable } from 'svelte/store';
import type { NodeType, EdgeType, GraphType } from './graph_api';
import type { Metadata } from './metadata';
export interface AnalysisTableType {
  columns: number[],
  rows: {
    base: number,
    row: boolean[],
  }[],
}
export interface ResultType {
  state_map: Record<number, any>
  removed_if_feasible: Record<number, Array<[number, number]>>
  removed_if_infeasible: Record<number, Array<[number, number]>>
  removed: Record<number, Array<[number, number]>>
  crash_id_to_state: Record<number, number>
  crash_test_result: Record<number, Array<number>>
  graph: { nodes: Set<number>, edges: Set<[number, number, string]> }
  patch_analysis: Record<number, Array<number>>
  table: AnalysisTableType
}

export interface InputSelectType {
  input: number,
  feasibility: boolean,
  used: boolean
}

export interface DirDataType {
  dir: string,
  inputs: InputSelectType[],
}

export const metaDataStore = writable<Metadata>({ id: 0, bug_id: '', benchmark: '', subject: '', vars: [], buggy: { code: '', id: '' }, correct: { code: '', id: '' }, target: '' });
export const mdTableStore = writable({ table: '' });
export const graphStore = writable<GraphType>({ nodes: [], edges: [] });
export const resultStore = writable<ResultType>({ state_map: {}, removed_if_feasible: {}, removed_if_infeasible: {}, removed: {}, crash_id_to_state: {}, crash_test_result: {}, graph: { nodes: new Set(), edges: new Set() }, patch_analysis: {}, table: { columns: [], rows: [] } });
export const dirDataStore = writable<DirDataType>({ dir: '', inputs: []});