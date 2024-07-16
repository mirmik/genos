#ifndef CROW_QOSBYTE_H
#define CROW_QOSBYTE_H

#undef round
#include <cassert>
#include <cmath>
#include <type_traits>

namespace crow
{
    static constexpr int quant_const = 10;
    struct qosbyte
    {
        uint8_t _qos : 2;
        uint8_t _quant_code : 6;

        static uint8_t quant_to_code(uint16_t quant)
        {
            auto code = std::round((float)(quant) / quant_const);
            assert(code >= 0);
            return code;
        }

        uint16_t ackquant() const
        {
            return _quant_code * quant_const + 4;
        }

        uint8_t qos() const
        {
            return _qos;
        }

        void set_ackquant(uint16_t quant)
        {
            _quant_code = quant_to_code(quant);
        }

        void set_qos(uint8_t quality)
        {
            _qos = quality;
        }
    };
    static_assert(sizeof(qosbyte) == 1, "qosbyte wrong size");

    static inline qosbyte make_qosbyte(uint8_t quality, uint16_t ackquant)
    {
        qosbyte q;
        q._qos = quality;
        q._quant_code = qosbyte::quant_to_code(ackquant);
        return q;
    }

}

#endif