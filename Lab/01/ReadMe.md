<p align="center">МИНИСТЕРСТВО НАУКИ  И ВЫСШЕГО ОБРАЗОВАНИЯ РОССИЙСКОЙ ФЕДЕРАЦИИ<br>
Федеральное государственное автономное образовательное учреждение высшего образования<br>
"КРЫМСКИЙ ФЕДЕРАЛЬНЫЙ УНИВЕРСИТЕТ им. В. И. ВЕРНАДСКОГО"<br>
ФИЗИКО-ТЕХНИЧЕСКИЙ ИНСТИТУТ<br>
Кафедра компьютерной инженерии и моделирования</p>
<br>
<h3 align="center">Отчёт по лабораторной работе № 1<br> по дисциплине "Программирование"</h3>
<br><br>
<p>студента 1 курса группы ПИ-б-о-202(2)<br>
Поливода Игоря Сергеевича<br>
направления подготовки 09.03.0X "Название направления подготовки"</p>
<br><br>
<table>
<tr><td>Научный руководитель<br> старший преподаватель кафедры<br> компьютерной инженерии и моделирования</td>
<td>(оценка)</td>
<td>Чабанов В.В.</td>
</tr>
</table>
<br><br>
<p align="center">Симферополь, 2020</p>
<hr>						

## Цель:
1. Закрепить навыки разработки многофайловыx приложений;
2. Изучить способы работы с API web-сервиса;
3. Изучить процесс сериализации/десериализации данных в/из json;
4. Получить базовое представление о сетевом взаимодействии приложений;
## Постановка задачи
Разработать сервис предоставляющий данные о погоде в городе Симферополе на момент запроса. В качестве источника данных о погоде использовать: http://openweathermap.org/. В состав сервиса входит: серверное приложение на языке С++ и клиентское приложение на языке Python. Серверное приложение (далее Сервер) предназначенное для обслуживания клиентских приложений и минимизации количества запросов к сервису openweathermap.org. Сервер должен обеспечивать возможность получения данных в формате JSON и виде html виджета (для вставки виджета на страницу будет использоваться iframe). Клиентское приложение должно иметь графический интерфейс отображающий сведения о погоде и возможность обновления данных по требованию пользователя.

## Выполнение работы
Для начала был зарегистрирован аккаунт openwheatermap.org. На этом сервере был создан API-ключ: 81bd532f5074849455017bcd3a66171a.

Затем были произведены несколько тестовых запросов.

Далее был создан запрос к серверу погоды с координатами г.Симферополя,ответ в градусах цельсия и т.д:

https://api.openweathermap.org/data/2.5/onecall?lat=44.952116&lon=34.102411&units=metric&lang=ru&exclude=daily&appid=81bd532f5074849455017bcd3a66171a

Запрос для сервера погоды включает в себя город, время которого необходимо получить:

http://worldtimeapi.org/api/timezone/Europe/Simferopol

Код сервера:

	#include <iostream>
	#include <cpp_httplib/httplib.h>
	#include <nlohmann/json.hpp>
	using json = nlohmann::json;
	using namespace httplib;
	using namespace std;

	int get_time() {
		Client cli("http://worldtimeapi.org");
		auto res = cli.Get("/api/timezone/Europe/Simferopol");
		if (res) {
			if (res->status == 200) {
				json j = json::parse(res->body);
				return j["unixtime"];
			}
		}

	}

	json get_weather(int time) {
		Client cli("http://api.openweathermap.org");
		auto res = cli.Get("/data/2.5/onecall?lat=44.952116&lon=34.102411&units=metric&lang=ru&exclude=daily&appid=81bd532f5074849455017bcd3a66171a");
		if (res) {
			if (res->status == 200) {
				json j = json::parse(res->body);
				json hourly = j["hourly"].get<json>();

				json hour_forecast;

				int last = hourly.size() - 1;

				if (hourly[last]["dt"] < time) {
					return json::object();
				}

				for (int i = 0; i <= last; i++) {
					if (hourly[i]["dt"] >= time) {
						return hourly[i];
					}
				}

				return json::object();
			} else {
				return json::object();
			}
		} else {
			return json::object();
		}
	}

	// В этой функции формируем ответ сервера на запрос
	void gen_response(const Request& req, Response& res) {
		// Команда set_content задаёт ответ сервера и тип ответа:
		// Hello, World! - тело ответа
		// text/plain - MIME тип ответа (в данном случае обычный текст)
		res.set_content(to_string(get_time()), "text/plain");
	}

	void raw_response(const Request& req, Response& res) {
		int current_time = get_time();
		json current_weather = get_weather(current_time);

		json result;

		result["temp"] = current_weather["temp"];
		result["description"] = current_weather["weather"][0]["description"];

		res.set_content(result.dump(4), "text/json");
	}

	int main() {
		Server svr;                    // Создаём сервер (пока-что не запущен)
		svr.Get("/", gen_response);    // Вызвать функцию gen_response если кто-то обратиться к корню "сайта"
		svr.Get("/raw", raw_response);
		std::cout << "Start server... OK\n";
		svr.listen("localhost", 3000); // Запускаем сервер на localhost и порту 1234
	}
Исходный код клиента:

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

Скриншот клиента:

![](C:\Users\innap\source\Lab.png)

Заключение:
Цель работы была успешно достигнута. Было выполнено:

- Создание сервера на языке С++, обращающегося к openweathermap.com и возвращающий виджет или описание и температуру в формате json
- Приложение с графическим интерфейсом, написанное на языке Python с использованием библиотеки Tkinter, получающее и обрабатывающее данные из сервера.