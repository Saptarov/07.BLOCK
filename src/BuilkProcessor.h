#ifndef BUILKPROCESSOR_H
#define BUILKPROCESSOR_H
#include "common.h"

class BulkProcessor {
public:
    BulkProcessor(int blockSize) : blockSize_(blockSize) {}

    void processCommands() {
        std::string command;
        while (std::getline(std::cin, command)) {
            if (command == "{") {
                startDynamicBlock();
            } else if (command == "}") {
                endDynamicBlock();
            } else {
                processCommand(command);
            }
        }
    }

    void processCommands(std::vector<std::string> commands) {
        for (auto& command : commands) {
            std::cout << command << std::endl;
            if (command == "{") {
                startDynamicBlock();
            } else if (command == "}") {
                endDynamicBlock();
            } else {
                processCommand(command);
            }
        }
    }

private:
    void processCommand(const std::string& command) {
        currentBlock_.push_back(command);
        if (dynamicBlocks_.empty() && currentBlock_.size() == blockSize_) {
            outputCurrentBlock();
        }
    }

    void startDynamicBlock() {
        if (dynamicBlocks_.empty()) {
            outputCurrentBlock();
            dynamicBlocks_.push_back(DynamicBlock(true));
        } else {
            dynamicBlocks_.push_back(DynamicBlock(false));
        }
    }

    void endDynamicBlock() {
        auto block = dynamicBlocks_.back();
        if (block.canWriteCommand()) {
            outputCurrentBlock();
        }
        dynamicBlocks_.pop_back();
    }

    void outputCurrentBlock() {
        if (currentBlock_.empty()) {
            return;
        }
        auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string filename = "b" + std::to_string(currentTime) + ".log";

        std::ofstream file(filename);
        for (const auto& command : currentBlock_) {
            file << command << std::endl;
        }
        file.close();

        std::cout << "bulk:";
        for (const auto& command : currentBlock_) {
            std::cout << " " << command;
        }
        std::cout << std::endl;

        currentBlock_.clear();
    }

private:
    struct DynamicBlock {
        DynamicBlock(bool lastClosedBracket)
            : _lastClosedBracket(lastClosedBracket)
        {}
        bool canWriteCommand() {
            return _lastClosedBracket;
        }
    private:
        bool _lastClosedBracket;
    };

    const size_t blockSize_;
    std::vector<std::string> currentBlock_;
    std::list<DynamicBlock> dynamicBlocks_;
};
#endif // BUILKPROCESSOR_H
