import tkinter
from tkinter import *

from registrationPage.registrationPage import Registration
from mainMenu.mainMenu import MainMenu
from battlePage.battlePage import BattlePage

class App(tkinter.Frame):
    host = 'http://47734bb8cb89.ngrok.io'
    def __init__(self, master=None):
        super().__init__(master)

        self.registration = Registration({
            'onLogin': self.openMenu,
            'host': self.host
        })
        self.registration.pack(pady = 200)

        self.pack()

    def openMenu(self, login):
        self.registration.pack_forget()
        self.mainMenu = MainMenu(self.startFight, login, self.host)
        self.mainMenu.pack(pady = 200)

    def startFight(self, playerOne, playerTwo, battleId, login):
        self.mainMenu.pack_forget()
        self.battlePage = BattlePage({
            'host': self.host,
            'login': login,
            'battleId': battleId,
            'playerOne': playerOne,
            'playerTwo': playerTwo
        }, self.returnToMenu)
        self.battlePage.pack()

    def returnToMenu(self):
        self.battlePage.pack_forget()
        self.mainMenu.pack(pady = 200)

def main():
    myapp = App()
    myapp.master.title('Combats Game')
    myapp.master.geometry('1200x800')
    myapp.master.mainloop()

if __name__ == '__main__':
    main()