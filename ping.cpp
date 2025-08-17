#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <chrono>
#include <ctime>
#include <iomanip>


const std::string INPUT_FILE = "ip_list.txt";
const std::string OUTPUT_FILE = "ping_log.txt";

// Function to ping a given IP address
// Returns true if the IP responded, false otherwise
bool ping_ip(const std::string& ip) {
    // 1 packet
    // 10 timeout
    std::string command = "ping -c 1 -w 10 " + ip + " 2>&1";

    // Use popen to execute the command and get a file pointer to read its output
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error: Could not run ping command for " << ip << std::endl;
        return false;
    }

    char buffer[128];
    std::string result = "";

    // append ping result to bufer
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL) {
            result += buffer;
        }
    }

    pclose(pipe);

    // check for success
    if (result.find("0 received") != std::string::npos ||
        result.find("100% packet loss") != std::string::npos) {
        return false; // ping fail
    } else {
        return true; // at least one ping succeeded
    }
}

int main() {
    std::ifstream input_file(INPUT_FILE);
    if (!input_file.is_open()) {
        std::cerr << "Error: Could not open input file '" << INPUT_FILE << "'. "
                  << "Please create it and list IPs, one per line." << std::endl;
        return 1;
    }

    std::ofstream output_file(OUTPUT_FILE, std::ios_base::app);
    if (!output_file.is_open()) {
        std::cerr << "Error: Could not open output file '" << OUTPUT_FILE << "' for logging." << std::endl;
        return 1;
    }

    std::string ip;
    std::vector<std::string> ips_to_ping;


    while (std::getline(input_file, ip)) {
        if (!ip.empty()) {
            size_t first = ip.find_first_not_of(" \t\n\r");
            size_t last = ip.find_last_not_of(" \t\n\r");
            if (std::string::npos == first) {
                // blank line
                continue;
            }
            ips_to_ping.push_back(ip.substr(first, (last - first + 1)));
        }
    }
    input_file.close();

    // log time
    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&current_time);

    std::ostringstream oss;
    oss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
    std::string timestamp = oss.str();

    // ping each ip
    for (const std::string& current_ip : ips_to_ping) {
        bool responded = ping_ip(current_ip);

        output_file << timestamp << " - IP: " << current_ip << " - Status: "
                    << (responded ? "Responded" : "Not Responded") << std::endl;
        std::cout << "Logged ping for " << current_ip << ": " << (responded ? "Responded" : "Not Responded") << std::endl;
    }
    output_file << std::endl;
    output_file.close();

    std::cout << "Ping operations completed and results logged to " << OUTPUT_FILE << std::endl;

    return 0;
}
