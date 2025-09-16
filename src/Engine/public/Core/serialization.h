#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <fstream>
#include <string>
#include <toml++/

namespace BloxEngine {
namespace Core {
class Serialization {
public:
  // Serialize an object to a file
  template <typename T>
  static void serialize(const T &object, const std::string &filename);

  // Deserialize an object from a file
  template <typename T>
  static void deserialize(T &object, const std::string &filename);

};
} // namespace Core
} // namespace BloxEngine

#include "serialization.tpp"

#endif // SERIALIZATION_H
