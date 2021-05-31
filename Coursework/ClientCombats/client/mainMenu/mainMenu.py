import tkinter
from tkinter import *
from tkinter import messagebox
import tkinter.ttk as ttk

import requests
import threading
from time import sleep

class MainMenu(tkinter.Frame):
    def __init__(self, onBattleFound, login, host):
        super().__init__(highlightbackground="#2e3a23", highlightthickness=1)

        self.onBattleFound = onBattleFound
        self.login = login
        self.host = host

        self.initView()
    
    font_header = ('Arial', 15)
    font_entry = ('Arial', 12)
    label_font = ('Arial', 11)
    base_padding = {'padx': 10, 'pady': 8}
    header_padding = {'padx': 10, 'pady': 12}

    def initView(self):
        text = 'Вы успешно вошли.\n Теперь нажмите кнопку "В бой"\n и победите своего соперника!'
        self.main_label = Label(self, text= text, font=self.font_header, justify=CENTER, **self.header_padding)

        self.main_label.pack()

        self.fight_btn = Button(self, text='В бой', command=self.findBattle)
        self.fight_btn.pack(**self.base_padding)
        
    def findBattle(self):
        self.fight_btn.pack_forget()

        self.progress_bar = ttk.Progressbar(self, orient="horizontal",
                              mode="indeterminate", length=150)
        self.progress_bar.pack(**self.base_padding)
        self.progress_bar.start()

        threading.Thread(target=self.requestBattle).start()

    def requestBattle(self):
        response = 	requests.post(self.host + '/findBattle', json={
            'login': self.login
        })

        if response.status_code != 200:
            sleep(1)
            self.requestBattle()
        else:
            jsonResponse = response.json()

            playerOne = jsonResponse['playerOne']
            playerTwo = jsonResponse['playerTwo']
            battleId = jsonResponse['battleId']

            self.progress_bar.stop()
            self.progress_bar.pack_forget()
            self.fight_btn.pack(**self.base_padding)

            self.onBattleFound(playerOne, playerTwo, battleId, self.login)
