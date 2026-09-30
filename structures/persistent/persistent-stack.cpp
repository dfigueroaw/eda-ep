#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vector>

template <typename T> class FPStack {
  struct Node {
    T value;
    std::shared_ptr<const Node> next;

    Node(const T &value_, std::shared_ptr<const Node> next_)
        : value(value_), next(next_) {}
  };

  std::vector<std::shared_ptr<const Node>> versions;

  void check_version(const size_t version) const {
    if (version >= versions.size()) {
      throw std::out_of_range("invalid stack version");
    }
  }

public:
  FPStack() : versions(1, nullptr) {}

  size_t current_version() const { return versions.size() - 1; }

  size_t versions_count() const { return versions.size(); }

  bool empty(const size_t version) const {
    check_version(version);
    return versions[version] == nullptr;
  }

  bool empty() const { return empty(current_version()); }

  const T &top(const size_t version) const {
    check_version(version);
    if (empty(version)) {
      throw std::underflow_error("top from empty stack");
    }

    return versions[version]->value;
  }

  const T &top() const { return top(current_version()); }

  size_t push(const size_t version, const T &value) {
    check_version(version);
    versions.emplace_back(std::make_shared<Node>(value, versions[version]));
    return current_version();
  }

  size_t push(const T &value) { return push(current_version(), value); }

  bool pop(const size_t version) {
    check_version(version);
    if (empty(version)) {
      return false;
    }

    versions.emplace_back(versions[version]->next);
    return true;
  }

  bool pop() { return pop(current_version()); }
};
