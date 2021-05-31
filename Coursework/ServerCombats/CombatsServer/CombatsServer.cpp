#include <iostream>
#include <string>
#include <cpp_httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <map>

using json = nlohmann::json;
using namespace std;
using namespace httplib;

const string database_path = "database.json";

const int HEAD_DAMAGE = 25;
const int TORSO_DAMAGE = 10;
const int GROIN_DAMAGE = 15;
const int LEGS_DAMAGE = 5;

json database;
vector<string> waitingUsers;

struct moveResult {
    int firstPlayerHp;
    int secondPlayerHp;
};

struct move {
    int attackTarget;
    int defenceTarget;
};

struct moveSet {
    struct move firstPlayerMove;
    struct move secondPlayerMove;
};

struct battle {
    int finishInformed;
    int id;
    string firstPlayer;
    int firstPlayerHp;
    string secondPlayer;
    int secondPlayerHp;
    vector<moveResult> movesResults;
    vector<json> moves;
};

map<string, int> usersBattleIds;
map<int, battle> currentBattles;
int lastBattleId = 0;

void gen_response(const Request& req, Response& res) {
    res.set_content(u8R"(Сервер запущен)", "text/plain; charset=UTF-8");
}

void login_handler(const Request& req, Response& res) {
    if (req.body.empty()) {
        res.status = 400;
        res.set_content(u8R"(В запросе нет данных)", "text/plain; charset=UTF-8");

        return;
    }

    json loginData = json::parse(req.body);

    string login = loginData["login"];
    string password = loginData["password"];

    boolean correctUser = false;

    for (json user : database["users"]) {
        if (user["login"] != login) {
            continue;
        }

        if (user["password"] == password) {
            correctUser = true;
        }
    }

    if (!correctUser) {
        res.status = 401;
        res.set_content(u8R"(Неверные данные пользователя)", "text/plain; charset=UTF-8");

        return;
    }

    res.status = 200;
    res.set_content(u8R"(Вход выполнен)", "text/plain; charset=UTF-8");
}

void register_handler(const Request& req, Response& res) {
    if (req.body.empty()) {
        res.status = 400;
        res.set_content(u8R"(В запросе нет данных)", "text/plain; charset=UTF-8");

        return;
    }

    json loginData = json::parse(req.body);

    string login = loginData["login"];
    string password = loginData["password"];

    json user = json::object();
    user["login"] = login;
    user["password"] = password;

    database["users"].push_back(user);

    res.status = 200;
    res.set_content(u8R"(Пользователь заругистрирован)", "text/plain; charset=UTF-8");

    ofstream database_file;

    database_file.open(database_path);

    if (!database_file.is_open()) {
        cout << "Ошибка открытия конфиг файла" << endl;
    }
    else {
        database_file << database.dump(4);
    }

    database_file.close();
}

int createBattle(string firstPlayer, string secondPlayer) {
    int id = lastBattleId + 1;

    currentBattles[id] = battle{ 0, id, firstPlayer, 100, secondPlayer, 100 };
    usersBattleIds[firstPlayer] = id;
    usersBattleIds[secondPlayer] = id;

    return id;
}

void findBattle_handler(const Request& req, Response& res) {
    if (req.body.empty()) {
        res.status = 400;
        res.set_content(u8R"(В запросе нет данных)", "text/plain; charset=UTF-8");

        return;
    }

    json reqData = json::parse(req.body);
    string login = reqData["login"];

    if (usersBattleIds.count(login) == 1) {
        int battleId = usersBattleIds[login];
        battle currentBattle = currentBattles[battleId];

        json result = json::object();
        result["battleId"] = battleId;
        result["playerOne"] = currentBattle.firstPlayer;
        result["playerTwo"] = currentBattle.secondPlayer;        

        res.status = 200;
        res.set_content(result.dump(4), "text/json; charset=UTF-8");

        return;
    }

    if (waitingUsers.size() == 0) {
        waitingUsers.push_back(login);

        res.status = 202;
        res.set_content(u8R"(Добавили в очередь)", "text/plain; charset=UTF-8");

        return;
    } else if (waitingUsers.size() == 1) {
        if (waitingUsers[0] == login) {
            res.status = 202;
            res.set_content(u8R"(Ждем противника)", "text/plain; charset=UTF-8");

            return;
        }
        else {
            int id = createBattle(waitingUsers[0], login);
            waitingUsers.clear();

            battle currentBattle = currentBattles[id];
            
            json result = json::object();
            result["battleId"] = id;
            result["playerOne"] = currentBattle.firstPlayer;
            result["playerTwo"] = currentBattle.secondPlayer;

            res.status = 200;
            res.set_content(result.dump(4), "text/json; charset=UTF-8");

            return;
        }
    }
}

