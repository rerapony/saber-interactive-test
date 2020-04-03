#pragma once

#include <map>

#define MAX_SIZE 1000

struct ListNode {
    ListNode *      prev;
    ListNode *      next;
    ListNode *      rand; // указатель на произвольный элемент данного списка либо NULL
    std::string     data;

    explicit ListNode(std::string data) :
            prev(nullptr), next(nullptr), rand(nullptr), data(std::move(data)) {};

    ListNode(const ListNode&) = delete;
    void operator =(const ListNode&) = delete;

    bool operator==(ListNode& other) {
        if (data != other.data)
            return false;

        return !((rand != nullptr || other.rand != nullptr) && (rand == nullptr || other.rand == nullptr));
    }

    bool operator!=(ListNode& other) {
        return !operator==(other);
    }

};

class List {
public:
    List(): head(nullptr), tail(nullptr), count(0) {}
    List(const List&) = delete;
    void operator =(const List&) = delete;

    ListNode * front();
    ListNode * back();

    void Serialize   (FILE * file);  // сохранение в файл (файл открыт с помощью fopen(path, "wb"))
    void Deserialize (FILE * file);  // загрузка из файла (файл открыт с помощью fopen(path, "rb"))

    // т.к. в задаче не требуется реализовывать методы, обойдемся простой вставкой в конец для тестирования
    void push_back(const std::string& data);
    void push_node(ListNode * node);

    bool operator==(List& other);
    bool operator!=(List& other);

    void clear();

    ~List() {
        clear();
    }

private:
    ListNode *      head;
    ListNode *      tail;
    int       count;
};

void List::push_back(const std::string& data) {
    auto * node = new ListNode(data);
    if (tail != nullptr) {
        node->prev = tail;
        tail->next = node;
    } else {
        head = node;
    }
    tail = node;
    count++;
}


void List::push_node(ListNode *node) {
    if (tail != nullptr) {
        node->prev = tail;
        tail->next = node;
    } else {
        head = node;
    }
    tail = node;
    count++;
}

void List::clear() {
    ListNode* node;
    while (head != nullptr) {
        node = head->next;
        delete head;
        head = node;
    }
}

// обойти список недостаточно из-за случайных указателей на ноды
// используем мапу, чтобы успешно сериализовать это поле (и впоследствии считать)
// с файлом в функции ничего не делаем
void List::Serialize(FILE *file) {
    std::map<ListNode *, int> m;
    int counter = 1;

    ListNode* node = head;
    while (node != nullptr) {
        m.insert(std::make_pair(node, counter));
        counter++;
        node = node->next;
    }

    fwrite(&count, sizeof(int), 1, file); // сколько всего элементов

    node = head;
    counter = 1;
    int ZERO = 0;
    int data_size = 0;

    while (node != nullptr) {
        fwrite(&counter, sizeof(int), 1, file);

        if (node->rand != nullptr) {
            fwrite(&m[node], sizeof(int), 1, file);
        } else {
            fwrite(&ZERO, sizeof(int), 1, file);
        }

        std::string my_str(node->data);
        data_size = my_str.size();

        fwrite(&data_size, sizeof(int), 1, file); // обозначаем размер строки
        fwrite(my_str.c_str(), sizeof(char), data_size, file);

        counter++;
        node = node->next;
    }
}

void List::Deserialize(FILE *file) {
    int counter;
    int key;
    int rand;
    int data_size;
    char buffer[MAX_SIZE];
    ListNode * node;

    std::map<int, std::pair<ListNode*, int>> m;

    fread(&counter, sizeof(int), 1, file);
    for (int i = 0; i < counter; ++i) {
        fread(&key, sizeof(int), 1, file);
        fread(&rand, sizeof(int), 1, file);
        fread(&data_size, sizeof(int), 1, file);
        fread(buffer, sizeof(char), data_size, file);

        std::string my_str = std::string(buffer, data_size);
        node = new ListNode(my_str);

        m.insert(std::make_pair(key, std::make_pair(node, rand)));
    }

    for (auto& elem : m) {
        key = elem.first;
        node = elem.second.first;
        rand = elem.second.second;

        if (rand != 0) {
            node->rand = m[rand].first;
        }

        this->push_node(node);
    }
}

bool List::operator==(List &other) {
    {
        ListNode * fst = head;
        ListNode * snd = other.head;

        while (fst != nullptr && snd != nullptr)
        {
            if (*fst != *snd)
                return false;

            fst = fst->next;
            snd = snd->next;
        }

        return (fst == nullptr && snd == nullptr);
    }
}

bool List::operator!=(List &other) {
    return !operator==(other);
}

ListNode *List::front() {
    return head;
}

ListNode *List::back() {
    return tail;
}
