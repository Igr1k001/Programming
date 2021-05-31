import json
import tkinter
from tkinter import *
import tkinter.ttk as ttk
from tkinter import messagebox
from functools import partial

import requests
import threading
from time import sleep

class BattlePage(tkinter.Frame):
    def __init__(self, params, onEndCallback):
        super().__init__()

        self.onEndCallback = onEndCallback

        self.host = params['host']
        self.battleId = params['battleId']
        self.login = params['login']
        self.playerOne = params['playerOne']
        self.playerTwo = params['playerTwo']
        self.whichPlayer = 'playerOne' if self.playerOne == self.login else 'playerTwo'
        self.enemy = self.playerOne if self.playerOne != self.login else self.playerTwo

        self.initView()

    targetMappings = {
        'head': 101,
        'torso': 102,
        'groin': 103,
        'legs': 104
    }

    moveParams = {
        'attackTarget': None,
        'defenceTarget': None
    }

    moveCount = 0
    
    font_header = ('Arial', 15)
    font_entry = ('Arial', 12)
    label_font = ('Arial', 11)
    base_padding = {'padx': 10, 'pady': 8}
    header_padding = {'padx': 10, 'pady': 12}

    def initView(self):
        self.main_label = Label(self, text="Бой", font=self.font_header, justify=CENTER, **self.header_padding)
        self.main_label.pack()

        self.send_btn = Button(self, text='Завершить ход', command=self.endMove)
        self.send_btn.pack(side=TOP)

        self.playersFighter = Fighter(self.login, self.setDefenceTarget, False)
        self.playersFighter.pack(side=LEFT, padx=200)

        self.enemyFighter = Fighter(self.enemy, self.setAttackTarget, True)
        self.enemyFighter.pack(side=RIGHT, padx=200)

    def setAttackTarget(self, target):
        self.moveParams['attackTarget'] = target

    def setDefenceTarget(self, target):
        self.moveParams['defenceTarget'] = target

    def endMove(self):
        attackTarget = self.moveParams['attackTarget']
        defenceTarget = self.moveParams['defenceTarget']

        if not(defenceTarget) or not(attackTarget):
            return

        response = requests.post(self.host + '/battle', json={
            'battleId': self.battleId,
            'login': self.login,
            'moveCount': self.moveCount,
            'attackTarget': self.targetMappings[attackTarget],
            'defenceTarget': self.targetMappings[defenceTarget]
        })

        self.send_btn.config(state=DISABLED)

        threading.Thread(target=self.getMoveResults).start()

    def getMoveResults(self):
        response = requests.get(self.host + '/battle', params={
            'battleId': self.battleId,
            'moveCount': self.moveCount,
            'login': self.login
        })

        if response.status_code != 200:
            sleep(1)
            self.getMoveResults()
        else:
            jsonResponse = response.json()
            print(jsonResponse)

            if self.whichPlayer == 'playerOne':
                playersHp = jsonResponse['playerOneHp']
                enemyHp = jsonResponse['playerTwoHp']
            else:
                playersHp = jsonResponse['playerTwoHp']
                enemyHp = jsonResponse['playerOneHp']

            self.playersFighter.changeHp(playersHp)
            self.enemyFighter.changeHp(enemyHp)

            if jsonResponse['gameFinished']:
                isVictory = jsonResponse[self.whichPlayer + 'Hp'] > 0
                messagebox.showinfo('Заголовок', 'Вы победили' if isVictory else 'Вы проиграли')

                self.endBattle()

            self.moveCount += 1

            self.send_btn.config(state=ACTIVE)
            self.playersFighter.unHighlightBodyPart(self.moveParams['defenceTarget'])
            self.playersFighter.dropChosenTarget()
            self.enemyFighter.unHighlightBodyPart(self.moveParams['attackTarget'])
            self.enemyFighter.dropChosenTarget()
            self.moveParams['defenceTarget'] = None
            self.moveParams['attackTarget'] = None

    def endBattle(self):
        self.playersFighter.pack_forget()
        self.enemyFighter.pack_forget()
        self.main_label.pack_forget()
        self.send_btn.pack_forget()
        self.onEndCallback()
   
class Fighter(tkinter.Frame):
    def __init__(self, nickname, setTarget, enemy):
        super().__init__(highlightbackground="#2e3a23", highlightthickness=1, width=300, height=600)

        self.nickname = nickname
        self.setTarget = setTarget
        self.enemy = enemy

        self.initView()

    chosenTarget = None

    font_header = ('Arial', 15)
    font_entry = ('Arial', 12)
    label_font = ('Arial', 11)
    base_padding = {'padx': 10, 'pady': 8}
    header_padding = {'padx': 10, 'pady': 12}

    def initView(self):
        self.nickName = Label(self, text=self.nickname, font=self.font_header, justify=CENTER, **self.header_padding)
        self.nickName.pack()

        self.progress_bar = ttk.Progressbar(self, orient="horizontal",
                              mode="determinate", maximum=100, value=100, length=150)
        self.progress_bar.pack()

        self.body = Frame(self)

        self.body.parts = {
            'head': Button(self.body, text='Голова', height=5, width=15, command=partial(self.onBodyClick, 'head')),
            'torso': Button(self.body, text='Торс', height=5, width=15, command=partial(self.onBodyClick, 'torso')),
            'groin': Button(self.body, text='Пах', height=5, width=15, command=partial(self.onBodyClick, 'groin')),
            'legs': Button(self.body, text='Ноги', height=5, width=15, command=partial(self.onBodyClick, 'legs'))
        }

        self.body.parts['head'].pack()
        self.body.parts['torso'].pack()
        self.body.parts['groin'].pack()
        self.body.parts['legs'].pack()

        self.body.pack()

    def changeHp(self, value):
        if value <= 0:
            self.progress_bar['value'] = 0    
        else:
            self.progress_bar['value'] = value

    def unHighlightBodyPart(self, target):
        self.body.parts[target].config(highlightbackground="#d9d9d9")

    def dropChosenTarget(self):
        self.chosenTarget = None

    def onBodyClick(self, target):
        if self.chosenTarget == target:
            return

        self.body.parts[target].config(
            highlightbackground = "#cc0605" if self.enemy else "#007dff"
        )
        self.chosenTarget and self.unHighlightBodyPart(self.chosenTarget)
        
        self.chosenTarget = target
        self.setTarget(target)
