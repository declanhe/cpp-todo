// cpp-todo - a tiny command-line todo list.
//
// The whole program is in this one file on purpose: read it top to bottom
// and you have seen everything. Tasks are saved to "todos.txt" in the
// current directory, one per line, in the form:  <0|1>|<text>

#include <algorithm>
#include <charconv>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {

constexpr const char* kDataFile = "todos.txt";

// ------------------------------------------------------------------ data --

struct Task {
    std::string text;
    bool done = false;
};

class TodoList {
public:
    void add(const std::string& text) { tasks_.push_back({text, false}); }

    // Both of these return false when there is no task with that number.
    bool setDone(int number, bool done) {
        Task* task = at(number);
        if (task == nullptr) return false;
        task->done = done;
        return true;
    }

    bool remove(int number) {
        if (at(number) == nullptr) return false;
        tasks_.erase(tasks_.begin() + (number - 1));
        return true;
    }

    int removeDone() {
        const std::size_t before = tasks_.size();
        tasks_.erase(std::remove_if(tasks_.begin(), tasks_.end(),
                                    [](const Task& task) { return task.done; }),
                     tasks_.end());
        return static_cast<int>(before - tasks_.size());
    }

    void print() const {
        if (tasks_.empty()) {
            std::cout << "  Nothing to do yet - add your first task!\n";
            return;
        }
        for (std::size_t i = 0; i < tasks_.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << (tasks_[i].done ? "[x] " : "[ ] ")
                      << tasks_[i].text << '\n';
        }
        const std::size_t finished = countDone();
        std::cout << "  " << tasks_.size() - finished << " left, " << finished << " done\n";
    }

    // --------------------------------------------------------------- files --

    void load() {
        tasks_.clear();
        std::ifstream in(kDataFile);
        std::string line;
        while (std::getline(in, line)) {
            if (line.size() < 3 || line[1] != '|') continue;  // skip junk lines
            tasks_.push_back({line.substr(2), line[0] == '1'});
        }
    }

    void save() const {
        std::ofstream out(kDataFile);
        for (const Task& task : tasks_) {
            out << (task.done ? '1' : '0') << '|' << task.text << '\n';
        }
    }

private:
    // Task numbers are 1-based, exactly as print() shows them.
    Task* at(int number) {
        if (number < 1 || number > static_cast<int>(tasks_.size())) return nullptr;
        return &tasks_[number - 1];
    }

    std::size_t countDone() const {
        return static_cast<std::size_t>(std::count_if(
            tasks_.begin(), tasks_.end(), [](const Task& task) { return task.done; }));
    }

    std::vector<Task> tasks_;
};

// --------------------------------------------------------------- console --

// Windows consoles still default to a legacy code page, which turns non-ASCII
// text into garbage. Switch to UTF-8 so typing and saving stay consistent.
void useUtf8Console() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

std::string askLine(const std::string& prompt) {
    std::cout << prompt << std::flush;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

// Returns -1 when the line is not a plain number.
int askNumber(const std::string& prompt) {
    const std::string line = askLine(prompt);
    int value = 0;
    const char* const end = line.data() + line.size();
    const auto result = std::from_chars(line.data(), end, value);
    if (result.ec != std::errc{} || result.ptr != end) return -1;
    return value;
}

void printMenu() {
    std::cout << "\n"
                 "  1) add        2) list       3) done\n"
                 "  4) undo       5) remove     6) clear done\n"
                 "  0) quit\n";
}

}  // namespace

int main() {
    useUtf8Console();

    TodoList todos;
    todos.load();

    std::cout << "=== cpp-todo ===\n";

    bool running = true;
    while (running) {
        todos.print();
        printMenu();

        const int choice = askNumber("> ");
        if (!std::cin) break;  // Ctrl+Z or piped input ran out: stop quietly

        switch (choice) {
            case 1: {
                const std::string text = askLine("  task: ");
                if (text.empty()) {
                    std::cout << "  (empty - nothing added)\n";
                } else {
                    todos.add(text);
                }
                break;
            }
            case 2:
                break;  // the list is already printed at the top of the loop
            case 3:
            case 4: {
                const int number = askNumber("  number: ");
                if (!todos.setDone(number, choice == 3)) {
                    std::cout << "  no task " << number << '\n';
                }
                break;
            }
            case 5: {
                const int number = askNumber("  number: ");
                if (!todos.remove(number)) {
                    std::cout << "  no task " << number << '\n';
                }
                break;
            }
            case 6:
                std::cout << "  cleared " << todos.removeDone() << " finished task(s)\n";
                break;
            case 0:
                running = false;
                break;
            default:
                std::cout << "  pick a number from the menu\n";
                break;
        }
    }

    todos.save();
    std::cout << "\n  saved to " << kDataFile << " - bye!\n";
    return 0;
}