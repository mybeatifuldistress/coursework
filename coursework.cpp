#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <locale.h>
#include <set>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>

using namespace std;

// Функция проверяет, является ли набор вершин 'vertices' независимым множеством в графе, представленном матрицей смежности 'adjacency_matrix'.
// 'isDirected' указывает, является ли граф ориентированным.
bool isIndependentSet(const vector < vector < int>>& adjacency_matrix, const set<int>& vertices, bool isDirected) {
    for (int vertex : vertices) { // Перебор всех вершин в наборе
        for (int adjacent_vertex : vertices) { // Перебор всех смежных вершин
            if (vertex != adjacent_vertex) { // Исключение самопетель
                // Проверка на наличие ребра между 'vertex' и 'adjacent_vertex'
                if (isDirected && (adjacency_matrix[vertex][adjacent_vertex] == 1 || adjacency_matrix[adjacent_vertex][vertex] == 1)) {
                    return false; // Найдено ребро в ориентированном графе, множество не независимо
                }
                if (!isDirected && adjacency_matrix[vertex][adjacent_vertex] == 1) {
                    return false; // Найдено ребро в неориентированном графе, множество не независимо
                }
            }
        }
    }
    return true; // Множество вершин независимо
}

// Функция находит все независимые множества в графе, представленном матрицей смежности 'adjacency_matrix'.
// 'isDirected' указывает, является ли граф ориентированным.
vector < set < int>> findAllIndependentSets(const vector < vector < int>>& adjacency_matrix, bool isDirected) {
    int num_vertices = adjacency_matrix.size() - 1; // Количество вершин в графе
    vector < set < int>> all_independent_sets; // Вектор для хранения всех независимых множеств

    // Перебор всех подмножеств вершин графа
    for (int mask = 1; mask < (1 << num_vertices); ++mask) {
        set<int> current_set; // Текущее рассматриваемое подмножество вершин

        // Создание множества вершин на основе битовой маски 'mask'
        for (int i = 0; i < num_vertices; ++i) {
            if (mask & (1 << i)) { // Побитовая операция И, проверяет установлен ли  i - й бит в текущей маске
                current_set.insert(i + 1); // Добавление вершины в множество
            }
        }

        // Проверка, является ли текущее множество независимым
        if (isIndependentSet(adjacency_matrix, current_set, isDirected)) {
            all_independent_sets.push_back(current_set); // Добавление независимого множества в список
        }
    }

    return all_independent_sets; // Возврат списка всех независимых множеств
}

// Функция выводит все найденные независимые множества, содержащиеся в 'all_independent_sets'.
void printAllIndependentSets(const vector<set<int>>& all_independent_sets) {
    if (all_independent_sets.empty()) {
        printf("\nНет независимых множеств.\n");
    }
    else {
        printf("\nВсе независимые множества:\n");
        for (const auto& independent_set : all_independent_sets) { // Перебор всех независимых множеств
            printf("{ ");
            for (int vertex : independent_set) { // Перебор вершин каждого множества
                printf("%d ", vertex); // Вывод номера вершины
            }
            printf("}\n");
        }
    }
}

// Функция генерирует случайную матрицу смежности для графа с 'num_vertices' вершинами и вероятностью наличия ребра 'edge_probability'.
// 'graphType' указывает на тип графа: 1 для неориентированного, 2 для ориентированного.
vector<vector<int>> generateAndPrintMatrix(int num_vertices, double edge_probability, int graphType) {
    vector<vector<int>> adjacency_matrix(num_vertices + 1, vector<int>(num_vertices + 1, 0)); // Матрица смежности

    // Генерация матрицы смежности
    for (int i = 1; i <= num_vertices; ++i) {
        for (int j = (graphType == 1 ? i + 1 : 1); j <= num_vertices; ++j) {
            if (i != j && static_cast<double>(rand()) / RAND_MAX < edge_probability) {
                adjacency_matrix[i][j] = 1; // Установка ребра
                if (graphType == 1) {
                    adjacency_matrix[j][i] = 1; // Для неориентированного графа устанавливаем ребро в обоих направлениях
                }
            }
        }
    }

    // Вывод матрицы смежности
    printf("Матрица смежности:\n");
    for (int i = 1; i <= num_vertices; ++i) {
        for (int j = 1; j <= num_vertices; ++j) {
            printf("%d ", adjacency_matrix[i][j]); // Вывод элемента матрицы
        }
        printf("\n");
    }

    return adjacency_matrix; // Возврат сгенерированной матрицы
}

