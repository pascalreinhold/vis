#pragma once

#include <cstdint>
#include <limits>

namespace lab {

enum HandleType {
    PipelineLayoutHandleType,
    PipelineHandleType,
    BufferHandleType,
    ModelHandleType,
    TextureHandleType,
};

// avoid having to rewrite the same code for each handle type
template <HandleType HT>
struct Handle {
    using underlying_type = uint32_t;
    underlying_type id = std::numeric_limits<uint32_t>::max();
    static constexpr uint32_t invalid_id = std::numeric_limits<uint32_t>::max();

    bool operator==(const Handle<HT>& other) const { return id == other.id; }
    bool operator!=(const Handle<HT>& other) const { return id != other.id; }
    bool operator<(const Handle<HT>& other) const { return id < other.id; }
    bool operator>(const Handle<HT>& other) const { return id > other.id; }
    bool operator<=(const Handle<HT>& other) const { return id <= other.id; }
    bool operator>=(const Handle<HT>& other) const { return id >= other.id; }
};

template struct Handle<PipelineLayoutHandleType>;
using PipelineLayoutHandle = Handle<PipelineLayoutHandleType>;
template struct Handle<PipelineHandleType>;
using PipelineHandle = Handle<PipelineHandleType>;
template struct Handle<BufferHandleType>;
using BufferHandle = Handle<BufferHandleType>;
template struct Handle<ModelHandleType>;
using ModelHandle = Handle<ModelHandleType>;
template struct Handle<TextureHandleType>;
using TextureHandle = Handle<TextureHandleType>;

} // namespace lab