#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <sstream>
#include <chrono>

using namespace std;

auto random_generator = std::mt19937(std::time(nullptr));

vector<vector<int>> create_matrix(size_t dimension) {
    vector<vector<int>> matrix(dimension, vector<int>(dimension)); 
    for (auto& row : matrix) {
        for (int& element : row) {
            element = random_generator() % RAND_MAX;
        }
    }
    return matrix;
}

void save_matrix(const vector<vector<int>>& matrix, const string& filename) {
    ofstream output_file(filename);
    for (const auto& row : matrix) {
        for (int value : row) {
            output_file << value << " ";
        }
        output_file << endl;
    }
}

vector<vector<int>> load_matrix(const string& filename) {
    ifstream input_file(filename);
    vector<vector<int>> matrix;
    string line;

    while (getline(input_file, line)) {
        istringstream stream(line);
        vector<int> row;
        int number;
        while (stream >> number) {
            row.push_back(number);
        }
        if (!row.empty()) {
            matrix.push_back(row);
        }
    }
    return matrix;
}

vector<vector<int>> multiply_matrices(const vector<vector<int>>& first_matrix, const vector<vector<int>>& second_matrix) {
    size_t dimension = first_matrix.size();
    vector<vector<int>> result_matrix(dimension, vector<int>(dimension, 0));
    
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            for (int k = 0; k < dimension; ++k) {
                result_matrix[i][j] += first_matrix[i][k] * second_matrix[k][j];
            }
        }
    }
    return result_matrix;
}

int main() {
    vector<int> dimensions = {50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1500, 2000, 3000};
    
    // Создание тестовых матриц
    for (const auto& dim : dimensions) {
        for (int num = 1; num < 3; ++num) {
            vector<vector<int>> test_matrix = create_matrix(dim);
            string filename = to_string(num) + "_matrix_" + to_string(dim) + ".txt";
            save_matrix(test_matrix, filename);
        }
    }

    vector<double> execution_times(dimensions.size(), 0.0);
    
    // Измерение времени умножения
    for (size_t idx = 0; idx < dimensions.size(); ++idx) {
        int current_dim = dimensions[idx];
        string file1 = "1_" + to_string(current_dim) + ".txt";
        string file2 = "2" + to_string(current_dim) + ".txt";
        string result_file = "result_matrix_" + to_string(current_dim) + ".txt";

        auto start = chrono::steady_clock::now();
        
        vector<vector<int>> matrix_a = load_matrix(file1);
        vector<vector<int>> matrix_b = load_matrix(file2);
        vector<vector<int>> multiplication_result = multiply_matrices(matrix_a, matrix_b);
        
        save_matrix(multiplication_result, result_file);
        
        auto end = chrono::steady_clock::now();
        execution_times[idx] = chrono::duration<double, milli>(end - start).count();
    }

    // Сохранение результатов измерений
    ofstream results_output("stats.txt");
    for (size_t idx = 0; idx < dimensions.size(); ++idx) {
        results_output << dimensions[idx] << ": " << execution_times[idx] << " ms" << endl;
    }

    return 0;
}