// Функция проверяет, является ли матрица 'matrix' валидной матрицей смежности для графа.
bool isValidMatrix(const vector<vector<int>>& matrix) {
    int num_vertices = matrix.size() - 1; // Количество вершин, предполагаемое по размеру матрицы

    // Проверка каждого элемента матрицы
    for (int i = 1; i <= num_vertices; ++i) {
        if (matrix[i].size() != num_vertices + 1) {
            return false; // Несоответствие размеров матрицы
        }
        for (int j = 1; j <= num_vertices; ++j) {
            if (matrix[i][j] != 0 && matrix[i][j] != 1) {
                return false; // Недопустимые значения в матрице (должны быть 0 или 1)
            }
        }
    }

    return true; // Матрица валидна
}
// Проверяет, является ли данная матрица валидной матрицей смежности для графа.
bool isMatrixValid(const vector<vector<int>>& matrix, bool isDirected) {
    int num_vertices = matrix.size() - 1; // Число вершин в графе
    for (int i = 1; i <= num_vertices; ++i) {
        for (int j = 1; j <= num_vertices; ++j) {
            // Для ориентированного графа: проверка, что на диагонали нули
            if (isDirected && i == j && matrix[i][j] != 0) return false;
            // Для неориентированного графа: проверка симметричности матрицы
            if (!isDirected && matrix[i][j] != matrix[j][i]) return false;
        }
    }
    return true;
}

