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

struct file_info {
    std::string name;
    std::uintmax_t size;
    std::filesystem::file_time_type last_modified;
};

bool compare_entries(const fs::directory_entry& a, const fs::directory_entry& b) {
    return a.path().filename().string() < b.path().filename().string();
}

std::string format_time(const std::filesystem::file_time_type& time) {
    using namespace std::chrono;
    auto time_point = time_point_cast<system_clock::duration>(time - fs::file_time_type::clock::now() + system_clock::now());
    std::time_t tt = system_clock::to_time_t(time_point);
    std::tm* ptm = std::localtime(&tt);
    char buffer[32];
    // Format: Mo, YYYY hh:mm:ss
    std::strftime(buffer, 32, "%b %d, %Y %H:%M:%S", ptm);
    return buffer;
}

std::string format_size(std::uintmax_t bytes) {
    const char* sizes[] = { "B", "KB", "MB", "GB", "TB" };
    int order = 0;
    double dblSize = bytes;

    while (dblSize >= 1024 && order < 4) {
        order++;
        dblSize /= 1024;
    }

    std::ostringstream formatted_size;
    formatted_size << std::fixed << std::setprecision(2) << dblSize << ' ' << sizes[order];
    return formatted_size.str();
}

std::string html_start() {
    return R"(
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

}

std::string end_table() {
    return "</table>";
}

std::string html_end() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");
    std::string current_datetime = ss.str();
    return "<footer>Generated on " + current_datetime + "</footer></body></html>";
}

void process_directory(const fs::path& dir_path, std::string& html_rows, const fs::path& current_dir) {
    std::vector<fs::directory_entry> entries;
    for (const auto& entry : fs::directory_iterator(dir_path)) {
        entries.push_back(entry);
    }

    std::sort(entries.begin(), entries.end(), compare_entries);

    for (const auto& entry : entries) {
        if (entry.is_directory()) {
            fs::path index_path = entry.path();
            if (fs::exists(index_path / "index.html")) {
                fs::path relative_path = fs::relative(index_path, current_dir);
                auto last_modified = fs::last_write_time(index_path / "index.html");
                std::string last_modified_str = format_time(last_modified);
                html_rows += "<tr><td><a href='" + (relative_path / "index.html").string() + "'>" + relative_path.string() + "</a></td><td>N/A</td><td>" + last_modified_str + "</td></tr>";
            }
            process_directory(entry, html_rows, current_dir);
        } else if (entry.is_regular_file()) {
            // For regular files within the directory, skip them for now
        }
    }
}

void process_files(const fs::path& current_dir, std::string& html_rows) {
    std::vector<fs::directory_entry> entries;
    for (const auto& entry : fs::directory_iterator(current_dir)) {
        if (entry.is_regular_file() && entry.path().filename().string()[0] != '.' && entry.path().filename() != "index.html") {
            entries.push_back(entry);
        }
    }

    std::sort(entries.begin(), entries.end(), compare_entries);

    for (const auto& entry : entries) {
        fs::path relative_path = fs::relative(entry.path(), current_dir);
        html_rows += "<tr><td><a href='" + relative_path.string() + "'>" + relative_path.string() + "</a></td><td>" + format_size(fs::file_size(entry.path())) + "</td><td>" + format_time(fs::last_write_time(entry.path())) + "</td></tr>";
    }
}

int main() {
    std::ofstream html_file("index.html");
    if (!html_file.is_open()) {
        std::cerr << "Unable to create HTML file" << std::endl;
        return 1;
    }

    std::string html_rows = "";
    fs::path current_dir = fs::current_path();
    process_directory(current_dir, html_rows, current_dir);
    process_files(current_dir, html_rows);

    html_file << html_start() << html_rows << end_table() << html_end();
    html_file.close();

    std::cout << "HTML file created successfully." << std::endl;
    return 0;
}

