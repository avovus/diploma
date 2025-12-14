#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

// Перечисление предметов
enum Item {
    ITEM_NONE,
    ITEM_ENERGY_DRINK,
    ITEM_GROUP_MAGAZINE,
    ITEM_PEPPERONI_PIZZA,
    ITEM_CHILLI_PIZZA,
    ITEM_PEPSI,
    ITEM_KEY_220,
    ITEM_PASS,
    ITEM_HEADPHONES,
    ITEM_FOLDING_KNIFE,
    ITEM_VINEGAR,
    ITEM_USA_YMZYA,
    ITEM_OCHUMFAHP,
    ITEM_PIECE,
    ITEM_CROWBAR,
    ITEM_BLUE_YELLOW_SCARF,
    ITEM_FLASHDRIVE_220,
    ITEM_INFERNAL_TRANSLATOR,
    ITEM_NOTE_WITH_PHRASE,
    ITEM_GRENADE,
    ITEM_YOUR_FLASHDRIVE,
    ITEM_BLACK_SPOTS // Черные пятна на коже
};

// Перечисление состояний игры
enum GameState {
    STATE_START,
    STATE_FLOOR1_HALL,
    STATE_FLOOR1_BENCHES,
    STATE_FLOOR1_GUARD_ROOM,
    STATE_FLOOR1_GUARD_ROOM_SEARCH,
    STATE_FLOOR1_CLOAKROOM,
    STATE_FLOOR1_CLOAKROOM_JACKETS,
    STATE_FLOOR1_CLOAKROOM_TABLE,
    STATE_FLOOR1_TOILET,
    STATE_FLOOR1_TOILET_OBSERVE,
    STATE_FLOOR1_TOILET_WASH,
    STATE_FLOOR1_TOILET_WASH_CHOICE,
    STATE_FLOOR1_TOILET_LEAVE,
    STATE_FLOOR2_HALL,
    STATE_FLOOR2_RIGHT,
    STATE_FLOOR2_DEPARTMENT,
    STATE_FLOOR2_STUDY_PART,
    STATE_FLOOR2_LEFT,
    STATE_FLOOR2_214,
    STATE_FLOOR2_220,
    STATE_FLOOR2_STAIRS,
    STATE_FLOOR2_STAIRS_UP,
    STATE_FLOOR3_HALL,
    STATE_FLOOR3_320,
    STATE_FLOOR3_320_AFTER,
    STATE_FLOOR3_323,
    STATE_FLOOR3_323_AFTER,
    STATE_FLOOR3_CORRIDOR,
    STATE_FLOOR3_306,
    STATE_FLOOR3_306_AFTER,
    STATE_FLOOR4_HALL,
    STATE_FLOOR4_408,
    STATE_FLOOR4_411,
    STATE_FLOOR4_DEFENSE,
    STATE_FLOOR4_DEFENSE_QUESTIONS,
    STATE_GAME_OVER,
    STATE_WIN,
    STATE_BAD_END_ROY,
    STATE_BAD_END_MONSTER,
    STATE_BAD_END_DARKNESS,
    STATE_BAD_END_NOT_ADMITTED,
    STATE_BAD_END_SYSTEM,
    STATE_BAD_END_HUMAN
};

// Класс игрока
class Player {
public:
    int health;
    int maxHealth;
    vector<Item> inventory;
    set<GameState> visitedStates;
    bool hasSignatures[3]; // 220, 323, 408
    bool hasAllSignatures;
    bool hasBlackSpots;
    
    Player() {
        health = 3;
        maxHealth = 3;
        hasSignatures[0] = hasSignatures[1] = hasSignatures[2] = false;
        hasAllSignatures = false;
        hasBlackSpots = false;
        inventory.push_back(ITEM_YOUR_FLASHDRIVE);
    }
    
    void addItem(Item item) {
        if (item != ITEM_NONE && find(inventory.begin(), inventory.end(), item) == inventory.end()) {
            inventory.push_back(item);
        }
    }
    
    bool hasItem(Item item) {
        return find(inventory.begin(), inventory.end(), item) != inventory.end();
    }
    
    void removeItem(Item item) {
        auto it = find(inventory.begin(), inventory.end(), item);
        if (it != inventory.end()) {
            inventory.erase(it);
        }
    }
    
    void takeDamage() {
        health--;
        if (health < 0) health = 0;
    }
    
    void heal() {
        if (health < maxHealth) {
            health++;
            cout << "Вы восстановили 1 единицу здоровья. Теперь у вас " << health << "/" << maxHealth << " HP.\n";
        } else {
            cout << "У вас и так максимальное здоровье.\n";
        }
    }
    
    void useEnergyDrink() {
        auto it = find(inventory.begin(), inventory.end(), ITEM_ENERGY_DRINK);
        if (it != inventory.end()) {
            inventory.erase(it);
            heal();
        } else {
            cout << "У вас нет энергетика.\n";
        }
    }
    
    void checkSignatures() {
        hasAllSignatures = hasSignatures[0] && hasSignatures[1] && hasSignatures[2];
    }
    
    void addBlackSpots() {
        if (!hasBlackSpots) {
            hasBlackSpots = true;
            addItem(ITEM_BLACK_SPOTS);
        }
    }
    
    void removeBlackSpots() {
        if (hasBlackSpots) {
            hasBlackSpots = false;
            removeItem(ITEM_BLACK_SPOTS);
        }
    }
};

// Класс игры
class Game {
private:
    GameState currentState;
    Player player;
    bool gameRunning;
    bool inCombat;
    bool requireItemChoice;
    vector<Item> currentItemChoices;
    GameState returnStateAfterItem;
    
    // Флаги событий
    bool benchesSearched;
    bool guardRoomSearched;
    bool cloakroomVisited;
    bool cloakroomJacketsSearched;
    bool cloakroomTableSearched;
    bool toiletGirlObserved;
    bool toiletWashedHands;
    bool departmentVisited;
    bool student214Calmed;
    bool entity220Visited;
    bool entity220GivenScarf;
    bool entity220TakenItems;
    bool stairsBoardsRemoved;
    bool stairsVisited;
    bool sect320Sacrificed;
    bool creature323Fed;
    bool creature323Translated;
    bool studentPassHelped;
    bool shadow306Fed;
    bool octopusParalyzed;
    bool creature408Calmed;
    bool figure411Helped;
    bool defenseQuestionsStarted;
    int defenseQuestionNum;
    
public:
    Game() {
        srand(time(0));
        currentState = STATE_START;
        gameRunning = true;
        inCombat = false;
        requireItemChoice = false;
        defenseQuestionsStarted = false;
        defenseQuestionNum = 0;
        resetFlags();
    }
    
