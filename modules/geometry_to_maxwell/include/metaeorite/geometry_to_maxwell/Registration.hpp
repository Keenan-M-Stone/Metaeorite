#pragma once

namespace metaeorite::geometry_to_maxwell {

/// Registers every engine provided by this module with
/// core::EngineRegistry<IGeometryToMaxwellEngine>. Call once during
/// application/test start-up before looking engines up by id.
void registerEngines();

} // namespace metaeorite::geometry_to_maxwell
