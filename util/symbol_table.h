#ifndef UTILS_SYMBOL_TABLE_H_
#define UTILS_SYMBOL_TABLE_H_

#include <initializer_list>
#include <string>
#include <unordered_map>
#include <vector>

namespace mprs {

template <typename T>
class SymbolTable {
 public:
  static const int NotFound;

  SymbolTable() {}

  SymbolTable(std::initializer_list<T> items) : objects_(items) {
    for (size_t i = 0; i < objects_.size(); ++i) {
      object_to_index_[objects_[i]] = i;
    }
  }

  int Lookup(const T& object) const {
    auto it = object_to_index_.find(object);
    if (it != object_to_index_.end()) {
      return it->second;
    }
    return NotFound;
  }

  int Insert(const T& object) {
    auto it = object_to_index_.find(object);
    if (it != object_to_index_.end()) {
      return it->second;
    }
    int index = objects_.size();
    objects_.push_back(object);
    object_to_index_[object] = index;
    return index;
  }

  const T& Lookup(int id) const {
    return objects_[id];
  }
  
  const std::vector<T>& All() const {
    return objects_;
  }

  size_t size() const {
    return objects_.size();
  }

 private:
  std::unordered_map<T, int> object_to_index_;
  std::vector<T> objects_;
};

template<typename T>
const int SymbolTable<T>::NotFound = -1;

typedef SymbolTable<std::string> StringTable;

} // namespace mrps
#endif // UTILS_SYMBOL_TABLE_H_