    void resetFlags() {
        benchesSearched = false;
        guardRoomSearched = false;
        cloakroomVisited = false;
        cloakroomJacketsSearched = false;
        cloakroomTableSearched = false;
        toiletGirlObserved = false;
        toiletWashedHands = false;
        departmentVisited = false;
        student214Calmed = false;
        entity220Visited = false;
        entity220GivenScarf = false;
        entity220TakenItems = false;
        stairsBoardsRemoved = false;
        stairsVisited = false;
        sect320Sacrificed = false;
        creature323Fed = false;
        creature323Translated = false;
        studentPassHelped = false;
        shadow306Fed = false;
        octopusParalyzed = false;
        creature408Calmed = false;
        figure411Helped = false;
        defenseQuestionsStarted = false;
        defenseQuestionNum = 0;
    }
    
    void run() {
        while (gameRunning) {
            displayCurrentState();
            handleInput();
            checkGameOver();
        }
    }
    
private:
    void displayCurrentState() {
        system("cls");
        cout << "========================================\n";
        cout << "Здоровье: " << player.health << "/" << player.maxHealth << "\n";
        cout << "Инвентарь: ";
        for (Item item : player.inventory) {
            if (item != ITEM_BLACK_SPOTS) {
                cout << getItemName(item) << "; ";
            }
        }
        if (player.hasBlackSpots) {
            cout << "Черные пятна на коже; ";
        }
        cout << "\n";
        
        if (player.hasSignatures[0] || player.hasSignatures[1] || player.hasSignatures[2]) {
            cout << "Подписи: ";
            if (player.hasSignatures[0]) cout << "220 ";
            if (player.hasSignatures[1]) cout << "323 ";
            if (player.hasSignatures[2]) cout << "408 ";
            cout << "\n";
        }
        
        cout << "========================================\n\n";
        
        switch(currentState) {
            case STATE_START:
                cout << "День защиты диплома. Вы не помните, как именно вы добрались сюда...\n";
                cout << "Теперь вы стоите в холле колледжа на первом этаже.\n";
                cout << "1. Обыскать лавочки\n";
                cout << "2. Пойти в комнату охраны\n";
                cout << "3. Пойти в раздевалку\n";
                cout << "4. Пойти в туалет\n";
                cout << "5. Пойти на второй этаж\n";
                break;
                
            case STATE_FLOOR1_BENCHES:
                if (!benchesSearched) {
                    cout << "Вы шаритесь по лавочкам и находите:\n";
                    // Случайный предмет как в сценарии
                    vector<Item> possibleItems = {ITEM_ENERGY_DRINK, ITEM_GROUP_MAGAZINE, ITEM_PEPSI};
                    if (rand() % 2 == 0) {
                        possibleItems.push_back(ITEM_PEPPERONI_PIZZA);
                    } else {
                        possibleItems.push_back(ITEM_CHILLI_PIZZA);
                    }
                    
                    Item found = possibleItems[rand() % possibleItems.size()];
                    player.addItem(found);
                    cout << "- " << getItemName(found) << "\n";
                    benchesSearched = true;
                } else {
                    cout << "Вы уже обыскали лавочки. Больше здесь ничего нет.\n";
                }
                cout << "\n1. Вернуться в холл\n";
                break;
                
            case STATE_FLOOR1_GUARD_ROOM:
                cout << "Вы заходите в небольшую комнатку, где мигает единственная лампочка.\n";
                cout << "По экрану с камерами поползли большие трещины.\n";
                cout << "На стойке с ключами висит один единственный ключ от аудитории 220.\n";
                cout << "На столе лежит пропуск на имя Эъпстчхя Иъптаэохяа.\n";
                cout << "1. Обыскать комнату охраны\n";
                cout << "2. Уйти\n";
                break;
                
            case STATE_FLOOR1_GUARD_ROOM_SEARCH:
                if (!guardRoomSearched) {
                    player.addItem(ITEM_KEY_220);
                    player.addItem(ITEM_PASS);
                    cout << "Вы забираете ключ от 220 и пропуск.\n";
                    guardRoomSearched = true;
                } else {
                    cout << "Вы уже все забрали отсюда.\n";
                }
                cout << "\n1. Вернуться\n";
                break;
                
            case STATE_FLOOR1_CLOAKROOM:
                if (!cloakroomVisited) {
                    cout << "Вы спускаетесь вниз по темной лестнице...\n";
                    cout << "В раздевалке горит слабый желтоватый свет.\n";
                    cout << "На обоях черные пятна почти повсюду.\n";
                    cout << "Гардеробщик стоит боком и смотрит за каждым вашим движением одним глазом.\n";
                    cout << "Вы понимаете, что он облизывает стену.\n";
                    cloakroomVisited = true;
                } else {
                    cout << "Вы в раздевалке. Гардеробщик все так же облизывает стену.\n";
                }
                cout << "1. Обыскать куртки\n";
                cout << "2. Обыскать стол\n";
                cout << "3. Уйти\n";
                break;
                
            case STATE_FLOOR1_CLOAKROOM_JACKETS:
                if (!cloakroomJacketsSearched) {
                    cout << "Не сводя глаз с гардеробщика, вы обшариваете куртки и находите:\n";
                    player.addItem(ITEM_ENERGY_DRINK);
                    player.addItem(ITEM_HEADPHONES);
                    player.addItem(ITEM_FOLDING_KNIFE);
                    cout << "- Банка энергетика\n- Наушники\n- Складной нож\n";
                    cloakroomJacketsSearched = true;
                } else {
                    cout << "Вы уже обыскали куртки. Больше ничего нет.\n";
                }
                cout << "\n1. Вернуться\n";
                break;
                
            case STATE_FLOOR1_CLOAKROOM_TABLE:
                if (!cloakroomTableSearched) {
                    cout << "На столе сидит маленькая игрушка непонятного существа.\n";
                    cout << "Рядом стоит бутылка уксуса.\n";
                    cout << "При касании вы чувствуете легкое жжение в руке.\n";
                    player.addItem(ITEM_VINEGAR);
                    player.addItem(ITEM_USA_YMZYA);
                    player.addItem(ITEM_OCHUMFAHP);
                    cout << "Вы забираете бутылку уксуса и странные предметы.\n";
                    cloakroomTableSearched = true;
                } else {
                    cout << "Стол пуст.\n";
                }
                cout << "\n1. Вернуться\n";
                break;
                
            case STATE_FLOOR1_TOILET:
                cout << "Вы идете по коридору первого этажа...\n";
                cout << "Доходите до туалета и входите внутрь.\n";
                cout << "Слышите оглушающе громкий звук.\n";
                cout << "Возле двери в кабинку стоит студентка. Она бьется головой об дверь.\n";
                cout << "1. Смотреть\n";
                if (player.hasBlackSpots) {
                    cout << "2. Помыть руки\n";
                }
                cout << "3. Уйти\n";
                break;
                
            case STATE_FLOOR1_TOILET_OBSERVE:
                if (!toiletGirlObserved) {
                    cout << "Вы видите, как девушка с каждым ударом становится все яростнее...\n";
                    cout << "Из её головы начинает идти кровь...\n";
                    cout << "Через несколько десятков ударов, она опускается на пол.\n";
                    cout << "1. Забрать кусочек\n";
                    cout << "2. Уйти\n";
                } else {
                    cout << "На полу лишь лужа крови.\n";
                    cout << "\n1. Вернуться\n";
                }
                break;
                
            case STATE_FLOOR1_TOILET_WASH:
                cout << "Из-под крана течет вода с явной примесью ржавчины.\n";
                cout << "Мыла вы не обнаруживаете.\n";
                cout << "1. Просто помыть руки\n";
                if (player.hasItem(ITEM_VINEGAR)) cout << "2. Применить уксус\n";
                if (player.hasItem(ITEM_PEPSI)) cout << "3. Применить пепси\n";
                if (player.hasItem(ITEM_ENERGY_DRINK)) cout << "4. Применить энергетик\n";
                cout << "5. Уйти\n";
                break;
                
            case STATE_FLOOR1_TOILET_WASH_CHOICE:
                // Обрабатывается в handleInput
                break;
                
            case STATE_FLOOR1_TOILET_LEAVE:
                if (player.hasBlackSpots && !player.hasItem(ITEM_VINEGAR)) {
                    cout << "Вы решаете не контактировать с водой, но жжение усиливается!\n";
                    cout << "Теперь все ваше тело покрыто черными пятнами.\n";
                    cout << "Ваше сознание затуманивается...\n";
                    cout << "Вы выходите из туалета и идете исполнять своё предназначение на благо роя.\n";
                    currentState = STATE_BAD_END_ROY;
                    return;
                }
                break;
                
            case STATE_FLOOR2_HALL:
                cout << "На втором этаже в холле все перевернуто вверх дном.\n";
                cout << "Ступеньки на 3 этаж завалены мебелью.\n";
                cout << "Из-под них торчат кисти рук...\n";
                cout << "1. Пойти направо\n";
                cout << "2. Посмотреть расписание\n";
                cout << "3. Пойти налево\n";
                break;
                
            case STATE_FLOOR2_RIGHT:
                cout << "Вы идете дальше по коридору.\n";
                cout << "Вы видите дверь на отделение и дверь в учебную часть.\n";
                cout << "1. Зайти на отделение\n";
                cout << "2. Зайти в учебную часть\n";
                cout << "3. Вернуться в холл\n";
                break;
                
            case STATE_FLOOR2_DEPARTMENT:
                if (!departmentVisited) {
                    cout << "Как только вы входите, все оборачиваются и пристально смотрят на вас...\n";
                    cout << "Их кожа гниет, глаза пусты.\n";
                    cout << "Дверь за вами заклинивает!\n";
                    cout << "Существа окружают вас: 'Нд охяияф уяхо?'\n";
                    cout << "Одно из них достает камеру и начинает вдавливать её вам в глазницу!\n";
                    
                    currentItemChoices.clear();
                    if (player.hasItem(ITEM_FOLDING_KNIFE)) currentItemChoices.push_back(ITEM_FOLDING_KNIFE);
                    if (player.hasItem(ITEM_GROUP_MAGAZINE)) currentItemChoices.push_back(ITEM_GROUP_MAGAZINE);
                    if (player.hasItem(ITEM_HEADPHONES)) currentItemChoices.push_back(ITEM_HEADPHONES);
                    if (player.hasItem(ITEM_PASS)) currentItemChoices.push_back(ITEM_PASS);
                    
                    requireItemChoice = true;
                    returnStateAfterItem = STATE_FLOOR2_RIGHT;
                } else {
                    cout << "Кабинет пуст.\n";
                    cout << "\n1. Вернуться\n";
                }
                break;
                
            case STATE_FLOOR2_STUDY_PART:
                cout << "Вы заходите в учебную часть...\n";
                cout << "Здесь пусто, лишь обелиск по центру комнаты.\n";
                cout << "Из обелиска торчат части тел. Вы замечаете рот.\n";
                cout << "Он говорит: 'Твоя защита в 422. Получи подписи в 220, 323, 408. Будь осторожнее с НИМИ'\n";
                cout << "\n1. Вернуться\n";
                break;
                
            case STATE_FLOOR2_LEFT:
                cout << "Вы поворачиваете налево.\n";
                cout << "Видите открытые двери в 214 и 220 кабинеты.\n";
                cout << "Проход на боковую лестницу заколочен досками.\n";
                cout << "1. Зайти в 214\n";
                cout << "2. Зайти в 220\n";
                if (player.hasItem(ITEM_CROWBAR)) {
                    cout << "3. Отодрать доски от прохода на лестницу\n";
                }
                cout << "4. Вернуться в холл\n";
                break;
                
            case STATE_FLOOR2_214:
                cout << "Вы заходите в кабинет 214 - сплошной хаос.\n";
                cout << "По полу разбросаны тела. Посреди всего сидит студент с ломиком.\n";
                
                if (!student214Calmed) {
                    cout << "Студент: 'Я больше никогда не приду на классный час...'\n";
                    cout << "'Как мне заткнуть их?'\n";
                    
                    if (player.hasItem(ITEM_HEADPHONES)) {
                        cout << "1. Дать наушники\n";
                    }
                    cout << "2. Не отвечать\n";
                    cout << "3. Попытаться забрать лом\n";
                    cout << "4. Уйти\n";
                } else {
                    cout << "Студент спокойно сидит с наушниками в ушах.\n";
                    cout << "\n1. Уйти\n";
                }
                break;
                
            case STATE_FLOOR2_220:
                if (!entity220Visited) {
                    cout << "Вы заходите в 220 кабинет, где царит порядок.\n";
                    cout << "На гвоздике двери висит сине-желтый шарф.\n";
                    cout << "На столе стоит банка с недопитым энергетиком.\n";
                    cout << "За столом сидит фигура, похожая на труп утопленника.\n";
                    entity220Visited = true;
                }
                
                if (!player.hasSignatures[0]) {
                    if (!entity220GivenScarf) {
                        cout << "Фигура поворачивается: 'Красный или синий?'\n";
                        cout << "1. Красный\n";
                        cout << "2. Синий\n";
                        if (!entity220TakenItems) {
                            cout << "3. Забрать шарф и энергетик\n";
                        }
                        cout << "4. Уйти\n";
                    } else {
                        cout << "Фигура молчит. Вы уже получили подпись.\n";
                        cout << "\n1. Уйти\n";
                    }
                } else {
                    cout << "Кабинет пуст.\n";
                    cout << "\n1. Уйти\n";
                }
                break;
                
            case STATE_FLOOR2_STAIRS:
                if (!stairsVisited) {
                    cout << "Вы достаете ломик и начинаете выдирать гвозди...\n";
                    cout << "Проход открыт!\n";
                    stairsBoardsRemoved = true;
                    stairsVisited = true;
                }
                
                cout << "Вы входите на боковую лестницу.\n";
                cout << "Там царит полутьма, стоит легкий гул снизу.\n";
                cout << "Вдруг вы замечаете, как на дальних пролётах начинают гаснуть лампы!\n";
                cout << "Тьма поднимается вверх!\n";
                cout << "1. Вверх\n";
                cout << "2. Вниз\n";
                cout << "3. Стоять\n";
                break;
                
            case STATE_FLOOR3_HALL:
                cout << "Вы на третьем этаже.\n";
                cout << "1. Зайти в 320 кабинет\n";
                cout << "2. Зайти в 323 кабинет\n";
                cout << "3. Пойти дальше по коридору\n";
                cout << "4. Вернуться на второй этаж\n";
                break;
                
            case STATE_FLOOR3_320:
                if (!sect320Sacrificed) {
                    cout << "320 кабинет залит красным светом.\n";
                    cout << "Вся мебель свалена в центре, образуя пирамиду.\n";
                    cout << "Вокруг в круг выстроились фигуры в тёмных балахонах.\n";
                    cout << "Один кричит: 'Принеси плоть в жертву кровавому богу!'\n";
                    
                    if (player.hasItem(ITEM_PIECE)) {
                        cout << "1. Принести кусочек плоти в жертву\n";
                    }
                    cout << "2. Уйти\n";
                } else {
                    cout << "Кабинет пуст. На полу лишь пентаграмма.\n";
                    if (!player.hasItem(ITEM_INFERNAL_TRANSLATOR)) {
                        cout << "На полу лежит черная книга.\n";
                        cout << "1. Подобрать переводчик с инфернального\n";
                    }
                    cout << "2. Уйти\n";
                }
                break;
                
            case STATE_FLOOR3_320_AFTER:
                // После жертвоприношения
                break;
                
            case STATE_FLOOR3_323:
                if (!creature323Fed) {
                    cout << "Дверь в 323 скрипит и распахивается.\n";
                    cout << "Вам преграждает путь существо со щупальцами.\n";
                    cout << "Из глаз и рта торчат трубки. Оно что-то требует.\n";
                    
                    if (player.hasItem(ITEM_PEPPERONI_PIZZA)) cout << "1. Дать пиццу Пепперони\n";
                    if (player.hasItem(ITEM_CHILLI_PIZZA)) cout << "2. Дать чиловую пиццу\n";
                    if (player.hasItem(ITEM_PIECE)) cout << "3. Дать кусочек\n";
                    cout << "4. Уйти\n";
                } else if (!player.hasSignatures[1]) {
                    cout << "Существо наслаждается угощением.\n";
                    cout << "Оно ждет, когда вы покажете проект для подписи.\n";
                    cout << "1. Показать проект\n";
                    if (player.hasItem(ITEM_INFERNAL_TRANSLATOR) && !creature323Translated) {
                        cout << "2. Осмотреть бумажку с символами\n";
                    }
                    cout << "3. Уйти\n";
                } else {
                    cout << "Кабинет пуст.\n";
                    cout << "\n1. Уйти\n";
                }
                break;
                
            case STATE_FLOOR3_323_AFTER:
                // После кормления
                break;
                
            case STATE_FLOOR3_CORRIDOR:
                cout << "Коридор третьего этажа сворачивает направо.\n";
                cout << "У лестницы стоит столик, висит стенд со списками на отчисление.\n";
                cout << "На столе сидит студент, держит приказ на отчисление и ноет.\n";
                cout << "Он бормочет, что застрял здесь навечно.\n";
                cout << "1. Пройти мимо\n";
                if (player.hasItem(ITEM_PASS)) {
                    cout << "2. Дать пропуск\n";
                }
                cout << "3. Вернуться\n";
                break;
                
            case STATE_FLOOR3_306:
                cout << "306 кабинет с виду кажется совершенно пустым.\n";
                cout << "Из тьмы под столами слышится искаженный голос: 'Пора на перерыв'\n";
                
                if (!shadow306Fed) {
                    if (player.hasItem(ITEM_PEPSI)) {
                        cout << "1. Дать пепси\n";
                    }
                    if (player.hasItem(ITEM_GROUP_MAGAZINE)) {
                        cout << "2. Дать журнал\n";
                    }
                    cout << "3. Уйти\n";
                } else {
                    cout << "Тень довольна. Журнал заполнен.\n";
                    cout << "\n1. Уйти\n";
                }
                break;
                
            case STATE_FLOOR4_HALL:
                cout << "Вы на четвертом этаже.\n";
                cout << "Вход в актовый зал завален непонятной субстанцией.\n";
                
                if (!octopusParalyzed) {
                    cout << "Перед вами существо с головой осьминога в строгом костюме.\n";
                    cout << "Вы парализованы невидимой силой!\n";
                    
                    if (player.hasItem(ITEM_NOTE_WITH_PHRASE)) {
                        cout << "1. Прочитать записку с шифром\n";
                    }
                    cout << "2. Смириться\n";
                } else {
                    cout << "Коридор свободен. Открыты кабинеты 408 и 411.\n";
                    cout << "Вдали виднеется дверь в 422 кабинет.\n";
                    cout << "1. Зайти в 408 кабинет\n";
                    cout << "2. Зайти в 411 кабинет\n";
                    cout << "3. Идти в 422\n";
                    cout << "4. Спуститься на 3 этаж\n";
                }
                break;
                
            case STATE_FLOOR4_408:
                if (!creature408Calmed) {
                    cout << "Вы заходите в 408 кабинет, дверь захлопывается!\n";
                    cout << "С потолка на вас прыгает нечто на 4 руках и 4 ногах!\n";
                    cout << "Оно кричит: 'Ты больше никогда не прогуляешь пару!'\n";
                    
                    if (player.hasItem(ITEM_GROUP_MAGAZINE)) {
                        cout << "1. Показать журнал группы\n";
                    }
                    cout << "2. Смириться\n";
                } else {
                    cout << "Кабинет пуст.\n";
                    cout << "\n1. Уйти\n";
                }
                break;
                
            case STATE_FLOOR4_411:
                cout << "Вы заходите в 411 кабинет.\n";
                cout << "Здесь сидит фигура с мертвенно-бледной кожей и черными глазницами.\n";
                cout << "Она говорит: 'Помоги'\n";
                
                if (!figure411Helped) {
                    if (player.hasItem(ITEM_VINEGAR)) cout << "1. Дать уксус\n";
                    if (player.hasItem(ITEM_FOLDING_KNIFE)) cout << "2. Дать нож\n";
                    cout << "3. Наблюдать\n";
                    cout << "4. Уйти\n";
                } else {
                    cout << "На полу лежит тело.\n";
                    if (!player.hasItem(ITEM_GRENADE)) {
                        cout << "Рядом валяется ручная граната.\n";
                        cout << "1. Подобрать гранату\n";
                    }
                    cout << "2. Уйти\n";
                }
                break;
                
            case STATE_FLOOR4_DEFENSE:
                player.checkSignatures();
                cout << "Входя в 422 кабинет, вы слышите в голове голос.\n";
                cout << "'Ты собрал все подписи?'\n";
                
                if (player.hasAllSignatures) {
                    cout << "1. Да\n";
                }
                cout << "2. Нет\n";
                break;
                
            case STATE_FLOOR4_DEFENSE_QUESTIONS:
                if (!defenseQuestionsStarted) {
                    cout << "Вы проходите в кабинет, залитый красным светом.\n";
                    cout << "Стены сделаны из мяса, они дышат и смотрят на вас.\n";
                    cout << "Приёмная комиссия оплавилась со стульями.\n";
                    cout << "Голос: 'Приступайте'\n";
                    defenseQuestionsStarted = true;
                }
                
                if (player.hasItem(ITEM_FLASHDRIVE_220)) {
                    cout << "У вас есть флешка из 220. Вставить её?\n";
                    cout << "1. Да, вставить флешку из 220\n";
                    cout << "2. Нет, использовать свою флешку\n";
                } else {
                    cout << "Вы вставляете свою флешку и начинаете защиту...\n";
                    cout << "Пришло время вопросов комиссии.\n";
                    askDefenseQuestion();
                }
                break;
                
            case STATE_BAD_END_ROY:
                cout << "КОНЦОВКА: Вы стали частью роя\n";
                cout << "Вы потеряли свою индивидуальность, но обрели единство с коллективом.\n";
                cout << "\n1. Начать заново\n";
                cout << "2. Выйти\n";
                break;
                
            case STATE_BAD_END_MONSTER:
                cout << "КОНЦОВКА 0: Вы создали монстра\n";
                cout << "Существо выросло до невероятных размеров и разрушило всё.\n";
                cout << "\n1. Начать заново\n";
                cout << "2. Выйти\n";
                break;
                
            case STATE_BAD_END_DARKNESS:
                cout << "КОНЦОВКА 0: Вы растворились во тьме\n";
                cout << "Тьма поглотила вас без остатка.\n";
                cout << "\n1. Начать заново\n";
                cout << "2. Выйти\n";
                break;
                
            case STATE_BAD_END_NOT_ADMITTED:
                cout << "КОНЦОВКА 1: Вы не допущены к защите\n";
                cout << "Ваша голова взорвалась от боли.\n";
                cout << "\n1. Начать заново\n";
                cout << "2. Выйти\n";
                break;
                
            case STATE_BAD_END_SYSTEM:
                cout << "КОНЦОВКА 2: Вы сломали систему, но забыли, что были её частью\n";
                cout << "Ад исчез, но исчезли и вы.\n";
                cout << "\n1. Начать заново\n";
                cout << "2. Выйти\n";
                break;
                
            case STATE_BAD_END_HUMAN:
                cout << "КОНЦОВКА 3: Вы умерли человеком\n";
                cout << "Комиссия не поверила, что человек может быть здесь.\n";
                cout << "Они приступили к трапезе.\n";
                cout << "\n1. Начать заново\n";
                cout << "2. Выйти\n";
                break;
                
            case STATE_WIN:
                cout << "КОНЦОВКА 4: Вы защитились!\n";
                cout << "Несмотря на все ужасы, вы достигли своей цели.\n";
                cout << "Диплом защищен. Что теперь?\n";
                cout << "\n1. Начать новую игру\n";
                cout << "2. Выйти из игры\n";
                break;
                
            case STATE_GAME_OVER:
                cout << "ВЫ ПОГИБЛИ\n";
                cout << "Но у вас есть еще шанс...\n";
                cout << "\n1. Начать заново\n";
                cout << "2. Выйти из игры\n";
                break;
        }
        
        if (!requireItemChoice && currentState != STATE_FLOOR4_DEFENSE_QUESTIONS && 
            currentState < STATE_BAD_END_ROY && currentState != STATE_WIN && 
            currentState != STATE_GAME_OVER) {
            cout << "\n0. Использовать энергетик для восстановления здоровья\n";
        }
    }
    
