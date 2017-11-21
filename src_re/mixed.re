/* 1-mixed.js style */
/* Note remaned to be consistent with React/OCaml naming conventions) */
/* See https://github.com/lucasmreis/react-patterns/blob/master/src/planet/1-mixed.js
for comparison */

type planet = {
	name: string,
	climate: string,
	terrain: string
};

type responseerror = Js.Promise.error;

type loadedstate =
 | Loading
 | Failed(responseerror)
 | Loaded(planet);

type state = {loadedstate: loadedstate};

type action =
	| ApiResponseReceived(loadedstate);

let defaultplanet = {
	name: "Jupiter",
	climate: "Gassy",
	terrain: "None"
};

let earthplanet = {
	name: "Earth",
	climate: "Oxygen",
	terrain: "Land"
};

let planetUrl = "https://swapi.co/api/planets/5/?format=json";

let parsePlanetJson = (json) : planet => {
	name: Json.Decode.field("name", Json.Decode.string, json),
	climate: Json.Decode.field("climate", Json.Decode.string, json),
	terrain: Json.Decode.field("terrain", Json.Decode.string, json)
};

module Dagobah = {
	let component = ReasonReact.reducerComponent("Dagobah");
	let fetchPlanet = () => {
		Bs_fetch.fetch(planetUrl)
		|> Js.Promise.then_(Bs_fetch.Response.text)
		|> Js.Promise.then_((jsonText) => {
				Js.Promise.resolve(
					Loaded(parsePlanetJson(Js.Json.parseExn(jsonText))))
			})
		|> Js.Promise.catch((error) => {
				Js.Promise.resolve(Failed(error))
			})
	};
	let make = (_children) => {
		...component,
		initialState: () => {loadedstate:Loading},
		didMount: self => {
			fetchPlanet()
				|> Js.Promise.then_(
			     (loadedstate) => {
			       /* Js.log (Js_array.toString shows); */
			       self.reduce((_) => ApiResponseReceived(loadedstate),());
			       Js.Promise.resolve()
					 })
				 |>  Js.Promise.catch((error) => {
        		self.reduce(() => ApiResponseReceived(Failed(error)),());
        		Js.Promise.resolve();
      		}) |> ignore;
				ReasonReact.NoUpdate;
    },
		reducer: (action, _state) =>
			{Js.log("About to reduce");
			switch action {
				| ApiResponseReceived(loadedstate) => ReasonReact.Update({loadedstate:loadedstate})
		}},
		render: (self) => {
			switch self.state.loadedstate {
       | Loading => <div>(ReasonReact.stringToElement("loading..."))</div>
			 | Failed(_responseerror) => <div>(ReasonReact.stringToElement("error!"))</div>
			 | Loaded(planet) => {
				 	<div>
				 		<div>(ReasonReact.stringToElement("On the planet of " ++ (planet.name) ++ ", the climate is " ++ (planet.climate) ++ ". "))</div>
					</div>
			 }
		 }
		}
	}
}
