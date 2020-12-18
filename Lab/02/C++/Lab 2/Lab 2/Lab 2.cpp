#include <iostream>
#include <string>
#include <cpp_httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;
using namespace std;
using namespace httplib;

const string config_path = "config.json";

json config;
 
void gen_response(const Request& req, Response& res) {
    res.set_content(u8R"(Сервер запущен)", "text/plain; charset=UTF-8");
}

string read_fragment_from_disk() {
    string path = "FragmentWEB.html";

    string html;
    ifstream file;

    file.open(path);
    if (!file.is_open()) {
        return "Ошибка чтения файла";
    }

    getline(file, html, '\0');

    file.close();

    return html;
}

string create_webhooks_list() {
    string const url_placeholder = "Webhook URL";

    string htmlFragment = read_fragment_from_disk();
    string result;

    for (string webhook : config["webhooks"]) {
        string fragment = htmlFragment;

        size_t found = fragment.find(url_placeholder);
        fragment.replace(found, url_placeholder.length(), webhook);

        size_t found2 = fragment.find(url_placeholder);
        fragment.replace(found2, url_placeholder.length(), webhook);

        result = result + fragment;
    }

    return result;
}

void get_webhooks_html(const Request& req, Response& res) {
    string path = "Webhooks.html";
    string webhooks_placeholder = "{webhooks_list}";

    string html;
    ifstream file;
    file.open(path);

    if (!file.is_open()) {
        res.set_content("Ошибка считывания файла", "text/plain");
        return;
    }

    getline(file, html, '\0');

    file.close();

    cout << config << endl;

    string webhooks_list = create_webhooks_list();

    size_t found = html.find(webhooks_placeholder);
    html.replace(found, webhooks_placeholder.length(), webhooks_list);

    res.set_content(html, "text/html");
}

void edit_webhooks(const Request& req, Response& res) {
    if (req.has_param("set")) {
        auto val = req.get_param_value("set");

        config["webhooks"].push_back(val);

        get_webhooks_html(req, res);
    }
    else if (req.has_param("del")) {
        auto val = req.get_param_value("del");

        auto webhooks = config["webhooks"].get<vector<string>>();

        vector<string>::iterator it = find(webhooks.begin(), webhooks.end(), val);
        if (it != webhooks.end()) webhooks.erase(it);

        config["webhooks"] = webhooks;

        get_webhooks_html(req, res);
    }
    else {
        res.set_content("Неправильные параметры запроса", "text/plain");
    }

    ofstream config_file;

    config_file.open(config_path);

    if (!config_file.is_open()) {
        cout << "Ошибка открытия конфиг файла" << endl;
    }
    else {
        config_file << config.dump(4);
    }

    config_file.close();
}

json get_config() {
    json config;
    ifstream file;
    
    file.open(config_path);

    if (file.is_open()) {
        string config_string;

        getline(file, config_string, '\0');
        config = json::parse(config_string);
    } 
    else {
        config["webhooks"] = json::array();
        
        ofstream config_file;

        config_file.open(config_path);

        if (!config_file.is_open()) {
            cout << "Ошибка создания конфиг файла" << endl;
        }
        else {
            config_file << config.dump(4);
        }

        config_file.close();
    }

    file.close();

    return config;
}

json help_response(string text) {
    json response = json::object();

    if (text.find(u8R"(выйти)") != -1) {
        response["text"] = u8R"(Обращайся ещё)";
        response["tts"] = u8R"(Обращайся ещё)";
        response["end_session"] = false;
    }
    else if (text.find(u8R"(говорить)") != -1) {
        response["text"] = u8R"(Ответы будут дублироваться голосом)";
        response["tts"] = u8R"(Ответы будут дублироваться голосом)";
        response["end_session"] = false;
    }
    else if (text.find(u8R"(без звука)") != -1) {
        response["text"] = u8R"(Навык перейдет в беззвучный режим)";
        response["tts"] = u8R"(Навык перейдет в беззвучный режим)";
        response["end_session"] = false;
    }
    else if (text.find(u8R"(добавить в корзину)") != -1) {
        response["text"] = u8R"(Эта команда добовляет товар в корзину)";
        response["tts"] = u8R"(Эта команда добовляет товар в корзину)";
        response["end_session"] = false;
    }
    else if (text.find(u8R"(убрать из корзины)") != -1) {
        response["text"] = u8R"(Эта команда убирает все добавленные товары из корзины)";
        response["tts"] = u8R"(Эта команда убирает все добавленные товары из корзины)";
        response["end_session"] = false;
    }
    else if (text.find(u8R"(сумма)") != -1) {
        response["text"] = u8R"(Эта команда выводит общую стоимость товаров в корзине)";
        response["tts"] = u8R"(Эта команда выводит общую стоимость товаров в корзине)";
        response["end_session"] = false;
    }
    else if (text.find(u8R"(что в корзине)") != -1) {
        response["text"] = u8R"(Эта команда показывает список товаров добавленных в корзину)";
        response["tts"] = u8R"(Эта команда показывает список товаров добавленных в корзину)";
        response["end_session"] = false;
    }
    else if (text.find(u8R"(покупка завершена)") != -1) {
        response["text"] = u8R"(Эта команда закрывает виртуальную корзину)";
        response["tts"] = u8R"(Эта команда закрывает виртуальную корзину)";
        response["end_session"] = false;
    }
    else {
        response["text"] = u8R"(Такой команды нет)";
        response["tts"] = u8R"(Такой команды нет)";
        response["end_session"] = false;
    }

    return response;
}