// Читает матрицу смежности из файла.
vector<vector<int>> readMatrixFromFile(const string& filename, int& num_vertices, bool isDirected) {
    ifstream file(filename); // Поток для чтения файла
    vector<vector<int>> matrix; // Матрица смежности

    if (file.is_open()) {
        file >> num_vertices; // Считывание числа вершин
        matrix.resize(num_vertices + 1, vector<int>(num_vertices + 1, 0)); // Инициализация матрицы

        for (int i = 1; i <= num_vertices; ++i) {
            for (int j = 1; j <= num_vertices; ++j) {
                file >> matrix[i][j]; // Заполнение матрицы значениями из файла
            }
        }

        // Обнуляем главную диагональ для любого типа графа
        for (int i = 1; i <= num_vertices; ++i) {
            matrix[i][i] = 0;
        }

        // Делаем матрицу симметричной для неориентированного графа
        if (!isDirected) {
            for (int i = 1; i <= num_vertices; ++i) {
                for (int j = i + 1; j <= num_vertices; ++j) {
                    matrix[j][i] = matrix[i][j];
                }
            }
        }

        // Вывод матрицы смежности
        printf("Матрица смежности из файла:\n");
        for (int i = 1; i <= num_vertices; ++i) {
            for (int j = 1; j <= num_vertices; ++j) {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
    }
    else {
        printf("Не удалось открыть файл.\n");
        num_vertices = 0; // Обнуляем количество вершин в случае ошибки
    }

    return matrix; // Возвращаем матрицу
}

// Сохраняет матрицу в файл.
void saveMatrixToFile(const vector < vector < int>>& matrix, const string& filename, ios_base::openmode mode) {
    ofstream file(filename, mode); // Открываем файл с указанным режимом
    if (file.is_open()) {
        // Начинаем с 1, если матрица индексируется с 1
        file << matrix.size() - 1 << "\n";
        for (int i = 1; i < matrix.size(); i++) {
            for (int j = 1; j < matrix[i].size(); j++) {
                file << matrix[i][j] << " ";
            }
            file << "\n";
        }
    }
    else {
        printf("Не удалось открыть файл для записи.\n");
    }
}

// Сохраняет наборы в файл.
void saveSetsToFile(const vector < set < int>>& sets, const string& filename, ios_base::openmode mode) {
    ofstream file(filename, mode); // Открывает файл с указанным режимом

    if (file.is_open()) {
        file << "\n";
        for (const auto& set : sets) {
            file << "{ ";
            for (const auto& element : set) {
                file << element << " "; // Записывает элементы набора в файл
            }
            file << "}\n"; // Новая строка после каждого набора
        }
        file << "------\n";
    }
    else {
        printf("Не удалось открыть файл для записи.\n");
    }
}

// Показывает предыдущие результаты из файла.
void viewPreviousResults(const string& filename) {
    ifstream file(filename); // Поток для чтения файла
    if (file.is_open()) {
        string line;
        printf("Содержимое файла %s:\n", filename.c_str());
        while (getline(file, line)) {
            cout << line << endl; // Выводит каждую строку из файла
        }
    }
    else {
        printf("Не удалось открыть файл для чтения.\n");
    }
}

int main() {

    srand(time(NULL));
    setlocale(LC_ALL, "");
    system("color 3F");
    vector<vector<int>> matrix;
    int num_vertices, i;
    double edge_probability;
    char input[100];
    char filename[100];
    int choice;
    char c;
    int f;
    printf("____________________________________________________________________________________\n");
    printf("|                   Министерство образования Российской Федерации                  |\n");
    printf("|                       Пензенский государственный университет                     |\n");
    printf("|                            Кафедра «Вычислительная техника»                      |\n");
    printf("|                                                                                  |\n");
    printf("|                                                                                  |\n");
    printf("|                                                                                  |\n");
    printf("|                                                                                  |\n");
    printf("|                                ПОЯСНИТЕЛЬНАЯ ЗАПИСКА                             |\n");
    printf("|                        К курсовому проектированию по курсу                       |\n");
    printf("|                  «Логика и основы алгоритмизации в инженерных задачах»           |\n");
    printf("|                   на тему «Алгоритм поиска независимых множеств графа»           |\n");
    printf("|                                                                                  |\n");
    printf("|                                                                                  |\n");
    printf("|                                                                                  |\n");
    printf("|                                                                                  |\n");
    printf("|                                                                                  |\n");
    printf("|                                                                                  |\n");
    printf("|                                                                                  |\n");
    printf("|                                                                                  |\n");
    printf("|                                                                                  |\n");
    printf("|                                                                         Выполнил:|\n");
    printf("|                                                             Студент группы 23ВВВ2|\n");
    printf("|                                                                       Кокарев Д.С|\n");
    printf("|                                                                           Принял:|\n");
    printf("|                                                                        Юрова О.В |\n");
    printf("|                                                                                  |\n");
    printf("|                                                                                  |\n");
    printf("|                                    Пенза 2024                                    |\n");
    printf("____________________________________________________________________________________\n");

    time_t startTime = time(nullptr);
    while (difftime(time(nullptr), startTime) < 3) {}
    system("cls");
    while (true) {

        printf("Выберите действие:\n");
        printf("1. Сгенерировать матрицу\n");
        printf("2. Посмотреть прошлые результаты\n");
        printf("3. Очистить файл\n");
        printf("4. Очистить консоль\n");
        printf("5. Завершить программу\n");
        printf("Ваш выбор: ");

        if (scanf("%d%c", &choice, &c) != 2 || c != '\n' || choice < 1 || choice > 5) {
            printf("Неверный ввод. Пожалуйста, введите число от 1 до 5.\n");
            while (getchar() != '\n'); // Очистка входного буфера
            continue;
        }

        switch (choice) {
        case 1: {
            int graphType, matrixCreationType, saveChoice, fileChoice;
            int num_vertices;
            double edge_probability;

            // Выбор типа графа
            do {
                printf("Выберите тип графа (1 - неориентированный, 2 - ориентированный): ");
                while (scanf("%d%c", &graphType, &c) != 2 || c != '\n' || graphType < 1 || graphType > 2) {
                    printf("Неверный ввод. Пожалуйста, введите 1 или 2.\n");
                    while (getchar() != '\n'); // Очистка входного буфера
                }
            } while (graphType != 1 && graphType != 2);

            // Выбор способа создания матрицы
            do {
                printf("Выберите способ создания матрицы (1 - случайная генерация, 2 - ввод вручную, 3 - из файла): ");
                while (scanf("%d%c", &matrixCreationType, &c) != 2 || c != '\n' || matrixCreationType < 1 || matrixCreationType > 3) {
                    printf("Неверный ввод. Пожалуйста, введите число от 1 до 3.\n");
                    while (getchar() != '\n'); // Очистка входного буфера
                }
            } while (matrixCreationType < 1 || matrixCreationType > 3);

            // Обработка выбора пользователя
            if (matrixCreationType == 1) {
                // Запрос количества вершин
                do {
                    printf("Введите количество вершин в графе: ");
                    while (scanf("%d%c", &num_vertices, &c) != 2 || c != '\n' || num_vertices <= 0) {
                        printf("Неверный ввод. Пожалуйста, введите положительное целое число.\n");
                        while (getchar() != '\n');
                    }
                } while (num_vertices <= 0);

                // Запрос вероятности наличия ребра
                do {
                    printf("Введите вероятность наличия ребра в графе (0 - 1) через запятую: ");
                    while (scanf("%lf%c", &edge_probability, &c) != 2 || c != '\n' || edge_probability < 0 || edge_probability > 1) {
                        printf("Неверный ввод. Введите число от 0 до 1 через запятую.\n");
                        while (getchar() != '\n');
                    }
                } while (edge_probability < 0 || edge_probability > 1);

                matrix = generateAndPrintMatrix(num_vertices, edge_probability, graphType);
            }

            else if (matrixCreationType == 2) {
                do {
                    printf("Введите количество вершин в графе: ");
                    while (scanf("%d%c", &num_vertices, &c) != 2 || c != '\n' || num_vertices <= 0) {
                        printf("Неверный ввод. Пожалуйста, введите положительное целое число.\n");
                        while (getchar() != '\n');
                    }
                } while (num_vertices <= 0);
                matrix.resize(num_vertices + 1, vector<int>(num_vertices + 1, 0));

                // Ввод матрицы смежности вручную
                for (int i = 1; i <= num_vertices; ++i) {
                    for (int j = 1; j <= num_vertices; ++j) {
                        if (graphType == 1 && i < j) {
                            do {
                                printf("Введите наличие ребра между вершинами %d и %d (0 - нет ребра или 1 - есть ребро): ", i, j);
                                if (scanf("%d%c", &matrix[i][j], &c) == 2 && (c == '\n') && (matrix[i][j] == 0 || matrix[i][j] == 1)) {
                                    break; // Валидный ввод (0 или 1)
                                }
                                else {
                                    printf("Неверный ввод. Введите 0 или 1.\n");
                                    while (getchar() != '\n'); // Очистка буфера ввода
                                }
                            } while (true);
                            matrix[j][i] = matrix[i][j]; // Для неориентированных графов
                        }
                        else if (graphType == 2 && i != j) {
                            do {
                                printf("Введите наличие ребра от вершины %d к вершине %d (0 - нет ребра или 1 - есть ребро): ", i, j);
                                if (scanf("%d%c", &matrix[i][j], &c) == 2 && (c == '\n') && (matrix[i][j] == 0 || matrix[i][j] == 1)) {
                                    break; // Валидный ввод (0 или 1)
                                }
                                else {
                                    printf("Неверный ввод. Введите 0 или 1.\n");
                                    while (getchar() != '\n'); // Очистка буфера ввода
                                }
                            } while (true);
                        }
                        else if (i == j) {
                            matrix[i][j] = 0; // Диагональные элементы должны быть нулями
                        }
                    }
                }

                printf("Введённая матрица смежности:\n");
                for (int i = 1; i <= num_vertices; ++i) {
                    for (int j = 1; j <= num_vertices; ++j) {
                        printf("%d ", matrix[i][j]);
                    }
                    printf("\n");
                }
            }
            else if (matrixCreationType == 3) {
                char filename[100];
                printf("Введите имя файла: ");
                scanf("%s", filename);
                int num_vertices; // Переменная для хранения числа вершин
                bool isDirected = (graphType == 2); // Переменная для хранения информации об ориентированности матрицы
                matrix = readMatrixFromFile(filename, num_vertices, isDirected);

            }

            bool isDirected = (graphType == 2);
            vector < set < int>> all_independent_sets = findAllIndependentSets(matrix, isDirected);
            printAllIndependentSets(all_independent_sets);

            // Проверка на сохранение результатов
            int saveResultsChoice;
            do {
                printf("Хотите сохранить результаты? (1 - Да, 2 - Нет): ");
                if (scanf("%d%c", &saveResultsChoice, &c) == 2 && c == '\n' && (saveResultsChoice == 1 || saveResultsChoice == 2)) {
                    break; // Валидный ввод
                }
                else {
                    printf("Неверный ввод. Введите 1 или 2.\n");
                    while (getchar() != '\n'); // Очистка буфера ввода
                }
            } while (true);

            if (saveResultsChoice == 1) {
                int saveFileChoice;
                do {
                    printf("Сохранить в существующий файл (1) или создать новый (2)?: ");
                    if (scanf("%d%c", &saveFileChoice, &c) == 2 && c == '\n' && (saveFileChoice == 1 || saveFileChoice == 2)) {
                        break; // Валидный ввод
                    }
                    else {
                        printf("Неверный ввод. Введите 1 или 2.\n");
                        while (getchar() != '\n'); // Очистка буфера ввода
                    }
                } while (saveFileChoice != 1 && saveFileChoice != 2);

                char filename[100];
                bool fileExists;

                do {
                    printf("Введите имя файла для сохранения результатов: ");
                    scanf("%s", filename);
                    ifstream testFile(filename);
                    fileExists = testFile.good();
                    testFile.close();

                    if (saveFileChoice == 1 && !fileExists) {
                        printf("Файл %s не существует. Попробуйте снова.\n", filename);
                    }
                    else if (saveFileChoice == 2 && fileExists) {
                        printf("Файл %s уже существует. Введите другое имя файла.\n", filename);
                    }
                } while ((saveFileChoice == 1 && !fileExists) || (saveFileChoice == 2 && fileExists));

                ofstream file(filename, ios::out | ios::app); // Открывает файл для добавления в конец
                if (file.is_open()) {
                    saveMatrixToFile(matrix, filename, ios::out | ios::app);
                    saveSetsToFile(all_independent_sets, filename, ios::out | ios::app);
                    printf("Данные сохранены в файл %s\n", filename);
                }
                else {
                    printf("Не удалось открыть файл для записи.\n");
                }
            }

            break;
        }
        case 2: {
            char filename[100];
            printf("Введите имя файла для просмотра: ");
            scanf("%s", filename);
            viewPreviousResults(filename);
            break;
        }
        case 3: {
            char filename[100];
            printf("Введите имя файла для очистки: ");
            scanf("%s", filename);

            ifstream testFile(filename);
            if (testFile.good()) { // Проверка на существование файла
                testFile.close(); // Закрываем файл после проверки
                ofstream file(filename, ios::out | ios::trunc); // Открывает файл для записи с очисткой содержимого

                if (file.is_open()) {
                    printf("Файл %s очищен.\n", filename);
                    file.close();
                }
                else {
                    printf("Не удалось открыть файл для очистки.\n");
                }
            }
            else {
                printf("Файл %s не существует.\n", filename);
            }
            break;
        }

        case 4:
            system("cls");
            break;
        case 5:
            return 0;

        }
    }
}