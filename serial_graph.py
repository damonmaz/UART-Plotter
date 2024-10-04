# -*- coding: utf-8 -*-
"""
CAPTURING 16 BIT INT AND FLOAT NUMBERS OVER A TIME INTERVAL
Time stamps received numbers separated by \n
Saves in Dataframe
Saves in csv
Plots received data versus time
"""

import numpy as np
import math 
import csv
import serial  # pip install pyserial  or conda install pyserial
import time
import pandas as pd
import plotly.express as px
import plotly.graph_objects as go
from plotly.subplots import make_subplots
from IPython.display import display

## IMPORTS BELOW ONLY NEEDED IF USING SPYDER IDE
import plotly.io as pio  # needed to plot plotly graphs in spyder
# pio.renderers.default = 'svg'  # to plot plotly graphs in spyder
pio.renderers.default = 'browser' # to plot plotly graphs in browser


## OPEN SERIAL PORT 
ser = serial.Serial(port= "COM9", baudrate = 9600, bytesize = 8, timeout =2, stopbits = serial.STOPBITS_ONE)

## INITIALIZATIONS
rxNumsStr = ''      #string to store received uint16_t numbers 
rxNumsList = []      #List to store received uint16_t numbers in int form 
rxVoltsList = []
rxTimesList = []   #list to store time stamps of received uint16_t numbers
startTime = time.time()   

## CAPTURE UART DATA
while(time.time() - startTime < 10):  #record data for 10 sec
    line =ser.readline() # reads uint16_t nums as single bytes till \n n stores in string
    if ((line != b' \n') and (line != b'\n')) : #removes any '\n' without num captures
        rxNumsStr = rxNumsStr + line.decode('Ascii')  # Converts string of received uint16_t num to ASCII and combines Rx nums into 1 string
        timeMeas = time.time() -startTime # Time stamp received number
        rxTimesList.append(timeMeas) #save time stamps in a list

## CLOSE SERIAL PORT    
ser.close()  # close any open serial ports

rxStr = rxNumsStr #checks
# print(rxStr)
# print(rxNumsStr)  
print(rxTimesList)


### Rx DATA CLEANUP AND STRING TO FLOAT CONVERSION
rxNumsStr = rxNumsStr.replace('\x00','')  #\x00 seems to be sent with Disp2String()
rxNumsStr = rxNumsStr.strip() # remove unwanted chars and spaces 
rxNumsList = rxNumsStr.split(' \n ')  # split string by \n n store in list
print(rxNumsList)
rxNumsList = [float(elem) for elem in rxNumsList]  # convert char in List into int

for i in rxNumsList:
    rxVoltsList.append(i * (3.0 / ((2 << 10)-1)))


### CONVERT Rx DATA INTO DATA FRAME
dF = pd.DataFrame()
dF['Time (sec)'] = rxTimesList
dF['Buffer'] = rxNumsList
dF['Voltage (V)'] = rxVoltsList

### DATA STATISTICS
print(dF.describe())
display(dF)


### COPY RX DATA AND RX TIME IN CSV AND XLS FILES
dF.to_csv('serialgraph.csv', index = True)

fig = make_subplots(rows=2, cols=1, subplot_titles=['Buffer vs Time', 'Voltage vs Time'])

# Add the first subplot (Buffer vs Time)
fig.add_trace(go.Scatter(x=dF['Time (sec)'], y=dF['Buffer'], mode='lines', name='Buffer'), row=1, col=1)

# Add the second subplot (Voltage vs Time)
fig.add_trace(go.Scatter(x=dF['Time (sec)'], y=dF['Voltage (V)'], mode='lines', name='Voltage'), row=2, col=1)

# Update layout
fig.update_layout(height=600, width=800, title_text="Buffer and Voltage vs Time", showlegend=True)

# Show the plot
fig.show()
