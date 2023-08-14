#include "BuilkProcessor.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <block_size>" << std::endl;
        return 1;
    }

    int blockSize = std::stoi(argv[1]);
    BulkProcessor bulkProcessor(blockSize);
    bulkProcessor.processCommands();

    return 0;
}
