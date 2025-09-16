#ifndef BLOXENGINE_SERIALIZATION_HPP
#define BLOXENGINE_SERIALIZATION_HPP

#include <iostream>
#include <fstream>
#include <string>

namespace BloxEngine {
    namespace Core {
        class Serializer {
        public:
            template <typename T>
            static void serialize(const T& data, const std::string& filename) {
                std::ofstream outFile(filename, std::ios::binary);
                if (outFile.is_open()) {
                    outFile.write(reinterpret_cast<const char*>(&data), sizeof(T));
                    outFile.close();
                } else {
                    std::cerr << "Unable to open file for writing: " << filename << std::endl;
                }
            }

            template <typename T>
            static void deserialize(T& data, const std::string& filename) {
                std::ifstream inFile(filename, std::ios::binary);
                if (inFile.is_open()) {
                    inFile.read(reinterpret_cast<char*>(&data), sizeof(T));
                    inFile.close();
                } else {
                    std::cerr << "Unable to open file for reading: " << filename << std::endl;
                }
            }
        };
    }
}

#endif // BLOXENGINE_SERIALIZATION_HPP