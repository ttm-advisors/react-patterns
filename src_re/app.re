let component = ReasonReact.statelessComponent("App");

let make = (_children) => {
  ...component,
  render: (_) =>
    <Mixed.Dagobah />
};
