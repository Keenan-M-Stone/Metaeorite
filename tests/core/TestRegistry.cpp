#include <memory>
#include <stdexcept>

#include <catch2/catch_test_macros.hpp>

#include "metaeorite/core/Registry.hpp"

namespace {

struct IThing {
    virtual ~IThing() = default;
    [[nodiscard]] virtual int value() const = 0;
};

struct ConcreteThing final : IThing {
    [[nodiscard]] int value() const override { return 42; }
};

} // namespace

using metaeorite::core::EngineRegistry;

TEST_CASE("EngineRegistry registers and creates engines by id", "[registry]") {
    auto& registry = EngineRegistry<IThing>::instance();
    registry.registerEngine("concrete", [] { return std::make_unique<ConcreteThing>(); });

    REQUIRE(registry.contains("concrete"));
    const auto instance = registry.create("concrete");
    REQUIRE(instance->value() == 42);
}

TEST_CASE("EngineRegistry throws for unknown id", "[registry]") {
    auto& registry = EngineRegistry<IThing>::instance();
    REQUIRE_THROWS_AS(registry.create("does-not-exist"), std::out_of_range);
}
