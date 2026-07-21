#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace metaeorite::core {

/// Generic factory registry used to make every transformation engine
/// pluggable: modules register concrete implementations under a string id,
/// and callers (the Pipeline, tests, CLI tools, ...) obtain instances purely
/// through the abstract `Interface`, never referencing concrete types.
///
/// Registration is explicit (each module exposes a `registerEngines()`
/// function that callers invoke at start-up) rather than relying on static
/// initialization order, which keeps behavior predictable and debuggable.
template <typename Interface>
class EngineRegistry {
public:
    using Factory = std::function<std::unique_ptr<Interface>()>;

    static EngineRegistry& instance() {
        static EngineRegistry registry;
        return registry;
    }

    /// Registers a factory under `id`. Re-registering the same id replaces
    /// the previous factory, which makes it easy to override the default
    /// engine (e.g. with a mock) from tests.
    void registerEngine(std::string id, Factory factory) {
        std::lock_guard lock(mutex_);
        factories_[std::move(id)] = std::move(factory);
    }

    [[nodiscard]] bool contains(const std::string& id) const {
        std::lock_guard lock(mutex_);
        return factories_.contains(id);
    }

    [[nodiscard]] std::unique_ptr<Interface> create(const std::string& id) const {
        std::lock_guard lock(mutex_);
        auto it = factories_.find(id);
        if (it == factories_.end()) {
            throw std::out_of_range("EngineRegistry: no engine registered under id '" + id + "'");
        }
        return it->second();
    }

    [[nodiscard]] std::vector<std::string> availableEngines() const {
        std::lock_guard lock(mutex_);
        std::vector<std::string> ids;
        ids.reserve(factories_.size());
        for (const auto& [id, factory] : factories_) {
            ids.push_back(id);
        }
        return ids;
    }

private:
    EngineRegistry() = default;

    mutable std::mutex mutex_;
    std::unordered_map<std::string, Factory> factories_;
};

} // namespace metaeorite::core
