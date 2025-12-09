#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

class Player {
private:
    int hp;
    vector <string> inventory;
    bool isAlive;
public:
    Player() {
        hp = 100;
        inventory.clear();
        isAlive = true;
    }
    void damage(int damage) {
        if (hp > damage)
            hp -= damage;
        else {
            hp = 0;
            isAlive = false;
        }
    }
    void heal(int h) {
        hp = (hp + h) % 101;
    }
    bool isPlayerAlive() {
        return isAlive;
    }
    int getHp() {
        return hp;
    }
    vector <string> invShow() {
        cout << "\nваш инвентарь:" << endl;
        if (inventory.size() > 0) {
            for (int i = 0; i < inventory.size(); ++i) {
                cout <<"\t" << i+1 << ". " << inventory[i] << endl;
                //printf("\t%d.................%s\n", i, inventory[i]);
            }
        }
        else
            cout << "\t*инвентарь пуст*" << endl;
        cout << "\n";
        return inventory;
    }
    void addItem(string item) {
        inventory.push_back(item);
        cout << "в инвернарь добавлен новый предмет: " << item.c_str() << "!" << endl;
    }

    string getItem(int n) {
        return inventory[n];
    }

};

/*
int banch();
int toilet();
int hall();
*/

vector <string> descrRooms(20, "");
int currentInd = 0;
int preInd = currentInd;
Player player;


class Room {
private:
    int ind;
    string name = "дефолтная комната";//с помощью вектора индексов близжайших комнат, определяем их имена(name) и выводим
    vector <int> near{};
    string des = "Это пустая комната. В ней ничего нет";
    vector <string> items{};
public:
    bool visited = false;

    Room() {
        ind = 0;
        des = "Это пустая комната. В ней ничего нет";
        visited = false;
    }

    Room(int number, string named, string descr, vector <int> nearest = {}, vector <string> mitems = {} ) {
        des = descr;
        ind = number;
        name = named;
        items = mitems;
        visited = false;
    }

    void search() {
        if (items.size() == 0) {
            cout << "вы обыскиваете комнату и ничего не находите\n";
        }
        else {
            cout << "вы обыскиваете комнату...\n";
            for (int i = 0; i < items.size(); ++i) {
                player.addItem(items[i]);
            }
        }
        items.clear();
        cout << "вы возвращаетесь на прежнее место... ";
    }

    string getName(){
        return name;
    }

    void event() {
        int playerAct;
        if (visited) {
            cout << "С вашего последнего посещения ничего не изменилось\n";
        }
        else {
            visited = true;
            cout << descrRooms[currentInd];
        }
        switch (currentInd) {
        case 0://нулевая комната(холл) и т.д.
            cout << "выберете действие:\n"
                "1.обыскать лавочки\n"
                "2.пойти в туалет\n"
                "3.обыскать комнату охраны\n"
                "4.пойти в гардероб\n";
            cin >> playerAct;
            switch (playerAct) {
            case 1:
                search();
                break;
            case 2:
                cout << "pokakai\n";
                break;
            case 3:
                currentInd = 1;//охрана
                break;
            case 4:
                currentInd = 2;
                break;
            default:
                cout << "вы ввели неправильную команду, повторите попытку" << endl;
                break;
            }
            break;
        case 1:
            cout << "выберете действие:\n"
                "1.обыскать комнату\n"
                "2.уйти\n";
            cin >> playerAct;
            switch (playerAct) {
            case 1:
                search();
                break;
            case 2:
                currentInd = preInd;
                preInd = currentInd;
                break;
            default:
                cout << "вы ввели неправильную команду, повторите попытку" << endl;
                break;
            }
            break;
        default:
            break;

        }
    }
};

vector <Room*> rooms;
//vector <bool> visited(10, false);

