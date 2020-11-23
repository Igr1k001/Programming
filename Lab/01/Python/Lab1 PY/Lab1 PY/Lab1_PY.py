from tkinter import *
import requests
import json

def loadWeather():
    weatherRequest = requests.get('http://localhost:3000/raw')
    return weatherRequest.json()

def updateWidget(event):
    weather = loadWeather()

    temperature = int(weather['temp'])
    description = weather['description'].encode('l1').decode()

    desc.config(text=description)
    temp.config(text=str(temperature) + "°C")

root = Tk()
root.title("Погода")
root.pack_propagate(0)

root.bind('<Button-3>', updateWidget)

weather = loadWeather()
temperature = int(weather['temp'])
description = weather['description'].encode('l1').decode()

top_frame = Frame(root, bg="darkgoldenrod1", width=100, height=30)
middle_frame = Frame(root, bg="white",  width=100, height=90)
bottom_frame = Frame(root, bg="darkgoldenrod1", width=100, height=30)

top_frame.pack(side=TOP, fill=X)
middle_frame.pack(expand=True, fill=BOTH)
bottom_frame.pack(side=BOTTOM, fill=X)


city = Label(top_frame, font=("Calibri Bold", 12), text="Симферополь", bg="darkgoldenrod1")
desc = Label(top_frame, font=("Calibri", 12), text = description, bg="darkgoldenrod1")
temp = Label(middle_frame, font=("Impact", 48), text = str(temperature) + "°C", bg="white")


city.pack(pady=0)
desc.pack(pady=0)
temp.pack(expand=True)


root.mainloop()