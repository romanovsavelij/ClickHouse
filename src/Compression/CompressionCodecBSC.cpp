#include <Compression/ICompressionCodec.h>
#include <Compression/CompressionInfo.h>
#include <Compression/CompressionFactory.h>
#include <Common/ErrorCodes.h>

#include <libbsc/libbsc.h>

namespace DB
{

class CompressionCodecBSC : public ICompressionCodec
{
public:
    CompressionCodecBSC();
    uint8_t getMethodByte() const override;

    void updateHash(SipHash & hash) const override;

protected:
    UInt32 doCompressData(const char * source, UInt32 source_size, char * dest) const override;
    void doDecompressData(const char * source, UInt32 source_size, char * dest, UInt32 uncompressed_size) const override;

    UInt32 getMaxCompressedDataSize(UInt32 uncompressed_size) const override;
    bool isCompression() const override { return true; }
    bool isGenericCompression() const override { return true; }
};

namespace ErrorCodes
{
    extern const int CANNOT_ALLOCATE_MEMORY;
    extern const int CANNOT_COMPRESS;
    extern const int CANNOT_DECOMPRESS;
}

CompressionCodecBSC::CompressionCodecBSC()
{
    setCodecDescription("BSC");
    int result = bsc_init(LIBBSC_FEATURE_NONE);
    if (result != LIBBSC_NO_ERROR)
    {
        throw Exception("Failed to initialize bsc", ErrorCodes::CANNOT_ALLOCATE_MEMORY);
    }
}

UInt32 CompressionCodecBSC::getMaxCompressedDataSize(UInt32 uncompressed_size) const
{
    return uncompressed_size + LIBBSC_HEADER_SIZE;
}

uint8_t CompressionCodecBSC::getMethodByte() const
{
    return static_cast<uint8_t>(CompressionMethodByte::BSC);
}

void CompressionCodecBSC::updateHash(SipHash & hash) const
{
    getCodecDesc()->updateTreeHash(hash);
}

UInt32 CompressionCodecBSC::doCompressData(const char * source, UInt32 source_size, char * dest) const
{
    int res = bsc_compress(
        reinterpret_cast<const unsigned char *>(source),
        reinterpret_cast<unsigned char *>(dest),
        source_size,
        LIBBSC_DEFAULT_LZPHASHSIZE,
        LIBBSC_DEFAULT_LZPMINLEN,
        LIBBSC_DEFAULT_BLOCKSORTER,
        LIBBSC_DEFAULT_CODER,
        LIBBSC_DEFAULT_FEATURES);
    if (res < 0)
    {
        throw Exception("Compression libbsc failed", ErrorCodes::CANNOT_COMPRESS);
    }
    return res;
}

void CompressionCodecBSC::doDecompressData(const char * source, UInt32 source_size, char * dest, UInt32 uncompressed_size) const
{
    int res = bsc_decompress(
        reinterpret_cast<const unsigned char *>(source),
        source_size,
        reinterpret_cast<unsigned char *>(dest),
        uncompressed_size,
        LIBBSC_FEATURE_NONE);
    if (res != LIBBSC_NO_ERROR)
    {
        throw Exception("Decompression libbsc failed", ErrorCodes::CANNOT_DECOMPRESS);
    }
}

void registerCodecBSC(CompressionCodecFactory & factory)
{
    factory.registerSimpleCompressionCodec(
        "BSC", static_cast<uint8_t>(CompressionMethodByte::BSC), [&]() { return std::make_shared<CompressionCodecBSC>(); });
}

}
