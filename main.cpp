#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <sstream>

namespace fs = std::filesystem;

struct FileInfo {
    std::string name;
    std::uintmax_t size;
    std::time_t last_modified;
};

std::string formatTime(const std::time_t& time) {
    std::tm * ptm = std::localtime(&time);
    char buffer[32];
    // Format: Mo, YYYY hh:mm:ss
    std::strftime(buffer, 32, "%b %d, %Y %H:%M:%S", ptm);  
    return buffer;
}

std::string formatSize(std::uintmax_t bytes) {
    const char* sizes[] = { "B", "KB", "MB", "GB", "TB" };
    int order = 0;
    double dblSize = bytes;

    while (dblSize >= 1024 && order < 4) {
        order++;
        dblSize /= 1024;
    }

    std::ostringstream formattedSize;
    formattedSize << std::fixed << std::setprecision(2) << dblSize << ' ' << sizes[order];
    return formattedSize.str();
}

int main() {
    std::string htmlStart = R"(
<html>
<head>
    <title>Index of Files</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background-color: #f4f4f4;
            color: #333;
            text-align: center;
        }
        h1 {
            color: #444;
        }
        table {
            margin: 20px auto;
            border-collapse: collapse;
            width: 80%;
        }
        th, td {
            padding: 8px 15px;
            border: 1px solid #ddd;
        }
        th {
            background-color: #f0f0f0;
            color: #333;
        }
        tr:nth-child(even) {
            background-color: #f2f2f2;
        }
        a {
            color: #0275d8;
            text-decoration: none;
        }
        a:hover {
            text-decoration: underline;
        }
    </style>
</head>
<body>
    <h1>Files</h1>
    <table>
        <tr>
            <th>Name</th>
            <th>Size</th>
            <th>Last Modified</th>
        </tr>
)";

    std::string htmlEnd = "</table></body></html>";
    std::string htmlRows = "";

    std::vector<FileInfo> files;

    // Iterate over the files in the current directory
    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (entry.is_regular_file()) {
            FileInfo fileInfo;
            fileInfo.name = entry.path().filename().string();
            fileInfo.size = entry.file_size();
            auto ftime = fs::last_write_time(entry);
            fileInfo.last_modified = decltype(ftime)::clock::to_time_t(ftime); // Safer conversion

            files.push_back(fileInfo);
        }
    }

    // Sort files by name
    std::sort(files.begin(), files.end(), [](const FileInfo& a, const FileInfo& b) {
        return a.name < b.name;
    });

    for (const auto& file : files) {
    htmlRows += "<tr><td><a href='" + file.name + "'>" + file.name + "</a></td><td>" + formatSize(file.size) + "</td><td>" + formatTime(file.last_modified) + "</td></tr>";
}

    // Create an HTML file
    std::ofstream htmlFile("index.html");

    // Check if the file is open
    if (!htmlFile.is_open()) {
        std::cerr << "Unable to create HTML file" << std::endl;
        return 1;
    }

    // Write to the HTML file
    htmlFile << htmlStart << htmlRows << htmlEnd;

    // Close the file
    htmlFile.close();

    std::cout << "HTML file created successfully." << std::endl;
    return 0;
}

