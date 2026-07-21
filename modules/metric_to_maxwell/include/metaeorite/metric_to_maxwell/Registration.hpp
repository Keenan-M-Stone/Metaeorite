#pragma once

namespace metaeorite::metric_to_maxwell {

/// Registers every engine provided by this module with
/// core::EngineRegistry<IMetricToMaxwellEngine>. Call once during
/// application/test start-up before looking engines up by id.
void registerEngines();

} // namespace metaeorite::metric_to_maxwell