    void askDefenseQuestion() {
        switch(defenseQuestionNum) {
            case 0:
                cout << "\nВопрос: 'Нд ьснсэ?'\n";
                cout << "1. Да\n";
                cout << "2. Нет\n";
                break;
            case 1:
                cout << "\nВопрос: 'Ьыъ хстияъноа юъшмучъ ч тяичтяъноа пъяфгтсонг?'\n";
                cout << "1. Нигде\n";
                cout << "2. Здесь\n";
                break;
            case 2:
                cout << "\nВопрос: 'Иъу эд кснчнъ шятчуянгоа рсофъ эдрмохя?'\n";
                cout << "1. Существовать\n";
                cout << "2. Разлагаться\n";
                break;
            case 3:
                cout << "\nВопрос: 'Хнс нд?'\n";
                cout << "Введите ваш ответ: ";
                break;
        }
    }
    
    void handleInput() {
        int choice;
        cout << "\nВаш выбор: ";
        
        if (currentState == STATE_FLOOR4_DEFENSE_QUESTIONS && defenseQuestionNum == 3) {
            string answer;
            cin >> answer;
            if (answer == "Человек" || answer == "человек") {
                currentState = STATE_BAD_END_HUMAN;
            } else {
                currentState = STATE_WIN;
            }
            return;
        }
        
        cin >> choice;
        
        if (requireItemChoice) {
            handleItemChoice(choice);
            return;
        }
        
        if (choice == 0 && currentState != STATE_FLOOR4_DEFENSE_QUESTIONS && 
            currentState < STATE_BAD_END_ROY && currentState != STATE_WIN && 
            currentState != STATE_GAME_OVER) {
            player.useEnergyDrink();
            return;
        }
        
        switch(currentState) {
            case STATE_START:
                switch(choice) {
                    case 1: currentState = STATE_FLOOR1_BENCHES; break;
                    case 2: currentState = STATE_FLOOR1_GUARD_ROOM; break;
                    case 3: currentState = STATE_FLOOR1_CLOAKROOM; break;
                    case 4: currentState = STATE_FLOOR1_TOILET; break;
                    case 5: currentState = STATE_FLOOR2_HALL; break;
                }
                break;
                
            case STATE_FLOOR1_BENCHES:
                if (choice == 1) currentState = STATE_START;
                break;
                
            case STATE_FLOOR1_GUARD_ROOM:
                switch(choice) {
                    case 1: currentState = STATE_FLOOR1_GUARD_ROOM_SEARCH; break;
                    case 2: currentState = STATE_START; break;
                }
                break;
                
            case STATE_FLOOR1_GUARD_ROOM_SEARCH:
                if (choice == 1) currentState = STATE_FLOOR1_GUARD_ROOM;
                break;
                
            case STATE_FLOOR1_CLOAKROOM:
                switch(choice) {
                    case 1: currentState = STATE_FLOOR1_CLOAKROOM_JACKETS; break;
                    case 2: currentState = STATE_FLOOR1_CLOAKROOM_TABLE; break;
                    case 3: currentState = STATE_START; break;
                }
                break;
                
            case STATE_FLOOR1_CLOAKROOM_JACKETS:
            case STATE_FLOOR1_CLOAKROOM_TABLE:
                if (choice == 1) currentState = STATE_FLOOR1_CLOAKROOM;
                break;
                
            case STATE_FLOOR1_TOILET:
                switch(choice) {
                    case 1: currentState = STATE_FLOOR1_TOILET_OBSERVE; break;
                    case 2: 
                        if (player.hasBlackSpots) {
                            currentState = STATE_FLOOR1_TOILET_WASH;
                        }
                        break;
                    case 3: 
                        currentState = STATE_FLOOR1_TOILET_LEAVE;
                        displayCurrentState();
                        if (currentState == STATE_FLOOR1_TOILET_LEAVE) {
                            currentState = STATE_START;
                        }
                        break;
                }
                break;
                
            case STATE_FLOOR1_TOILET_OBSERVE:
                if (!toiletGirlObserved) {
                    switch(choice) {
                        case 1: 
                            player.addItem(ITEM_PIECE);
                            toiletGirlObserved = true;
                            cout << "Вы забираете кусочек плоти.\n";
                            break;
                        case 2: 
                            currentState = STATE_FLOOR1_TOILET;
                            break;
                    }
                } else {
                    if (choice == 1) currentState = STATE_FLOOR1_TOILET;
                }
                break;
                
            case STATE_FLOOR1_TOILET_WASH:
                switch(choice) {
                    case 1: // Просто помыть руки
                        player.addBlackSpots();
                        player.takeDamage();
                        cout << "Ваши руки покрыты черными пятнами! [-1 жизнь]\n";
                        currentState = STATE_FLOOR1_TOILET;
                        break;
                    case 2: // Уксус
                        if (player.hasItem(ITEM_VINEGAR)) {
                            player.removeItem(ITEM_VINEGAR);
                            player.removeBlackSpots();
                            cout << "Черные пятна пропадают!\n";
                            currentState = STATE_FLOOR1_TOILET;
                        }
                        break;
                    case 3: // Пепси
                        if (player.hasItem(ITEM_PEPSI)) {
                            player.removeItem(ITEM_PEPSI);
                            player.addBlackSpots();
                            player.takeDamage();
                            cout << "Черные пятна распространяются! [-1 жизнь]\n";
                            currentState = STATE_FLOOR1_TOILET;
                        }
                        break;
                    case 4: // Энергетик
                        if (player.hasItem(ITEM_ENERGY_DRINK)) {
                            player.removeItem(ITEM_ENERGY_DRINK);
                            player.addBlackSpots();
                            player.takeDamage();
                            cout << "Черные пятна распространяются! [-1 жизнь]\n";
                            currentState = STATE_FLOOR1_TOILET;
                        }
                        break;
                    case 5: // Уйти
                        currentState = STATE_FLOOR1_TOILET;
                        break;
                }
                break;
                
            case STATE_FLOOR2_HALL:
                switch(choice) {
                    case 1: currentState = STATE_FLOOR2_RIGHT; break;
                    case 2: 
                        cout << "Расписание искажено и нечитаемо.\n";
                        system("pause");
                        break;
                    case 3: currentState = STATE_FLOOR2_LEFT; break;
                }
                break;
                
            case STATE_FLOOR2_RIGHT:
                switch(choice) {
                    case 1: currentState = STATE_FLOOR2_DEPARTMENT; break;
                    case 2: currentState = STATE_FLOOR2_STUDY_PART; break;
                    case 3: currentState = STATE_FLOOR2_HALL; break;
                }
                break;
                
            case STATE_FLOOR2_DEPARTMENT:
                if (departmentVisited && choice == 1) {
                    currentState = STATE_FLOOR2_RIGHT;
                }
                break;
                
            case STATE_FLOOR2_STUDY_PART:
                if (choice == 1) currentState = STATE_FLOOR2_RIGHT;
                break;
                
            case STATE_FLOOR2_LEFT:
                switch(choice) {
                    case 1: currentState = STATE_FLOOR2_214; break;
                    case 2: currentState = STATE_FLOOR2_220; break;
                    case 3: 
                        if (player.hasItem(ITEM_CROWBAR)) {
                            currentState = STATE_FLOOR2_STAIRS;
                        }
                        break;
                    case 4: currentState = STATE_FLOOR2_HALL; break;
                }
                break;
                
            case STATE_FLOOR2_214:
                if (!student214Calmed) {
                    switch(choice) {
                        case 1:
                            if (player.hasItem(ITEM_HEADPHONES)) {
                                player.removeItem(ITEM_HEADPHONES);
                                student214Calmed = true;
                                player.addItem(ITEM_CROWBAR);
                                cout << "Студент берет наушники и успокаивается.\n";
                                cout << "Он оставляет вам лом.\n";
                            }
                            break;
                        case 2:
                            cout << "Вы молчите. Студент продолжает сидеть.\n";
                            break;
                        case 3:
                            cout << "Студент вцепляется в лом и пытается ударить вас!\n";
                            player.takeDamage();
                            cout << "Вы потеряли 1 единицу здоровья.\n";
                            break;
                        case 4: currentState = STATE_FLOOR2_LEFT; break;
                    }
                } else {
                    if (choice == 1) currentState = STATE_FLOOR2_LEFT;
                }
                break;
                
            case STATE_FLOOR2_220:
                if (!player.hasSignatures[0]) {
                    switch(choice) {
                        case 1: // Красный
                            if (!entity220GivenScarf && player.hasItem(ITEM_BLUE_YELLOW_SCARF)) {
                                player.removeItem(ITEM_BLUE_YELLOW_SCARF);
                                entity220GivenScarf = true;
                                player.addItem(ITEM_FLASHDRIVE_220);
                                player.hasSignatures[0] = true;
                                cout << "Существо ослабляет хватку, видя шарф.\n";
                                cout << "Оно бросает вам флешку и ставит подпись!\n";
                            }
                            break;
                        case 2: // Синий
                            if (!entity220GivenScarf && player.hasItem(ITEM_BLUE_YELLOW_SCARF)) {
                                entity220GivenScarf = true;
                                player.addItem(ITEM_FLASHDRIVE_220);
                                player.hasSignatures[0] = true;
                                cout << "Существо ухмыляется и бросает вам флешку.\n";
                                cout << "Оно ставит подпись!\n";
                            }
                            break;
                        case 3: // Забрать предметы
                            if (!entity220TakenItems) {
                                player.addItem(ITEM_BLUE_YELLOW_SCARF);
                                player.addItem(ITEM_ENERGY_DRINK);
                                entity220TakenItems = true;
                                cout << "Вы забираете шарф и энергетик.\n";
                            }
                            break;
                        case 4: currentState = STATE_FLOOR2_LEFT; break;
                    }
                } else {
                    if (choice == 1) currentState = STATE_FLOOR2_LEFT;
                }
                break;
                
            case STATE_FLOOR2_STAIRS:
                switch(choice) {
                    case 1: // Вверх
                        currentState = STATE_FLOOR3_HALL;
                        break;
                    case 2: // Вниз
                    case 3: // Стоять
                        currentState = STATE_BAD_END_DARKNESS;
                        break;
                }
                break;
                
            case STATE_FLOOR3_HALL:
                switch(choice) {
                    case 1: currentState = STATE_FLOOR3_320; break;
                    case 2: currentState = STATE_FLOOR3_323; break;
                    case 3: currentState = STATE_FLOOR3_CORRIDOR; break;
                    case 4: currentState = STATE_FLOOR2_HALL; break;
                }
                break;
                
            case STATE_FLOOR3_320:
                if (!sect320Sacrificed) {
                    if (choice == 1 && player.hasItem(ITEM_PIECE)) {
                        player.removeItem(ITEM_PIECE);
                        sect320Sacrificed = true;
                        cout << "Вы приносите кусочек плоти в жертву...\n";
                        cout << "Сектанты исчезают в портале!\n";
                        system("pause");
                        currentState = STATE_FLOOR3_HALL;
                    } else if (choice == 2) {
                        currentState = STATE_FLOOR3_HALL;
                    } else if (choice != 1) {
                        cout << "Сектанты нападают на вас!\n";
                        player.takeDamage();
                        cout << "Вы потеряли 1 единицу здоровья.\n";
                        system("pause");
                    }
                } else {
                    if (choice == 1 && !player.hasItem(ITEM_INFERNAL_TRANSLATOR)) {
                        player.addItem(ITEM_INFERNAL_TRANSLATOR);
                        cout << "Вы подобрали переводчик с инфернального.\n";
                        system("pause");
                        currentState = STATE_FLOOR3_HALL;
                    } else if (choice == 2 || (choice == 1 && player.hasItem(ITEM_INFERNAL_TRANSLATOR))) {
                        currentState = STATE_FLOOR3_HALL;
                    }
                }
                break;
                
            case STATE_FLOOR3_323:
                if (!creature323Fed) {
                    switch(choice) {
                        case 1: // Пепперони
                            if (player.hasItem(ITEM_PEPPERONI_PIZZA)) {
                                player.removeItem(ITEM_PEPPERONI_PIZZA);
                                creature323Fed = true;
                                cout << "Существо наслаждается пиццей!\n";
                                cout << "Оно готово подписать ваш проект.\n";
                            }
                            break;
                        case 2: // Чиловая
                            if (player.hasItem(ITEM_CHILLI_PIZZA)) {
                                player.removeItem(ITEM_CHILLI_PIZZA);
                                player.takeDamage();
                                cout << "Существо выплевывает пиццу! [-1 жизнь]\n";
                            }
                            break;
                        case 3: // Кусочек
                            if (player.hasItem(ITEM_PIECE)) {
                                currentState = STATE_BAD_END_MONSTER;
                                return;
                            }
                            break;
                        case 4: currentState = STATE_FLOOR3_HALL; break;
                    }
                } else if (!player.hasSignatures[1]) {
                    if (choice == 1) {
                        player.hasSignatures[1] = true;
                        cout << "Существо ставит подпись на вашем проекте!\n";
                    } else if (choice == 2 && player.hasItem(ITEM_INFERNAL_TRANSLATOR) && !creature323Translated) {
                        creature323Translated = true;
                        player.addItem(ITEM_NOTE_WITH_PHRASE);
                        cout << "Вы расшифровали записку: 'А шяьфатмф э ьфяшя эоъфътохсьс мщяоя...'\n";
                        cout << "Вы записали фразу на листочек.\n";
                    } else if (choice == 3) {
                        currentState = STATE_FLOOR3_HALL;
                    }
                } else {
                    if (choice == 1) currentState = STATE_FLOOR3_HALL;
                }
                break;
                
            case STATE_FLOOR3_CORRIDOR:
                switch(choice) {
                    case 1: // Пройти мимо
                        cout << "Вы проходите мимо студента.\n";
                        cout << "Дальше есть 306 кабинет и лестница на 4 этаж.\n";
                        cout << "1. Зайти в 306\n";
                        cout << "2. Подняться на 4 этаж\n";
                        cout << "3. Вернуться\n";
                        
                        int nextChoice;
                        cin >> nextChoice;
                        switch(nextChoice) {
                            case 1: currentState = STATE_FLOOR3_306; break;
                            case 2: currentState = STATE_FLOOR4_HALL; break;
                            case 3: currentState = STATE_FLOOR3_HALL; break;
                        }
                        break;
                    case 2: // Дать пропуск
                        if (player.hasItem(ITEM_PASS)) {
                            player.removeItem(ITEM_PASS);
                            studentPassHelped = true;
                            cout << "Студент кричит: 'Наконец-то! Я свободен!'\n";
                            cout << "Из его кожи вылезает трехметровое существо!\n";
                            cout << "Оно уползает вниз, раскидывая преграды.\n";
                            system("pause");
                            currentState = STATE_FLOOR3_HALL;
                        }
                        break;
                    case 3: currentState = STATE_FLOOR3_HALL; break;
                }
                break;
                
            case STATE_FLOOR3_306:
                if (!shadow306Fed) {
                    switch(choice) {
                        case 1: // Пепси
                            if (player.hasItem(ITEM_PEPSI)) {
                                player.removeItem(ITEM_PEPSI);
                                shadow306Fed = true;
                                cout << "Тень поглощает пепси. 'Давай журнал'\n";
                                if (player.hasItem(ITEM_GROUP_MAGAZINE)) {
                                    player.removeItem(ITEM_GROUP_MAGAZINE);
                                    cout << "Журнал заполнен непонятными буквами.\n";
                                    cout << "На месте вашего имени: 'Человек'\n";
                                }
                            }
                            break;
                        case 2: // Журнал
                            if (player.hasItem(ITEM_GROUP_MAGAZINE)) {
                                cout << "Тень требует сначала пепси!\n";
                            }
                            break;
                        case 3: currentState = STATE_FLOOR3_CORRIDOR; break;
                    }
                } else {
                    if (choice == 1) currentState = STATE_FLOOR3_CORRIDOR;
                }
                break;
                
            case STATE_FLOOR4_HALL:
                if (!octopusParalyzed) {
                    switch(choice) {
                        case 1: // Записка
                            if (player.hasItem(ITEM_NOTE_WITH_PHRASE)) {
                                octopusParalyzed = true;
                                cout << "Вы читаете записку... Существо отпускает вас!\n";
                                system("pause");
                            }
                            break;
                        case 2: // Смириться
                            cout << "Существо высасывает вашу душу!\n";
                            player.takeDamage();
                            cout << "Вы потеряли 1 единицу здоровья.\n";
                            system("pause");
                            break;
                    }
                } else {
                    switch(choice) {
                        case 1: currentState = STATE_FLOOR4_408; break;
                        case 2: currentState = STATE_FLOOR4_411; break;
                        case 3: currentState = STATE_FLOOR4_DEFENSE; break;
                        case 4: currentState = STATE_FLOOR3_HALL; break;
                    }
                }
                break;
                
            case STATE_FLOOR4_408:
                if (!creature408Calmed) {
                    if (choice == 1 && player.hasItem(ITEM_GROUP_MAGAZINE)) {
                        player.removeItem(ITEM_GROUP_MAGAZINE);
                        creature408Calmed = true;
                        player.hasSignatures[2] = true;
                        cout << "Существо изучает журнал и успокаивается.\n";
                        cout << "Оно ставит подпись!\n";
                    } else if (choice == 2) {
                        cout << "Существо кусает вас!\n";
                        player.takeDamage();
                        cout << "Вы потеряли 1 единицу здоровья.\n";
                    }
                } else {
                    if (choice == 1) currentState = STATE_FLOOR4_HALL;
                }
                break;
                
            case STATE_FLOOR4_411:
                if (!figure411Helped) {
                    switch(choice) {
                        case 1: // Уксус
                            if (player.hasItem(ITEM_VINEGAR)) {
                                player.removeItem(ITEM_VINEGAR);
                                figure411Helped = true;
                                cout << "Фигура выпивает уксус и падает замертво.\n";
                            }
                            break;
                        case 2: // Нож
                            if (player.hasItem(ITEM_FOLDING_KNIFE)) {
                                player.removeItem(ITEM_FOLDING_KNIFE);
                                figure411Helped = true;
                                cout << "Фигура перерезает себе горло.\n";
                            }
                            break;
                        case 3: // Наблюдать
                        case 4: // Уйти
                            cout << "Фигура взрывает гранату!\n";
                            player.takeDamage();
                            figure411Helped = true;
                            cout << "Вы потеряли 1 единицу здоровья.\n";
                            break;
                    }
                } else {
                    if (choice == 1 && !player.hasItem(ITEM_GRENADE)) {
                        player.addItem(ITEM_GRENADE);
                        cout << "Вы подобрали ручную гранату.\n";
                    } else if (choice == 2 || (choice == 1 && player.hasItem(ITEM_GRENADE))) {
                        currentState = STATE_FLOOR4_HALL;
                    }
                }
                break;
                
            case STATE_FLOOR4_DEFENSE:
                player.checkSignatures();
                if (choice == 1 && player.hasAllSignatures) {
                    currentState = STATE_FLOOR4_DEFENSE_QUESTIONS;
                } else {
                    currentState = STATE_BAD_END_NOT_ADMITTED;
                }
                break;
                
            case STATE_FLOOR4_DEFENSE_QUESTIONS:
                if (player.hasItem(ITEM_FLASHDRIVE_220)) {
                    if (choice == 1) {
                        currentState = STATE_BAD_END_SYSTEM;
                    } else if (choice == 2) {
                        player.removeItem(ITEM_FLASHDRIVE_220);
                        defenseQuestionNum = 0;
                        displayCurrentState();
                    }
                } else {
                    if (defenseQuestionNum < 3) {
                        bool correct = false;
                        switch(defenseQuestionNum) {
                            case 0: correct = (choice == 1); break; // Да
                            case 1: correct = (choice == 1); break; // Нигде
                            case 2: correct = (choice == 2); break; // Разлагаться
                        }
                        
                        if (!correct) {
                            player.takeDamage();
                            cout << "Неверный ответ! [-1 жизнь]\n";
                            if (player.health <= 0) {
                                currentState = STATE_GAME_OVER;
                                return;
                            }
                        }
                        
                        defenseQuestionNum++;
                        if (defenseQuestionNum == 3) {
                            displayCurrentState();
                        }
                    }
                }
                break;
                
            case STATE_BAD_END_ROY:
            case STATE_BAD_END_MONSTER:
            case STATE_BAD_END_DARKNESS:
            case STATE_BAD_END_NOT_ADMITTED:
            case STATE_BAD_END_SYSTEM:
            case STATE_BAD_END_HUMAN:
            case STATE_WIN:
            case STATE_GAME_OVER:
                switch(choice) {
                    case 1: 
                        resetGame();
                        break;
                    case 2: 
                        gameRunning = false;
                        break;
                }
                break;
        }
    }
    
