//Projekt 1 Projektowanie efektwnych algorytmów
//Autor Cyprian Kozubek nr.indeksu 272959
//Problem komiwojazera: przegląd zupełny, metoda najbliszych sasiadow, losowa
//Projekt 1 - kopia
#include <iostream>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <ctime>


using namespace std;

//Elementy zajmujące się mierzeniem czasu
double t1 = 0.0;
double t2 = 0.0;
double t3 = 0.0;
double PCFreq = 0.0;
__int64 CounterStart = 0;
void StartCounter(){
    LARGE_INTEGER li;
    if( !QueryPerformanceFrequency( & li ) )
        cout << "QueryPerformanceFrequency failed!\n";
    PCFreq = double( li.QuadPart ) / 1000.0;
    QueryPerformanceCounter( & li );
    CounterStart = li.QuadPart;
}
double GetCounter(){
    LARGE_INTEGER li;
    QueryPerformanceCounter( & li );
    return double( li.QuadPart - CounterStart ) / PCFreq;
}
int ** Matrix = nullptr;
int matrix_size;
// Funkcja alokująca pamięć dla macierzy
void allocateMatrix(int rows, int cols) {
    Matrix = new int*[rows];   // Alokacja tablicy wskaźników
    for (int i = 0; i < rows; i++) {
        Matrix[i] = new int[cols]; // Alokacja pamięci dla każdego wiersza
    }
}
//Wczytywanie danych z pliku
void loadGraphFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename << endl;
        exit(1);
    }
    int n;
    file >> n;
    matrix_size = n;
    allocateMatrix(n,n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file >> Matrix[i][j];
        }
    }
    file.close();
}
//Generowanie losowego grafu (asymetrycznego)
void generate_graph(){
    int n = 0;
    cout<<"Podaj rozmiar grafu: ";
    cin >> n;
    matrix_size = n;
    allocateMatrix(n,n);
    for(int i =0; i < n; i++){
        for(int j = 0; j<n; j++){
            if(i == j)  Matrix[i][j] = -1;
            else{
                int liczba = rand() % 100 + 1;
                Matrix[i][j] = liczba;
            }
        }
    }
}
//Generowanie losowego grafu (symetrycznego)
void generate_graph_sym() {
    int n = 0;
    cout << "Podaj rozmiar grafu: ";
    cin >> n;
    matrix_size = n;
    allocateMatrix(n, n);

    // Generowanie wartości tylko dla górnego trójkąta macierzy
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                Matrix[i][j] = -1;  // Na przekątnej umieszczamy -1
            } else if (i < j) {
                int liczba = rand() % 100 + 1;  // Losowanie wartości
                Matrix[i][j] = liczba;
                Matrix[j][i] = liczba;  // Symetryczne przypisanie
            }
        }
    }
}
//Wyswietlanie macierzy
void print_matrix(int size){
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cout<<Matrix[i][j]<<" ";
        }
        cout<<endl;
    }
}
//Funkcja do zamiany dwóch elementów
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
//Problem komiwojazera - przeglad zupelny
// Funkcja do obliczania długości trasy dla danej permutacji
int calculate_route(const int* cities) {
    int current_route = 0;
    int current_city = 0; // Zaczynamy od miasta 0
    // Przechodzimy przez kolejne miasta w permutacji
    for (int i = 0; i < matrix_size - 1; i++) {
        current_route += Matrix[current_city][cities[i]];
        current_city = cities[i];
    }
    // Dodajemy drogę powrotną do miasta początkowego (0)
    current_route += Matrix[current_city][0];
    return current_route;
}
// Funkcja rekurencyjna do generowania permutacji i znalezienia najkrótszej trasy
void permute(int* cities, int start, int end, int& min_route, int* best_route) {
    if (start == end) {
        // Obliczamy trasę dla bieżącej permutacji
        int current_route = calculate_route(cities);
        if (current_route < min_route) {
            min_route = current_route;
            // Zapisujemy bieżącą permutację jako najlepszą trasę
            for (int i = 0; i < matrix_size - 1; i++) {
                best_route[i] = cities[i];
            }
        }
        return;
    }
    for (int i = start; i <= end; i++) {
        swap(&cities[start], &cities[i]);   // Zamiana elementów
        permute(cities, start + 1, end, min_route, best_route); // Rekurencyjne wywołanie dla reszty elementów
        swap(&cities[start], &cities[i]);   // Cofanie zmiany (backtracking)
    }
}
// Problem komiwojażera - przegląd zupełny
void travel_brute_force() {
    int cities[matrix_size - 1]; // Tablica miast (bez miasta początkowego 0)
    for (int i = 1; i < matrix_size; i++) {
        cities[i - 1] = i; // Wypełniamy tablicę miastami 1, 2, ..., matrix_size - 1
    }
    int min_route = INT_MAX;
    int best_route[matrix_size - 1]; // Tablica do przechowywania najlepszej permutacji
    // Generujemy wszystkie permutacje miast
    permute(cities, 0, matrix_size - 2, min_route, best_route);
    // Zapis do pliku
    ofstream outfile("wyniki_brute_force.txt");
    outfile << "Algorytm Brute Force\n";
    outfile << "Najmniejsza droga: " << min_route << "\n";
    outfile << "Trasa: 0 -> "; // Zaczynamy od miasta 0
    for (int i = 0; i < matrix_size - 1; ++i) {
        outfile << best_route[i] << " -> ";
    }
    outfile << "0\n"; // Powrót do miasta początkowego
    outfile.close();

    // Wyświetlenie wyników
    cout << "Najmniejsza droga: " << min_route << endl;
    cout << "Trasa: 0 -> "; // Zaczynamy od miasta 0
    for (int i = 0; i < matrix_size - 1; ++i) {
        cout << best_route[i] << " -> ";
    }
    cout << "0" << endl; // Powrót do miasta początkowego
}
// Problem komiwojażera - metoda najbliższych sąsiadów
void travel_nearest_neighbor() {
    int best_distance = INT_MAX; // Zmienna do przechowywania najlepszej znalezionej odległości
    int best_route[matrix_size]; // Zmienna do przechowywania najlepszej trasy

    // Iterujemy przez każde miasto jako punkt startowy
    for (int start_city = 0; start_city < matrix_size; ++start_city) {
        bool visited[matrix_size]; // Tablica śledząca odwiedzone miasta
        for (int i = 0; i < matrix_size; i++)
            visited[i] = false;

        visited[start_city] = true; // Oznaczamy punkt startowy jako odwiedzony
        int total_distance = 0; // Łączna odległość
        int route[matrix_size]; // Przechowuje kolejność odwiedzanych miast
        route[0] = start_city; // Ustawiamy punkt startowy na pierwszym miejscu

        int current_city = start_city; // Rozpoczynamy od aktualnego miasta

        // Główna pętla, która przechodzi przez wszystkie miasta
        for (int step = 1; step < matrix_size; ++step) {
            int nearest_city = -1;
            int min_distance = INT_MAX;

            // Szukamy najbliższego nieodwiedzonego miasta
            for (int i = 0; i < matrix_size; ++i) {
                if (!visited[i] && Matrix[current_city][i] < min_distance && Matrix[current_city][i] != -1) {
                    nearest_city = i;
                    min_distance = Matrix[current_city][i];
                }
            }

            // Przemieszczamy się do najbliższego miasta
            if (nearest_city != -1) {
                current_city = nearest_city;
                visited[current_city] = true;
                route[step] = current_city;
                total_distance += min_distance;
            }
        }

        // Powrót do miasta początkowego
        total_distance += Matrix[current_city][start_city];

        // Sprawdzamy, czy ta trasa jest lepsza od najlepszej dotychczas
        if (total_distance < best_distance) {
            best_distance = total_distance;
            // Zapisujemy najlepszą trasę
            for (int i = 0; i < matrix_size; ++i) {
                best_route[i] = route[i];
            }
        }
    }

    // Zapis do pliku
    ofstream outfile("wyniki_nearest_neighbor.txt");
    outfile << "Algorytm Najbliższego Sąsiada\n";
    outfile << "Najmniejsza droga: " << best_distance << "\n";
    outfile << "Trasa: ";
    for (int i = 0; i < matrix_size; ++i) {
        outfile << best_route[i] << " -> ";
    }
    outfile << best_route[0] << "\n";  // Powrót do punktu początkowego
    outfile.close();

    // Wyświetlenie wyników
    cout << "Najmniejsza droga (NN): " << best_distance << endl;
    cout << "Trasa: ";
    for (int i = 0; i < matrix_size; ++i) {
        cout << best_route[i] << " -> ";
    }
    cout << best_route[0] << endl;  // Powrót do punktu początkowego
}
// Funkcja tasująca tablicę miast - algorytm Fishera-Yatesa
void random_shuffle(int* array, int size) {
    for (int i = size - 1; i > 0; --i) {
        int j = rand() % (i + 1); // Losowy indeks z zakresu [0, i]
        swap(&array[i], &array[j]); // Zamiana elementów
    }
}
// Funkcja obliczająca długość trasy dla permutacji miast
int calculate_route_random(const int* cities) {
    int current_route = 0;
    int current_city = 0; // Zaczynamy od miasta 0
    for (int i = 0; i < matrix_size - 1; i++) {
        current_route += Matrix[current_city][cities[i]];
        current_city = cities[i];
    }
    current_route += Matrix[current_city][0]; // Powrót do miasta początkowego
    return current_route;
}
// Problem komiwojażera - metoda losowa
void travel_random(int num_iterations) {
    int cities[matrix_size - 1];
    for (int i = 1; i < matrix_size; i++) {
        cities[i - 1] = i; // Inicjalizujemy tablicę miastami 1, 2, ..., n-1
    }
    int min_route = INT_MAX;
    int best_route[matrix_size - 1];
    // Powtarzamy losowanie przez określoną liczbę iteracji
    for (int i = 0; i < num_iterations; ++i) {
        random_shuffle(cities, matrix_size - 1); // Tasowanie miast
        int current_route = calculate_route_random(cities); // Obliczenie długości trasy
        if (current_route < min_route) {
            min_route = current_route;
            // Zapisujemy bieżącą trasę jako najlepszą
            for (int j = 0; j < matrix_size - 1; j++) {
                best_route[j] = cities[j];
            }
        }
    }
    // Zapis do pliku
    ofstream outfile("wyniki_random.txt");
    outfile << "Algorytm Losowy\n";
    outfile << "Najmniejsza droga: " << min_route << "\n";
    outfile << "Trasa: 0 -> "; // Zaczynamy od miasta 0
    for (int i = 0; i < matrix_size - 1; ++i) {
        outfile << best_route[i] << " -> ";
    }
    outfile << "0\n"; // Powrót do miasta początkowego
    outfile.close();

    // Wyświetlenie wyników
    cout << "Najmniejsza droga (random): " << min_route << endl;
    cout << "Trasa: 0 -> "; // Zaczynamy od miasta 0
    for (int i = 0; i < matrix_size - 1; ++i) {
        cout << best_route[i] << " -> ";
    }
    cout << "0" << endl; // Powrót do miasta początkowego
}

