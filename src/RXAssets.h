#pragma once

#include <filesystem>

#include <bitsery/bitsery.h>
#include <bitsery/traits/vector.h>
#include "bitsery/adapter/buffer.h"

namespace RxAssets
{
    // struct AssetIndex;

    typedef bitsery::OutputBufferAdapter<std::vector<uint8_t>> output_buffer_adapter;
    typedef bitsery::InputBufferAdapter<std::vector<uint8_t>> input_buffer_adapter;
#if 0
    void SaveIndex(std::filesystem::path indexFile, RXAssets::AssetIndex & index);

    void LoadIndex(std::filesystem::path indexFile, RXAssets::AssetIndex & index);

    void DeletePackFile(std::filesystem::path assetFile);

    void ImportAsset(std::filesystem::path assetFile, std::string & asset, void * data, size_t size);
#endif
    template<typename T>
    size_t QuickSerialise(std::vector<uint8_t> & buf, T data)
    {
        return bitsery::quickSerialization<output_buffer_adapter>(buf, data);
    }
}
