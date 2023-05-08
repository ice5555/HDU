from flask import Flask, render_template, request
import pymysql
import pandas as pd
import folium
import os
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import pymysql.cursors

app = Flask(__name__)
conn = pymysql.connect(
        host='localhost',
        port=3306,
        user='root',
        password='123456',
        db='traffic_data'
)

def get_map_html(date=''):

    road_data = pd.read_sql("SELECT * FROM load_track_detail", conn)
    road_data.rename(columns={'id': 'road_id'}, inplace=True)

    if date:
        table_name = f"five_carflow{date}"
        carflow_data = pd.read_sql(f"SELECT * FROM {table_name}", conn)
        carflow_grouped = carflow_data.groupby('road_id').sum()
        data = pd.merge(road_data, carflow_grouped, on='road_id')
        data.dropna(inplace=True)
    else:
        
        data = road_data
        data.dropna(inplace=True)

    m = folium.Map(location=[data['lat1'].mean(), data['lng1'].mean()], zoom_start=12)

    for i, row in data.iterrows():
        latitudes = [row[f'lat{j}'] for j in range(1, 5)]
        longitudes = [row[f'lng{j}'] for j in range(1, 5)]
        locations = list(zip(latitudes, longitudes))
        folium.PolyLine(locations, color='blue', weight=5).add_to(m)
        if date:
            popup_html = f"road_id: {row['road_id']}<br>car_flow_in: {row['car_flow_in']}<br>car_flow_out: {row['car_flow_out']}"
            folium.Marker(location=(latitudes[0], longitudes[0]), popup=popup_html).add_to(m)

    map_html = m.get_root().render()

    return map_html



def get_chart(date,road_id):
    table_name = f"five_carflow{date}"
    sql_query = f"SELECT car_flow_out, car_flow_in, time_minute FROM {table_name} WHERE road_id = {road_id} ORDER BY time_minute"
    df = pd.read_sql_query(sql_query, con=conn)

    df["time_hour"] = df["time_minute"] // 60
    df_hourly = df.groupby("time_hour").agg({"car_flow_out": "sum", "car_flow_in": "sum"})
    plt.clf() 

    plt.plot(df_hourly.index, df_hourly["car_flow_out"], label="Out")
    plt.plot(df_hourly.index, df_hourly["car_flow_in"], label="In")
    plt.xlabel("Time (hour)")
    plt.ylabel("Traffic flow (vehicle/hour)")
    plt.title(f"Day {date} Road ID {road_id} Traffic Flow")
    plt.legend()

    img_folder = "static/"+date 
    if not os.path.exists(img_folder):
        os.makedirs(img_folder)
    img_path = img_folder + f"/{road_id}.png"
    plt.savefig(img_path)
    plt.clf()

    return img_path
  

def get_carflow_comparison(date=''):

    if date:
        table_name = f"five_carflow{date}"
        carflow_data = pd.read_sql(f"SELECT * FROM {table_name}", conn)
        carflow_grouped = carflow_data.groupby('road_id').sum()
   
    carflow_grouped['total_car_flow'] = carflow_grouped['car_flow_in'] + carflow_grouped['car_flow_out']
    sorted_data = carflow_grouped.sort_values(by='total_car_flow', ascending=False)

    top_10 = sorted_data.head(10)
    top_10 = top_10.sort_index() 
    fig, ax = plt.subplots()

    ax.bar(top_10.index, top_10['car_flow_in'], label='Car Flow In')
    ax.bar(top_10.index, top_10['car_flow_out'], bottom=top_10['car_flow_in'], label='Car Flow Out')
    ax.plot(top_10.index, top_10['total_car_flow'], label='ALL')

    for i, row in top_10.iterrows():
        ax.text(i, row['car_flow_in']-10000, f"{i}", ha="center", va="bottom", fontsize=8)
        ax.text(i, row['car_flow_in']+row['car_flow_out']+700, f"{row['car_flow_in']+row['car_flow_out']}", ha="center", va="bottom", fontsize=8)

    ax.set_xlabel('Road ID')
    ax.set_ylabel('Number of Cars')
    ax.set_title(f'Day {date} Top 10 Roads by Car Flow')

    if not os.path.exists('static/' + date):
        os.mkdir('static/' + date)
    fig.savefig(f'static/{date}/carflow_comparison.png')

    return f'{date}/carflow_comparison.png'


last_date=''
last_road_id=''
@app.route('/', methods=['GET', 'POST'])
def index():
    global last_date
    global last_road_id
    if request.method == 'POST':
        date = request.form['datepicker']
        if date!='':
            last_date=date
        print(date)
        print("1"+last_date)
        map_html = get_map_html(date)
        carflow_comparison = get_carflow_comparison(last_date)
        if 'road_id' in request.form and request.form['road_id']!='':
            road_id = request.form['road_id']
            date=last_date
            last_road_id=road_id
            map_html = get_map_html(date)
            img_path = get_chart(date,road_id)
            carflow = img_path.split("/")[-1]
            print(''+img_path+carflow)
        else:
            carflow = ''
            road_id =last_road_id
            print('lst'+road_id)
    else:
        date = ''
        map_html = get_map_html()
        carflow = ''
        carflow_comparison=''
        road_id=''
    return render_template('index.html', date=date, map_html=map_html, carflow=carflow,carflow_comparison=carflow_comparison,road_id=road_id)



if __name__ == '__main__':
    # app.run(debug=True)
    app.run()
