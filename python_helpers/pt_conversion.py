import pyfluids as pf
import numpy as np
import pandas as pd
import plotly.graph_objects as go
import plotly.express as px

gas_list = ["R290", "R32", "R410A"]

def p2t(p, fluid):
    state = pf.Fluid(pf.FluidsList[fluid]).dew_point_at_pressure(p)
    return state.temperature

def t2p(t, fluid):
    state = pf.Fluid(pf.FluidsList[fluid]).dew_point_at_temperature(t)
    return state.pressure

def create_points(fluid):
    pressures = []
    temperatures = []
    min_temperature = -40
    max_temperature = pf.Fluid(pf.FluidsList[fluid]).critical_temperature
    for t in np.linspace(min_temperature, max_temperature, 15):
        try:
            p = t2p(t, gas)
        except Exception as e:
            print(e)
            p = np.nan
        pressures.append(p)
        temperatures.append(t.item())
    return pressures, temperatures


def generate_p2t_plot(fluid):
    df = pd.DataFrame()
    min_pressure = pf.Fluid(pf.FluidsList[fluid]).dew_point_at_temperature(-40).pressure
    max_pressure = pf.Fluid(pf.FluidsList[fluid]).critical_pressure
    for p in np.linspace(min_pressure, max_pressure, 100):
        try:
            t = p2t(p, fluid)
        except Exception as e:
            t = np.nan
        df = pd.concat([df, pd.DataFrame({"Pressure": [p], "Temperature": [t]})])
    fig = px.line(df, x="Pressure", y="Temperature", title=f"{fluid} Pressure vs Temperature", labels={"Pressure": "Pressure (bar)", "Temperature": "Temperature (°C)"})
    pressures, temperatures = create_points(fluid)
    fig.add_trace(go.Scatter(x=pressures, y=temperatures, mode="markers", name="Points"))
    fig.write_image(f"python_helpers/output/pt/{fluid}_pt.png")
    fig.write_html(f"python_helpers/output/pt/{fluid}_pt.html")

def generate_p2t_table(fluid):
    pressures, temperatures = create_points(fluid)
    pressures_mBar = [round(p / 100) for p in pressures]
    temperatures_cC = [round(t * 10) for t in temperatures]
    with open(f"python_helpers/output/pt/{fluid}_p2t.txt", "w") as file:
        file.write(f"Pressure A (mBar): {pressures_mBar}\n")
        file.write(f"Temperature (°dC): {temperatures_cC}\n")
        

if __name__ == "__main__":
    for gas in gas_list:
        generate_p2t_plot(gas)
        generate_p2t_table(gas)
