// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include <iostream>
#include <iomanip>
#include <locale>
#include <unordered_map>
#include <cstring>
#include <ctime>
#include <vector>

constexpr auto kBufferSize = 1024 * 1024;
char Buffer[kBufferSize];

struct Fields {
    std::uint32_t time = 0;
    std::size_t mallocs = 0;
    std::size_t reallocs = 0;
    std::size_t frees = 0;
    std::size_t unknownReallocs = 0;
    std::size_t unknownFrees = 0;
};
Fields State;
std::unordered_map<std::uint64_t, std::size_t> Map;
std::vector<Fields> Snapshots;

void WriteTime(std::uint32_t time) {
    std::time_t t = std::time_t(time);
    const auto parsed = std::localtime(&t);
    std::cout
        << std::setw(2) << std::setfill('0') << parsed->tm_hour
        << ":"
        << std::setw(2) << std::setfill('0') << parsed->tm_min
        << ":"
        << std::setw(2) << std::setfill('0') << parsed->tm_sec;
}

void PrintState() {
    if (!State.time) {
        return;
    }
    WriteTime(State.time);
    auto full = std::uint64_t(0);
    for (const auto &[address, amount] : Map) {
        full += amount;
    }

    class NumPunct final : public std::numpunct<char> {
    protected:
        char do_thousands_sep() const override { return '\''; }
        std::string do_grouping() const override { return "\03"; }
    };

    const auto &locale = std::cout.getloc();
    std::cout.imbue(std::locale(std::locale::classic(), new NumPunct()));
    std::cout
        << ": "
        << std::setw(13) << std::setfill(' ') << full
        << " (unknown: "
        << State.unknownFrees
        << ")"
        << std::endl;
    std::cout.imbue(locale);
}

void Add(std::uint64_t address, std::uint64_t size) {
    const auto i = Map.find(address);
    if (i != end(Map)) {
        std::cout
            << "WARNING: Repeated entry for "
            << address
            << " (size: "
            << size
            << ")."
            << std::endl;
        return;
    }
    Map.emplace(address, size);
}

void ParseMalloc(const char *buffer) {
    const auto size = *reinterpret_cast<const std::uint64_t*>(buffer);
    const auto address = *reinterpret_cast<const std::uint64_t*>(buffer + 8);
    Add(address, size);
    ++State.mallocs;
}

void ParseRealloc(const char *buffer) {
    const auto old = *reinterpret_cast<const std::uint64_t*>(buffer);
    const auto size = *reinterpret_cast<const std::uint64_t*>(buffer + 8);
    const auto address = *reinterpret_cast<const std::uint64_t*>(buffer + 16);
    const auto i = Map.find(old);
    if (i == end(Map)) {
        ++State.unknownReallocs;
        Add(address, size);
    } else if (old != address) {
        Map.erase(i);
        Add(address, size);
        ++State.reallocs;
    } else {
        i->second = size;
        ++State.reallocs;
    }
}

void ParseFree(const char *buffer) {
    const auto address = *reinterpret_cast<const std::uint64_t*>(buffer);
    const auto i = Map.find(address);
    if (i == end(Map)) {
        ++State.unknownFrees;
    } else {
        Map.erase(i);
        ++State.frees;
    }
}

long Parse(const char *buffer, const char *end) {
    auto result = 0;
    while (end > buffer) {
        const auto command = buffer[0];
        auto entry = 0;
        switch (command) {
        case 1: entry = 5 + 2 * sizeof(std::uint64_t); break;
        case 2: entry = 5 + 3 * sizeof(std::uint64_t); break;
        case 3: entry = 5 + sizeof(std::uint64_t); break;
        default:
            std::cout
                << "WARNING: Garbage in trace file, command: "
                << int(command)
                << "."
                << std::endl;
            return -1;
        }
        if (end - buffer < entry) {
            break;
        }
        const auto time = *reinterpret_cast<const std::uint32_t*>(++buffer);
        buffer += 4;
        if (time > State.time) {
            PrintState();
            State.time = time;
        } else if (time < State.time) {
            std::cout
                << "WARNING: Time "
                << time
                << " after "
                << State.time
                << "."
                << std::endl;
        }
        switch (command) {
        case 1: ParseMalloc(buffer); break;
        case 2: ParseRealloc(buffer); break;
        case 3: ParseFree(buffer); break;
        }
        buffer += entry - 5;
        result += entry;
    }
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout
            << "Usage 'allocation_trace_reader [trace_file_path]'."
            << std::endl;
        return -1;
    }
    const auto file = fopen(argv[1], "rb");
    if (!file) {
        std::cout
            << "ERROR: Could not open '"
            << argv[1]
            << "'."
            << std::endl;
        return -1;
    }
    char *data = Buffer;
    while (true) {
        const auto read = fread(data, 1, Buffer + kBufferSize - data, file);
        if (read == 0) {
            if (data != Buffer) {
                std::cout
                    << "WARNING: Trace file end is corrupt, could not parse: "
                    << std::size_t(data - Buffer)
                    << std::endl;
            }
            break;
        }
        data += read;
        const auto parsed = Parse(Buffer, data);
        if (parsed < 0) {
            break;
        }
        data -= parsed;
        if (data - Buffer > 0) {
            std::memmove(Buffer, Buffer + parsed, data - Buffer);
        }
    }
    PrintState();
    std::cout << "Mallocs: " << State.mallocs << "." << std::endl;
    std::cout << "Reallocs: " << State.reallocs << "." << std::endl;
    std::cout << "Frees: " << State.frees << "." << std::endl;
    if (State.unknownReallocs) {
        std::cout
            << "Unknown realloc() calls: "
            << State.unknownReallocs
            << "."
            << std::endl;
    }
    return 0;
}
