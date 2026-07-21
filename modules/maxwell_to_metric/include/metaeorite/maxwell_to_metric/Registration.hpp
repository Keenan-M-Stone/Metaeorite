#pragma once

namespace metaeorite::maxwell_to_metric {

/// Registers every engine provided by this module with
/// core::EngineRegistry<IMaxwellToMetricEngine>. Call once during
/// application/test start-up before looking engines up by id.
void registerEngines();

} // namespace metaeorite::maxwell_to_metric
