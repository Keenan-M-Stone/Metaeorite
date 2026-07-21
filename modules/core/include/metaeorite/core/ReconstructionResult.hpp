#pragma once

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

namespace metaeorite::core {

/// Generic container for inverse-problem results (Maxwell -> Metric,
/// Maxwell -> Geometry) which are generally non-unique: engines return zero
/// or more candidates, each with a confidence score and the assumptions
/// made while reconstructing it, plus free-form `notes` (e.g. explaining why
/// no candidates were found).
template <typename T>
class ReconstructionResult {
public:
    struct Candidate {
        T value;
        double confidence{0.0};
        std::vector<std::string> assumptions;
    };

    void addCandidate(T value, double confidence, std::vector<std::string> assumptions = {}) {
        candidates_.push_back(Candidate{std::move(value), confidence, std::move(assumptions)});
    }

    void addNote(std::string note) { notes_.push_back(std::move(note)); }

    [[nodiscard]] bool empty() const noexcept { return candidates_.empty(); }
    [[nodiscard]] std::size_t size() const noexcept { return candidates_.size(); }
    [[nodiscard]] const std::vector<Candidate>& candidates() const noexcept { return candidates_; }
    [[nodiscard]] const std::vector<std::string>& notes() const noexcept { return notes_; }

    /// The candidate with the highest confidence. Throws if there are no
    /// candidates.
    [[nodiscard]] const Candidate& best() const {
        if (candidates_.empty()) {
            throw std::out_of_range("ReconstructionResult::best(): no candidates available");
        }
        return *std::max_element(candidates_.begin(), candidates_.end(),
                                  [](const Candidate& a, const Candidate& b) { return a.confidence < b.confidence; });
    }

private:
    std::vector<Candidate> candidates_;
    std::vector<std::string> notes_;
};

} // namespace metaeorite::core