    void handleItemChoice(int choice) {
        if (choice > 0 && choice <= (int)currentItemChoices.size()) {
            Item selectedItem = currentItemChoices[choice - 1];
            
            if (currentState == STATE_FLOOR2_DEPARTMENT) {
                if (selectedItem == ITEM_FOLDING_KNIFE) {
                    cout << "Вы раните существо ножом и сбегаете!\n";
                    player.removeItem(ITEM_FOLDING_KNIFE);
                } else {
                    cout << "Ваша попытка отбиться бесполезна!\n";
                    player.takeDamage();
                    cout << "Вы потеряли 1 единицу здоровья.\n";
                    if (selectedItem != ITEM_FOLDING_KNIFE) {
                        player.removeItem(selectedItem);
                    }
                }
                departmentVisited = true;
            }
            
        } else if (choice == currentItemChoices.size() + 1) {
            // Смириться
            if (currentState == STATE_FLOOR2_DEPARTMENT) {
                cout << "Вы смиряетесь... Камера оказывается в вашем черепе.\n";
                cout << "Теперь за вашими мыслями будет удобно следить.\n";
                departmentVisited = true;
            }
        }
        
        requireItemChoice = false;
        currentState = returnStateAfterItem;
        system("pause");
    }
    
    void checkGameOver() {
        if (player.health <= 0 && currentState != STATE_GAME_OVER && 
            currentState < STATE_BAD_END_ROY && currentState != STATE_FLOOR4_DEFENSE_QUESTIONS) {
            cout << "\n\nВЫ ПОТЕРЯЛИ ВСЕ ЖИЗНИ!\n";
            cout << "Но что-то возвращает вас в начало...\n";
            system("pause");
            resetGame();
        }
    }
    
