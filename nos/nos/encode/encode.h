#ifndef NOS_CODEC_ENCODE_H
#define NOS_CODEC_ENCODE_H

namespace nos 
{
    /// \brief Encode utf8 string to binary
    /// \param str The string to encode
    /// \return The encoded binary
    static inline std::vector<uint8_t> encode_utf8(const std::string& str) 
    {
        std::vector<uint8_t> res;
        res.reserve(str.size());
        std::copy(str.begin(), str.end(), std::back_inserter(res));
        return res;
    }

    /// \brief Decode binary to utf8 string
    /// \param buf The binary to decode
    /// \return The decoded string
    static inline std::string decode_utf8(const std::vector<uint8_t>& buf) 
    {
        return std::string(buf.begin(), buf.end());
    }
}

#endif