void initRooms() {
    descrRooms[0] = "День защиты диплома.Вы не помните, как именно вы добрались сюда,\n"
        "лишь что на улице стояла странная туманная погода и вовсе не было людей,\n"
        "что для раннего утра было ни капли не странно.Однако то,\n"
        "что на входе в колледж вы не увидели ни одной живой души,\n"
        "было уже чуть более удивительно.Свет почему - то был тусклым, довольно теплым,\n"
        "даже с красноватыми оттенками, а в воздухе стоял запах серы.Ваш колледж запомнился вам другим,\n"
        "но вы списали это на то, что вас давно здесь не было, да и вообще,\n"
        "это место всегда было жутким и недружелюбным, просто иначе.\n"
        "Вы забыли пропуск и хотели зайти по студенческому билету, однако охрана также не показывалась.\n"
        "Пришлось пролезть под турникетом.Теперь вы стоите в холле колледжа на первом этаже.\n"
        "Спереди вас лестница, за ней гардероб, сбоку от входа будка охраны и лавочки.\n"
        "С собой у тебя студенческий, папка с дипломом и флешка с проектом,\n"
        "и только одна цель - защитить диплом любой ценой\n";

    descrRooms[1] = "Вы заходите в небольшую комнатку, где мигает единственная лампочка.\n"
        "По экрану, на котором обычно видно записи с камер, поползли большие трещины.\n"
        "На стойке с ключами висит один единственный ключ от аудитории 220,\n"
        "а на столе лежит пропуск на имя Эъпстчхя Иъптаэохяа.\n"
        "На фотографии лицо девушки искажено в неестественной и жуткой гримасе\n";

    descrRooms[2] = "Вы спускаетесь вниз по темной лестнице,"
        "по ходу замечая, что привычной двери, ведущей на задний двор, попросту нет,"
        "вместо неё обычная стена. В раздевалке также горит слабый желтоватый свет, который,"
        "падая на такие же желтые стены, создает немного тошнотворную картину. Всматриваясь в них,"
        "вы замечаете на обоях черные пятна почти повсюду, словно бы это какой-то грибок."
        "Здесь почти не висит курток, буквально несколько штук."
        "Также здесь стоит стол с потерянными вещами, он также покрыт грибком."
        "Обычно за ним сидит гардеробщик, но сейчас он стоит в задней части возле стены,"
        "на которой нет этих странных пятен."
        "Он стоит к вам боком и смотрит за каждым вашим движением одним глазом."
        "Присмотревшись повнимательнее, вы понимаете, что в такой странной позе он застыл,"
        "так как облизывает эту стену";

    //заканчиваем все описания комнат

    rooms.clear();
    vector <string> items = { "пицца какая-т", "энергос", "пропуск на нику" };
    rooms.push_back(new Room(0, "холл", descrRooms[0],{1,2}, items));
    rooms.push_back(new Room(1, "будка охраны", descrRooms[1]));
    rooms.push_back(new Room(2, "гардероб", descrRooms[2]));
}


/*
int banch() {
    srand(time(0));
    int playerAct = 0;
    int index = 1;
    if (visited[index] == true) {
        cout << "С вашего последнего посещения ничего не изменилось\n";
    }
    else {
        visited[index] = true;
        cout << "ебейшее описание комнаты чтоб трахать трах трах щперма\n"
            "обыскав лавочку, вы нашли несколько полезных предметов!\n";
        player.addItem("журнал ИС-41"); player.addItem("пепси"); player.addItem("энергос");
        int coin = rand() % 2;//орёл или решка типа
        if(coin)
            player.addItem("пицца пепперони");
        else
            player.addItem("чиловая пицца");

    }
    cout << "выберете действие:\n"
        "1.вернуться\n";
    cin >> playerAct;
    switch (playerAct) {
    case 1:
        hall();
        return 0;
    default:
        hall();
        return 0;
    }
}

int toilet() {
    int playerAct = 0;
    int index = 2;
    if (visited[index] == true) {
        cout << "С вашего последнего посещения ничего не изменилось\n";
    }
    else {
        visited[index] = true;
        cout << "ебейшее описание комнаты чтоб трахать трах трах щперма\n";
    }
    cout << "выберете действие:\n"
        "1.вернуться\n";
        cin >> playerAct;
    switch (playerAct) {
    case 1:
        hall();
        return 0;
    default:
        hall();
        return 0;
    }
}

int hall() {
    int playerAct = 0;
    int index = 0;
    if (visited[index] == true) {
        cout << "С вашего последнего посещения ничего не изменилось\n";
    }
    else {
        visited[index] = true;
        cout << "ебейшее описание комнаты чтоб трахать трах трах щперма\n";
    }

    cout << "выберете действие:\n"
        "1.обыскать лавочки\n"
        "2.пойти в туалет\n"
        "3.обыскать комнату охраны\n"
        "4.пойти в гардероб\n";
    cin >> playerAct;
    switch (playerAct) {
    case 1:
        banch();
        return 0;
    case 2:
        toilet();
        return 0;
        
        case 3:
            security();
            return 0;
        case 4:
            cloackroom();
            return 0;
    default:
        banch();
        return 0;
    }
}

void start() {
    cout << "начальное описание сюжета\n";
    player.addItem("студенческий билет");
    player.addItem("папка с дипломным проектом");
    player.addItem("флешка с копией проекта");
    hall();
}
*/

int main(){
    setlocale(LC_ALL, "Russian");
    initRooms();
    int playerAct = 0;
    int roomCount = 0;
    
    /*
    vector <Room> rooms{Room holl("вахта", 0, "Вы не помните, как именно"), Room holl()};

    Room room("холл", 0, "Перед вами довольно просторный холл.\n"
        "По бокам от входа стоят лавочки, на которых переобувались студенты\n"
        "Спереди стоит три турникета, угрожающе горящие красными огоньками.");

    
    */

    while (true){
        rooms[currentInd]->event();
    }
    
}

