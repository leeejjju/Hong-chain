#include <iostream>
#include <string>

// Function to convert a single line of text
std::string binary_to_text(const std::string& binary_data) {
    // Here, we assume binary_data is already text
    // In a real-world scenario, you might want to add conversion logic if needed
    return binary_data;
}

int main() {
    std::string word;

    // Read one line at a time from stdin
    // std::getline() reads an entire line from the input stream and stores it in the string 'line'
    // It returns true if the read was successful, false if EOF or an error occurs
    while (std::cin >> word) {
        // Convert the single line to text
        std::string text_result = word;

        // Output the conversion result
        if (!text_result.empty()) {
            std::cout << "Text data: " << text_result << std::endl;
        } else {
            // Print a warning message if the conversion fails
            std::cerr << "Warning: Failed to decode binary data to text." << std::endl;
        }
    }

    return 0;
}
