import tkinter
from tkinter import *
from tkinter import messagebox

import requests
import json

class Registration(tkinter.Frame):
    def __init__(self, params):
        super().__init__(highlightbackground="#2e3a23", highlightthickness=1, relief="groove")

        self.onLogin = params['onLogin']
        self.host = params['host']

        self.initView()

    font_header = ('Arial', 15)
    font_entry = ('Arial', 12)
    label_font = ('Arial', 11)
    base_padding = {'padx': 10, 'pady': 8}
    header_padding = {'padx': 10, 'pady': 12}

    def initView(self):
        self.main_label = Label(self, text='FIGHT CLUB', font=self.font_header, justify=CENTER, **self.header_padding)
        self.main_label.pack()

        self.username_label = Label(self, text='Логин', font=self.label_font , **self.base_padding)
        self.username_label.pack()

        self.username_entry = Entry(self, bg='#fff', fg='#444', font=self.font_entry)
        self.username_entry.pack()

        self.password_label = Label(self, text='Пароль', font=self.label_font , **self.base_padding)
        self.password_label.pack()

        self.password_entry = Entry(self, bg='#fff', fg='#444', font=self.font_entry)
        self.password_entry.pack()

        self.login_btn = Button(self, text='Войти', command=self.logIn)
        self.login_btn.pack(**self.base_padding)

        self.register_btn = Button(self, text='Регистрация', command=self.register)
        self.register_btn.pack(**self.base_padding)
    
    def logIn(self):
        login = self.username_entry.get()
        password = self.password_entry.get()

        response = 	requests.post(self.host + '/login', json={
            'login': login,
            'password': password
        })

        if response.status_code == 200:
            self.onLogin(login)
        else:
            messagebox.showinfo('Заголовок', 'Неверные данные для входа')

    def register(self):
        login = self.username_entry.get()
        password = self.password_entry.get()

        response = 	requests.post(self.host + '/register', json={
            'login': login,
            'password': password
        })

        if response.status_code == 200:
            self.onLogin(login)
        else:
            messagebox.showinfo('Заголовок', 'Произошла ошибка')