void battle_handler(const Request& req, Response& res) {
    if (req.body.empty()) {
        res.status = 400;
        res.set_content(u8R"(В запросе нет данных)", "text/plain; charset=UTF-8");

        return;
    }

    json reqData = json::parse(req.body);

    string login = reqData["login"];
    int battleId = reqData["battleId"];
    int moveCount = reqData["moveCount"];
    int attackTarget = reqData["attackTarget"];
    int defenceTarget = reqData["defenceTarget"];

    if (currentBattles.count(battleId) != 1) {
        res.status = 404;
        res.set_content(u8R"(Битвы с этим идентификатором не существует)", "text/plain; charset=UTF-8");

        return;
    }

    // у меня не получилось сделать обращение к нужным элементам через указатель
    // теперь я не сохраняю укуазатель в переменную а обращаюсь постоянно в map
    // это плохо надо чето делать
    //battle* currentBattle = &currentBattles[battleId];

    string whichPlayer;

    if (login == currentBattles[battleId].firstPlayer) {
        whichPlayer = "firstPlayerMove";
    }
    else if (login == currentBattles[battleId].secondPlayer) {
        whichPlayer = "secondPlayerMove";
    }
    else {
        res.status = 404;
        res.set_content(u8R"(В данной битве нет такого пользователя)", "text/plain; charset=UTF-8");

        return;
    }

    if (moveCount >= currentBattles[battleId].moves.size()) {
        json move = json::object();
        move[whichPlayer] = json::object();
        move[whichPlayer]["attackTarget"] = attackTarget;
        move[whichPlayer]["defenceTarget"] = defenceTarget;

        currentBattles[battleId].moves.push_back(move);

        res.status = 200;
        res.set_content(u8R"(Ход зарегистрирован)", "text/plain; charset=UTF-8");

        return;
    }
    else {
        json currentMove = currentBattles[battleId].moves.at(moveCount);

        if (!currentMove[whichPlayer].empty()) {
            res.status = 400;
            res.set_content(u8R"(Ход уже существует)", "text/plain; charset=UTF-8");

            return;
        }
        else {
            json move = json::object();
            move["attackTarget"] = attackTarget;
            move["defenceTarget"] = defenceTarget;

            currentBattles[battleId].moves.at(moveCount)[whichPlayer] = move;

            res.status = 200;
            res.set_content(u8R"(Ход зарегистрирован)", "text/plain; charset=UTF-8");

            return;
        }
    }

    res.status = 500;
    res.set_content(u8R"(Вот тут нас быть не должно)", "text/plain; charset=UTF-8");
}

int calculateDamage(int currentHp, int target) {
    if (target == 101) {
        return currentHp - HEAD_DAMAGE;
    }

    if (target == 102) {
        return currentHp - TORSO_DAMAGE;
    }

    if (target == 103) {
        return currentHp - GROIN_DAMAGE;
    }

    if (target == 104) {
        return currentHp - LEGS_DAMAGE;
    }
}

moveResult calculateMoveResult(struct move firstPlayerMove, struct move secondPlayerMove, int firstPlayerHp, int secondPlayerHp) {
    struct moveResult result;

    bool firstPlayerDamaged = firstPlayerMove.defenceTarget != secondPlayerMove.attackTarget;

    if (!firstPlayerDamaged) {
        result.firstPlayerHp = firstPlayerHp;
    }
    else {
        result.firstPlayerHp = calculateDamage(firstPlayerHp, secondPlayerMove.attackTarget);
    }

    bool secondPlayerDamaged = secondPlayerMove.defenceTarget != firstPlayerMove.attackTarget;

    if (!secondPlayerDamaged) {
        result.secondPlayerHp = secondPlayerHp;
    }
    else {
        result.secondPlayerHp = calculateDamage(secondPlayerHp, firstPlayerMove.attackTarget);
    }

    return result;
}