json help_buttons = {
    {
        {"title", u8R"(Говорить)"},
        {"hide", true}
    },
    {
        {"title", u8R"(Без звука)"},
        {"hide", true}
    },
    {
        {"title", u8R"(Добавить в корзину)"},
        {"hide", true}
    },
    {
        {"title", u8R"(Убрать из корзины)"},
        {"hide", true}
    },
    {
        {"title", u8R"(Сумма)"},
        {"hide", true}
    },
    {
        {"title", u8R"(Что в корзине)"},
        {"hide", true}
    },
    {
        {"title", u8R"(Покупка завершена)"},
        {"hide", true}
    },
    {
        {"title", u8R"(Выйти из помощи)"},
        {"hide", true}
    },
};

void alice_response(const Request& req, Response& res) {
    json aliceData = json::parse(req.body);
    bool is_new_session = aliceData["session"]["new"];
    string request_text = aliceData["request"]["command"];

    json result;

    result["version"] = "1.0";

    json response = json::object();
    json session_state = json::object();

    if (is_new_session) {
        response["text"] = u8R"(Здравствуйте! Я помогу вам с покупками.)";
        response["tts"] = u8R"(Здравствуйте! Я помогу вам с покупками.)";
        response["end_session"] = false;

        json help_button = json::object();
        help_button["title"] = u8R"(Помощь)";
        help_button["hide"] = true;

        json mute_button = json::object();
        mute_button["title"] = u8R"(Без звука)";
        mute_button["hide"] = true;

        response["buttons"] = json::array();
        response["buttons"].push_back(help_button);
        response["buttons"].push_back(mute_button);

        session_state["voice"] = true;
        session_state["mode"] = "main";
        session_state["cart"] = json::array();

        result["response"] = response;
        result["session_state"] = session_state;

        res.set_content(result.dump(4), "text/json; charset=UTF-8");
        return;
    }

    json current_state = aliceData["state"]["session"];

    if (current_state.empty()) {
        response["text"] = u8R"(Данные сессии невалидны. Сессия завершена.)";
        response["tts"] = u8R"(Данные сессии невалидны. Сессия завершена.)";
        response["end_session"] = true;

        result["response"] = response;

        res.set_content(result.dump(4), "text/json; charset=UTF-8");
        return;
    }

    json cart = current_state["cart"];
    string mode = current_state["mode"];
    bool voice = current_state["voice"];

    if (mode == "help") {
        json response = help_response(request_text);

        session_state["voice"] = voice;
        session_state["mode"] = request_text.find(u8R"(выйти)") != -1 ? "main" : mode;
        session_state["cart"] = cart;

        if (!voice) {
            response["tts"] = " ";
        }

        response["buttons"] = help_buttons;

        result["response"] = response;
        result["session_state"] = session_state;

        res.set_content(result.dump(4), "text/json; charset=UTF-8");
        return;
    }

    if (request_text.find(u8R"(помощь)") != -1) {
        mode = "help";

        response["text"] = u8R"(Корзина. Поможет организовать покупки. О чём рассказать еще?)";
        response["tts"] = u8R"(Корзина. Поможет организовать покупки. О чём рассказать еще?)";
        response["end_session"] = false;

        session_state["voice"] = voice;
        session_state["mode"] = mode;
        session_state["cart"] = cart;

        if (!voice) {
            response["tts"] = " ";
        }

        response["buttons"] = help_buttons;

        result["response"] = response;
        result["session_state"] = session_state;

        res.set_content(result.dump(4), "text/json; charset=UTF-8");
        return;
    }

    if (request_text.find(u8R"(без звука)") != -1) {
        voice = false;

        response["text"] = u8R"(Молчу, молчу)";
        response["end_session"] = false;
    }
    else if (request_text.find(u8R"(говори)") != -1) {
        voice = true;

        response["text"] = u8R"(Хорошо)";
        response["tts"] = u8R"(Хорошо)";
        response["end_session"] = false;
    }
    else if (request_text.find(u8R"(добавить в корзину)") == 0) {
        json nlu = aliceData["request"]["nlu"];
        json entities = nlu["entities"];
        json tokens = nlu["tokens"];

        int price = 0;
        int price_index;

        for (json item : entities) {
            if (item["type"] == "YANDEX.NUMBER") {
                price = item["value"];
                price_index = item["tokens"]["start"];
                break;
            }
        }

        if (price == 0) {
            response["text"] = u8R"(Вы не указали стоимость)";
            response["tts"] = u8R"(Вы не указали стоимость)";
            response["end_session"] = false;
        }
        else {
            string name = u8R"()";

            for (int i = 3; i < price_index; i++) {
                name += tokens[i];

                if (i + 1 != price_index) name += " ";
            }

            if (name == u8R"()") {
                response["text"] = u8R"(Вы не указали название товара)";
                response["tts"] = u8R"(Вы не указали название товара)";
                response["end_session"] = false;
            }
            else {
                json item = json::object();
                item["item"] = name;
                item["price"] = price;

                cart.push_back(item);

                response["text"] = u8R"(Окей)";
                response["tts"] = u8R"(Окей)";
                response["end_session"] = false;
            }
        }
    }
    else if (request_text.find(u8R"(удалить из корзины)") == 0) {
        json nlu = aliceData["request"]["nlu"];
        json tokens = nlu["tokens"];

        string item_name = u8R"()";
        int size = tokens.size();

        for (int i = 3; i < size; i++) {
            item_name += tokens[i];

            if (i + 1 != size) item_name += " ";
        }

        int cart_size = cart.size();
        int cart_index = 999999;

        for (int i = 0; i < cart_size; i++) {
            if (cart[i]["item"] == item_name) {
                cart_index = i;
                break;
            }
        }

        if (cart_index == 999999) {
            response["text"] = u8R"(Такого товара нет в корзине)";
            response["tts"] = u8R"(Такого товара нет в корзине)";
            response["end_session"] = false;
        }
        else {
            cart.erase(cart.begin() + cart_index);

            response["text"] = u8R"(Окей)";
            response["tts"] = u8R"(Окей)";
            response["end_session"] = false;
        }
    }
    else if (request_text.find(u8R"(очистить корзину)") != -1) {
        cart = json::array();

        response["text"] = u8R"(Корзина пуста)";
        response["tts"] = u8R"(Корзина пуста)";
        response["end_session"] = false;
    }
    else if (request_text.find(u8R"(что в корзине)") != -1) {
        string result = u8R"()";

        if (cart.empty()) {
            result += u8R"(Корзина пуста)";
        }
        else {
            for (json item : cart) {
                result += item["item"];
                result += " ";
                result += to_string(item["price"]);
                result += u8R"( рублей)";
                result += "\n";
            }
        }

        response["text"] = result;
        response["tts"] = result;
        response["end_session"] = false;
    }
    else if (request_text.find(u8R"(сумма)") != -1) {
        int sum = 0;

        for (json item : cart) {
            sum += item["price"];
        }

        response["text"] = to_string(sum) + u8R"( рублей)";
        response["tts"] = to_string(sum) + u8R"( рублей)";
        response["end_session"] = false;
    }
    else if (request_text.find(u8R"(покупка завершена)") != -1) {
        json webhooks_data = json::object();

        webhooks_data["user_id"] = aliceData["session"]["user"]["user_id"];
        webhooks_data["check"] = cart;

        //for (string webhook : config["webhooks"]) {
        //    //TODO Отправка на вебхуки из списка
        //    Client cli("http://localhost:5000");
        //    cli.Post("/", webhooks_data.dump(4), "application/json");
        //}

        for (string webhook : config["webhooks"]) {
            // начинаем искать слэш после http://
            int slash_index = webhook.find('/', 7);

            Client cli(webhook.substr(0, slash_index).c_str());
            cli.Post(webhook.substr(slash_index, -1).c_str(), webhooks_data.dump(4), "application/json");
        }

        response["text"] = u8R"(Заходите ещё)";
        response["tts"] = u8R"(Заходите ещё)";
        response["end_session"] = true;
    }
    else {
        response["text"] = u8R"(Мне неизвестна эта команда)";
        response["tts"] = u8R"(Мне неизвестна эта команда)";
        response["end_session"] = false;
    }

    session_state["voice"] = voice;
    session_state["mode"] = mode;
    session_state["cart"] = cart;

    if (!voice) {
        response["tts"] = " ";
    }

    json help_button = json::object();
    help_button["title"] = u8R"(Помощь)";
    help_button["hide"] = true;

    json voice_button = json::object();
    voice_button["title"] = voice ? u8R"(Без звука)" : u8R"(Говорить)";
    voice_button["hide"] = true;

    response["buttons"] = json::array();
    response["buttons"].push_back(help_button);
    response["buttons"].push_back(voice_button);

    result["response"] = response;
    result["session_state"] = session_state;

    res.set_content(result.dump(4), "text/json; charset=UTF-8");
}

int main() {
    config = get_config();

    Server svr;

    svr.Get("/webhooks", get_webhooks_html);
    svr.Post("/webhooks", edit_webhooks);
    svr.Post("/alice", alice_response);
    svr.Get("/", gen_response);

    std::cout << "Start server... OK\n"; // cout использовать нельзя

    svr.listen("localhost", 1234);
}
