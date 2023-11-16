export const load: (params: { id: string }) => {id: string} = ({ params}) => {
  return { id: params.id };
}