import pyfluids as pf
import pandas as pd
import numpy as np
import plotly.graph_objects as go
from statsmodels.formula.api import ols

gas_list = ["R290", "R32", "R410A"]
def get_density(fluid, T_e):
    return pf.Fluid(pf.FluidsList[fluid]).dew_point_at_temperature(T_e).density

def create_points(fluid):
    densities = []
    temperatures = []
    min_temperature = -40
    max_temperature = pf.Fluid(pf.FluidsList[fluid]).critical_temperature 
    for t in np.linspace(min_temperature, max_temperature, 10):
        try:
            d = get_density(fluid, t)
        except Exception as e:
            print(e)
            d = np.nan
        densities.append(d)
        temperatures.append(t.item())
    return densities, temperatures

def generate_plot(fluid):
    df = pd.DataFrame()
    min_temperature = -40
    max_temperature = pf.Fluid(pf.FluidsList[fluid]).critical_temperature
    for t in np.linspace(min_temperature, max_temperature, 100):
        try:
            d = get_density(fluid, t)
        except Exception as e:
            d = np.nan
        df = pd.concat([df, pd.DataFrame({"Temperature": [t], "Density": [d]})])
    fig = go.Figure()
    fig.add_trace(go.Scatter(x=df["Temperature"], y=df["Density"], mode="lines", name="Density"))
    densities, temperatures = create_points(fluid)  
    fig.add_trace(go.Scatter(x=temperatures, y=densities, mode="markers", name="Points"))
    fig.update_layout(title=f"{fluid} Density vs Temperature", xaxis_title="Temperature (°C)", yaxis_title="Density (kg/m³)")
    fig.write_image(f"python_helpers/output/density/{fluid}_density.png")
    fig.write_html(f"python_helpers/output/density/{fluid}_density.html")

def generate_density_table(fluid):
    densities, temperatures = create_points(fluid)
    densities_mg_cc = [round(d * 10) for d in densities]
    temperatures_cC = [round(t * 10) for t in temperatures]
    with open(f"python_helpers/output/density/{fluid}_density.txt", "w") as file:
        file.write(f"Density 0.1(kg/m3): {densities_mg_cc}\n")
        file.write(f"Temperature (0.1°C): {temperatures_cC}\n")
        

if __name__ == "__main__":
    for gas in gas_list:
        generate_plot(gas)
        generate_density_table(gas)