void battleInfo_handler(const Request& req, Response& res) {
    if (!req.has_param("battleId") || !req.has_param("moveCount") || !req.has_param("login")) {
        res.status = 400;
        res.set_content(u8R"(В запросе нет данных)", "text/plain; charset=UTF-8");

        return;
    }

    int battleId = stoi(req.get_param_value("battleId"));
    int moveCount = stoi(req.get_param_value("moveCount"));
    string login = req.get_param_value("login");

    if (currentBattles.count(battleId) != 1) {
        res.status = 404;
        res.set_content(u8R"(Битвы с этим идентификатором не существует)", "text/plain; charset=UTF-8");

        return;
    }

    if (currentBattles[battleId].movesResults.size() >= moveCount + 1) {
        auto moveResult = currentBattles[battleId].movesResults.at(moveCount);

        json result = json::object();
        result["playerOneHp"] = moveResult.firstPlayerHp;
        result["playerTwoHp"] = moveResult.secondPlayerHp;

        if (moveResult.firstPlayerHp <= 0 || moveResult.secondPlayerHp <= 0) {
            result["gameFinished"] = true;
            currentBattles[battleId].finishInformed += 1;
            usersBattleIds.erase(login);

            if (currentBattles[battleId].finishInformed >= 2) {
                currentBattles.erase(battleId);
            }
        }
        else {
            result["gameFinished"] = false;
        }

        res.status = 200;
        res.set_content(result.dump(4), "text/json; charset=UTF-8");

        return;
    }

    if (moveCount >= currentBattles[battleId].moves.size()) {
        res.status = 204;
        res.set_content(u8R"(Нет данных о ходе)", "text/plain; charset=UTF-8");

        return;
    }

    json currentMove = currentBattles[battleId].moves.at(moveCount);

    if (currentMove["firstPlayerMove"].empty() || currentMove["secondPlayerMove"].empty()) {
        res.status = 204;
        res.set_content(u8R"(Один из игроков не сделал ход)", "text/plain; charset=UTF-8");

        return;
    }

    struct move firstPlayerMove;
    firstPlayerMove.attackTarget = currentMove["firstPlayerMove"]["attackTarget"];
    firstPlayerMove.defenceTarget = currentMove["firstPlayerMove"]["defenceTarget"];

    struct move secondPlayerMove;
    secondPlayerMove.attackTarget = currentMove["secondPlayerMove"]["attackTarget"];
    secondPlayerMove.defenceTarget = currentMove["secondPlayerMove"]["defenceTarget"];

    int firstPlayerHp = currentBattles[battleId].firstPlayerHp;
    int secondPlayerHp = currentBattles[battleId].secondPlayerHp;

    auto moveResult = calculateMoveResult(firstPlayerMove, secondPlayerMove, firstPlayerHp, secondPlayerHp);

    currentBattles[battleId].firstPlayerHp = moveResult.firstPlayerHp;
    currentBattles[battleId].secondPlayerHp = moveResult.secondPlayerHp;

    currentBattles[battleId].movesResults.push_back(moveResult);

    json result = json::object();
    result["playerOneHp"] = moveResult.firstPlayerHp;
    result["playerTwoHp"] = moveResult.secondPlayerHp;


    if (moveResult.firstPlayerHp <= 0 || moveResult.secondPlayerHp <= 0) {
        result["gameFinished"] = true;
        currentBattles[battleId].finishInformed += 1;
        usersBattleIds.erase(login);

        if (currentBattles[battleId].finishInformed >= 2) {
            currentBattles.erase(battleId);
        }
    }
    else {
        result["gameFinished"] = false;
    }

    res.status = 200;
    res.set_content(result.dump(4), "text/json; charset=UTF-8");
}

json get_database() {
    json database;
    ifstream file;

    file.open(database_path);

    if (file.is_open()) {
        string database_string;

        getline(file, database_string, '\0');
        database = json::parse(database_string);
    }
    else {
        database["users"] = json::array();
        database["battles"] = json::array();

        ofstream database_file;

        database_file.open(database_path);

        if (!database_file.is_open()) {
            cout << "Ошибка создания файла базы данных" << endl;
        }
        else {
            database_file << database.dump(4);
        }

        database_file.close();
    }

    file.close();

    return database;
}

int main() {
    database = get_database();

    Server svr;

    svr.Get("/", gen_response);
    svr.Post("/login", login_handler);
    svr.Post("/register", register_handler);
    svr.Post("/findBattle", findBattle_handler);
    svr.Post("/battle", battle_handler);
    svr.Get("/battle", battleInfo_handler);

    std::cout << "Start server... OK\n";

    svr.listen("localhost", 1234);
}