void Menu_Type(int n){
    if(n == 1){
        generate_graph();
    } else if(n == 2)
        generate_graph_sym();
}
//Menu wyboru algorytmow
void Menu_Alg(){
    int wybor = 1;
    while (wybor != 0) {
        cout << "Wybierz algorytm:"<<endl;
        cout << "1.Przeglad zupelny" << endl;
        cout << "2.Metoda najblizszych sasiadow" << endl;
        cout << "3.Metoda losowa" << endl;
        cout << "0.Wyjscie" << endl;
        cin >> wybor;
        switch(wybor){
            case 1: {

                StartCounter();
                travel_brute_force();
                t1 += GetCounter();
                cout<<"Czas dzialania w ms: "<<t1<<endl;
                break;
            }
            case 2: {
                StartCounter();
                travel_nearest_neighbor();
                t2 += GetCounter();
                cout << "Czas dzialania w ms: " << t2 << endl;
                break;

            }
            case 3: {
                int num_iterations = 10000;
                cout << "Podaj liczbe iteracji dla metody losowej: ";
                cin >> num_iterations;
                StartCounter();
                travel_random(num_iterations);
                t3 += GetCounter();
                cout << "Czas dzialania w ms: " << t3 << endl;
                break;
            }
            default: break;
        }
    }
}
//Menu glowne programu
void Menu(){ //Dane1(6x6): 150, Dane2(8x8): 136, Dane3(11x11): 149
    int wybor = -1;
    while (wybor != 0) {
        cout <<endl<< "1.Wczytanie danych z pliku" << endl;
        cout << "2.Wygenerowanie danych losowych" << endl;
        cout << "3.Wyswietlenie ostatnio wczytanych lub wygenerowanych danych" << endl;
        cout << "4.Uruchomienie danego algorytmu dla ostatnio wczytanych/wygenerowanych danych i wyswietlenie wynikow" << endl;
        //(należy wyświetlić długość ścieżki, ciąg wierzchołków oraz czas wykonania algorytmu)
        cout << "0.Wyjscie" << endl;
        cout << "Podaj jaka operacje wykonac: ";
        cin >> wybor;
        switch(wybor){
            case 1: {
                string filename;
                cout<<"Podaj nazwe pliku do wczytania(sciezka absolutna): ";
                cin >> filename;
                loadGraphFromFile(filename);
                break;
            }
            case 2: {
                int n = -1;
                cout<<"Podaj jaki graf wygenerowac:"<<endl;
                cout<<"1.Asymetryczny"<<endl;
                cout<<"2.Symetryczny"<<endl;
                cin>>n;
                Menu_Type(n);
                break;
            }
            case 3:{
                print_matrix(matrix_size);
                break;
            }
            case 4: {
                Menu_Alg();
                break;
            }
            default: break;
        }
    }
}
int main() {
    Menu();
    return 0;
}