    void resetGame() {
        player = Player();
        currentState = STATE_START;
        defenseQuestionsStarted = false;
        defenseQuestionNum = 0;
        resetFlags();
    }
    
    string getItemName(Item item) {
        switch(item) {
            case ITEM_ENERGY_DRINK: return "Энергетик";
            case ITEM_GROUP_MAGAZINE: return "Журнал группы";
            case ITEM_PEPPERONI_PIZZA: return "Пицца пепперони";
            case ITEM_CHILLI_PIZZA: return "Чиловая пицца";
            case ITEM_PEPSI: return "Пепси";
            case ITEM_KEY_220: return "Ключ от 220";
            case ITEM_PASS: return "Пропуск";
            case ITEM_HEADPHONES: return "Наушники";
            case ITEM_FOLDING_KNIFE: return "Складной нож";
            case ITEM_VINEGAR: return "Уксус";
            case ITEM_USA_YMZYA: return "Уса ымзя";
            case ITEM_OCHUMFAHP: return "Очумфахп";
            case ITEM_PIECE: return "Кусочек плоти";
            case ITEM_CROWBAR: return "Лом";
            case ITEM_BLUE_YELLOW_SCARF: return "Сине-желтый шарф";
            case ITEM_FLASHDRIVE_220: return "Флешка из 220";
            case ITEM_INFERNAL_TRANSLATOR: return "Переводчик с инфернального";
            case ITEM_NOTE_WITH_PHRASE: return "Записка с фразой";
            case ITEM_GRENADE: return "Ручная граната";
            case ITEM_YOUR_FLASHDRIVE: return "Ваша флешка с проектом";
            case ITEM_BLACK_SPOTS: return "Черные пятна";
            default: return "Неизвестный предмет";
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    
    Game game;
    game.run();
    
    cout << "\nСпасибо за игру!\n";
    return 0;
}
