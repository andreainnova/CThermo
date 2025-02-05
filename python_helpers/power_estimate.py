import pyfluids as pf
import pandas as pd
import numpy as np
import plotly.graph_objects as go
from statsmodels.formula.api import ols

gas_list = ["R290", "R32", "R410A"]

def get_dH_estimates(fluid, T_e, T_c):
    if T_c < T_e:
        raise ValueError("T_c must be greater than T_e")
    fluid = pf.Fluid(pf.FluidsList[fluid])
    S_such = fluid.dew_point_at_temperature(T_e)
    S_disch = S_such.compression_to_pressure(fluid.dew_point_at_temperature(T_c).pressure, isentropic_efficiency=75)
    S_eev = fluid.bubble_point_at_temperature(T_c)

    dH_evap = S_such.enthalpy - S_eev.enthalpy
    dH_cond = S_disch.enthalpy - S_eev.enthalpy
    dH_comp = S_disch.enthalpy - S_such.enthalpy

    return dH_evap, dH_cond, dH_comp

def generate_dH_df(fluid):
    T_MIN, T_MAX = -20, pf.Fluid(pf.FluidsList[fluid]).critical_temperature -20
    df = pd.DataFrame()
    for T_e in np.linspace(T_MIN, T_MAX, 20):
        for T_c in np.linspace(T_MIN, T_MAX, 20):
            try:
                dH_evap, dH_cond, dH_comp = get_dH_estimates(fluid, T_e, T_c)
            except:
                dH_evap, dH_cond, dH_comp = np.nan, np.nan, np.nan
            df = pd.concat([df, pd.DataFrame({"T_e": [T_e], "T_c": [T_c], "dH_evap": [dH_evap], "dH_cond": [dH_cond], "dH_comp": [dH_comp]})])
    return df

def generate_dH_plots(df):
    model_plot = go.Figure()
    model_plot.add_trace(go.Scatter3d(x=df["T_e"], y=df["T_c"], z=df["dH_evap"], mode="markers", name="dH_evap"))
    model_plot.add_trace(go.Scatter3d(x=df["T_e"], y=df["T_c"], z=df["dH_cond"], mode="markers", name="dH_cond"))
    model_plot.add_trace(go.Scatter3d(x=df["T_e"], y=df["T_c"], z=df["dH_comp"], mode="markers", name="dH_comp"))
    model_plot.add_trace(go.Surface(
        z=df["dH_evap_pred"].values.reshape(20, 20),
        x=df["T_e"].values.reshape(20, 20),
        y=df["T_c"].values.reshape(20, 20),
        surfacecolor=df["dH_evap_pred"].values.reshape(20, 20),
        colorscale=[[0, 'rgba(0, 0, 255, 0.5)'], [1, 'rgba(0, 0, 255, 0.5)']],
        opacity=0.5,
        name="dH_evap_pred"
    ))
    model_plot.add_trace(go.Surface(
        z=df["dH_cond_pred"].values.reshape(20, 20),
        x=df["T_e"].values.reshape(20, 20),
        y=df["T_c"].values.reshape(20, 20),
        surfacecolor=df["dH_cond_pred"].values.reshape(20, 20),
        colorscale=[[0, 'rgba(255, 0, 0, 0.5)'], [1, 'rgba(255, 0, 0, 0.5)']],
        opacity=0.5,
        name="dH_cond_pred"
    ))
    model_plot.add_trace(go.Surface(
        z=df["dH_comp_pred"].values.reshape(20, 20),
        x=df["T_e"].values.reshape(20, 20),
        y=df["T_c"].values.reshape(20, 20),
        surfacecolor=df["dH_comp_pred"].values.reshape(20, 20),
        colorscale=[[0, 'rgba(0, 255, 0, 0.5)'], [1, 'rgba(0, 255, 0, 0.5)']],
        opacity=0.5,
        name="dH_comp_pred"
    ))
    model_plot.update_layout(scene=dict(xaxis_title="T_e (°C)", yaxis_title="T_c (°C)", zaxis_title="dH (J/kg)"))

    error_plot = go.Figure()
    error_plot.add_trace(go.Scatter3d(x=df["T_e"], y=df["T_c"], z=df["dH_evap_err"], mode="markers", name="dH_evap_err"))
    error_plot.add_trace(go.Scatter3d(x=df["T_e"], y=df["T_c"], z=df["dH_cond_err"], mode="markers", name="dH_cond_err"))
    error_plot.add_trace(go.Scatter3d(x=df["T_e"], y=df["T_c"], z=df["dH_comp_err"], mode="markers", name="dH_comp_err"))
    error_plot.update_layout(scene=dict(xaxis_title="T_e (d°C)", yaxis_title="T_c (d°C)", zaxis_title="dH Error (fraction)"))

    return model_plot, error_plot

if __name__ == "__main__":
    for gas in gas_list:
        df = generate_dH_df(gas)
        model_evap, model_cond, model_comp = ols("dH_evap ~ T_e + T_c", data=df).fit(), ols("dH_cond ~ T_e + T_c", data=df).fit(), ols("dH_comp ~ 0 + T_e + T_c", data=df).fit() 

        df["dH_evap_pred"] = model_evap.predict(df)
        df["dH_evap_err"] = (df["dH_evap"] - df["dH_evap_pred"]) / df["dH_evap"]
        df["dH_cond_pred"] = model_cond.predict(df)
        df["dH_cond_err"] = (df["dH_cond"] - df["dH_cond_pred"]) / df["dH_cond"]
        df["dH_comp_pred"] = model_comp.predict(df)
        df["dH_comp_err"] = (df["dH_comp"] - df["dH_comp_pred"]) / df["dH_comp"]
        model_plot, error_plot = generate_dH_plots(df)    

        model_plot.write_image(f"python_helpers/output/power/{gas}_dH_model.png")
        model_plot.write_html(f"python_helpers/output/power/{gas}_dH_model.html")
        error_plot.write_image(f"python_helpers/output/power/{gas}_dH_error.png")
        error_plot.write_html(f"python_helpers/output/power/{gas}_dH_error.html")

        with open(f"python_helpers/output/power/{gas}_dH_model.txt", "w") as file:
            file.write(f"Evap Model: {model_evap.summary()}\n")
            file.write(f"Cond Model: {model_cond.summary()}\n")
            file.write(f"Comp Model: {model_comp.summary()}\n")